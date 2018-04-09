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
#include "../BigInteger.h"

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

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
 * from https://github.com/adamwalker/mmul/blob/master/crypto.c (mulhilo)
 */        
void BigInteger::mult(BigInteger* r, BigInteger* a, BigInteger* b)
{
    if (extraChecks) 
    {
        assert(r->getNumBits() >= (a->getLength() + b->getLength()));
    }
    
    r->zero();

    int asize = a->getLimbLength();
    int bsize = b->getLimbLength();
    int rsize = r->m_size;
    uint64_t carry;
    uint64_t accum;
    unsigned int adata;
    int idx;
    uint32_t lo, hi;
    
    for (int i=0; i< asize; i++)
    {
        carry = 0;
        adata = a->m_data[i];
        
        for (int j=0; j<bsize; j++)
        {
            idx = i + j;

            BigInteger::mult(adata, b->m_data[j], &hi, &lo);

            accum = r->m_data[idx] + carry + lo;
            
            carry = (accum >> 32);
            carry += hi;
            
            r->m_data[idx] = accum;
        }
        
        if ((i + bsize)< rsize)
            r->m_data[i + bsize] = carry;
    }
}    

void BigInteger::mult(unsigned int digit)
{
    BigInteger ref(*this);
    BigInteger::mult(this, &ref, digit);
}

void BigInteger::mult(BigInteger* r, BigInteger* a, unsigned int digit)
{
    if (extraChecks) 
    {
        assert(r->m_size >= (a->getLimbLength() + 1));
    }
    
    r->zero();

    int asize = a->getLimbLength();
    int bsize = 1; // b->m_size;
    int rsize = r->m_size;
    uint64_t carry;
    uint64_t accum;
    unsigned int adata;
    int idx;
    uint32_t lo, hi;
    
    for (int i=0; i< asize; i++)
    {
        carry = 0;
        adata = a->m_data[i];
        
        //for (int j=0; j<bsize; j++)
        {
            idx = i; // + j;

            BigInteger::mult(adata, digit, &hi, &lo);

            accum = r->m_data[idx] + carry + lo;
            
            carry = (accum >> 32);
            carry += hi;
            
            r->m_data[idx] = accum;
            
            
        }
        
        if ((i + bsize)< rsize)
            r->m_data[i + 1 /*b->m_size*/ ] = carry;
    }
}


        
/**
 * from https://github.com/adamwalker/mmul/blob/master/crypto.c (big_mul_low)
 */        
void BigInteger::multLow(BigInteger* r, BigInteger* a, BigInteger* b)
{
    if (extraChecks) 
    {
        assert(r != a);
        assert(r != b);
    }

    int i, j;

    r->zero();

    int atop = a->getLimbLength();
    int btop = b->getLimbLength();
    int rsize = r->m_size;
    
    uint32_t lo, hi;
    uint64_t accum;
    
    for(i=0; i<atop; i++)
    {
        uint64_t carry = 0;
        for(j=0; j<btop; j++)
        {
            int idx = i + j;
            if (idx < rsize)
            {
                mult(a->m_data[i], b->m_data[j], &hi, &lo);

                accum  = r->m_data[idx] + carry + lo;
                carry  = (accum >> 32) + hi;
                r->m_data[idx] = accum;
            }
        }
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