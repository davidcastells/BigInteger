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
    void testShiftRight();
    void testShiftLeft();
    void testRange();
    void testAdd();
    void testAddShifted();
    void testMult();
    void testMultMod();
    void testDiv();
    void testModBase();
    void testInverseMod();
    void testMultMontgomeryForm();
    void testParseNumbers();
    
    void checkResultMatchsExpected(BigInteger* r, BigInteger* expected);
    void checkMultMod(const char* msg, const char* a, const char* b, const char* m, const char* exp );
    void checkDivision(const char* msg, const char* a, const char* b, const char* eq, const char* er);
private:
    int stopAtFirstError;
};

#endif	/* CORRECTNESSTEST_H */

