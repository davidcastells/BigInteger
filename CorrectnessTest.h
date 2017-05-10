/* 
 * File:   CorrectnessTest.h
 * Author: dcr
 *
 * Created on April 24, 2017, 11:42 AM
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
    void testRange();
    void testShiftRight();
    void testShiftLeft();
    void testSquareMod();
    
    void checkResultMatchsExpected(BigInteger* r, BigInteger* expected);
    void checkMultMod(const char* msg, const char* a, const char* b, const char* m, const char* exp );
    void checkDivision(const char* msg, const char* a, const char* b, const char* eq, const char* er);
private:
    int stopAtFirstError;
};

#endif	/* CORRECTNESSTEST_H */

