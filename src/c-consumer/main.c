/*
 based on example by Alan Antonuk
 LICENSE: MIT
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <assert.h>
#include "utils.h"

#define SUMMARY_EVERY_US 1000000

static void run(amqp_connection_state_t conn) {
  uint64_t start_time = now_microseconds ();
  int received = 0;
  int previous_received = 0;
  uint64_t previous_report_time = start_time;
  uint64_t next_summary_time = start_time + SUMMARY_EVERY_US;
  amqp_frame_t frame;
  uint64_t now;
  printf("for loop\n");
  for (;;) {
      amqp_rpc_reply_t ret;
      amqp_envelope_t envelope;
      now = now_microseconds();
      if (now > next_summary_time) {
        int countOverInterval = received - previous_received;
        double intervalRate = countOverInterval / ((now - previous_report_time) / 1000000.0);
        printf(
          "%d ms: Received %d - %d since last report (%d Hz)\n",
          (int) (now - start_time) / 1000,
          received,
          countOverInterval,
          (int) intervalRate
        );

	  previous_received = received;
	  previous_report_time = now;
	  next_summary_time += SUMMARY_EVERY_US;
	}

      amqp_maybe_release_buffers(conn);
      ret = amqp_consume_message(
        conn, // state - the connection object
        &envelope, // envelope - a pointer to a amqp_envelope_t object. Caller should call amqp_destroy_envelope() when it is done using the fields in the envelope object. The caller is responsible for allocating/destroying the amqp_envelope_t object itself.
        NULL, // timeout - a timeout to wait for a message delivery. Passing in NULL will result in blocking behavior.
        0 // flags - pass in 0. Currently unused.
      );

      if(AMQP_RESPONSE_NORMAL != ret.reply_type) {
        if( AMQP_RESPONSE_LIBRARY_EXCEPTION == ret.reply_type && AMQP_STATUS_UNEXPECTED_STATE == ret.library_error) {
          if (AMQP_STATUS_OK != amqp_simple_wait_frame (conn, &frame)) {
            return;
		}
		if(AMQP_FRAME_METHOD == frame.frame_type) {
		  switch (frame.payload.method.id) {
		    case AMQP_BASIC_ACK_METHOD:
		      /* if we've turned publisher confirms on, and we've published a message here is a message being confirmed. */
		      break;
		    case AMQP_BASIC_RETURN_METHOD: {
		      /* if a published message couldn't be routed and the mandatory flag was set this is what would be returned. The message then needs to be read. */
		      amqp_message_t message;
		      ret = amqp_read_message(conn, frame.channel, &message, 0);
		      if (AMQP_RESPONSE_NORMAL != ret.reply_type) { return; }
		      amqp_destroy_message(&message);
		    }
		    break;
		    case AMQP_CHANNEL_CLOSE_METHOD:
		      /* a channel.close method happens when a channel exception occurs,
		       * this can happen by publishing to an exchange that doesn't exist
		       * for example.
		       *
		       * In this case you would need to open another channel redeclare
		       * any queues that were declared auto-delete, and restart any
		       * consumers that were attached to the previous channel.
		       */
		      return;

		    case AMQP_CONNECTION_CLOSE_METHOD:
		      /* a connection.close method happens when a connection exception
		       * occurs, this can happen by trying to use a channel that isn't
		       * open for example.
		       *
		       * In this case the whole connection must be restarted.
		       */
		      return;

		    default:
		      fprintf(
		        stderr,
		        "An unexpected method was received %u\n",
		        frame.payload.method.id
		      );
		      return;
		    }
		  }
	    }
	  }
	else {
	  amqp_destroy_envelope(&envelope);
	}
	  received++;
  }
}

int main (int argc, char const *const *argv) {
  char const *hostname;
  int port;
  int status;
  char const *exchange;
  char const *queuename;
  char const *bindingkey;

  amqp_exchange_declare_ok_t *exchange_struct;
  amqp_queue_declare_ok_t *queue_struct;
  amqp_socket_t *socket = NULL;
  amqp_connection_state_t conn;

  hostname = "localhost";
  port = atoi("32777");
  exchange = "try-c-consumer";
  queuename = "try-c-consumer";
  bindingkey = "c-consumer";

  printf("connecting\n");

  conn = amqp_new_connection();
  socket = amqp_tcp_socket_new(conn);
  if (!socket) { die("creating TCP socket");}

  status = amqp_socket_open(
    socket, // self - A socket object.
    hostname, // host - Connect to this host.
    port // port - Connect on this remote port.
  );
  if (status)
    {
      die ("opening TCP socket");
    }

  printf("logging in\n");

  die_on_amqp_error(amqp_login(
      conn, // state - the connection object
      "/", // vhost - the virtual host to connect to on the broker. The default on most brokers is "/"
      0, // channel_max - the limit for number of channels for the connection. 0 means no limit, and is a good default (AMQP_DEFAULT_MAX_CHANNELS) Note that the maximum number of channels the protocol supports is 65535 (2^16, with the 0-channel reserved). The server can set a lower channel_max and then the client will use the lowest of the two
      AMQP_DEFAULT_FRAME_SIZE, // frame_max - the maximum size of an AMQP frame on the wire to request of the broker for this connection. 4096 is the minimum size, 2^31-1 is the maximum, a good default is 131072 (128KB); AMQP_DEFAULT_FRAME_SIZE
      0, // heartbeat - the number of seconds between heartbeat frames to request of the broker. A value of 0 disables heartbeats. Note rabbitmq-c only has partial support for heartbeats, as of v0.4.0 they are only serviced during amqp_basic_publish() and amqp_simple_wait_frame()/amqp_simple_wait_frame_noblock()
      AMQP_SASL_METHOD_PLAIN, // sasl_method - the SASL method to authenticate with the broker.
      "guest",
      "guest"
      ),
    "Logging in"
  );
  amqp_channel_open(conn, 1);

  die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

  exchange_struct = amqp_exchange_declare(
      conn, // state - connection state
      1,  // channel - the channel to do the RPC on
      amqp_cstring_bytes(exchange), // exchange
      amqp_cstring_bytes("topic"), // type
      0, // passive
      0, // durable
      0, // auto_delete
      0, // internal
      amqp_empty_table // arguments
    );

  queue_struct = amqp_queue_declare(
        conn, // state - connection state
        1, // channel - the channel to do the RPC on
        amqp_cstring_bytes(queuename), // queue - queue
        0, // passive - passive
        0, // durable - durable
        0, // exclusive - exclusive
        0, // auto_delete - auto_delete
        amqp_empty_table // arguments - arguments
    );




  printf("consumer_count");




  amqp_basic_qos(
    conn, // state - connection state
    1, // channel - the channel to do the RPC on
    1, // prefetch_size
    1, // prefetch_count
    1 // global
  );

  amqp_queue_bind(
    conn, // state - connection state
    1, // channel - the channel to do the RPC on
    amqp_cstring_bytes(queuename), // queue
    amqp_cstring_bytes(exchange), // exchange
    amqp_cstring_bytes(bindingkey), // routing_key
    amqp_empty_table // arguments - arguments
    );

  amqp_basic_consume(
    conn, // state - connection state
    1, // channel - the channel to do the RPC on
    queue_struct->queue, // queue
    amqp_empty_bytes, // consumer_tag
    0, // no_local
    1, // no_ack
    0, // exclusive
    amqp_empty_table // arguments
  );

  run(conn);

  die_on_amqp_error(amqp_channel_close(
       conn, // state - the connection object
       1, // channel - the channel identifier
       AMQP_REPLY_SUCCESS // code - the reason for closing the channel, AMQP_REPLY_SUCCESS is a good default
       ),
     "Closing channel"
  );

  die_on_amqp_error(amqp_connection_close(
      conn, // state - the connection object
      AMQP_REPLY_SUCCESS // code - the reason for closing the channel, AMQP_REPLY_SUCCESS is a good default
    ),
    "Closing connection"
  );
  die_on_error(amqp_destroy_connection(conn), "Ending connection");

  return 0;
}
