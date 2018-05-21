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

#ifndef CORRECTNESSTEST_H
#define	CORRECTNESSTEST_H

#include "BigInteger.h"
#include "big_integer_array.h"
#include "big_integer_apint_radix.h"

class CorrectnessTest {
public:
    CorrectnessTest();
    CorrectnessTest(const CorrectnessTest& orig);
    virtual ~CorrectnessTest();
    
public:
    void run();
    
    
    void testAdd();
    void testAddC();
    void testAddShifted();
    void testDiv();
    void testDivC();
    void testGetLength();
    void testMod();
    void testModC();
    void testModBase();
    void testMontgomeryReduction();
    void testMontgomeryMult();
    void testMult();
    void testMultC();
    void testMultMod();
    void testMultModC();
    void testInverseMod();
    void testIsBiggerThan();
    void testIsLessThan();
    void testIsLessThanC();
    void testIsLessThanEqual();
    void testIsNegative();
    void testMultMontgomeryForm();
    void testParseNumbers();
    void testParseNumbersC();
    void testPowerMod();
    void testPowerModC();
    void testRandom();
    void testRandomC();
    void testRange();
    void testSubtract();
    void testSubtractC();
    void testShiftRight();
    void testShiftLeft();
    void testSquareMod();
    void testSquareModC();
    
    void checkIsLessThanEqual(const char* msg, const char* sa, const char* sb, int exp);
    void checkShiftRight(const char* msg, const char* sa, int bits, const char* sexp);
    void checkShiftLeft(const char* msg, const char* sa, int bits, const char* sexp);
    void checkRandomValid(BigInteger* a, int expBits);
    void checkRandomValidC(unsigned int* a_data, unsigned int a_base, unsigned int a_size, int expBits);
    void checkResultMatchsExpected(int r, int expected);
    void checkResultMatchsExpected(BigInteger* r, BigInteger* expected);
    void checkResultMatchsExpectedApintRadix(limbs_radix_array r, unsigned int * exp_data, unsigned int exp_size);
    void checkResultMatchsExpectedArray(limbs_array r, limbs_array exp);
    void checkResultMatchsExpectedCBase(unsigned int * r_data, unsigned int r_base, unsigned int r_size, unsigned int * exp_data, unsigned int exp_base, unsigned int exp_size);
    void checkAdd(const char* msg, const char* sa, const char* sb, const char* sexp);
    void checkAddShifted(const char* msg, const char* sa, const char* sb, int shift, const char* sexp);
    void checkMult(const char* msg, const char* sa, const char* sb, const char* sexp);
    void checkMultC(const char* msg, const char* sa, const char* sb, const char* sexp);
    void checkMultMod(const char* msg, const char* a, const char* b, const char* m, const char* exp );
    void checkMultModC(const char* msg, const char* sa, const char* sb, const char* sm, const char* sexp );
    void checkDivision(const char* msg, const char* a, const char* b, const char* eq, const char* er);
    void checkDivisionC(const char* msg, const char* a, const char* b, const char* eq, const char* er);
    void checkGetLength(const char* msg, const char* sa, int exp);
    void checkPowerMod(const char* msg, const char* sa, const char* se, const char* sm, const char* ser);
    void checkPowerModC(const char* msg, const char* sa, const char* se, const char* sm, const char* ser);
    void checkRange(const char* msg, const char* sa, int upper, int lower, const char* sexp);
    void checkSubtract(const char* msg, const char* sa, const char* sb, const char* sexp);
    void checkSubtractC(const char* msg, const char* sa, const char* sb, const char* sexp);
    void checkSquareMod(const char* msg, const char* sa, const char* sm, const char* sexp);
    void checkInverseMod(const char* msg, const char* sa, const char* sm, const char* sexp);
    void checkMontgomeryReduction(const char* msg, const char* sx, const char* sradix, const char* sm, const char* sexp);
    void checkMontgomeryMult(const char* msg, const char* sx, const char* sy, const char* sm, const char* sexp);
    void checkMontgomeryMultBase2(const char* msg, const char* sx, const char* sy, const char* sm, const char* sexp);
    
    
private:
    int stopAtFirstError;

public:
    int verbosity;
};

#endif	/* CORRECTNESSTEST_H */

