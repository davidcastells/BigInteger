# BigInteger
Simple BigInteger C++ class 

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
