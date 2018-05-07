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
#include "../assertf.h"

ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_squareMod(ap_uint<NUM_BIG_INTEGER_APINT_BITS> v, 
        ap_uint<NUM_BIG_INTEGER_APINT_BITS> m)
{
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> vmod = big_integer_apint_mod(v, m);
    
    //big_integer_array_multMod(r, v, v, m);
    return big_integer_apint_squareMod_interleaved(vmod, m);
    
}

ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_squareMod_interleaved(ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_v,
	ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_m)
{
    return big_integer_apint_multMod_interleaved(ap_v, ap_v, ap_m);
}