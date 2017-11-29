/*
 * Copyright (C) 2017 Universitat Autonoma de Barcelona - David Castells-Rufas <david.castells@uab.cat>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <sstream> 
#include <string>

#include "BigInteger.h"
//#include "BigNum.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <time.h>

#define VERBOSITY_LEVEL_DIV             5
#define VERBOSITY_LEVEL_GET_LENGTH      6
#define VERBOSITY_LEVEL_MULT_KARATSUBA  5
#define VERBOSITY_LEVEL_MULT_MONTGOMERY 3
#define VERBOSITY_LEVEL_MONTGOMERY      4
#define VERBOSITY_LEVEL_POWER_MOD       2
#define VERBOSITY_LEVEL_INV_MOD         4
#define VERBOSITY_LEVEL_RANGE           6

template <typename T>
std::string to_string(T value)
{
  //create an output string stream
  std::ostringstream os ;

  //throw the value into the string stream
  os << value ;

  //convert the string stream into a string and return
  return os.str() ;
}


int BigInteger::verbosity = 0;
int BigInteger::extraChecks = 1;

/**
 * Default constructor 
 */
BigInteger::BigInteger()
{
    m_data = NULL;
    m_size = 0;
}

/**
 * Copy constructor
 * @param orig
 */
BigInteger::BigInteger(const BigInteger& orig)
{
    m_data = NULL;
    m_size = 0;
    
    initSize(orig.m_size);
        
    copy(&orig);
}



void BigInteger::setIntValue(unsigned int v)
{
    zero();
    m_data[0] = v;    
}

/**
 * 
 * @param orig
 */
void BigInteger::copy(const BigInteger* orig)
{
    int minCopySize = (m_size < orig->m_size)? m_size : orig->m_size;
    int i;
    for (i=0; i < minCopySize; i++)
        m_data[i] = orig->m_data[i];
    
    // zero the rest of the data 
    for (; i < m_size; i++)
        m_data[i] = 0;
}

void BigInteger::copy(unsigned int* limbs)
{
    for (int i=0; i < m_size; i++)
        m_data[i] = limbs[i];    
}

BigInteger::~BigInteger()
{
    if (m_data != NULL)
        delete [] m_data;
    
    m_data = NULL;
}

void BigInteger::add(BigInteger* y)
{
    BigInteger ref(*this);
    BigInteger::add(this, &ref, y);
}
     
void BigInteger::add(BigInteger* r, BigInteger* a, BigInteger* b)
{
    unsigned int carryIn = 0;
    unsigned int carryOut = 0;
            
    unsigned int* px = a->m_data;
    unsigned int* py = b->m_data;
    unsigned int* pr = r->m_data;

        
    unsigned int sum;

    for (int i=0; i < r->m_size; i++)
    {
        sum = 0;

        if (i < a->m_size) sum += px[i];
        if (i < b->m_size) sum += py[i];

        if ((i < a->m_size) && (sum < px[i]))
        {
            if (carryIn) sum++;
            carryOut = 1;
        }
        else if (carryIn)
        {
            sum++;
            carryOut = (sum == 0)? 1: 0;
        }

        carryIn = carryOut;
        pr[i] = sum;
    }
}




/**
 * Computes r = a + (b << shift)
 * @param r
 * @param a
 * @param b
 * @param shift left shifts in bits
 */
void BigInteger::addShifted(BigInteger* r, BigInteger* a, BigInteger* b, int shift)
{
    unsigned int carryIn = 0;
    unsigned int carryOut = 0;
            
    unsigned int* px = a->m_data;
    unsigned int* py = b->m_data;
    unsigned int* pr = r->m_data;

    if ((shift % 32) != 0)        
        std::cerr << "## ERROR ## shift should be multiple of 32" << std::endl;
    
    int shiftedLimbs = shift / 32;
    
    
    for (int ia=0; ia < shiftedLimbs; ia++)
    {
        unsigned int sum = 0;

        if (ia < a->m_size) sum += px[ia];
        
        pr[ia] = sum;
    }
    
    for (int ia=shiftedLimbs, ib = 0; ia < r->m_size; ia++, ib++)
    {
        unsigned int sum = 0;

        if (ia < a->m_size) sum += px[ia];
        if (ib < b->m_size) sum += py[ib];

        if ((ia < a->m_size) && (sum < px[ia]))
        {
            if (carryIn) sum++;
            carryOut = 1;
        }
        else if (carryIn)
        {
            sum++;
            carryOut = (sum == 0)? 1: 0;
        }

        carryIn = carryOut;
        pr[ia] = sum;
    }
}

/**
 * Returns the value of the ith bit of the integer
 * @param bitnum
 * @return 
 */
int BigInteger::getBit(int bitnum)
{
   int limbIndex = bitnum / 32;
   int bitIndex = bitnum % 32;

   return (m_data[limbIndex] >> bitIndex) & 1;
}

 /**
  * @return the number of bits that the number can store
  */
int BigInteger::getNumBits()
{
    return m_size * 32;
}

/**
 * 
 * @return the number of the most significant active bit
 */
int BigInteger::getLength()
{
    int len = 0;
    int greaterActiveLimb = 0;

    // find the highest limb with an active bit
    for (int i=m_size-1; (i > 0) && (greaterActiveLimb == 0); i--)
    {
        if (m_data[i] > 0)
            greaterActiveLimb = i;
    }

    if (verbosity > VERBOSITY_LEVEL_GET_LENGTH) std::cout << "BigInteger::getLength greaterActiveLimb = " << greaterActiveLimb << std::endl;

    // find the highest bit 
    unsigned int test = m_data[greaterActiveLimb];
    int numActiveBits = 0;

    if (verbosity > VERBOSITY_LEVEL_GET_LENGTH) std::cout << "BigInteger::getLength test = " << test << std::endl;

    while (test > 0)
    {
        test = test >> 1;
        numActiveBits++;
    }

    len = (numActiveBits + greaterActiveLimb * 32);

    return len;
}
        
        /**
         * Based of parseStr_shiftAdd
         * @param str
         */
        /*void BigInteger::parseString(const char* str)
        {
            // multiply by x*10 is x*(8+2) -> 8*x + 2*x
            int l = strlen(str);
            
            zero();
            
            BigInteger digit;
            digit.initSize(1);
            
            // V = V*10+s[i];
            for (int i=0; i < l; i++)
            {
                //  x=x*10
                BigInteger ref(*this);
                BigInteger::shiftLeft(&ref, &ref, 1);
                BigInteger::shiftLeft(this, this, 3);
                BigInteger::add(this, this, &ref);
                
//                std::cout << toString() << " + " << str[i] << " = "; 
                
                digit.setIntValue(str[i]-'0');
                BigInteger::add(this, this, &digit);
                
//                std::cout << toString() << std::endl;
            }
        }*/
        
        /**
         * 
         * @param v
         * @return 1 if the two numbers are equal, ignoring their size
         */
        int BigInteger::isEqual(BigInteger* a, BigInteger* b)
        {
            unsigned int* pa = a->m_data;
            unsigned int* pb = b->m_data;
            
            int minSize = (a->m_size > b->m_size)? b->m_size : a->m_size;

            // check if there is any different limb
            for (int i=minSize-1; i >= 0; i--)
                if (pa[i] != pb[i])
                    return false;
            
            // if sizes are different, ensure the bigger part is zero
            for (int i=minSize; i < a->m_size; i++)
                if (pa[i] != 0)
                    return false;
            
            for (int i=minSize; i < b->m_size; i++)
                if (pb[i] != 0)
                    return false;
            
            return true;
        }
        
/**
 * Returns true if this < v
 * Returns false if this >= v
 * 
 * @param v
 * @return 
 */
int BigInteger::isLessThan(BigInteger* v)
{
    unsigned int* pv = v->m_data;

    // if there is any 1 in a bigger size v before the range of this
    // then return true
    for (int i=v->m_size-1; i >= m_size; i--)
        if (pv[i])
            return true;

    for (int i=m_size-1; i >= 0; i--)
        if (i >= v->m_size)
        {
            if (m_data[i] != 0)
                return false;
        }
        else if (m_data[i] < pv[i])
            return true;
        else if (m_data[i] > pv[i])
            return false;

    return false;
}
        
/**
 * Initializes size and parses hex string
 * @param str
 */
void BigInteger::initFromHexString(const char* str)
{
    initSize((strlen(str)+7)/8);
    parseHexString(str);
}
        
        
/**
 * Initialize the size of the limbs array which are 32 bits numbers
 * @param s number of limbs 
 */
void BigInteger::initSize(int s)
{
    if  (m_size > 0)
        delete m_data;
        
    m_size = s;
    m_data = new unsigned int [m_size];
    
    assert(m_data);
}

        
        
        
        
        
   
        
        
void BigInteger::div(BigInteger* m)
{
    BigInteger ref(*this);
    BigInteger r(*this);

    BigInteger::div_naive(&ref, m, this, &r);
}
        
        
/**
 * Compute division, so that
 *  nq = x / y
 *  nr = x % y
 * 
 * @param x
 * @param y
 * @param nq
 * @param nr
 */
void BigInteger::div_naive(BigInteger* x,
        BigInteger* y, 
        BigInteger* q, 
        BigInteger* r)
{
    if (verbosity> VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div x = " << x->toHexString() << std::endl;
    if (verbosity> VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div /   " << y->toHexString() << std::endl;

    
    //q.initSize(x->m_size);
    //r.initSize(x->m_size);
    q->zero();

    if (x->isLessThan(y))
    {
        r->copy(x);
        //q->copy(&q);
        return;
    }


    // get the length of y
    int yl = y->getLength();
    int rl = x->getLength();

    int downBit = rl-yl;

    if (extraChecks) assert(downBit >= 0);

    // get the yl most significant bits of ref
    //r->copy(&ref);
    //if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div r " <<  r->toHexString() << std::endl;

    BigInteger::range(r, x, rl-1, downBit--);

    if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div r range(" << to_string(rl-1) << "," << to_string(downBit+1) << ") = " << r->toHexString() << std::endl;

    if (r->isLessThan(y))
    {
        if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div r < divisor" << std::endl;

        // take another bit from ref
        BigInteger::range(r, x, rl-1, downBit--);

        if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div r range(" << to_string(rl-1) << "," << to_string(downBit+1) << ") = " << r->toHexString() << std::endl;
    }

    if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div term = " << r->toHexString() << std::endl;
    if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div      - " << y->toHexString() << std::endl;

    q->inc();
    r->subtract(y);

    if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div      = " << r->toHexString() << std::endl;
    if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div    q = " << q->toHexString() << std::endl;

    // take another bit
    while (downBit >= 0)
    {
        r->shiftLeft(1);
        if (x->getBit(downBit--))
            r->inc();

        if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div term = " << r->toHexString() << std::endl;

        if (r->isLessThan(y))
        {
            q->shiftLeft(1);    // put a zero in q
            //goto loop;

            if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div  0 q = " << q->toHexString() << std::endl;
        }
        else
        {
            q->shiftLeft(1);     // put a one in q
            q->inc();

        if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div  1 q = " << q->toHexString() << std::endl;

        if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div term = " << r->toHexString() << std::endl;
        if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div      - " << y->toHexString() << std::endl;

            r->subtract(y);
            //goto loop;

        if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div      = " << r->toHexString() << std::endl;

        }
    } 


    //nr->copy(&r);
    //nq->copy(&q);

    if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div     nr = " << r->toHexString() << std::endl;
    if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div     nq = " << q->toHexString() << std::endl;
}
        
        
        /**
         * 
         * @return true is the number is odd
         */
        int BigInteger::isOdd()
        {
            return m_data[0] & 0x1;
        }
        
        /**
         * 
         * @return true is the number is zero
         */
        int BigInteger::isZero()
        {
            unsigned int* p = m_data;
            
            for (int i=0; i<m_size; i++)
            {
                if (p[i] != 0)
                    return 0;   // exit if it is not zero
            }
            
            return 1;
        }
        
        int BigInteger::isOne()
        {
            for (int i=1; i<m_size; i++)
                if (m_data[i] != 0)
                    return 0;   // exit if it is not zero
            
            return (m_data[0] == 1);
        }
        
        int BigInteger::isNegative()
        {
            return m_data[m_size-1] & 0x80000000;
        }
        
/**
 * Increment a big number
 */
void BigInteger::inc()
{
    int i = 0;
    int doRun;
    do
    {                
        m_data[i]++;

        doRun = (m_data[i] == 0);

        i++;


    } while(doRun);
}
        

void BigInteger::mod(BigInteger* m)
{
    BigInteger ref(*this);
    BigInteger q(*this);

    BigInteger::div_naive(&ref, m, &q, this);
}
        
/**
 * from https://github.com/adamwalker/mmul/blob/master/crypto.c (big_mul_low)
 */        
void BigInteger::multLow(BigInteger* r, BigInteger* a, BigInteger* b)
{
    if (extraChecks) 
    {
        assert(r->m_size >= a->m_size);
        assert(r->m_size >= b->m_size);
    }

    int i, j;

    r->zero();

    int asize = a->m_size;
    int bsize = b->m_size;
    int rsize = r->m_size;
    
    uint32_t lo, hi;
    
    for(i=0; i<asize; i++)
    {
        uint64_t carry = 0;
        for(j=0; j<bsize; j++)
        {
            int idx = i + j;
            if (idx < rsize)
            {
                mult(a->m_data[i], b->m_data[j], &hi, &lo);

                uint64_t accum  = r->m_data[idx] + carry + lo;
                carry           = (accum >> 32) + hi;
                r->m_data[idx]          = accum;
            }
        }
    }
}

/**
 * from https://github.com/adamwalker/mmul/blob/master/crypto.c (mulhilo)
 */        
void BigInteger::mult(BigInteger* r, BigInteger* a, BigInteger* b)
{
    if (extraChecks) 
    {
        assert(r->m_size >= (a->m_size + b->m_size));
    }
    
    r->zero();

    int asize = a->m_size;
    int bsize = b->m_size;
    int rsize = r->m_size;
    uint64_t carry;
    uint64_t accum;
    unsigned int adata;
    int idx;
    uint32_t lo, hi;
    unsigned int* prdata;
    
    for (int i=0; i< asize; i++)
    {
        carry = 0;
        adata = a->m_data[i];
        prdata = &r->m_data[i];
        
        for (int j=0; j<bsize; j++)
        {
            idx = i + j;

            BigInteger::mult(adata, b->m_data[j], &hi, &lo);

            //accum = r->m_data[idx] + carry + lo;
            accum = *prdata;
            accum += carry;
            accum += lo;
            
            carry = (accum >> 32);
            carry += hi;
            
            //r->m_data[idx] = accum;
            *prdata = accum;
            
            prdata++;
        }
        
        if ((i + bsize)< rsize)
            r->m_data[i + b->m_size] = carry;
    }
}        
        
/**
 * Multiply two big numbers.
 * Better is a > b
 * @param r result, r = a * b
 * @param a first operand
 * @param b second operand
 */
void BigInteger::mult_naive(BigInteger* r, BigInteger* a, BigInteger* b)
{
    BigInteger term;
    term.initSize(r->m_size);

    if (extraChecks)
        if (!a->isNegative() && !b->isNegative())
            assert((r->m_size*32) >= ((a->getLength()+b->getLength())));

    int i=0;
    int bDigit = 0;
    r->zero();

//            cout << endl;

    while (bDigit < b->m_size)
    {
        unsigned int digit = b->m_data[bDigit] ;   // take a 2^32 base digit

        if (digit != 0)
        {
            term.copy(a);

            // multiply term by digit
            unsigned long long carry = 0;
            unsigned long long pr_low, pr_high;

            for (int k=0; k < term.m_size-1; k+=2)
            {
                unsigned long long td_low = term.m_data[k] ;
                unsigned long long td_high = term.m_data[k+1] ;


                pr_low = digit * (td_low) + carry;     // partial result (lower part)
//                        std::cout << "[" << k << "] " << td_low << " * " << digit << " + " << carry << " = ";

                carry = pr_low >> 32;                           // carry is the higher part
                pr_low = pr_low & 0xFFFFFFFF;                  

//                        std::cout << pr_low << std::endl;

                pr_high = digit * (td_high) + carry;

                carry = pr_high >> 32;
                pr_high = pr_high & 0xFFFFFFFF;

                term.m_data[k] = pr_low;
                term.m_data[k+1] = pr_high;
            }

            addShifted(r, r, &term, i);
        }

        bDigit++;

        i+=32;
    }
}
        
        /**
         * 
         * @param r
         * @param x
         * @param y
         */
        void BigInteger::mult_karatsubaRecursive(BigInteger* r, BigInteger* x, BigInteger* y)
        {
            if (x->isZero() || y->isZero())
            {
                r->zero();
            }
            else
            {
                int maxSize = (x->m_size > y->m_size)? x->m_size : y->m_size;
                int resultSize = r->m_size;

                if (maxSize < 4)
                {
                    BigInteger::mult(r, x, y);
                    return;
                }

                int halfSize = (maxSize+1) / 2;


                BigInteger x1, x0;
                BigInteger y1, y0;
                x1.initSize(halfSize);
                x0.initSize(halfSize);
                y1.initSize(halfSize);
                y0.initSize(halfSize);
                BigInteger::range(&x1, x, x->getNumBits()-1, halfSize*32);
                BigInteger::range(&x0, x, halfSize*32-1, 0);
                BigInteger::range(&y1, y, y->getNumBits()-1, halfSize*32);
                BigInteger::range(&y0, y, halfSize*32-1, 0);

                //const int m = numBits/2;

                BigInteger z0;
                BigInteger z1;
                BigInteger z2;
                z0.initSize(maxSize+1);
                z2.initSize(maxSize+1);

                BigInteger::mult_karatsubaRecursive(&z0, &x0, &y0);
                BigInteger::mult_karatsubaRecursive(&z2, &x1, &y1);



                BigInteger t1;
                t1.initSize(halfSize+1);

                BigInteger::add(&t1, &y1, &y0);

                BigInteger t2;
                t2.initSize(halfSize+1);

                BigInteger::add(&t2, &x1, &x0);
                z1.initSize(t1.m_size + t2.m_size);
                BigInteger::mult_karatsubaRecursive(&z1, &t1, &t2);

                // z1 = z1 - z2-z0
                z1.subtract(&z2);
                z1.subtract(&z0);


                BigInteger z1z2;
                z1z2.initSize(maxVal(z1.m_size, z2.m_size+halfSize));
                z1z2.zero();
                r->zero();
                BigInteger::addShifted(&z1z2, &z1, &z2, halfSize*32);
//                std::cout << "  z2 (" << z2.m_size << ") = " << z2.toHexString() << std::endl;
//                std::cout << "  z1 (" << z1.m_size << ") = " << z1.toHexString() << std::endl;
//                std::cout << "  z1+z2<<" << halfSize*32 << " (" << z1z2.m_size << ") = " << z1z2.toHexString() << std::endl;
                
                BigInteger::addShifted(r, &z0, &z1z2, halfSize*32);
                
//                std::cout << "  z0+z1z2 <<hs (" << r->m_size << ") = " << r->toHexString() << std::endl;
                
                if (extraChecks)
                {
                    BigInteger r2(*r);
                    mult(&r2, x, y);

                    if (!isEqual(r, &r2))
                    {

                        std::cout << "  ### ERROR ### "  << std::endl;

                        std::cout << "  X (" << x->m_size << ") = " << x->toHexString() << std::endl;
                        std::cout << "* Y (" << y->m_size << ") = " << y->toHexString() << std::endl;
                        std::cout << "= R (" << r->m_size << ") = " << r->toHexString() << std::endl;
                        std::cout << "= e (" << r2.m_size << ") = " << r2.toHexString() << std::endl;
                        std::cout << "  x0 (" << x0.m_size << ") = " << x0.toHexString() << std::endl;
                        std::cout << "  x1 (" << x1.m_size << ") = " << x1.toHexString() << std::endl;
                        std::cout << "  y0 (" << y0.m_size << ") = " << y0.toHexString() << std::endl;
                        std::cout << "  y1 (" << y1.m_size << ") = " << y1.toHexString() << std::endl;
                        exit(-1);
                    }
                }
            }            
            

        }
        
void BigInteger::mult(BigInteger* y)
{
    BigInteger ref(*this);
    BigInteger::mult(this, &ref, y);
}
        
/**
 * Multiply two 32 bits integers and put the result on two integers (high part
 * and low part)
 * r = x * y
 * rhi = r[63..32]
 * rlo = r[31..0]
 *
 * from https://github.com/adamwalker/mmul/blob/master/crypto.c (mulhilo)
 *  
 * @param x
 * @param y
 * @param rHight
 * @param rLow
 */
void BigInteger::mult(unsigned int x, unsigned int y, unsigned int *rHight, unsigned int *rLow)
{
    unsigned long long res = (unsigned long long) x * (unsigned long long) y;
    *rLow = res;
    *rHight = res >> 32;
}

/**
 * As implemented in "Handbook of Applied Cryptography" algorithm 14.36
 * we compute r = x * y * R^(-1) mod m
 * 
 * b is the base of our number system (in our case 2^32)
 * n is the number of limbs
 * R = b ^n, the R , so that R > m
 * 
 * @param r         this is the return value, r = x*y*R^(−1) mod m.
 * @param x
 * @param y
 * @param m         modulo value
 * @param mprime    m prime is defined as m'= -m^(-1) mod b, where b is the base
 */
void BigInteger::multMontgomeryForm(BigInteger* r, BigInteger* x, BigInteger* y, BigInteger* m, BigInteger* mprime)
{
    assert(r->m_size >= (m->m_size-1));

    r->zero();

    BigInteger temp;
    BigInteger temp2;
    temp2.initSize(m->m_size+1);
    BigInteger temp3;
    temp3.initSize(temp2.m_size+r->m_size);
    BigInteger temp4;
    temp4.initSize(x->m_size+2);

    BigInteger bxi;
    bxi.initSize(1);
    temp.initSize(y->m_size+bxi.m_size);

    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  x:" << x->toHexString() << std::endl;
    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  y:" << y->toHexString() << std::endl;
    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  m:" << m->toHexString() << std::endl;
    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  mprime:" << mprime->toHexString() << std::endl;

    for (int i=0; i < r->m_size; i++)
    {
        // u[i] = (r[0] + x[i]*y[0]) * mprime mod b
        //      applying (i*j) mod n  = ((i mod n) * ( j mod n)) mod n ...
        // u[i] = ((r[0] + x[i]*y[0]) mod b) * (mprime mod b) mod b
        //      mprime mod b is mprime[0] because b is 2^32 (a limb)
        //      and applying  (i+j) mod n =  ((i mod n) + (j mod n)) mod n;
        // (a[0] + x[i]*y[0]) mod b = a[0] mod b + (x[i] * y[0]) mod b
        //      and because mod b is basically automatically done by using limbs...
        // r[i] = (a[0] + x[i] * y[0]) * mprime[0]
        unsigned int ui = (r->m_data[0] + x->m_data[i] * y->m_data[0]) * mprime->m_data[0];

        // R = (R + x[i]*y + u[i]*m)/b
        bxi.setIntValue(x->m_data[i]);

        BigInteger::mult(&temp, y, &bxi);         // temp = y*x[i]

        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  temp:" << temp.toHexString() << std::endl;
        
        BigInteger bui;
        bui.initSize(1);
        bui.setIntValue(ui);

        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  ui:" << bui.toHexString() << std::endl;
        
        BigInteger::mult(&temp2, m, &bui);        // temp2 = m*u[i]

        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  temp2:" << temp2.toHexString() << std::endl;
        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  r:" << r->toHexString() << std::endl;
        
        BigInteger::add(&temp3, r, &temp2);         // temp3 = r + m*u[i]
        
        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  temp3:" << temp3.toHexString() << std::endl;
        
        BigInteger::add(&temp4, &temp3, &temp);     // temp4 = r + m*u[i] + y*x[i]
        
        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  temp4:" << temp4.toHexString() << std::endl;
        
        //BigInteger::shiftRight(r, &temp4, 32);      // r = temp4 / b
        BigInteger::range(r, &temp4, r->m_size*32, 32);
        
        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  r:" << r->toHexString() << std::endl;
    }
}

/**
 * Inspired in https://github.com/adamwalker/mmul/blob/master/crypto.c
 * 
 * @param r
 * @param x
 * @param y
 * @param m
 * @param mprime
 */
void BigInteger::multMontgomeryForm2(BigInteger* r, BigInteger* x, BigInteger* y, BigInteger* m, BigInteger* mprime)
{
//void mmul(uint32_t *res, uint32_t *x, uint32_t *y, uint32_t *m, uint32_t *mprime)
//    uint32_t t[128], tm[64], tmm[128], u[128];
    int i;

    if (extraChecks)
    {
        assert(r != x);
        assert(r != y);
        assert(r != m);
        assert(r != mprime);
    }
    
    BigInteger t;
    t.initSize(x->m_size + y->m_size);
    BigInteger pretm;
    pretm.initSize(t.m_size);
    
    BigInteger tm;
    tm.initSize(m->m_size);
    BigInteger tmm;
    tmm.initSize(tm.m_size + m->m_size);
    
    BigInteger::mult(&t, x, y);
    BigInteger::multLow(&pretm, &t, mprime);  // was mult low
    tm.copy(&pretm);
    BigInteger::mult(&tmm, &tm, m);


    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY)
    {
        std::cout << "BigInteger::multMontgomeryForm2 t:" << t.toHexString() << std::endl;
        std::cout << "BigInteger::multMontgomeryForm2 mprime:" << mprime->toHexString() << std::endl;
        std::cout << "BigInteger::multMontgomeryForm2 pretm:" << pretm.toHexString() << std::endl;
        std::cout << "BigInteger::multMontgomeryForm2 tm:" << tm.toHexString() << std::endl;
        std::cout << "BigInteger::multMontgomeryForm2 tmm:" << tmm.toHexString() << std::endl;
    }

    BigInteger u;
    u.initSize(tmm.m_size);
    BigInteger::add(&u, &t, &tmm);


    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm2 u:" << u.toHexString() << std::endl;

    BigInteger::shiftRight(r, &u, x->m_size*32);


    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm2 r:" << r->toHexString() << std::endl;

    if (m->isLessThan(r))   // if (r >= m) r = r - m
    {
        r->subtract(m);

        if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm2 r:" << r->toHexString() << std::endl;

    }
}

/**
 * 
 * @param radix
 * @param m
 */        
void BigInteger::radixFromMontgomeryMod(BigInteger* radix, BigInteger* m)
{
    assert(radix->m_size > m->m_size);
    
    int mLen = m->m_size*32;

    radix->setIntValue(1);
    radix->shiftLeft(mLen);      
}


/**
 * Sets the integer to a random value (using all the limbs)
 */
void BigInteger::random()
{
    int lc = clock();
    while (lc == clock());
    
    srand(clock());

    
    for (int i=0; i < m_size; i++)
    {
        m_data[i] = rand();
        
        // Mingw random seems to return 16 bit numbers
        #ifdef __MINGW32__
            m_data[i] <<= 16;
            m_data[i] |= rand();
        #endif

        // Windows random seems to return 16 bit numbers also
        #ifdef WIN32
            m_data[i] <<= 16;
            m_data[i] |= rand();
        #endif
    }
}


/**
 * Creates a random number having n bits
 */
void BigInteger::random(int n)
{
    assert(n < getNumBits());
    
    random();
    int shift = getNumBits() - n;
    shiftRight(shift);
    
}

/**
 * 
 * @param up
 * @param down
 */
void BigInteger::range(int up, int down)
{
   BigInteger ref(*this);
   BigInteger::range(this, &ref, up, down);
}

/**
* 
* @param r
* @param x
* @param upper
* @param lower
* /
void BigInteger::range(BigInteger* r, BigInteger* x, int upper, int lower)
{
    if (extraChecks)
    {
        assert(upper <= x->getNumBits());
        assert(lower <= x->getNumBits());
        //assert((upper-lower) <= r->getNumBits());
    }
    
    r->zero();

   int maxSize = (r->m_size > x->m_size)? r->m_size : x->m_size;
   BigInteger temp, temp2;
   temp.initSize(maxSize);
   temp2.initSize(maxSize);

  int zl = temp.getNumBits() -1-(upper); 

    if (extraChecks)
        assert(zl >= 0);
  
  if (verbosity > VERBOSITY_LEVEL_RANGE) std::cout << "BigInteger.range x = " << x->toHexString() << std::endl;

  BigInteger::shiftLeft(&temp, x, zl);

  if (verbosity > VERBOSITY_LEVEL_RANGE) std::cout << "BigInteger.range x << " << to_string(zl) << " = " << temp.toHexString() << std::endl;

  BigInteger::shiftRight(&temp2, &temp, zl+lower);      // clear left bits

  if (verbosity > VERBOSITY_LEVEL_RANGE) std::cout << "BigInteger.range x >> " << to_string(zl+lower) << " = " << temp2.toHexString() << std::endl;

  r->copy(&temp2);

}*/

/**
 * Sets to zero the higher bits
 * if the number has less bits, ignore it 
 */
void BigInteger::zeroHighBits(BigInteger* r, int fromBit)
{
    int fromLimb = fromBit / 32;
    int fromLimbBit = fromBit % 32;
    
    if ((fromLimb == r->m_size) && (fromLimbBit == 0))
        return;
    
    if (fromLimb >= r->m_size)
        return;    
    
    int mask = ((1 << fromLimbBit)-1);
    
    r->m_data[fromLimb] = r->m_data[fromLimb] & mask;
    fromLimb++;
    
    while (fromLimb < r->m_size)
    {
        r->m_data[fromLimb] = 0;
        fromLimb++;
    }
}

/**
 * copy [upper, lower] bits from x to r
 * 
 * @param r return value
 * @param x original value
 * @param upper the index of the upper bit
 * @param lower the index of the lower bit (zero is the minimum value)
 */
void BigInteger::range(BigInteger* r, BigInteger* x, int upper, int lower)
{
    if (extraChecks)
    {
        assert(upper <= x->getNumBits());
        assert(lower <= x->getNumBits());
        assert(r->getNumBits() >= (upper-lower));
    }
 
    // we move 
    if (lower > 0)
        shiftRight(r, x, lower);
    else
        r->copy(x);
    
    zeroHighBits(r, upper-lower+1);
}


int BigInteger::maxVal( int x,  int y)
{
    return (x > y) ? x:y;
}
/**
 * Computes m' in the montgomery domain, this is a value such m * m' mod radix = -1 
 * 
 * To compute it we first compute 
 *      m' = radix - (m ^ -1) mod radix
 * 
 * If number is not invertible, then mprime is zero
 * 
 * @param mprime
 * @param m
 * @param radix
 */
void BigInteger::mprimeFromMontgomeryRadix(BigInteger* mprime, BigInteger* m, BigInteger* radix)
{
    BigInteger temp;
    temp.initSize(radix->m_size);
    temp.copy(m);
    BigInteger invTemp;
    invTemp.initSize(radix->m_size);
    BigInteger::inverseMod(&invTemp, &temp, radix);    // invTemp = (m mod radix) ^ (-1) mod radix
 
    if (invTemp.isZero())
    {
        mprime->zero();
    }
    else
    {
        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::mprimeFromMontgomeryRadix m = " << m->toHexString() << std::endl;
        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::mprimeFromMontgomeryRadix m^-1 = " << invTemp.toHexString() << std::endl;

        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::mprimeFromMontgomeryRadix radix = " << radix->toHexString() << std::endl;

        mprime->copy(radix);
        mprime->subtract(&invTemp);


        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::mprimeFromMontgomeryRadix mprime = " << mprime->toHexString() << std::endl;
    }
}

    
        /**
         * Multiply two numbers using the Karatsuba algorithm
         * @param r
         * @param x
         * @param y
         */
        void BigInteger::mult_karatsuba(BigInteger* r, BigInteger* x, BigInteger* y)
        {
            if (x->isZero() || y->isZero())
            {
                r->zero();
            }
            else
            {
                int maxSize = (x->m_size > y->m_size)? x->m_size : y->m_size;
                int halfSize = (maxSize+1) / 2;

                BigInteger x1, x0;
                BigInteger y1, y0;
                x1.initSize(halfSize);
                x0.initSize(halfSize);
                y1.initSize(halfSize);
                y0.initSize(halfSize);
                BigInteger::range(&x1, x, x->getNumBits()-1, halfSize*32);
                BigInteger::range(&x0, x, halfSize*32-1, 0);
                BigInteger::range(&y1, y, y->getNumBits()-1, halfSize*32);
                BigInteger::range(&y0, y, halfSize*32-1, 0);

                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  x " << x->toHexString() << std::endl;
                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  x1 " << x1.toHexString() << std::endl;
                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  x0 " << x0.toHexString() << std::endl;
                
                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  y " << y->toHexString() << std::endl;
                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  y1 " << y1.toHexString() << std::endl;
                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  y0 " << y0.toHexString() << std::endl;
                
                //const int m = numBits/2;

                BigInteger z0;
                BigInteger z2;
                z0.initSize(maxSize+1);
                z2.initSize(maxSize+1);

                BigInteger::mult(&z2, &x1, &y1);
                BigInteger::mult(&z0, &x0, &y0);

                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  z2=x1*y1 " << z2.toHexString() << std::endl;
                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  z0=x0*y0 " << z0.toHexString() << std::endl;
                

                BigInteger t1;
                t1.initSize(halfSize+1);

                BigInteger::add(&t1, &y1, &y0);

                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  t1=y1+y0 " << t1.toHexString() << std::endl;
                
                BigInteger t2;
                t2.initSize(halfSize+1);

                BigInteger::add(&t2, &x1, &x0);

                BigInteger z1;
                z1.initSize(t1.m_size + t2.m_size);

                BigInteger::mult(&z1, &t1, &t2);

                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  t2=x1+x0 " << t2.toHexString() << std::endl;
                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  z1=t1*t2 " << z1.toHexString() << std::endl;

                z1.subtract(&z2);
                
                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  z1=z1-z2 " << z1.toHexString() << std::endl;
                
                z1.subtract(&z0);

                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  z1=z1-z0 " << z1.toHexString() << std::endl;
                
                // r = z2 << hs*2  + z1 << hs + z0
                BigInteger z1z2;
                z1z2.initSize(maxVal(z1.m_size, z2.m_size+halfSize));
                z1z2.zero();
                r->zero();
                BigInteger::addShifted(&z1z2, &z1, &z2, halfSize*32);
                
                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  z1z2=z1+z2<< " << halfSize << "*32  " << z1z2.toHexString() << std::endl;
                
                BigInteger::addShifted(r, &z0, &z1z2, halfSize*32);
                
                if (verbosity > VERBOSITY_LEVEL_MULT_KARATSUBA) std::cout << "  r=z0+z1z2<<  " << halfSize << "*32  " << r->toHexString() << std::endl;
            }            
            
//            BigInteger r2(*r);
//            mult(&r2, x, y);
            
//            if (!isEqual(r, &r2))
//            {
//                std::cout << "  X (" << x->m_size << ") = " << x->toHexString() << std::endl;
//                std::cout << "* Y (" << y->m_size << ") = " << y->toHexString() << std::endl;
//                std::cout << "= R (" << r->m_size << ") = " << r->toHexString() << std::endl;
//                std::cout << "= e (" << r2.m_size << ") = " << r2.toHexString() << std::endl;
//                
//            }
        }
        
        /**
         * Computes this = this * b mod m
         * @param b
         * @param m
         */
        void BigInteger::multMod(BigInteger* b, BigInteger* m)
        {
            BigInteger ref(*this);
            BigInteger::multMod(this, &ref, b, m);
        }
        
void BigInteger::multMod(BigInteger* r, BigInteger* a, BigInteger* b, BigInteger* mod)
{
    BigInteger m;
    m.initSize(a->m_size + b->m_size);
    BigInteger q;
    q.initSize(a->m_size);

    mult(&m, a, b);
    div_naive(&m, mod, &q, r);
}
        
        
void BigInteger::parseString(const char* str)
{
    // multiply by x*10 is x*(8+2) -> 8*x + 2*x
    int l = strlen(str);

    zero();

    BigInteger digit;
    digit.initSize(1);

    // V = V*10+s[i];
    for (int i=0; i < l; i++)
    {
        //  x=x*10
        BigInteger ref(*this);
        ref.shiftLeft(1);
        shiftLeft(3);
        add(&ref);

//                std::cout << toString() << " + " << str[i] << " = "; 

        digit.setIntValue(str[i]-'0');
        add(&digit);

//                std::cout << toString() << std::endl;
    }
}
        
/**
 * Parses an hexadecimal string
 * @param str
 */
void BigInteger::parseHexString(const char* str)
{
    // multiply by x*10 is x*(8+2) -> 8*x + 2*x
    int l = strlen(str);

    zero();

    BigInteger digit;
    digit.initSize(1);

    // V = V*10+s[i];
    for (int i=0; i < l; i++)
    {
        //  x=x*10
        shiftLeft(4); // *16


        if ((str[i] >= '0') && (str[i] <= '9'))
            digit.setIntValue(str[i]-'0');
        else if ((str[i] >= 'A') && (str[i] <= 'F'))
            digit.setIntValue(10 + (str[i]-'A'));
        else
            digit.setIntValue(10 + (str[i]-'a'));

        if (verbosity > 10)
            std::cout << toHexString() << " + " << digit.toHexString() << " = "; 

        add(&digit);

        if (verbosity > 10)
            std::cout << toHexString() << std::endl;
    }
}
        
        
        /**
         * 
         * @param r
         * @param x
         * @param power
         * @param mod
         */
        void BigInteger::powerMod(BigInteger* r, BigInteger* v, BigInteger* power, BigInteger* mod)
        {
            if (extraChecks)
            {
                assert(r != v);
                assert(r != power);
                assert(r != mod);
            }
            
            BigInteger x(*v);
            BigInteger y(*power);

            // we use a simple square-and-multiply algorithm,
            if (verbosity > VERBOSITY_LEVEL_POWER_MOD) std::cout << "BigInteger::powerMod v " << v->toHexString() << std::endl;
            if (verbosity > VERBOSITY_LEVEL_POWER_MOD) std::cout << "BigInteger::powerMod power " << power->toHexString() << std::endl;
            if (verbosity > VERBOSITY_LEVEL_POWER_MOD) std::cout << "BigInteger::powerMod mod " << mod->toHexString() << std::endl;
        
            r->setIntValue(1);

            while(!y.isZero())
            {
                    if (y.isOdd())
                        r->multMod(&x, mod);

                    y.shiftRight(1);    // / 2
                    x.squareMod(mod); 
            }
            
            if (verbosity > VERBOSITY_LEVEL_POWER_MOD) std::cout << "BigInteger::powerMod r " << r->toHexString() << std::endl;
	}
        
        /**
         * As implemented in "Handbook of Applied Cryptography" algorithm 14.94
         * we compute r = x ^ e mod m
         * @param r
         * @param v
         * @param mod
         */
        void BigInteger::powerModMontgomery(BigInteger* r, BigInteger* x, BigInteger* e, BigInteger* m, BigInteger* mprime, BigInteger* radix)
        {
            if (extraChecks)
            {
                assert(!m->isZero());
                assert(!mprime->isZero());
                assert(r != x);
                assert(r != e);
                assert(r != m);
                assert(r != mprime);
                assert(r != radix);
            }
            BigInteger radix2;
            radix2.initSize(m->m_size);
            squareMod(&radix2, radix, m);

            if (verbosity > VERBOSITY_LEVEL_POWER_MOD)
                std::cout << "BigInteger::powerModMontgomery radix: " << radix->toHexString() << std::endl;
            if (verbosity > VERBOSITY_LEVEL_POWER_MOD)
                std::cout << "BigInteger::powerModMontgomery m: " << m->toHexString() << std::endl;
            if (verbosity > VERBOSITY_LEVEL_POWER_MOD)
                std::cout << "BigInteger::powerModMontgomery radix ^2 mod m: " << radix2.toHexString() << std::endl;
            
            BigInteger xprime;
            xprime.initSize(m->m_size);
            
            multMontgomeryForm2(&xprime, x, &radix2, m, mprime);
            
            BigInteger a(*radix);
            a.mod(m);
            a.reduceWorkingSize(m->m_size);
            
            int t = e->getLength();
            
            for (int i=t-1; i >= 0; i--)
            {
                int ei = e->getBit(i);
                
                BigInteger temp(a);
                multMontgomeryForm2(&a, &temp, &temp, m, mprime);
                
                if (ei)
                {
                    temp.copy(&a);
                    multMontgomeryForm2(&a, &temp, &xprime, m, mprime);
                }   
            }
            
            BigInteger one;
            one.initSize(1);
            one.setIntValue(1);
            
            multMontgomeryForm2(r, &a, &one, m, mprime);
        }
        

        /**
         * Reduce the working size of the integer (without reallocating memory)
         * @param s new size
         */
        void BigInteger::reduceWorkingSize(int s)
        {
            assert(m_size >= s);
            for (int i=s; i < m_size; i++)
                assert(m_data[i] == 0);
            
            m_size = s;
        }

/**
 * 
 * @param r
 * @param x
 * @param y
 */
void BigInteger::subtract(BigInteger* r, BigInteger* x, BigInteger* y)
{
    unsigned int* px = x->m_data;
    unsigned int* py = y->m_data;
    unsigned int* pr = r->m_data;

    unsigned int carryIn = 0;
    unsigned int carryOut = 0;

    if (extraChecks)
    {
        assert(r != x);
        assert(r != y);
    }
    
    for (int i=0; i < x->m_size; i++)
    {
        unsigned int sum = 0;

        if (i < x->m_size) sum += px[i];
        if (i < y->m_size) sum -= py[i];

        if (sum > px[i])
        {
            if (carryIn) sum--;
            carryOut = 1;
        }
        else if (carryIn)
        {
            carryOut = (sum == 0)? 1: 0;
            sum--;
        }

        carryIn = carryOut;
        pr[i] = sum;
    }
}

/**
 * Computes r = v^2 mod m
 * 
 * @param r
 * @param v
 * @param mod
 */
void BigInteger::squareMod(BigInteger* r, BigInteger* v, BigInteger* m)
{
    if (extraChecks) 
    {
        assert(r != v);
        assert(r != m);
    }

    multMod(r, v, v, m);
}
        
        void BigInteger::squareMod(BigInteger* m)
        {
            BigInteger ref(*this);
            squareMod(this, &ref, m);
        }
        
void BigInteger::shiftLeft(int bits)
{
    BigInteger ref(*this);
    BigInteger::shiftLeft(this, &ref, bits );
}


/**
 * 
 * @param r
 * @param a
 * @param sv
 */
void BigInteger::shiftLeft(BigInteger* r, BigInteger* a, int sv)
{
    unsigned int* pa = a->m_data;
    unsigned int* pr = r->m_data;
    
    unsigned int carry = 0;
        
    if (extraChecks)
        assert(r != a);
    
    // 
    unsigned int limbsShifted = sv / 32;
    
    if (limbsShifted)    // (sv == 32)
    {
        int slots = limbsShifted;

        for (int i=r->m_size-1; i >= slots; i--)
        {
            pr[i] = pa[i-slots];
        }
        for (int i=slots-1; i >= 0; i--)
             pr[i] = 0;
        
        sv -= limbsShifted * 32;
    }
    else
        if (r != a)
            r->copy(a);
    
    if (sv != 0)
    {
        int cs = 32-sv;

        for (int i=0; i < r->m_size; i++)
        {
            unsigned int nv = (pr[i] << sv) | carry;

            carry = ~((0xFFFFFFFF << sv) >> sv);
            carry &= pr[i];
            carry = carry >> cs;

            pr[i] = nv;
        }
    }
}

void BigInteger::shiftRight(int bits)
{
    BigInteger ref(*this);
    BigInteger::shiftRight(this, &ref, bits );
}
        
        void BigInteger::subtract(BigInteger* y)
        {
            BigInteger ref(*this);
            BigInteger::subtract(this, &ref, y);
        }
        
        /**
         * Compute r = (r - x ) mod m
         * If r < x we add the m until it is bigger
         * @param x
         * @param mod
         */
        void BigInteger::subtractMod(BigInteger* x, BigInteger* m)
        {
            assert(m_size >= m->m_size);
            assert(m_size >= x->m_size);
            
            while (isLessThan(x))
            {
//                std::cout << " submod: r = r - x, and r < x!!" << std::endl;
//                std::cout << " r: " << this->toHexString() << std::endl;
//                std::cout << " x: " << x->toHexString() << std::endl;
//                std::cout << " m: " << m->toHexString() << std::endl;
                add(m);
//                std::cout << " r+m: " << this->toHexString() << std::endl;
            }
            
            subtract(x);
        }


/**
 * Shift right a number of bits, zeros are inserted at the left
 * First we move slots (limb multiples)
 * 
 *  For instance if slots is 2
 * 
 *  ... a[6] a[5] a[4] a[3] a[2]
 *  ... r[4] r[3] r[2] r[1] r[0]
 * 
 * @param r retsult r = a >> sv
 * @param a
 * @param sv    number of bits to shift to the right
 */
void BigInteger::shiftRight(BigInteger* r, BigInteger* a, int sv)
{
    if (extraChecks)
    {
        assert(r != a);
        // check that the result number can hold the resulting value
//        int targetLen = r->m_size*32;
//        int reqLen = a->getLength() - sv;
//        assert(targetLen >= reqLen);
    }

    unsigned int* pa = a->m_data;
    unsigned int* pr = r->m_data;
    unsigned int carry = 0;

    unsigned int limbsShifted = sv / 32;
    unsigned int limbBitsShifted = sv % 32;
    unsigned int cs = 32 - limbBitsShifted;
    
    for (int i=r->m_size; i >= 0; i--)
    {
        if (limbsShifted+i >= a->m_size)
        {
            if (i < r->m_size)
                pr[i] = 0;
            continue;
        }
        
        unsigned int nv = (pa[limbsShifted+i] >> limbBitsShifted) | carry;

        carry = ~((0xFFFFFFFF >> limbBitsShifted) << limbBitsShifted);
        carry &= pa[limbsShifted+i];
        carry = carry << cs;

        if (i < r->m_size)
            pr[i] = nv;
    }
}
    

        std::string BigInteger::toHexString()
        {
            std::string s = "";
            
            for (int i=0; i < m_size; i++)
            {
                char buf[10];
                sprintf(buf, "%08X ", m_data[i]);
                s = s + std::string(buf);
            }
            
            return s;
        }
        
        std::string BigInteger::toString()
        {
            if (isNegative())
            {
                BigInteger c2;
                c2.initSize(m_size+1);
                c2.setIntValue(1); 
                c2.shiftLeft(getNumBits());
                c2.subtract(this);
                return "-" + c2.toString();
            }
            
            BigInteger ref(*this);
            BigInteger divisor;
            divisor.initSize(m_size);
            divisor.setIntValue(10);
            std::string s = "";
            
            if (ref.isZero())
                s = "0";
            
            BigInteger r;
            BigInteger q;
            r.initSize(m_size);
            q.initSize(m_size);

            while (!ref.isZero())
            {
                //BigNum<numBits> d(ref);
                //d.mod(divisor);
                
                BigInteger::div_naive(&ref, &divisor, &q, &r);
                
                s = to_string(r.m_data[0]%10) + s;
                
                ref.copy(&q);
                //ref.div(divisor);
            }
            
            return s;
        }
        
        /**
         * Converts the number (n) to its representation in the Montgomery residue system
         * 
         *  nprime = n * R mod m
         * 
         * @param r
         * @param n
         * @param R
         * @param m
         */
        void BigInteger::toMontgomeryDomain(BigInteger* nprime, BigInteger* n, BigInteger* R, BigInteger* m)
        {
           BigInteger temp; 
           temp.initSize(n->m_size+R->m_size);
           BigInteger::mult(&temp, n, R);
           
           BigInteger q; 
           q.initSize(nprime->m_size);
           BigInteger::div_naive(&temp, m, &q, nprime);
        }
        
        /**
         * Converts back a number from the Montgomery domain into the normal domain
         * 
         *  n = nprime * R^(-1) mod m
         * 
         * @param n
         * @param nprime
         * @param Rinv
         * @param m
         */
        void BigInteger::fromMontgomeryDomain(BigInteger* n, BigInteger* nprime, BigInteger* Rinv, BigInteger* m)
        {
            BigInteger temp; 
            temp.initSize(nprime->m_size+Rinv->m_size);
            BigInteger::mult(&temp, nprime, Rinv);
           
            if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::fromMontgomeryDomain temp:" << temp.toHexString() << std::endl;
            if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::fromMontgomeryDomain m:" << m->toHexString() << std::endl;
            
            BigInteger q; 
            q.initSize(n->m_size);
            BigInteger::div_naive(&temp, m, &q, n);
            
            if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::fromMontgomeryDomain q:" << q.toHexString() << std::endl;
            if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::fromMontgomeryDomain n:" << n->toHexString() << std::endl;
        }
        
        
        
        /**
         * Compute the inverse modulo fr = x^(-1) mod m
         * so that fr * x mod m = 1
         * 
         * 
         * from https://rosettacode.org/wiki/Modular_inverse
         * 
         * If the number is non invertible returns zero
         * 
         * @param fr    result
         * @param x     number we are computing the inverse
         * @param n     modulo   
         */
        void BigInteger::inverseMod(BigInteger* ret, BigInteger* a, BigInteger* b)
        {
            if (extraChecks)
            {
                //assert(a->isLessThan(n));
                //assert(n->m_size == fr->m_size);
                //assert(a->m_size == fr->m_size);
            }
            
            int maxSize = ret->m_size;
            if (a->m_size > maxSize) maxSize = a->m_size;
            if (b->m_size > maxSize) maxSize = b->m_size;
            
            
            // int t, nt, r, nr, q, tmp;
            // if (b < 0) b = -b;   // not necessary, always positive
            // if (a < 0) a = b - (-a % b); // not necessary, always positive
        
            BigInteger t; t.initSize(maxSize);
            BigInteger nt; nt.initSize(maxSize);
            BigInteger r; r.initSize(maxSize);
            BigInteger nr; nr.initSize(maxSize);
            BigInteger q; q.initSize(maxSize);
            BigInteger tmp; tmp.initSize(maxSize);
            BigInteger p; p.initSize(maxSize*2);
            
            t.zero();                       // t = 0;  
            nt.setIntValue(1);              // nt = 1;  
            r.copy(b);                      // r = b;  
            BigInteger::div_naive(a, b, &tmp, &nr);   // nr = a % b;
            
            while (!nr.isZero())            // while (nr != 0) 
            {
//                std::cerr << "r: " << r.toHexString() << std::endl;
//                std::cerr << "nr: " << nr.toHexString() << std::endl;
//                std::cerr << "t: " << t.toHexString() << std::endl;
//                std::cerr << "nt: " << nt.toHexString() << std::endl;
                
                BigInteger::div_naive(&r, &nr, &q, &tmp);   // q = r/nr;
                tmp.copy(&nt);                              // tmp = nt;
                BigInteger::mult(&p, &q, &nt);
                nt.copy(&t);
                nt.subtract(&p);                            // nt = t - q*nt;
                t.copy(&tmp);                               // t = tmp;
                
//                std::cerr << "r: " << r.toHexString() << std::endl;
//                std::cerr << "nr: " << nr.toHexString() << std::endl;
//                std::cerr << "t: " << t.toHexString() << std::endl;
//                std::cerr << "nt: " << nt.toHexString() << std::endl;
                
                tmp.copy(&nr);                              // tmp = nr; 
                BigInteger::mult(&p, &q, &nr);
                nr.copy(&r);
                nr.subtract(&p);                            // nr = r - q*nr;
                r.copy(&tmp);
            }
            
            BigInteger one;
            one.initSize(1);
            one.setIntValue(1);
            
            if (one.isLessThan(&r))     // if (r > 1) return -1;  / * No inverse * /
            {
                if (verbosity > VERBOSITY_LEVEL_INV_MOD)
                {
                    std::cerr << "BigInteger::inverseMod Number not invertible " << std::endl;
                    std::cerr << "BigInteger::inverseMod a: " << a->toHexString() << std::endl;
                    std::cerr << "BigInteger::inverseMod b: " << b->toHexString() << std::endl;
                    std::cerr << "BigInteger::inverseMod r: " << r.toHexString() << std::endl;
                }
                //assert(false);
                t.zero();
            }
            else if (t.isNegative()) // if (t < 0)
            {
                t.add(b);       //  t += b;
                
                if (verbosity > VERBOSITY_LEVEL_INV_MOD) std::cerr << "BigInteger::inverseMod t: " << t.toHexString() << std::endl;
            }
            
            ret->copy(&t);
        }
        
                
void BigInteger::zero()
{
    for (int i=0; i < m_size; i++)
        m_data[i] = 0;    
}


#define SLIDING_WINDOW_SIZE 5

/**
 *  Sliding Window Exponentiation 
 * @from https://github.com/an4/Applied-Security-Mod/blob/master/modmul.c
 */
int BigInteger::binary_to_decimal(int start, int end) 
{
    int i;
    int result = 0;
    int g = 1;
    for(i = end; i<=start; i++) 
    {
        int bit = getBit(i);; // mpz_tstbit(input, i);
        if(bit) 
        {
            result += g * bit;
        }
        g <<= 1;
    }
    return result;
}

/**
 * Computes the modular exponentiation using the sliding window algorithm, which
 * precomputes a table of products to reuse them
 * 
 * Inspired in https://github.com/an4/Applied-Security-Mod/blob/master/modmul.c
 *
 * @param r return value
 * @param v
 * @param exp
 * @param m
 */
void BigInteger::powerModSlidingWindow(BigInteger* r, BigInteger* v, BigInteger* exp, BigInteger* m)
{
    const int table_length = 1 << (SLIDING_WINDOW_SIZE - 1);
    BigInteger table[table_length];
    BigInteger square_base;
    BigInteger temp;
    int i;
    int exp_length;
    int l;
    // value in window
    int u;

    temp.initSize(r->m_size);
    
    //mpz_init(temp);
    //mpz_init(square_base);

    // T[0] = x
    table[0].initSize(v->m_size);
    table[0].copy(v);
    
    //mpz_init_set(table[0], base);
    
    // x^2 mod N
    square_base.initSize(m->m_size);
    square_base.copy(v);
    square_base.squareMod(m);
    
    //mpz_mul(square_base, base, base);
    //mpz_mod(square_base, square_base, modulus);
    
    // Compute lookup table.
    for(i = 1; i < table_length; i++) 
    {
        // table[i] = table[i-1] * v ^ 2 mod m
        table[i].initSize(m->m_size);
        BigInteger::multMod(&table[i], &table[i-1], &square_base, m);
//        mpz_init(table[i]);
//        mpz_mul(table[i], table[i-1], square_base);
//        mpz_mod(table[i], table[i], modulus);
    }
    
    // t = 1
    r->setIntValue(1);
    //mpz_init_set_ui(output, 1);
    
    // |y|
    exp_length = exp->getNumBits();
    //exp_length = mpz_size(exp) * mp_bits_per_limb;
    
    // |y| - 1
    i = exp_length - 1;
    while(i >= 0) 
    {
        if(! exp->getBit(i))    // mpz_tstbit(exp, i)) 
        {
            l = i;
            u = 0;
        }
        else
        {
            l = ((i - SLIDING_WINDOW_SIZE + 1) > 0) ? (i - SLIDING_WINDOW_SIZE + 1) : 0;

            while(! exp->getBit(l)) // mpz_tstbit(exp,l)) 
            {
                l++;
            }
            // Set u = exp bits between i and l
            u = exp->binary_to_decimal(i, l);
        }

        // t' = t
        temp.copy(r);
        
        // t^p
        int p = 1 << (i - l + 1);
        if(p>0) 
        {
            // t = t'^p (mod N)
            BigInteger bip;
            bip.initSize(1);
            bip.setIntValue(p);
            
            BigInteger::powerMod(r, &temp, &bip, m);
            //mpz_powm_ui(output, temp, p, modulus);
        }
        
        if(u != 0) 
        {
            // t = t * T[(u-1)/2] mod N
            
            r->multMod(&table[(u-1)/2], m);
            //mpz_mul(output, output, table[(u-1)/2]);
            //mpz_mod(output, output, modulus);
        }
        i = l - 1;
    }

//    mpz_clear(square_base);
//    mpz_clear(temp);
//    for(i=0 ; i<table_length; i++) {
//        mpz_clear(table[i]);
//    }
}
