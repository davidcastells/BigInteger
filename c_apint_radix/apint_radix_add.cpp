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
#include "../big_integer_apint_radix.h"

#include "../to_string.h"

void big_integer_apint_radix_add_short(limbs_radix_array r, limbs_radix_array b)
{
    big_integer_apint_radix_add(r, r, b);
}


void big_integer_apint_radix_add(limbs_radix_array r, limbs_radix_array a, limbs_radix_array b)
{
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> carryIn = 0;
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> carryOut = 0;
            
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> sum;
    unsigned int i;
    
//    unsigned int minSize = big_integer_base_minVal(big_integer_base_minVal(a_size, b_size), r_size);
        
    for (i=0; i < NUM_BIG_INTEGER_APINT_RADIX_LIMBS; i++)
    {
//        std::cout << "a[" << to_string(i) << "] = " << a[i].toHexString() << std::endl; 
//        std::cout << "b[" << to_string(i) << "] = " << b[i].toHexString() << std::endl; 
//        std::cout << "carry = " << carryIn.toHexString() << std::endl; 
        
        sum = a[i] + b[i] + carryIn;

        if (sum < a[i]) carryIn = 1;
        else if (sum < b[i]) carryIn = 1;
        else carryIn = 0;
            
        r[i] = sum;
        
//        std::cout << "sum = " << r[i].toHexString() << std::endl; 
    }
}

void big_integer_apint_radix_add_big(limbs_radix_array2 r, limbs_radix_array2 a, limbs_radix_array2 b)
{
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> carryIn = 0;
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> carryOut = 0;
            
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> sum;
    unsigned int i;
    
//    unsigned int minSize = big_integer_base_minVal(big_integer_base_minVal(a_size, b_size), r_size);
        
    for (i=0; i < (NUM_BIG_INTEGER_APINT_RADIX_LIMBS*2); i++)
    {
        sum = a[i] + b[i] + carryIn;

        if (sum < a[i]) carryIn = 1;
        else if (sum < b[i]) carryIn = 1;
        else carryIn = 0;
            
        r[i] = sum;
    }
}


void big_integer_apint_radix_inc(limbs_radix_array m)
{
    int i = 0;
    int doRun = 1;
    while (doRun)
    {                
        m[i] += 1;

        doRun = (m[i] == 0);

        i++;
    }
}