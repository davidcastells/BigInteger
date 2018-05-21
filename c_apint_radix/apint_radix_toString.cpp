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

const char* big_integer_apint_radix_toHexString(limbs_radix_array data)
{
    static const int sSize = NUM_BIG_INTEGER_APINT_RADIX_LIMBS;
    static const int n32Limbs = sSize * (NUM_BIG_INTEGER_APINT_RADIX/32);
    
    static char s[9*(n32Limbs+1)];
    
    int limbs = big_integer_apint_radix_getLimbLength(data);

    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> lmask = 0xFFFFFFFF;    // 32 bits mask
    
    
    int c = 0;
    for (int i=0; i < limbs; i++)
    {
        ap_uint<NUM_BIG_INTEGER_APINT_RADIX> ldata = data[i];
        
        
        for (int k=0; k < NUM_BIG_INTEGER_APINT_RADIX/32; k++)
        {
            char buf[10];
            ap_uint<NUM_BIG_INTEGER_APINT_RADIX> temp = ldata & lmask;
            
            sprintf(buf, "%s", temp.toHexString());
        //s = s + std::string(buf);
        
            strcpy(&s[c], buf);
            c += (unsigned int) strlen(buf);
            
            ldata >>= 32;
        }
    }

    return s;
}

const char* big_integer_apint_radix_toHexString_big(limbs_radix_array2 data)
{
    static const int sSize = NUM_BIG_INTEGER_APINT_RADIX_LIMBS * 2;
    static const int n32Limbs = sSize * (NUM_BIG_INTEGER_APINT_RADIX*2/32);
    
    static char s[9*(n32Limbs+1)];
    
    int limbs = big_integer_apint_radix_getLimbLength(data);

    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> lmask = 0xFFFFFFFF;    // 32 bits mask
    
    
    int c = 0;
    for (int i=0; i < limbs; i++)
    {
        ap_uint<NUM_BIG_INTEGER_APINT_RADIX> ldata = data[i];
        
        
        for (int k=0; k < NUM_BIG_INTEGER_APINT_RADIX/32; k++)
        {
            char buf[10];
            ap_uint<NUM_BIG_INTEGER_APINT_RADIX> temp = ldata & lmask;
            
            sprintf(buf, "%s", temp.toHexString());
        //s = s + std::string(buf);
        
            strcpy(&s[c], buf);
            c += (unsigned int) strlen(buf);
            
            ldata >>= 32;
        }
    }

    return s;
}