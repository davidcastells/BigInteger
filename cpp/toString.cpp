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

#include <stdio.h>
#include <assert.h>
#include <iostream>


    

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