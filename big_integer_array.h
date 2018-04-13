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
#include <errno.h>


#ifdef __cplusplus
extern "C" {
#endif

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

int big_integer_array_isBiggerThan(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int v[NUM_BIG_INTEGER_ARRAY_LIMBS]);
int big_integer_array_isLessThan(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int v[NUM_BIG_INTEGER_ARRAY_LIMBS]);
int big_integer_array_isLessThanEqual(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int v[NUM_BIG_INTEGER_ARRAY_LIMBS]);
int big_integer_array_isOdd(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
int big_integer_array_isZero(unsigned int data[NUM_BIG_INTEGER_ARRAY_LIMBS]);
int big_integer_array_getBit(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS], int bitnum);
int big_integer_array_getLength(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
int big_integer_array_getLimbLength(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
int big_integer_array_getNumBits(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);

void big_integer_array_add(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int b[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_add_short(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int b[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_init(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int* data, unsigned int size);
void big_integer_array_copy(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int orig[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_inc(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_mod(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_mod_short(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_mod_naive(unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int y[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_montgomeryMultBase2(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int y[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_multMod_interleaved(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int b[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int mod[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_powerModMontgomeryBase2_noradix(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int e[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_powerModMontgomeryBase2(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int e[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int radix[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_powerModMontgomeryBase2_xprime(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int e[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int xprime[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_radixFromMontgomeryModBase2(unsigned int radix[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_range(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], int upper, int lower);
void big_integer_array_setIntValue(unsigned int data[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int v);
void big_integer_array_shiftLeft(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS], int sv);
void big_integer_array_shiftLeft_short(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], int bits);
void big_integer_array_shiftRight(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS], int sv);
void big_integer_array_shiftRight_short(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], int sv);
void big_integer_array_subtract(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int y[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_subtract_short(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int y[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_squareMod(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int v[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_zero(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS]);
void big_integer_array_zeroHighBits(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], int fromBit);
const char* big_integer_array_toHexString(unsigned int data[NUM_BIG_INTEGER_ARRAY_LIMBS]);

#ifdef __cplusplus
}
#endif

#endif /* BIG_INTEGER_ARRAY_H */

