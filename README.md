# BigInteger
Simple BigInteger C++ class , and a simple C alternative version

By BigInteger I mean an integer number with a large number of bits, much more 
than 32 or 64, which are the sizes that we commonly found in digital computers.

I implement them as a variable length number. As many similar implementations I store the number information 
as an array of 32 bits integers (called limbs).

The motivation of starting this simple C++ class was to understand the implementation details of various large integer algorithms, 
such as modular exponentiation using montgomery form.

There are other implementations out there, but from my point of view they have several drawbacks, which I try to describe below

* GMP (https://gmplib.org/), big library with a lot of optimizations for different platforms. The code is not easyly read because of 
lack of use of selfDescribing variable names and lack of comments. In summary, too big, too difficult to read.

So my goals are:

* Readible Code
* Small Size
* Be able to use it as a baseline for GPU/FPGA optimization (this is why basic C is handy)


## Building the code

A simple Makefile is used targeting my system (sorry about that)

I use cygwin and have MinGW installed

```
make
```

Will make the cygwin version of the code (BigInteger.cyg.exe) without any optimization (-O0). This is useful for debugging

```
make perf
```

Will make the cygwin (BigInteger.cyg.exe), the mingw (BigInteger.mingw.exe), and the cygwin version using the ZZ lib (ZZTest.cyg.exe). 
The MinGW version is compiled with full optimization -O3
The ZZ lib version is to be used as a performace reference.

I also planned to have a version using the Microsoft Compiler, but disabled until I solve the problems I get trying to initialize vectors in the stack with dynamic sizes (not using constants).

## Running the code

I use a cygwin terminal and run one of the applications

```
./BigInteger.cyg.exe
./BigInteger.mingw.exe
./ZZTest.cyg.exe
```

you can use the following parameters 

--help                  Shows a brief help information

--test-correctness      It executes unit tests on all the functions

--test-all-performance  It tries to test the performance of most of the functions

--test-performance      It focus on testing the performance of most important functions (specially modular exponentiation)


## Different implementation folders

* cpp: A C++ implementation (BigInteger class). It both have (object) member functions and static functions to perform various operations.

* c: An Object Oriented C based implementation. It's very similar to previous (cpp) but in this case I use an structure big_integer to store the 
info (limb array and size) instead of a C++ class. Functions start with the big_integer_ prefix.
 
* c_base: A C implementation. All functions start with the big_integer_base_ prefix. It assumes that a big array is used to store the limbs 
 of different numbers. So functions take a pointer to the big array and also an offset index where the number starts. 
The main reason for this kind of memory access is to be able to directly address global memory in OpenCL kernels.

* c_array: A C implementation using static arrays. All functions start with the big_integer_array_ prefix. All arrays are statically defined, so 
there is no support for variable length numbers.

* c_apint: A C implementation using the ap_uint template type (unsigned version of ap_int). This type is used by several HLS tools to describe arbitrary length integer
numbers. 
