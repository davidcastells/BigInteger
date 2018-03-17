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

#include "../BigInteger/big_integer_array.h"
#include "big_integer.h"

#include <assert.h>

void big_integer_array_radixFromMontgomeryModBase2(unsigned int radix[NUM_BIG_INTEGER_ARRAY_LIMBS], 
        unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    if (big_integer_array_extraChecks)
        assert(big_integer_array_getNumBits(radix) > big_integer_array_getLimbLength(m));
    
    int mLen = big_integer_array_getLength(m);

    big_integer_array_setIntValue(radix, 1);
    big_integer_array_shiftLeft_short(radix, mLen);      
}