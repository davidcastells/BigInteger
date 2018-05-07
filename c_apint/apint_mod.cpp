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

#include "../big_integer_apint.h"


ap_uint<NUM_BIG_INTEGER_APINT_BITS>  big_integer_apint_mod(ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_x, ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_y)
{
    ap_uint<NUM_BIG_INTEGER_APINT_BITS*2> x2 = ap_x;
    ap_uint<NUM_BIG_INTEGER_APINT_BITS*2> y2 = ap_y;
    
    y2 <<=  NUM_BIG_INTEGER_APINT_BITS;
    ap_uint<NUM_BIG_INTEGER_APINT_BITS*2> r2 = 0;

    {
        // get the length of y
        r2 = x2;
        
        for (int i=0; i < NUM_BIG_INTEGER_APINT_BITS; i++)
        {
            r2 <<= 1;
            
            if (r2 >= y2)
                r2 -= y2;            
        }
        
        ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_r;
        
        ap_r = (r2 >> NUM_BIG_INTEGER_APINT_BITS);
        return ap_r;
    }    
}