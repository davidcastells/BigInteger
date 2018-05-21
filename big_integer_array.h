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
 * File:   big_integer_array.h
 * Author: dcr
 *
 * Created on March 14, 2018, 6:11 PM
 */

#ifndef BIG_INTEGER_ARRAY_H
#define BIG_INTEGER_ARRAY_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "to_string.h"


#define VERBOSITY_LEVEL_DIV             5
#define VERBOSITY_LEVEL_GET_LENGTH      6
#define VERBOSITY_LEVEL_MULT_KARATSUBA  5
#define VERBOSITY_LEVEL_MULT_MONTGOMERY 4
#define VERBOSITY_LEVEL_MONTGOMERY      3
#define VERBOSITY_LEVEL_POWER_MOD       2
#define VERBOSITY_LEVEL_INV_MOD         4
#define VERBOSITY_LEVEL_RANGE           6
    
#define NUM_BIG_INTEGER_ARRAY_BITS    2080    
#define NUM_BIG_INTEGER_ARRAY_LIMBS  (NUM_BIG_INTEGER_ARRAY_BITS/32)



extern int big_integer_array_extraChecks;
extern int big_integer_array_verbosity;

typedef unsigned int limbs_array[NUM_BIG_INTEGER_ARRAY_LIMBS];   // big integer array
typedef unsigned int limbs_array2[NUM_BIG_INTEGER_ARRAY_LIMBS*2];  // big integer array (double size)


int big_integer_array_isBiggerThan(limbs_array m, limbs_array v);
int big_integer_array_isLessThan(limbs_array m, limbs_array v);
int big_integer_array_isLessThanEqual(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int v[NUM_BIG_INTEGER_ARRAY_LIMBS]);
int big_integer_array_isNegative(unsigned int v[NUM_BIG_INTEGER_ARRAY_LIMBS]);
int big_integer_array_isOdd(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
int big_integer_array_isZero(unsigned int data[NUM_BIG_INTEGER_ARRAY_LIMBS]);
int big_integer_array_getBit(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS], int bitnum);
int big_integer_array_getLength(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
int big_integer_array_getLimbLength(limbs_array m);
int big_integer_array_getLimbLength_big(limbs_array2 m);
int big_integer_array_getNumBits(limbs_array m);

void big_integer_array_add(limbs_array r, limbs_array a, limbs_array b);
void big_integer_array_add_big(limbs_array2 r, limbs_array2 a, limbs_array2 b);
void big_integer_array_add_short(limbs_array r, limbs_array b);
void big_integer_array_init(limbs_array r, unsigned int* data, unsigned int size);
void big_integer_array_initFromHexString(limbs_array v, const char* str);
void big_integer_array_copy(limbs_array m, limbs_array orig);
void big_integer_array_div_naive(unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS],unsigned int y[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int q[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_inc(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_inverseMod(unsigned int ret[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int b[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_mod(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_mod_short(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_mod_naive(unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int y[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_montgomeryMultBase2(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int y[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_multMontgomeryForm3(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int y[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int mprime[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_mult(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int b[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_mult_big(limbs_array2 r, limbs_array a, limbs_array b);
void big_integer_array_multLow(limbs_array r, limbs_array2 a_big, limbs_array b, unsigned int rsize);
void big_integer_array_multMod_interleaved(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int b[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int mod[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_powerModMontgomery(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int e[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_powerModMontgomery_mprime(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int e[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int mprime[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int radix[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_powerModMontgomeryBase2_noradix(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int e[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_powerModMontgomeryBase2(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int e[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int radix[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_powerModMontgomeryBase2_xprime(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int e[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int xprime[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_radixFromMontgomeryMod(unsigned int radix[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_radixFromMontgomeryModBase2(unsigned int radix[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_mprimeFromMontgomeryRadix(unsigned int mprime[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int radix[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_parseHexString(limbs_array v, const char* str);
void big_integer_array_range(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], int upper, int lower);
void big_integer_array_random(limbs_array v);
void big_integer_array_random_bits(limbs_array v, int n);
void big_integer_array_setIntValue(limbs_array data,  unsigned int v);
void big_integer_array_shiftLeft(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS], int sv);
void big_integer_array_shiftLeft_short(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], int bits);
void big_integer_array_shiftRight(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS], int sv);
void big_integer_array_shiftRight_short(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], int sv);
void big_integer_array_shiftRight_limbs(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS], int limbsShifted);
void big_integer_array_subtract(limbs_array r, limbs_array x, limbs_array y);
void big_integer_array_subtract_short(limbs_array r, limbs_array y);
void big_integer_array_squareMod(limbs_array r, limbs_array v, limbs_array m);
void big_integer_array_zero(limbs_array r);
void big_integer_array_zero_big(limbs_array2 r);
void big_integer_array_zeroHighBits(limbs_array r, int fromBit);
const char* big_integer_array_toHexString(limbs_array data);
const char* big_integer_array_toHexString_big(limbs_array2 data);



#endif /* BIG_INTEGER_ARRAY_H */

