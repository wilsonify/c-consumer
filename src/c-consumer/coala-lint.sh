docker run \
-u=$(UID):$(GID) \
-v=$(pwd):/app \
--workdir=/app \
coala/base \
coala --ci \
--bears GNUIndentBear \
--files main.c \
-s