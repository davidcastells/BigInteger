/* 
 * File:   CorrectnessTest.cpp
 * Author: dcr
 * 
 * Created on April 24, 2017, 11:42 AM
 */

#include "CorrectnessTest.h"
#include "BigInteger.h"
#include <iostream>
#include <stdlib.h>

using namespace std;



CorrectnessTest::CorrectnessTest()
{
    stopAtFirstError = 1;
}

CorrectnessTest::CorrectnessTest(const CorrectnessTest& orig)
{
    stopAtFirstError = 1;
}

CorrectnessTest::~CorrectnessTest()
{
}

void CorrectnessTest::run()
{
    cout << "[Correctnes TESTS]" << endl;
    
    testShiftLeft();
    testShiftRight();
    testRange();
    testAdd();
    testAddShifted();
    testMult();
}

/**
 * 
 */
void CorrectnessTest::testShiftLeft()
{
    BigInteger a;
    BigInteger r;
    BigInteger expected;
    a.initSize(64/32),
    r.initSize(64/32);
    expected.initSize(64/32);
    a.parseHexString("ABCDEF9876543210");
    expected.parseHexString("7654321000000000");
    
    BigInteger::shiftLeft(&r, &a, 32);
    
    cout << "Shift Left " ;
    
    if (BigInteger::isEqual(&r, &expected))
        cout << "[OK]" << endl;
    else
    {
        cout << "## ERROR ##" << endl;
        cout << "Expected: " << expected.toHexString() << endl;
        cout << "Result: " << r.toHexString() << endl;
    }
}

/**
 * 
 */
void CorrectnessTest::testShiftRight()
{
    BigInteger a;
    BigInteger r;
    BigInteger expected;
    a.initSize(64/32),
    r.initSize(64/32);
    expected.initSize(64/32);
    a.parseHexString("ABCDEF9876543210");
    expected.parseHexString("ABCDEF98");
    
    BigInteger::shiftRight(&r, &a, 32);
    
    cout << "Shift Right " ;
    
    if (BigInteger::isEqual(&r, &expected))
        cout << "[OK]" << endl;
    else
    {
        cout << "## ERROR ##" << endl;
        cout << "Expected: " << expected.toHexString() << endl;
        cout << "Result: " << r.toHexString() << endl;
    }
}

void CorrectnessTest::testRange()
{
    BigInteger a;
    BigInteger r;
    BigInteger exp;
    a.initSize(64/32),
    r.initSize(32/32);
    exp.initSize(32/32);
    a.parseHexString("ABCDEF9876543210");
    
    exp.parseHexString("9876543210");
    BigInteger::range(&r, &a, 39, 0);
    int eq1 = BigInteger::isEqual(&r, &exp);
    
    cout << "Range " ;
    
    if (!eq1)
    {
        cout << "## ERROR ##" << endl;
        cout << "Expected: " << exp.toHexString() << endl;
        cout << "Result: " << r.toHexString() << endl;
    }
    
    exp.parseHexString("ABCDEF");
    BigInteger::range(&r, &a, 63, 40);
    int eq2 = BigInteger::isEqual(&r, &exp);
    
    if (!eq2)
    {
        cout << "## ERROR ##" << endl;
        cout << "Expected: " << exp.toHexString() << endl;
        cout << "Result: " << r.toHexString() << endl;
    }
    
    if (eq1 && eq1)
        cout << "[OK]" << endl;
}

void CorrectnessTest::testAdd()
{
    BigInteger a;
    BigInteger b;
    BigInteger r;
    BigInteger exp;
    
    a.initSize(2048/32),
    b.initSize(2048/32);
    r.initSize(2048/32);
    exp.initSize(2048/32);
    
    a.parseString("8768279873802716238987346287098787657656763502221946787");
    b.parseString("1231230932483459873495094398734762765654543128761987338");
    exp.parseString("9999510806286176112482440685833550423311306630983934125");
    
    BigInteger::add(&r, &a, &b);
    
    cout << "Add (standard) ";
    
    if (BigInteger::isEqual(&r, &exp))
        cout << "[OK]" << endl;
    else
    {
        cout << "## ERROR ##" << endl;
        cout << "Expected: " << exp.toHexString() << endl;
        cout << "Result: " << r.toHexString() << endl;
    }
    
}

/*
 *  z2 (5) = 5D89046E2103349A4BE7F542BB2B7720    00000000
  z1 (5) = A9E2ADA64B7DE037C3FD063AF92C4E94    00000000
 */

void CorrectnessTest::testAddShifted()
{
    BigInteger a;
    BigInteger b;
    BigInteger r;
    BigInteger exp;
    
    a.initSize(5),
    b.initSize(5);
    r.initSize(9);
    exp.initSize(9);
    
    a.parseHexString("A9E2ADA64B7DE037C3FD063AF92C4E94");
    b.parseHexString("5D89046E2103349A4BE7F542BB2B7720");
    exp.parseHexString("5D89046E2103349AF5CAA2E906A95757C3FD063AF92C4E94");
    
    BigInteger::addShifted(&r, &a, &b, 64);
    
    cout << "AddShifted ";
    
    if (BigInteger::isEqual(&r, &exp))
        cout << "[OK]" << endl;
    else
    {
        cout << "## ERROR ##" << endl;
        cout << "a: " << a.toHexString() << endl;
        cout << "b: " << b.toHexString() << endl;
        cout << "Expected: " << exp.toHexString() << endl;
        cout << "Result: " << r.toHexString() << endl;
        
        if (stopAtFirstError)
            exit(-1);
    }
    
}
/**
 * Test the multiplication 
 */
void CorrectnessTest::testMult()
{
    BigInteger a;
    BigInteger b;
    BigInteger r;
    BigInteger exp;
    
    a.initSize(512/32),
    b.initSize(512/32);
    r.initSize(512/32);
    exp.initSize(512/32);
    
    a.parseString("8768279873802716238987346287098787657656763502221946787");
    b.parseString("1231230932483459873495094398734762765654543128761987338");
    exp.parseString("10795777405298072177618168577285717325589251812726064155447348434996647761182520574284706593966188053403783006");
    
    cout << "Mult (standard) ";
    
    BigInteger::mult(&r, &a, &b);
    
    if (BigInteger::isEqual(&r, &exp))
        cout << "[OK]" << endl;
    else
    {
        cout << "## ERROR ##" << endl;
        cout << "Expected: " << exp.toHexString() << endl;
        cout << "Result: " << r.toHexString() << endl;
        cout << "Result: " << r.toString() << endl;
    }
    
    cout << "Mult (karatsuba) ";
    
    BigInteger::mult_karatsuba(&r, &a, &b);
    
    if (BigInteger::isEqual(&r, &exp))
        cout << "[OK]" << endl;
    else
    {
        cout << "## ERROR ##" << endl;
        cout << "Expected: " << exp.toHexString() << endl;
        cout << "Result: " << r.toHexString() << endl;
        cout << "Result: " << r.toString() << endl;
    }
    
    cout << "Mult (karatsuba recursive) ";
    
    BigInteger::mult_karatsubaRecursive(&r, &a, &b);
    
    if (BigInteger::isEqual(&r, &exp))
        cout << "[OK]" << endl;
    else
    {
        cout << "## ERROR ##" << endl;
        cout << "Expected: " << exp.toHexString() << endl;
        cout << "Result: " << r.toHexString() << endl;
        cout << "Result: " << r.toString() << endl;
    }
    
    
}