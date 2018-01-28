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

#ifndef BIG_INTEGER_BASE_H
#define BIG_INTEGER_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * I will refactor the code to use and structure instead of this 
 * simplistic argument passing

 typedef struct tag_big_integer
 {
  unsigned int* m_data;
  unsigned int m_base;
  unsigned int m_size;  
 } big_integer;

*/
extern int base_verbosity;

#define VERBOSITY_LEVEL_DIV             5
#define VERBOSITY_LEVEL_GET_LENGTH      6
#define VERBOSITY_LEVEL_MULT_KARATSUBA  5
#define VERBOSITY_LEVEL_MULT_MONTGOMERY 4
#define VERBOSITY_LEVEL_MONTGOMERY      3
#define VERBOSITY_LEVEL_POWER_MOD       2
#define VERBOSITY_LEVEL_INV_MOD         4
#define VERBOSITY_LEVEL_RANGE           6
    
int big_integer_base_minVal( int x,  int y);
int big_integer_base_maxVal( int x,  int y);

int big_integer_base_getBit(unsigned int* m_data, const unsigned int base, const unsigned int size, int bitnum);
int big_integer_base_getLength(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size);
int big_integer_base_getLimbLength(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size);
int big_integer_base_isLessThan(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size,  unsigned int* v_data, const unsigned int v_base, const unsigned int v_size);
int big_integer_base_isLessThanEqual(unsigned int* m_data, unsigned int m_base, unsigned int m_size, unsigned int* v_data, unsigned int v_base, unsigned int v_size);
int big_integer_base_isNegative(unsigned int * m_data, const unsigned int m_base, const unsigned int m_size);
int big_integer_base_isZero(unsigned int * data, const unsigned int base, const unsigned int size);
int big_integer_base_isOdd(unsigned int * data, const unsigned int base, const unsigned int size);
int big_integer_base_base_isEqual(unsigned int * a_data, unsigned int a_base, unsigned int a_size, unsigned int * b_data, unsigned int b_base, unsigned int b_size);
int big_integer_base_isBiggerThan(unsigned int* m_data, unsigned int m_base, unsigned int m_size, unsigned int* v_data, unsigned int v_base, unsigned int v_size);
int big_integer_base_getNumBits(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size);

void big_integer_base_error(unsigned int*  r_data,unsigned int r_base,unsigned int r_size);
void big_integer_base_zero(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size);
void big_integer_base_inc(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size);
void big_integer_base_setIntValue(unsigned int* data, const unsigned int base, const unsigned int size, unsigned int v);
void big_integer_base_range(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* x_data, const unsigned int x_base, const unsigned int x_size, int upper, int lower);
void big_integer_range_short(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size, int up, int down);
void big_integer_base_add(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* a_data, const unsigned int a_base, const unsigned int a_size,  unsigned int* b_data, const unsigned int b_base, const unsigned int b_size);
void big_integer_base_add_short(unsigned int * r_data, unsigned int r_base, unsigned int r_size, unsigned int* b_data, const unsigned int b_base, const unsigned int b_size);
void big_integer_base_copy(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size, unsigned int* orig_data, const unsigned int orig_base, const unsigned int orig_size);
void big_integer_base_div_naive(unsigned int* x_data, const unsigned int x_base, const unsigned int x_size, unsigned int* y_data, const unsigned int y_base, const unsigned int y_size,  unsigned int* nq_data, const unsigned int nq_base, const unsigned int nq_size, unsigned int* nr_data, const unsigned int nr_base, const unsigned int nr_size);
void big_integer_base_initSize(unsigned int * data, unsigned int base, unsigned int size, int s);
void big_integer_base_inverseMod(unsigned int* ret_data, unsigned int ret_base, const unsigned int ret_size, unsigned int* a_data, unsigned int a_base, unsigned int a_size, unsigned int* b_data, unsigned int b_base, unsigned int b_size);
void big_integer_base_mod(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* x_data, const unsigned int x_base, const unsigned int x_size, unsigned int* m_data, const unsigned int m_base, const unsigned int m_size);
void big_integer_base_mod_short(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,  unsigned int* m_data, const unsigned int m_base, const unsigned int m_size);
void big_integer_base_mod_naive(unsigned int* x_data, const unsigned int x_base, const unsigned int x_size, unsigned int* y_data, const unsigned int y_base, const unsigned int y_size, unsigned int* r_data, const unsigned int r_base, const unsigned int r_size);
void big_integer_base_mprimeFromMontgomeryRadix(unsigned int* mprime_data, unsigned int mprime_base, const unsigned int mprime_size, unsigned int* m_data, unsigned int m_base, const unsigned int m_size, unsigned int* radix_data, unsigned int radix_base, const unsigned int radix_size);
void big_integer_base_mult(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* a_data, const unsigned int a_base, const unsigned int a_size,  unsigned int* b_data, const unsigned int b_base, const unsigned int b_size);
void big_integer_base_multLow(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* a_data, const unsigned int a_base, const unsigned int a_size,  unsigned int* b_data, const unsigned int b_base, const unsigned int b_size);
void big_integer_base_multMod(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* a_data, const unsigned int a_base, const unsigned int a_size, unsigned int* b_data, const unsigned int b_base, const unsigned int b_size, unsigned int* mod_data, const unsigned int mod_base, const unsigned int mod_size);
void big_integer_base_multMod_short(unsigned int * r_data, unsigned int r_base, unsigned int r_size, unsigned int * b_data, unsigned int b_base, unsigned int b_size, unsigned int * m_data, unsigned int m_base, unsigned int m_size);
void big_integer_base_radixFromMontgomeryMod(unsigned int* radix_data, unsigned int radix_base, unsigned int radix_size, unsigned int* m_data, unsigned int m_base, unsigned int m_size);
void big_integer_base_squareMod(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* v_data, const unsigned int v_base, const unsigned int v_size, unsigned int* m_data, const unsigned int m_base, const unsigned int m_size);
void big_integer_base_squareMod_interleaved(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* v_data, const unsigned int v_base, const unsigned int v_size, unsigned int* m_data, const unsigned int m_base, const unsigned int m_size);
void big_integer_base_multMod_interleaved(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* a_data, const unsigned int a_base, const unsigned int a_size, unsigned int* b_data, const unsigned int b_base, const unsigned int b_size, unsigned int* mod_data, const unsigned int mod_base, const unsigned int mod_size);
void big_integer_base_multMod_interleaved_short(unsigned int * r_data, unsigned int r_base, unsigned int r_size, unsigned int * b_data, unsigned int b_base, unsigned int b_size, unsigned int * m_data, unsigned int m_base, unsigned int m_size);
void big_integer_base_multMontgomeryForm3(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* x_data, const unsigned int x_base, const unsigned int x_size, unsigned int* y_data, const unsigned int y_base, const unsigned int y_size, unsigned int* m_data, const unsigned int m_base, const unsigned int m_size, unsigned int* mprime_data, const unsigned int mprime_base, const unsigned int mprime_size);
void big_integer_base_shiftRight(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* a_data, const unsigned int a_base, const unsigned int a_size, int sv);
void big_integer_base_shiftRight_short(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, int bits);
void big_integer_base_shiftLeft(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* a_data, const unsigned int a_base, const unsigned int a_size, int sv);
void big_integer_base_shiftLeft_short(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, int bits);
void big_integer_base_squareMod_short(unsigned int * r_data, unsigned int r_base, unsigned int r_size, unsigned int * m_data, unsigned int m_base, unsigned int m_size);
void big_integer_base_squareMod_interleaved(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* v_data, const unsigned int v_base, const unsigned int v_size, unsigned int* m_data, const unsigned int m_base, const unsigned int m_size);
void big_integer_base_squareMod_interleaved_short(unsigned int * r_data, const unsigned int r_base, const unsigned int r_size, unsigned int * m_data, const unsigned int m_base, const unsigned int m_size);
void big_integer_base_subtract(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* x_data, const unsigned int x_base, const unsigned int x_size, unsigned int* y_data, const unsigned int y_base, const unsigned int y_size);
void big_integer_base_subtract_short(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,  unsigned int* y_data, const unsigned int y_base, const unsigned int y_size);
void big_integer_base_powerMod(unsigned int * r_data, const unsigned int r_base, const unsigned int r_size, unsigned int * v_data, const unsigned int v_base, const unsigned int v_size, unsigned int * power_data, const unsigned int power_base, const unsigned int power_size, unsigned int * mod_data, const unsigned int mod_base, const unsigned int mod_size);
void big_integer_base_powerMod_interleaved(unsigned int * r_data, const unsigned int r_base, const unsigned int r_size, unsigned int * v_data, const unsigned int v_base, const unsigned int v_size, unsigned int * power_data, const unsigned int power_base, const unsigned int power_size, unsigned int * mod_data, const unsigned int mod_base, const unsigned int mod_size);
void big_integer_base_powerModMontgomery(unsigned int * r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* x_data, const unsigned int x_base, const unsigned int x_size, unsigned int* e_data, const unsigned int e_base, const unsigned int e_size, unsigned int* m_data, const unsigned int m_base, const unsigned int m_size, unsigned int* mprime_data, const unsigned int mprime_base, const unsigned int mprime_size, unsigned int* radix_data, const unsigned int radix_base, const unsigned int radix_size);
void big_integer_base_powerModMont(unsigned int * r_data, const unsigned int r_base, const unsigned int r_size, unsigned int* x_data, const unsigned int x_base, const unsigned int x_size, unsigned int* e_data, const unsigned int e_base, const unsigned int e_size,unsigned int* m_data, const unsigned int m_base, const unsigned int m_size);
void big_integer_base_initFromHexString(unsigned int * data, unsigned int base, unsigned int size, const char* str);
void big_integer_base_parseHexString(unsigned int* data, unsigned int base, unsigned int size, const char* str);
void big_integer_base_parseString(unsigned int * data, unsigned int base, unsigned int size, const char* str);
void big_integer_base_random(unsigned int* data, unsigned int base, unsigned int size);
void big_integer_base_random_bits(unsigned int* data, unsigned int base, unsigned int size, int n);
void big_integer_base_zeroHighBits(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, int fromBit);

const char* big_integer_base_toHexString(unsigned int* data, const unsigned int base, const unsigned int size);

#ifdef __cplusplus
}
#endif

#endif /* BIG_INTEGER_H */

