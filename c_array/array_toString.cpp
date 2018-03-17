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
#include "../big_integer_array.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

const char* big_integer_array_toHexString(unsigned int data[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    static const int sSize = NUM_BIG_INTEGER_ARRAY_LIMBS;
    static char s[9*(sSize+1)];
    
    assert(NUM_BIG_INTEGER_ARRAY_LIMBS <= sSize);

    int c = 0;
    for (int i=0; i < NUM_BIG_INTEGER_ARRAY_LIMBS; i++)
    {
        char buf[10];
        sprintf(buf, "%08X ", data[i]);
        //s = s + std::string(buf);
        
        strcpy(&s[c], buf);
        c += strlen(buf);
    }

    return s;
}