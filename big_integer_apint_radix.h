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
 * File:   big_integer_apint_radix.h
 * Author: dcr
 *
 * Created on May 16, 2018, 10:32 AM
 */

#ifndef BIG_INTEGER_APINT_RADIX_H
#define BIG_INTEGER_APINT_RADIX_H

#include "ap_int.h"

#define NUM_BIG_INTEGER_APINT_RADIX         64    
#define NUM_BIG_INTEGER_APINT_RADIX_BITS    (2048+NUM_BIG_INTEGER_APINT_RADIX)
#define NUM_BIG_INTEGER_APINT_RADIX_LIMBS   (NUM_BIG_INTEGER_APINT_RADIX_BITS/NUM_BIG_INTEGER_APINT_RADIX)

extern int big_integer_apint_radix_extraChecks;

typedef ap_uint<NUM_BIG_INTEGER_APINT_RADIX> limbs_radix_array[NUM_BIG_INTEGER_APINT_RADIX_LIMBS];   // big integer array
typedef ap_uint<NUM_BIG_INTEGER_APINT_RADIX> limbs_radix_array2[NUM_BIG_INTEGER_APINT_RADIX_LIMBS*2];  // big integer array (double size)

int big_integer_apint_radix_isEqual(limbs_radix_array a, limbs_radix_array b);
int big_integer_apint_radix_getLength(limbs_radix_array m);
int big_integer_apint_radix_getLimbLength(limbs_radix_array m);
int big_integer_apint_radix_isNegative(limbs_radix_array v);

void big_integer_apint_radix_add(limbs_radix_array r, limbs_radix_array a, limbs_radix_array b);
void big_integer_apint_radix_mult(limbs_radix_array r, limbs_radix_array a, limbs_radix_array b);
void big_integer_apint_radix_zero(limbs_radix_array r);
void big_integer_apint_radix_initfromLimbArray(limbs_radix_array r, unsigned int* m_data, unsigned int limbs);

const char* big_integer_apint_radix_toHexString(limbs_radix_array data);

#endif /* BIG_INTEGER_APINT_RADIX_H */

