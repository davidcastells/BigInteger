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

class CorrectnessTest {
public:
    CorrectnessTest();
    CorrectnessTest(const CorrectnessTest& orig);
    virtual ~CorrectnessTest();
    
public:
    void run();
    
    
    void testAdd();
    void testAddShifted();
    void testDiv();
    void testModBase();
    void testMult();
    void testMultMod();
    void testInverseMod();
    void testIsLessThan();
    void testMultMontgomeryForm();
    void testParseNumbers();
    void testPowerMod();
    void testRandom();
    void testRange();
    void testShiftRight();
    void testShiftLeft();
    void testSquareMod();
    
    void checkResultMatchsExpected(BigInteger* r, BigInteger* expected);
    void checkAddShifted(const char* msg, const char* sa, const char* sb, int shift, const char* sexp);
    void checkMult(const char* msg, const char* sa, const char* sb, const char* sexp);
    void checkMultMod(const char* msg, const char* a, const char* b, const char* m, const char* exp );
    void checkDivision(const char* msg, const char* a, const char* b, const char* eq, const char* er);
    void checkPowerMod(const char* msg, const char* sa, const char* se, const char* sm, const char* ser);
    
private:
    int stopAtFirstError;

public:
    int verbosity;
};

#endif	/* CORRECTNESSTEST_H */

