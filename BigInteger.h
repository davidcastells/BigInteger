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

#ifndef BIGINTEGER_H
#define	BIGINTEGER_H

#include <assert.h>
#include <errno.h>
#include <string.h>

#include <string>
#include <sstream> 
#include <iostream>

//class BigNumBase;

#define VERBOSITY_LEVEL_DIV             5
#define VERBOSITY_LEVEL_GET_LENGTH      6
#define VERBOSITY_LEVEL_MULT_KARATSUBA  5
#define VERBOSITY_LEVEL_MULT_MONTGOMERY 4
#define VERBOSITY_LEVEL_MONTGOMERY      3
#define VERBOSITY_LEVEL_POWER_MOD       2
#define VERBOSITY_LEVEL_INV_MOD         4
#define VERBOSITY_LEVEL_RANGE           6


/**
 * BigInteger contains an integer with variable precission
 */        
class BigInteger {
public:
    BigInteger();
    BigInteger(const BigInteger& orig);
    BigInteger(int size);
    
    virtual ~BigInteger();
    
public:
    void initSize(int s);
    void reduceWorkingSize(int s);
    int getLength();
    int getLimbLength();
    int getNumBits();
    void zero();
    void setIntValue(unsigned int v);
    void copy(const BigInteger* orig);
    void copy(unsigned int* limbs);
    void parseString(const char* str);
    void initFromHexString(const char* str);
    void parseHexString(const char* str);
    int isBiggerThan(BigInteger* v);
    int isLessThan(BigInteger* v);
    int isLessThanEqual(BigInteger* v);
    
    // bit manipulation 
    int isZero();
    int isOne();
    int isOdd();
    int isNegative();
    int getBit(int bitnum);
    int getLowestSetBit(); 

    std::string toString();
    std::string toHexString();
    
    
    
    
    

// non static operations    
public:
    void add(BigInteger* y);
    void add(unsigned int y);
    void div(BigInteger* m);
    void inc();
    int incLimb(int limb, unsigned int carry);
    void mod(BigInteger* m);
    void square();
    void mult(BigInteger* m);
    void mult(unsigned int digit);
    void multMod(BigInteger* b, BigInteger* m);
    void multMod_interleaved(BigInteger* b, BigInteger* mod);
    void random();
    void random(int bits);
    void range(int upper, int lower);
    void shiftLeft(int shift);
    void shiftRight(int shift);
    void squareMod(BigInteger* m);
    void squareMod_interleaved(BigInteger* m);
    void subtract(BigInteger* y);
    void subtractMod(BigInteger* x, BigInteger* m);
    
    int binary_to_decimal(int start, int end);
    
    // montgomery functions
    void montgomeryReduction(BigInteger* m, BigInteger* radix, unsigned int mprime);
    void montgomeryMult(BigInteger* y, BigInteger* m, BigInteger* radix, unsigned int mprime);
    void montgomeryMult(BigInteger* y, BigInteger* m, BigInteger* radix, BigInteger* mprime);
    void montgomerySquare(BigInteger* m, BigInteger* radix, unsigned int mprime);
    void montgomerySquare(BigInteger* m, BigInteger* radix, BigInteger* mprime);
    
// Static functions
public:
    static int isEqual(BigInteger* a, BigInteger* b);
    
    static void add(BigInteger* r, BigInteger* a, BigInteger* b);
    static void add(BigInteger* r, BigInteger* a, unsigned int b);
    static void addShifted(BigInteger* r, BigInteger* a, BigInteger* b, int shift);
    static void subtract(BigInteger* r, BigInteger* x, BigInteger* y);
    static void mult(BigInteger* r, BigInteger* a, BigInteger* b);
    static void mult(BigInteger* r, BigInteger* a, unsigned int digit);
    static void multLow(BigInteger* r, BigInteger* a, BigInteger* b);
    static void mult_naive(BigInteger* r, BigInteger* a, BigInteger* b);
    static void mult_karatsuba(BigInteger* r, BigInteger* x, BigInteger* y);
    static void mult_karatsubaRecursive(BigInteger* r, BigInteger* x, BigInteger* y);
    static void multMod(BigInteger* r, BigInteger* a, BigInteger* b, BigInteger* mod);
    static void multMod_interleaved(BigInteger* r, BigInteger* a, BigInteger* b, BigInteger* mod);
    static void squareMod(BigInteger* r, BigInteger* v, BigInteger* mod);
    static void squareMod_interleaved(BigInteger* r, BigInteger* v, BigInteger* m);
    static void powerMod(BigInteger* r, BigInteger* v, BigInteger* p, BigInteger* mod);
    static void powerMod_interleaved(BigInteger* r, BigInteger* v, BigInteger* p, BigInteger* mod);
    static void powerModSlidingWindow(BigInteger* r, BigInteger* v, BigInteger* exp, BigInteger* mod);
    static void powerMod_ColinPlumb(BigInteger* r, BigInteger* v, BigInteger* exp, BigInteger* mod);
    static void powerMod_ColinPlumb(BigInteger* r, BigInteger* x, BigInteger* exp, BigInteger* mod, BigInteger* mprime, BigInteger* radix);
    
    //static void mod(BigInteger* r, BigInteger* v, BigInteger* mod);
    static void shiftLeft(BigInteger* r, BigInteger* a, int shift);
    static void shiftRight(BigInteger* r, BigInteger* a, int shift);
    static void div_naive(BigInteger* x, BigInteger* divisor, BigInteger* q, BigInteger* r);
    static void mod_naive(BigInteger* x, BigInteger* y, BigInteger* r);
    static void range(BigInteger* r, BigInteger* v, int upper, int lower);
    static void inverseMod(BigInteger* fr, BigInteger* x, BigInteger* mod);
    
    // Montgomery Functions
    static void toMontgomeryDomain(BigInteger* nprime, BigInteger* n, BigInteger* R, BigInteger* m);
    static void fromMontgomeryDomain(BigInteger* n, BigInteger* nprime, BigInteger* Rinv, BigInteger* m);
    static void multMontgomeryForm(BigInteger* r, BigInteger* x, BigInteger* y, BigInteger* m, BigInteger* mprime);
    static void multMontgomeryForm2(BigInteger* r, BigInteger* x, BigInteger* y, BigInteger* m, BigInteger* mprime);
    static void multMontgomeryForm3(BigInteger* r, BigInteger* x, BigInteger* y, BigInteger* m, BigInteger* mprime);
    static void radixFromMontgomeryMod(BigInteger* radix, BigInteger* m);
    static void radixFromMontgomeryModBase2(BigInteger* radix, BigInteger* m);
    static void radixInvFromMontgomeryMod(BigInteger* radixInv, BigInteger* radix, BigInteger* mod);
    static void mprimeFromMontgomeryRadix(BigInteger* mprime, BigInteger* m, BigInteger* radix);
    static void powerModMontgomery(BigInteger* r, BigInteger* x, BigInteger* e, BigInteger* m);
    static void powerModMontgomeryBase2(BigInteger* r, BigInteger* x, BigInteger* e, BigInteger* m);
    static void powerModMontgomery(BigInteger* r, BigInteger* x, BigInteger* e, BigInteger* m, BigInteger* mprime, BigInteger* radix);
    static void powerModMontgomeryBase2(BigInteger* r, BigInteger* x, BigInteger* e, BigInteger* m, BigInteger* radix);
    static void montgomeryReduction(BigInteger* r, BigInteger* x, BigInteger* m, BigInteger* radix, unsigned int mprime);
    static void montgomeryMult(BigInteger* r, BigInteger* x, BigInteger* y, BigInteger* m, BigInteger* radix, unsigned int mprime);
    static void montgomeryMultBase2(BigInteger* r, BigInteger* x, BigInteger* y, BigInteger* m);
    
    static int verbosity;
    static int extraChecks;
    
private:
    static int maxVal( int x,  int y);
    static int minVal( int x,  int y);
    static void zeroHighBits(BigInteger* r, int fromBit);
    static void mult(unsigned int x, unsigned int y, unsigned int *rHight, unsigned int *rLow);

public:
    unsigned int* m_data;
    unsigned int m_size;
};

template <typename T>
std::string to_string(T value)
{
  //create an output string stream
  std::ostringstream os ;

  //throw the value into the string stream
  os << value ;

  //convert the string stream into a string and return
  return os.str() ;
}

template <typename T>
std::string to_hex_string(T value)
{
  //create an output string stream
  std::ostringstream os ;

  //throw the value into the string stream
  os << std::hex << value ;

  //convert the string stream into a string and return
  return os.str() ;
}

#endif	/* BIGINTEGER_H */

