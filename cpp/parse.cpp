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
#include <string.h>
#include <assert.h>
#include <iostream>

/**
 * 
 * @param str
 */
void BigInteger::parseString(const char* str)
{
    // multiply by x*10 is x*(8+2) -> 8*x + 2*x
    int l = (int) strlen(str);

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
    int l = (int) strlen(str);

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

