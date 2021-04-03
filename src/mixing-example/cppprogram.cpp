    #include <iostream>
    #include "include/cpp-library/cppfunction.h"

    extern "C" {
        #include "include/f-library/ffunction.h"
    }

    extern "C" {
      #include "include/c-library/cfunction.h"
    }

    int main() {
      float a=1.0, b=2.0;

      std::cout << "Before running Fortran function:" << std::endl;
      std::cout << "a=" << a << std::endl;
      std::cout << "b=" << b << std::endl;

      ffunction_(&a,&b);

      std::cout << "After running Fortran function:" << std::endl;
      std::cout << "a=" << a << std::endl;
      std::cout << "b=" << b << std::endl;

      std::cout << "Before running C function:" << std::endl;
      std::cout << "a=" << a << std::endl;
      std::cout << "b=" << b << std::endl;

      cfunction(&a,&b);

      std::cout << "After running C function:" << std::endl;
      std::cout << "a=" << a << std::endl;
      std::cout << "b=" << b << std::endl;

      std::cout << "Before running C++ function:" << std::endl;
      std::cout << "a=" << a << std::endl;
      std::cout << "b=" << b << std::endl;

      cppfunction(&a,&b);

      std::cout << "After running C++ function:" << std::endl;
      std::cout << "a=" << a << std::endl;
      std::cout << "b=" << b << std::endl;

      return 0;
    }