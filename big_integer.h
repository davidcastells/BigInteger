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
 * File:   big_integer.h
 * Author: dcr
 *
 * Created on December 19, 2017, 8:10 AM
 */

#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#define VERBOSITY_LEVEL_DIV             5
#define VERBOSITY_LEVEL_GET_LENGTH      6
#define VERBOSITY_LEVEL_MULT_KARATSUBA  5
#define VERBOSITY_LEVEL_MULT_MONTGOMERY 4
#define VERBOSITY_LEVEL_MONTGOMERY      3
#define VERBOSITY_LEVEL_POWER_MOD       2
#define VERBOSITY_LEVEL_INV_MOD         4
#define VERBOSITY_LEVEL_RANGE           6

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Big Number structure
 */
typedef struct _big_integer_tag
{
    unsigned int* m_data;
    unsigned int m_size;
} big_integer;


extern int big_integer_verbosity;


/**
 * To produce more readable code "big_integer x;" should be declared outside the
 * macro, so user is totally aware that x is declared
 * @param x
 * @param m
 */
#define STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(x, size) \
    unsigned int x##_data[(size)]; \
    unsigned int x##_size = (size); \
    big_integer_init(&x, x##_data, x##_size);

int big_integer_minVal( int x,  int y);
int big_integer_maxVal( int x,  int y);
int big_integer_getLength(big_integer* x);
int big_integer_getLimbLength(big_integer* x);
int big_integer_isBiggerThan(big_integer* x, big_integer* y);
int big_integer_isLessThan(big_integer* x, big_integer* y);
int big_integer_isLessThanEqual(big_integer* x, big_integer* y);
int big_integer_isNegative(big_integer* x);
int big_integer_isOdd(big_integer* x);
int big_integer_isZero(big_integer* x);
int big_integer_getBit(big_integer* x, int bitnum);
int big_integer_getNumBits(big_integer* x);

const char* big_integer_toHexString(big_integer* x);

void big_integer_init(big_integer* x, unsigned int* data, unsigned int size);

void big_integer_add(big_integer* r, big_integer* a, big_integer* b);
void big_integer_add_short(big_integer* r, big_integer* b);
void big_integer_copy(big_integer* x, big_integer* orig);
void big_integer_div_naive(big_integer* x, big_integer* y, big_integer* q, big_integer* r);
void big_integer_error(big_integer* x);
void big_integer_inc(big_integer* x);
void big_integer_inverseMod(big_integer* ret, big_integer* a, big_integer* b);
void big_integer_mod(big_integer* r, big_integer* x, big_integer* y);
void big_integer_mod_short(big_integer* x, big_integer* m);
void big_integer_mod_naive(big_integer* x, big_integer* y, big_integer* r);
void big_integer_mprimeFromMontgomeryRadix(big_integer* mprime, big_integer* m, big_integer* radix);
void big_integer_mult(big_integer* r, big_integer* a, big_integer* b);
void big_integer_mult32(unsigned int x, unsigned int y, unsigned int *rHight, unsigned int *rLow);
void big_integer_multLow(big_integer* r, big_integer* a, big_integer* b);
void big_integer_multMontgomeryForm3(big_integer* r, big_integer* x, big_integer* y, big_integer* m, big_integer* mprime);
void big_integer_multMod_interleaved(big_integer* r, big_integer* a, big_integer* b, big_integer* mod);
void big_integer_multMod_interleaved_short(big_integer* x, big_integer* b, big_integer* m);
void big_integer_powerMod(big_integer* r, big_integer* v, big_integer* power, big_integer* mod);
void big_integer_powerModMont(big_integer* r, big_integer* x, big_integer* e, big_integer* m);
void big_integer_powerModMontgomery(big_integer* r, big_integer* x, big_integer* e, big_integer* m, big_integer* mprime, big_integer* radix);
void big_integer_radixFromMontgomeryMod(big_integer* radix, big_integer* m);
void big_integer_range(big_integer* r, big_integer* x, int upper, int lower);
void big_integer_setIntValue(big_integer* x, unsigned int v);
void big_integer_shiftLeft(big_integer* r, big_integer* a, int sv);
void big_integer_shiftLeft_short(big_integer* r, int bits);
void big_integer_shiftRight(big_integer* r, big_integer* a, int sv);
void big_integer_shiftRight_short(big_integer* x, int bits);
void big_integer_squareMod(big_integer* r, big_integer* v, big_integer* m);
void big_integer_squareMod_short(big_integer* x, big_integer* m);
void big_integer_subtract_short(big_integer* r, big_integer* y);
void big_integer_subtract(big_integer* r, big_integer* x, big_integer* y);
void big_integer_zero(big_integer* x);
void big_integer_zeroHighBits(big_integer* r, int fromBit);


#ifdef __cplusplus
}
#endif

#endif /* BIG_INTEGER_H */

