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

ap_uint<NUM_XMP_BITS> big_integer_apint_multMod_interleaved_safe(ap_uint<NUM_XMP_BITS> ap_a,
	ap_uint<NUM_XMP_BITS> ap_b,
	ap_uint<NUM_XMP_BITS> ap_mod)
{
    ap_uint<NUM_XMP_BITS> safe_a = big_integer_apint_mod(ap_a, ap_mod);
    ap_uint<NUM_XMP_BITS> safe_b = big_integer_apint_mod(ap_b, ap_mod);
    
    return big_integer_apint_multMod_interleaved(safe_a, safe_b, ap_mod);
}


ap_uint<NUM_XMP_BITS> big_integer_apint_multMod_interleaved(ap_uint<NUM_XMP_BITS> ap_a,
	ap_uint<NUM_XMP_BITS> ap_b,
	ap_uint<NUM_XMP_BITS> ap_mod) 
{
//    assert(a->isLessThan(mod));
//    assert(b->isLessThan(mod));
    ap_uint<NUM_XMP_BITS> ap_r = 0;
    
    // @todo find the smallest number and change order accordingly
    int n = big_integer_apint_getLength(ap_a);
    
    //ap_uint<1> ai[NUM_XMP_BITS];
    //fromAPIntToBitArray(ai, ap_a);
    ap_uint<NUM_XMP_BITS*2> a2 = ap_a;
    ap_uint<NUM_XMP_BITS*2> a3 = 0;
    
    
    for (int i=NUM_XMP_BITS-1; i >= 0; i--)
    {
        a2 <<= 1;
        a3 = a2 >> NUM_XMP_BITS;
        
    //for (int i=n-1; i>=0; i-- )
        if (i < n)
        {
            ap_r <<= 1;

            //int ai = big_integer_apint_getBit(ap_a, i);
            int ai = a3 & 0x01;
            
            if (ai)
            {
                ap_r += ap_b;
            }

//            for (int k=0; k < 2; k++)
                if (ap_mod <= ap_r)
                {
                    ap_r -= ap_mod;
                }

// check if the previous for is better            
            if (ap_mod <= ap_r)
            {
                ap_r -= ap_mod;
            }

            //if (isLess) big_integer_array_subtract_short(r_data, mod_data);
        }
    }
    
    return ap_r;
}