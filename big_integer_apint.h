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
 * File:   big_integer_apint.h
 * Author: dcr
 *
 * Created on April 8, 2018, 10:25 AM
 */

#ifndef BIG_INTEGER_APINT_H
#define BIG_INTEGER_APINT_H

#include "big_integer_base.h"
#include "ap_int.h"

#include <stdio.h>
#include <string.h>

#define NUM_BIG_INTEGER_APINT_BITS    2080    
#define NUM_BIG_INTEGER_APINT_LIMBS  (NUM_BIG_INTEGER_APINT_BITS/32)


extern int big_integer_apint_extraChecks;
extern int big_integer_apint_verbosity;
  

int big_integer_apint_getLength(ap_uint<NUM_BIG_INTEGER_APINT_BITS> apint);
int big_integer_apint_getBit(ap_uint<NUM_BIG_INTEGER_APINT_BITS> apint, int i);
int big_integer_apint_isOdd(ap_uint<NUM_BIG_INTEGER_APINT_BITS> v);
int big_integer_apint_isZero(ap_uint<NUM_BIG_INTEGER_APINT_BITS> v);

ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_mod(ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_x, ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_y);
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_montgomeryMultBase2(ap_uint<NUM_BIG_INTEGER_APINT_BITS> x, ap_uint<NUM_BIG_INTEGER_APINT_BITS> y, ap_uint<NUM_BIG_INTEGER_APINT_BITS> m);
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_multMod_interleaved(ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_a, ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_b, ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_mod); 
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_radixFromMontgomeryModBase2(ap_uint<NUM_BIG_INTEGER_APINT_BITS> m);
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_powerModMontgomeryBase2_noradix(ap_uint<NUM_BIG_INTEGER_APINT_BITS> x, ap_uint<NUM_BIG_INTEGER_APINT_BITS> e, ap_uint<NUM_BIG_INTEGER_APINT_BITS> m);
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_powerModMontgomeryBase2(ap_uint<NUM_BIG_INTEGER_APINT_BITS> x, ap_uint<NUM_BIG_INTEGER_APINT_BITS> e, ap_uint<NUM_BIG_INTEGER_APINT_BITS> m, ap_uint<NUM_BIG_INTEGER_APINT_BITS> radix);
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_powerModMontgomeryBase2_xprime(ap_uint<NUM_BIG_INTEGER_APINT_BITS> a, ap_uint<NUM_BIG_INTEGER_APINT_BITS> e, ap_uint<NUM_BIG_INTEGER_APINT_BITS> m, ap_uint<NUM_BIG_INTEGER_APINT_BITS> xprime);
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_squareMod(ap_uint<NUM_BIG_INTEGER_APINT_BITS> v,  ap_uint<NUM_BIG_INTEGER_APINT_BITS> m);
ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_squareMod_interleaved(ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_v, ap_uint<NUM_BIG_INTEGER_APINT_BITS> ap_m);

#endif /* BIG_INTEGER_APINT_H */

