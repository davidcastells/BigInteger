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
 * File:   array_parse.cpp
 * Author: dcr
 *
 * Created on May 7, 2018, 12:36 PM
 */
#include "../big_integer_array.h"

void big_integer_array_parseHexString(limbs_array v, const char* str)
{
    // multiply by x*10 is x*(8+2) -> 8*x + 2*x
    int l = (int) strlen(str);

    big_integer_array_zero(v);

    limbs_array digit;
    
    // V = V*10+s[i];
    for (int i=0; i < l; i++)
    {
        //  x=x*10
        big_integer_array_shiftLeft_short(v,4); // *16


        if ((str[i] >= '0') && (str[i] <= '9'))
            big_integer_array_setIntValue(digit, str[i]-'0');
        else if ((str[i] >= 'A') && (str[i] <= 'F'))
            big_integer_array_setIntValue(digit, 10 + (str[i]-'A'));
        else
            big_integer_array_setIntValue(digit, 10 + (str[i]-'a'));

        if (big_integer_array_verbosity > 10)
            std::cout << big_integer_array_toHexString(v) << " + " << big_integer_array_toHexString(digit) << " = "; 

        big_integer_array_add_short(v, digit);

        if (big_integer_array_verbosity > 10)
            std::cout << big_integer_array_toHexString(v) << std::endl;
    }
}

