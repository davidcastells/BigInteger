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
}

void BigInteger::zero()
{
    for (int i=0; i < m_size; i++)
        m_data[i] = 0;    
}

void BigInteger::setIntValue(int v)
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
    if (m_data == NULL)
        delete m_data;
    
    m_data = NULL;
}

void BigInteger::add(BigInteger* r, BigInteger* a, BigInteger* b)
{
    unsigned int carryIn = 0;
    unsigned int carryOut = 0;
            
    unsigned int* px = a->m_data;
    unsigned int* py = b->m_data;
    unsigned int* pr = r->m_data;

            
    for (int i=0; i < r->m_size; i++)
    {
        unsigned int sum = 0;

        if (i < a->m_size) sum += px[i];
        if (i < b->m_size) sum += py[i];

        if (sum < px[i])
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
 * @param shift
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

        if (sum < px[ia])
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
 *
 */
    void BigInteger::shiftRight(BigInteger* r, BigInteger* a, int sv)
    {
        unsigned int* pa = a->m_data;
        unsigned int* pr = r->m_data;
        unsigned int carry = 0;

        unsigned int limbsShifted = sv / 32;

        if (limbsShifted)
        {
            int slots = sv / 32;
			unsigned int val;
            for (int i=0; i < r->m_size-slots; i++)
            {
				(i + slots < a->m_size) ? val = pa[i + slots] : 0;
                pr[i] = val;
            }

            for (int i=slots; i > 0; i--)
                pr[r->m_size-i] = 0;
            
            sv -= limbsShifted * 32;
        }
        else 
            if (r != a)
                r->copy(a);
            
        int cs = 32-sv;

        for (int i=r->m_size-1; i >= 0; i--)
        {
            unsigned int nv = (pr[i] >> sv) | carry;

            carry = ~((0xFFFFFFFF >> sv) << sv);
            carry &= pr[i];
            carry = carry << cs;

            pr[i] = nv;
        }
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
            
//            cout << "[JNI] getLength greaterActiveLimb = " << greaterActiveLimb << endl;
            
            // find the highest bit 
            unsigned int test = m_data[greaterActiveLimb];
            int numActiveBits = 0;
            
//            cout << "[JNI] test = " << test << endl;
            
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
        
        int BigInteger::isLessThan(BigInteger* v)
        {
            unsigned int* p = m_data;
            unsigned int* pv = v->m_data;
            
            for (int i=m_size-1; i >= 0; i--)
                if (i >= v->m_size)
                {
                    if (p[i] != 0)
                        return false;
                }
                else if (p[i] < pv[i])
                    return true;
                else if (p[i] > pv[i])
                    return false;
                
            return false;
        }
        
        void BigInteger::range(int up, int down)
        {
            BigInteger ref(*this);
            BigInteger::range(this, &ref, up, down);
        }
        
        void BigInteger::range(BigInteger* r, BigInteger* x, int upper, int lower)
        {
            
           int zl = x->getNumBits() -1-(upper+1);
           BigInteger::shiftLeft(r, x, zl);
           BigInteger::shiftRight(r, r, zl+lower);      // clear left bits
           
        }
        
        int BigInteger::getBit(int bitnum)
        {
            int limbIndex = bitnum / 32;
            int bitIndex = bitnum % 32;
            
            return (m_data[limbIndex] >> bitIndex) & 1;
        }
        
        void BigInteger::shiftLeft(int bits)
        {
            BigInteger ref(*this);
            BigInteger::shiftLeft(this, &ref, bits );
        }
        
        void BigInteger::subtract(BigInteger* y)
        {
            BigInteger ref(*this);
            BigInteger::subtract(this, &ref, y);
        }
        
        void BigInteger::div_naive(BigInteger* x,
                BigInteger* y, 
                BigInteger* nq, BigInteger* nr)
        {
            BigInteger ref(*x);
            
            BigInteger divisor(*y);
            BigInteger q;
            BigInteger r;


            if (verbosity) std::cout << "BigInteger.div x = " << ref.toHexString() << std::endl;
            if (verbosity) std::cout << "BigInteger.div /   " << divisor.toHexString() << std::endl;

            q.initSize(x->m_size);
            r.initSize(x->m_size);
            q.zero();
            
            if (x->isLessThan(y))
            {
                nr->copy(x);
                nq->copy(&q);
                return;
            }
            
            
            // get the length of y
            int yl = y->getLength();
            int rl = ref.getLength();
            
            int downBit = rl-yl;
            
            //if (downBit < 0) assert(false);
            
            // get the yl most significant bits of ref
            r.copy(&ref);
            r.range(rl-1, downBit--);
            
            if (verbosity) std::cout << "BigInteger.div r range(" << to_string(rl-1) << "," << to_string(downBit+1) << ") = " << r.toHexString() << std::endl;

            if (r.isLessThan(&divisor))
            {
                if (verbosity) std::cout << "BigInteger.div r < divisor" << std::endl;

                // take another bit from ref
                r.copy(&ref);
                r.range(rl-1, downBit--);

                if (verbosity) std::cout << "BigInteger.div r range(" << to_string(rl-1) << "," << to_string(downBit+1) << ") = " << r.toHexString() << std::endl;
            }
            
            if (verbosity) std::cout << "BigInteger.div term = " << r.toHexString() << std::endl;
            if (verbosity) std::cout << "BigInteger.div      - " << divisor.toHexString() << std::endl;
            
            q.inc();
            r.subtract(&divisor);

            if (verbosity) std::cout << "BigInteger.div      = " << r.toHexString() << std::endl;
            if (verbosity) std::cout << "BigInteger.div    q = " << q.toHexString() << std::endl;
            
            // take another bit
            while (downBit >= 0)
            {
                r.shiftLeft(1);
                if (ref.getBit(downBit--))
                    r.inc();

                if (verbosity) std::cout << "BigInteger.div term = " << r.toHexString() << std::endl;

                if (r.isLessThan(&divisor))
                {
                    q.shiftLeft(1);    // put a zero in q
                    //goto loop;
                    
                    if (verbosity) std::cout << "BigInteger.div  0 q = " << q.toHexString() << std::endl;
                }
                else
                {
                    q.shiftLeft(1);     // put a one in q
                    q.inc();

                if (verbosity) std::cout << "BigInteger.div  1 q = " << q.toHexString() << std::endl;
      
                if (verbosity) std::cout << "BigInteger.div term = " << r.toHexString() << std::endl;
                if (verbosity) std::cout << "BigInteger.div      - " << divisor.toHexString() << std::endl;

                    r.subtract(&divisor);
                    //goto loop;
                    
                if (verbosity) std::cout << "BigInteger.div      = " << r.toHexString() << std::endl;
        
                }
            } 
            
            
            nr->copy(&r);
            nq->copy(&q);
        }
        
        
#ifdef MACRO_INLINING
    #define __kernel
    #define __global

    #define KERNEL_NUM_LIMBS	(2048/32)

    int get_global_id(int x)        { return 0;}
        
        /**
         * 
         * @param x
         * @param divisor
         * @param q
         * @param nr
         */
/*        void BigInteger::div_naive(BigInteger* x,
                BigInteger* divisor, 
                BigInteger* q, BigInteger* r)*/
        
        __kernel void BigInteger::kernel_div_naive(__global unsigned int* x, __global unsigned int* y, __global unsigned int* nq, __global unsigned int* nr)                
        {
        	int numIndex = get_global_id(0);
        	const int numLimbs = KERNEL_NUM_LIMBS;
        	int base = numIndex * numLimbs;
	

        	unsigned int ref[KERNEL_NUM_LIMBS];
        	KERNEL_BIG_INTEGER_COPY(ref, 0, numLimbs, x, base, numLimbs);
            
            unsigned int divisor[KERNEL_NUM_LIMBS];
            KERNEL_BIG_INTEGER_COPY(divisor, 0, numLimbs, y, base, numLimbs);
            //BigInteger q;
            //BigInteger r;
            unsigned int q[KERNEL_NUM_LIMBS];
            unsigned int r[KERNEL_NUM_LIMBS];

            //q.initSize(x->m_size);
            //r.initSize(x->m_size);
            KERNEL_BIG_INTEGER_ZERO(q, 0, numLimbs);
            
            int temp;
            KERNEL_BIG_INTEGER_IS_LESS_THAN(x, base, numLimbs, y, base, numLimbs, temp); 
            if (temp)
            //if (x->isLessThan(y))
            {
                KERNEL_BIG_INTEGER_COPY(nr, base, numLimbs, x, base, numLimbs);
                KERNEL_BIG_INTEGER_COPY(nq, base, numLimbs, q, 0, numLimbs);
                return;
            }
            
            
            // get the length of y
            int yl; // = y->getLength();
            int rl; //= ref.getLength();
            KERNEL_BIG_INTEGER_GET_LENGTH(y, base, numLimbs, yl);
            KERNEL_BIG_INTEGER_GET_LENGTH(ref, 0, numLimbs, rl);
            
            int downBit = rl-yl;
            
            //if (downBit < 0) assert(false);
            
            // get the yl most significant bits of ref
            //r = ref;
            //r.range(rl-1, downBit--);
            KERNEL_BIG_INTEGER_COPY(r, 0, numLimbs, ref, 0, numLimbs);
            KERNEL_BIG_INTEGER_RANGE1(r, 0, numLimbs, (rl-1), downBit);
            downBit--;
            
            KERNEL_BIG_INTEGER_IS_LESS_THAN(r, 0, numLimbs, divisor, 0, numLimbs, temp);
            if (temp)
            //if (r.isLessThan(&divisor))
            {
                // take another bit from ref
                //r = ref;
                //r.range(rl-1, downBit--);
                KERNEL_BIG_INTEGER_COPY(r, 0, numLimbs, ref, 0, numLimbs);
                KERNEL_BIG_INTEGER_RANGE1(r, 0, numLimbs, (rl-1), downBit);downBit--; 
            }
            
//            cout << " term = " << term.toString() << endl;
//            cout << "      - " << divisor.toString() << endl;
            
            //q.inc();
            //r.subtract(&divisor);
            KERNEL_BIG_INTEGER_INC(q, 0, numLimbs);
            KERNEL_BIG_INTEGER_SUBTRACT1(r, 0, numLimbs, divisor, 0, numLimbs);

//            cout << "      = " << term.toString() << endl;
//            cout << "    q = " << q.toString() << endl;
            
            // take another bit
            while (downBit >= 0)
            {
                //r.shiftLeft(1);
                KERNEL_BIG_INTEGER_SHIFT_LEFT1(r, 0, numLimbs, 1);
                int getBit;
                KERNEL_BIG_INTEGER_GET_BIT(ref, 0, numLimbs, downBit, getBit); downBit--;
                if (getBit)
                //if (ref.getBit(downBit--))
                {
                    //r.inc();
                    KERNEL_BIG_INTEGER_INC(r, 0, numLimbs);
                }

//                cout << " term = " << term.toString() << endl;
				
				KERNEL_BIG_INTEGER_IS_LESS_THAN(r, 0, numLimbs, divisor, 0, numLimbs, temp);
				if (temp)
                //if (r.isLessThan(&divisor))
                {
                    //q.shiftLeft(1);    // put a zero in q
                    KERNEL_BIG_INTEGER_SHIFT_LEFT1(q, 0, numLimbs, 1);
                    //goto loop;
                    
//                    cout << "  0 q = " << q.toString() << endl;

                }
                else
                {
                    //q.shiftLeft(1);     // put a one in q
                    //q.inc();
                    KERNEL_BIG_INTEGER_SHIFT_LEFT1(q, 0, numLimbs, 1);
                    KERNEL_BIG_INTEGER_INC(q, 0, numLimbs);

//                    cout << "  1 q = " << q.toString() << endl;
//                    
//                    cout << " term = " << term.toString() << endl;
//                    cout << "      - " << divisor.toString() << endl;

                    //r.subtract(&divisor);
                    KERNEL_BIG_INTEGER_SUBTRACT1(r, 0, numLimbs, divisor, 0, numLimbs);
                    //goto loop;
                    
//                    cout << "      = " << term.toString() << endl;
        
                }
            } 
            

            KERNEL_BIG_INTEGER_COPY(nr, base, numLimbs, r, 0, numLimbs);
            KERNEL_BIG_INTEGER_COPY(nq, base, numLimbs, q, 0, numLimbs);
        }

#endif        
        
        int BigInteger::isOdd()
        {
            return m_data[0] & 0x1;
        }
        
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
         * 
         * @param r
         * @param a
         * @param b
         */
        void BigInteger::mult(BigInteger* r, BigInteger* a, BigInteger* b)
        {
            BigInteger term;
            term.initSize(r->m_size);

            
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
                    
//                    std::cout << " shift left " << i << std::endl;
//                    shiftLeft(&term, &term, i);
//                    add(r, r, &term);
                    
                    addShifted(r, r, &term, i);
                }
                
                bDigit++;
                
                i+=32;
            }
        }
        
#ifdef MACRO_INLINING        
        /**
         * r can be a or b since we have an intermediate value
         * @param r
         * @param a
         * @param b
         * @param mod
         */
        __kernel void BigInteger::multMod(BigInteger* r, BigInteger* a, BigInteger* b, BigInteger* mod)
        {
        	int numIndex = get_global_id(0);
        	const int numLimbs = KERNEL_NUM_LIMBS;
        	int base = numIndex * numLimbs;
        	
            unsigned int m[KERNEL_NUM_LIMBS+KERNEL_NUM_LIMBS];
            //m.initSize(a->m_size + b->m_size);
            unsigned int q[KERNEL_NUM_LIMBS];
            //unsigned int r[KERNEL_NUM_LIMBS];
            //q.initSize(a->m_size);
            
            //mult(&m, a, b);
            KERNEL_BIG_INTEGER_MULT(m, 0, (KERNEL_NUM_LIMBS+KERNEL_NUM_LIMBS), a, base, KERNEL_NUM_LIMBS, b, base, KERNEL_NUM_LIMBS);
            //div_naive(&m, mod, &q, r);
            KERNEL_BIG_INTEGER_DIV_NAIVE(m, 0, (KERNEL_NUM_LIMBS+KERNEL_NUM_LIMBS), 
                    mod, base, KERNEL_NUM_LIMBS, 
                    q, 0, KERNEL_NUM_LIMBS, 
                    r, base, KERNEL_NUM_LIMBS);
            
            
        }

#endif
        
        void BigInteger::multMod(BigInteger* r, BigInteger* a, BigInteger* b, BigInteger* mod)
        {
            BigInteger m;
            m.initSize(a->m_size + b->m_size);
            BigInteger q;
            q.initSize(a->m_size);
            
            mult(&m, a, b);
            div_naive(&m, mod, &q, r);
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
            // x = this, y = power, n = mod
//            	var k1,k2,kn,np;
//	if(s7.length!=n.length)
//		s7=dup(n);

                BigInteger x(*v);
                BigInteger y(*power);
                
	//for even modulus, use a simple square-and-multiply algorithm,
	//rather than using the more complex Montgomery algorithm.
	//if (mod.isEven())
        
                r->setIntValue(1);
                
		while(!y.isZero())
                {
			if (y.isOdd())
                            multMod(r, r, &x, mod);
				
			shiftRight(&y, &y, 1);    // / 2
			squareMod(&x, &x, mod); 
		}
		return;
	}
        
        /**
         * Objects r and v can be the same
         * @param r
         * @param v
         * @param mod
         */
        void BigInteger::squareMod(BigInteger* r, BigInteger* v, BigInteger* mod)
        {
            multMod(r, v, v, mod);
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
                BigInteger::shiftLeft(&ref, &ref, 1);
                BigInteger::shiftLeft(this, this, 3);
                BigInteger::add(this, this, &ref);
                
//                std::cout << toString() << " + " << str[i] << " = "; 
                
                digit.setIntValue(str[i]-'0');
                BigInteger::add(this, this, &digit);
                
//                std::cout << toString() << std::endl;
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
        
