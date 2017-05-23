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

#include <string>

class BigNumBase;


/**
 * BigInteger contains an integer with variable precission
 */        
class BigInteger {
public:
    BigInteger();
    BigInteger(const BigInteger& orig);
    
    virtual ~BigInteger();
    
public:
    void initSize(int s);
    void reduceWorkingSize(int s);
    int getLength();
    int getNumBits();
    void zero();
    void setIntValue(unsigned int v);
    void copy(const BigInteger* orig);
    void copy(unsigned int* limbs);
    void parseString(const char* str);
    void initFromHexString(const char* str);
    void parseHexString(const char* str);
    int isLessThan(BigInteger* v);
    int isZero();
    int isOne();
    int isOdd();
    int isNegative();
    void inc();
    int getBit(int bitnum);
    
    std::string toString();
    std::string toHexString();
    
    
    
    
    

// non static operations    
public:
    void add(BigInteger* y);
    void div(BigInteger* m);
    void mod(BigInteger* m);
    void mult(BigInteger* m);
    void multMod(BigInteger* b, BigInteger* m);
    void random();
    void random(int bits);
    void range(int upper, int lower);
    void shiftLeft(int shift);
    void shiftRight(int shift);
    void squareMod(BigInteger* m);
    void subtract(BigInteger* y);
    void subtractMod(BigInteger* x, BigInteger* m);
    
    int binary_to_decimal(int start, int end);
    
// Static functions
public:
    static void add(BigInteger* r, BigInteger* a, BigInteger* b);
    static void addShifted(BigInteger* r, BigInteger* a, BigInteger* b, int shift);
    static void subtract(BigInteger* r, BigInteger* x, BigInteger* y);
    static void mult(BigInteger* r, BigInteger* a, BigInteger* b);
    static void multLow(BigInteger* r, BigInteger* a, BigInteger* b);
    static void mult_naive(BigInteger* r, BigInteger* a, BigInteger* b);
    static void mult_karatsuba(BigInteger* r, BigInteger* x, BigInteger* y);
    static void mult_karatsubaRecursive(BigInteger* r, BigInteger* x, BigInteger* y);
    static void multMod(BigInteger* r, BigInteger* a, BigInteger* b, BigInteger* mod);
    static void squareMod(BigInteger* r, BigInteger* v, BigInteger* mod);
    static void powerMod(BigInteger* r, BigInteger* v, BigInteger* p, BigInteger* mod);
    static void powerModSlidingWindow(BigInteger* r, BigInteger* v, BigInteger* exp, BigInteger* mod);
    
    
    //static void mod(BigInteger* r, BigInteger* v, BigInteger* mod);
    static void shiftLeft(BigInteger* r, BigInteger* a, int shift);
    static void shiftRight(BigInteger* r, BigInteger* a, int shift);
    static void div_naive(BigInteger* x, BigInteger* divisor, BigInteger* q, BigInteger* r);
    static void range(BigInteger* r, BigInteger* v, int upper, int lower);
    static void inverseMod(BigInteger* fr, BigInteger* x, BigInteger* mod);
    
    // Montgomery Functions
    static void toMontgomeryDomain(BigInteger* nprime, BigInteger* n, BigInteger* R, BigInteger* m);
    static void fromMontgomeryDomain(BigInteger* n, BigInteger* nprime, BigInteger* Rinv, BigInteger* m);
    static void multMontgomeryForm(BigInteger* r, BigInteger* x, BigInteger* y, BigInteger* m, BigInteger* mprime);
    static void multMontgomeryForm2(BigInteger* r, BigInteger* x, BigInteger* y, BigInteger* m, BigInteger* mprime);
    static void radixFromMontgomeryMod(BigInteger* radix, BigInteger* m);
    static void mprimeFromMontgomeryRadix(BigInteger* mprime, BigInteger* m, BigInteger* radix);
    static void powerModMontgomery(BigInteger* r, BigInteger* x, BigInteger* e, BigInteger* m, BigInteger* mprime, BigInteger* radix);


    static int isEqual(BigInteger* a, BigInteger* b);
    
    
    static int verbosity;
    static int extraChecks;
    
private:
    static int maxVal( int x,  int y);
    static void mult(unsigned int x, unsigned int y, unsigned int *rHight, unsigned int *rLow);

public:
    unsigned int* m_data;
    unsigned int m_size;
};

#endif	/* BIGINTEGER_H */

