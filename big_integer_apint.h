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

/* 
 * File:   big_integer_apint.h
 * Author: dcr
 *
 * Created on April 8, 2018, 10:25 AM
 */

#ifndef BIG_INTEGER_APINT_H
#define BIG_INTEGER_APINT_H

#include "big_integer_base.h"

#include <stdio.h>
#include <string.h>

#define NUM_BIG_INTEGER_APINT_BITS    2080    
#define NUM_BIG_INTEGER_APINT_LIMBS  (NUM_BIG_INTEGER_APINT_BITS/32)


extern int big_integer_apint_extraChecks;
extern int big_integer_apint_verbosity;
  
template <int T>
class ap_uint
{
public:
    ap_uint<T>()
    {
    }
    
    ap_uint<T>(const ap_uint<T>& v)
    {
        for (int i=0; i<((T+31)/32); i++)
            m_data[i] = v.m_data[i];
    }
    
    ap_uint<T>(const int v)
    {
        for (int i=0; i<((T+31)/32); i++)
            m_data[i] = 0;
        
        m_data[0] = v;
    }
    
    ap_uint<T>& operator=(const ap_uint<T>& v)
    {
        for (int i=0; i<((T+31)/32); i++)
            m_data[i] = v.m_data[i];
        
        return *this;
    }
    
    ap_uint<T>& operator=(const ap_uint<2*T>& v)
    {
        for (int i=0; i<((T+31)/32); i++)
            m_data[i] = v.m_data[i];
        
        return *this;
    }
    
    ap_uint<T>& operator=(const int v)
    {
        for (int i=0; i<((T+31)/32); i++)
            m_data[i] = 0;
        
        m_data[0] = v;
        
        return *this;
    }
    
    
    ap_uint<T> operator>>(const int s)
    {
        ap_uint<T> ret=*this;
        
        big_integer_base_shiftRight(ret.m_data, 0, (T+31)/32,  m_data,0, (T+31)/32, s);
        
        return ret;        
    }
    
    ap_uint<T>& operator>>=(const int s)
    {
        big_integer_base_shiftRight(m_data, 0, (T+31)/32,  m_data, 0, (T+31)/32, s);
        
        return *this;        
    }
    
    ap_uint<T> operator<<(int s)
    {
        ap_uint<T> ret=*this;
        
        big_integer_base_shiftLeft(ret.m_data, 0, (T+31)/32, m_data, 0, (T+31)/32, s);
        
        return ret;        
    }
    
    
    
    ap_uint<T>& operator<<=(const int s)
    {
        big_integer_base_shiftLeft(m_data, 0, (T+31)/32, m_data, 0, (T+31)/32, s);
        
        return *this;        
    }
    
    ap_uint<T> operator&(ap_uint<T>& v)
    {
        ap_uint<T> ret=*this;
        
        for (int i=0; i<((T+31)/32); i++)
            ret.m_data[i] &= v.m_data[i];
        
        return ret;
    }
    
    ap_uint<T> operator+(const int v)
    {
        ap_uint<T> ret=*this;
        ap_uint<T> vv(v);
        
        big_integer_base_add(ret.m_data, 0, ((T+31)/32), m_data, 0, ((T+31)/32), vv.m_data, 0, ((T+31)/32));
        
        return ret;
    }
    
    ap_uint<T>& operator+=(const int v)
    {
        ap_uint<T> vv(v);
        
        big_integer_base_add(m_data, 0, ((T+31)/32), m_data, 0, ((T+31)/32), vv.m_data, 0, ((T+31)/32));
        
        return *this;
    }
    
    ap_uint<T> operator+(const ap_uint<T>& v)
    {
        ap_uint<T> ret=*this;
        
        big_integer_base_add(ret.m_data, 0, ((T+31)/32), m_data, 0, ((T+31)/32), (unsigned int*)v.m_data, 0, ((T+31)/32));
        
        return ret;
    }
    
    ap_uint<T>& operator+=(const ap_uint<T>& v)
    {
        big_integer_base_add((unsigned int*)m_data, 0, ((T+31)/32), 
                m_data, 0, ((T+31)/32), (unsigned int*) v.m_data, 0, ((T+31)/32));
        
        return *this;
    }
    
    
    
    ap_uint<T> operator-(const ap_uint<T>& v)
    {
        ap_uint<T> ret=*this;
        
        big_integer_base_subtract((unsigned int*) ret.m_data, 0, ((T+31)/32), m_data, 0, ((T+31)/32), (unsigned int*) v.m_data, 0, ((T+31)/32));
        
        return ret;
    }
    
    ap_uint<T>& operator-=(const ap_uint<T>& v)
    {
        big_integer_base_subtract((unsigned int*) m_data, 0, ((T+31)/32), m_data, 0, ((T+31)/32), (unsigned int*) v.m_data, 0, ((T+31)/32));
        
        return *this;
    }
    
    ap_uint<T> operator|(ap_uint<T>& v)
    {
        ap_uint<T> ret=*this;
        
        for (int i=0; i<((T+31)/32); i++)
            ret.m_data[i] |= v.m_data[i];
        
        return ret;
    }
    
    ap_uint<T>& operator|=(ap_uint<T>& v)
    {
        for (int i=0; i<((T+31)/32); i++)
            m_data[i] = v.m_data[i];
        
        return *this;
    }
    
    int operator<(ap_uint<T>& v)
    {
        ap_uint<T> diff = *this -v;
        
        int isNeg = diff.m_data[((T+31)/32)-1] & 0x80000000;
        
        return isNeg;
    }
    
    int operator>(ap_uint<T>& v)
    {
        ap_uint<T> diff = v - *this;
        
        int isNeg = diff.m_data[((T+31)/32)-1] & 0x80000000;
        
        return isNeg;
    }
    
    int operator==(ap_uint<T>& v)
    {
        for (int i=0; i < ((T+31)/32); i++)
            if (m_data[i] != v.m_data[i])
                return 0;
        
        return 1;
    }
    
    
    int operator<=(ap_uint<T>& v)
    {
        int isLess = (*this < v);
        int isEq = (*this == v);
        
        return (isLess | isEq);
    }
    
    int operator>=(ap_uint<T>& v)
    {
        int isMore = (*this > v);
        int isEq = (*this == v);
        
        return (isMore | isEq);
    }
    
    
    operator int() const
    {
        return m_data[0];
    }
    
    operator ap_uint<T*2>() const
    {
        ap_uint<T*2> ret;
        
        for (int i=0; i<((T*2+31)/32); i++)
            ret.m_data[i] = 0;
        
        for (int i=0; i<((T+31)/32); i++)
            ret.m_data[i] = m_data[i];
                    
        return ret;
    }
    
    int getLimbLength()
    {
        int len = 0;

        // find the highest limb with an active bit
        for (int i=((T+31)/32)-1; (i > 0); i--)
        {
            if (m_data[i] > 0)
                return i+1;
        }

        return 1;
    }
    
    const char* toHexString()
    {
        static const int sSize = 128;
        static char s[9*(sSize+1)];

        //assert(size <= sSize);

        int c = 0;
        for (int i=0; i < getLimbLength(); i++)
        {
            char buf[10];
            sprintf(buf, "%08X ", m_data[i]);
            //s = s + std::string(buf);

            strcpy(&s[c], buf);
            c += strlen(buf);
        }

        return s;
    }
    
    void parseHexString(const char* str)
    {
        // multiply by x*10 is x*(8+2) -> 8*x + 2*x
        int l = strlen(str);

        *this = 0;

        ap_uint<T> digit;
        

        // V = V*10+s[i];
        for (int i=0; i < l; i++)
        {
            //  x=x*10
            *this <<= 4; // *16


            if ((str[i] >= '0') && (str[i] <= '9'))
                digit = (int)(str[i]-'0');
            else if ((str[i] >= 'A') && (str[i] <= 'F'))
                digit = (int)(10 + (str[i]-'A'));
            else
                digit = (int)(10 + (str[i]-'a'));

            *this += digit;
        }
    }
    
    unsigned int m_data[(T+31)/32];    
};

int big_integer_apint_getLength(ap_uint<NUM_BIG_INTEGER_APINT_BITS> apint);
int big_integer_apint_getBit(ap_uint<NUM_BIG_INTEGER_APINT_BITS> apint, int i);
int big_integer_apint_isOdd(ap_uint<NUM_BIG_INTEGER_APINT_BITS> v);
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_mod(ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_x, ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_y);
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_montgomeryMultBase2(ap_uint<NUM_BIG_INTEGER_APINT_BITS> x, ap_uint<NUM_BIG_INTEGER_APINT_BITS> y, ap_uint<NUM_BIG_INTEGER_APINT_BITS> m);
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_multMod_interleaved(ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_a, ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_b, ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_mod); 
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_radixFromMontgomeryModBase2(ap_uint<NUM_BIG_INTEGER_APINT_BITS> m);
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_powerModMontgomeryBase2_noradix(ap_uint<NUM_BIG_INTEGER_APINT_BITS> x, ap_uint<NUM_BIG_INTEGER_APINT_BITS> e, ap_uint<NUM_BIG_INTEGER_APINT_BITS> m);
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_powerModMontgomeryBase2(ap_uint<NUM_BIG_INTEGER_APINT_BITS> x, ap_uint<NUM_BIG_INTEGER_APINT_BITS> e, ap_uint<NUM_BIG_INTEGER_APINT_BITS> m, ap_uint<NUM_BIG_INTEGER_APINT_BITS> radix);
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_squareMod(ap_uint<NUM_BIG_INTEGER_APINT_BITS> v,  ap_uint<NUM_BIG_INTEGER_APINT_BITS> m);
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_squareMod_interleaved(ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_v, ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_m);

#endif /* BIG_INTEGER_APINT_H */

