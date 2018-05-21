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
#include "verbosity.h"

#define NUM_BIG_INTEGER_APINT_RADIX         64    
#define NUM_BIG_INTEGER_APINT_RADIX_BITS    (2048+NUM_BIG_INTEGER_APINT_RADIX)
#define NUM_BIG_INTEGER_APINT_RADIX_LIMBS   (NUM_BIG_INTEGER_APINT_RADIX_BITS/NUM_BIG_INTEGER_APINT_RADIX)

extern int big_integer_apint_radix_extraChecks;
extern int big_integer_apint_radix_verbosity;



typedef ap_uint<NUM_BIG_INTEGER_APINT_RADIX> limbs_radix_array[NUM_BIG_INTEGER_APINT_RADIX_LIMBS];   // big integer array
typedef ap_uint<NUM_BIG_INTEGER_APINT_RADIX> limbs_radix_array2[NUM_BIG_INTEGER_APINT_RADIX_LIMBS*2];  // big integer array (double size)

int big_integer_apint_radix_getBit(limbs_radix_array m, int bitnum);
int big_integer_apint_radix_getLength(limbs_radix_array m);
int big_integer_apint_radix_getLimbLength(limbs_radix_array m);
int big_integer_apint_radix_getLimbLength_big(limbs_radix_array2 m);
int big_integer_apint_radix_getNumBits(limbs_radix_array m);
int big_integer_apint_radix_isEqual(limbs_radix_array a, limbs_radix_array b);
int big_integer_apint_radix_isBiggerThan(limbs_radix_array m, limbs_radix_array v);
int big_integer_apint_radix_isLessThan(limbs_radix_array m, limbs_radix_array v);
int big_integer_apint_radix_isLessThanEqual(limbs_radix_array m, limbs_radix_array v);
int big_integer_apint_radix_isNegative(limbs_radix_array v);
int big_integer_apint_radix_isZero(limbs_radix_array data);

void big_integer_apint_radix_add(limbs_radix_array r, limbs_radix_array a, limbs_radix_array b);
void big_integer_apint_radix_add_big(limbs_radix_array2 r, limbs_radix_array2 a, limbs_radix_array2 b);
void big_integer_apint_radix_add_short(limbs_radix_array r, limbs_radix_array b);
void big_integer_apint_radix_copy(limbs_radix_array m, limbs_radix_array orig);
void big_integer_apint_radix_div_naive(limbs_radix_array x, limbs_radix_array y, limbs_radix_array q, limbs_radix_array r);
void big_integer_apint_radix_inc(limbs_radix_array m);
void big_integer_apint_radix_initfromLimbArray(limbs_radix_array r, unsigned int* m_data, unsigned int limbs);
void big_integer_apint_radix_inverseMod(limbs_radix_array ret, limbs_radix_array a, limbs_radix_array b);
void big_integer_apint_radix_mprimeFromMontgomeryRadix(limbs_radix_array mprime, limbs_radix_array m, limbs_radix_array radix);
void big_integer_apint_radix_mult(limbs_radix_array r, limbs_radix_array a, limbs_radix_array b);
void big_integer_apint_radix_mult_big(limbs_radix_array2 r_big, limbs_radix_array a, limbs_radix_array b);
void big_integer_apint_radix_multLow(limbs_radix_array r, limbs_radix_array2 a_big, limbs_radix_array b, unsigned int rsize);
void big_integer_apint_radix_multMod_interleaved(limbs_radix_array r, limbs_radix_array a, limbs_radix_array b, limbs_radix_array mod);
void big_integer_apint_radix_multMontgomeryForm3(limbs_radix_array r, limbs_radix_array x, limbs_radix_array y, limbs_radix_array m, limbs_radix_array mprime);
void big_integer_apint_radix_radixFromMontgomeryMod(limbs_radix_array radix, limbs_radix_array m);
void big_integer_apint_radix_range(limbs_radix_array r, limbs_radix_array x, int upper, int lower);
void big_integer_apint_radix_setIntValue(limbs_radix_array data, unsigned int v);
void big_integer_apint_radix_shiftLeft(limbs_radix_array r, limbs_radix_array a, int sv);
void big_integer_apint_radix_shiftLeft_short(limbs_radix_array r, int bits);
void big_integer_apint_radix_shiftRight(limbs_radix_array r, limbs_radix_array a, int sv);
void big_integer_apint_radix_shiftRight_limbs(limbs_radix_array r, limbs_radix_array a, int limbsShifted);
void big_integer_apint_radix_subtract(limbs_radix_array r, limbs_radix_array x, limbs_radix_array y);
void big_integer_apint_radix_subtract_short(limbs_radix_array r, limbs_radix_array y);
void big_integer_apint_radix_zero(limbs_radix_array r);
void big_integer_apint_radix_zero_big(limbs_radix_array2 r);
void big_integer_apint_radix_zeroHighBits(limbs_radix_array r, int fromBit);

const char* big_integer_apint_radix_toHexString(limbs_radix_array data);
const char* big_integer_apint_radix_toHexString_big(limbs_radix_array2 data);

#endif /* BIG_INTEGER_APINT_RADIX_H */

