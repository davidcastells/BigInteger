/* 
 * File:   CorrectnessTest.cpp
 * Author: dcr
 * 
 * Created on April 24, 2017, 11:42 AM
 */

#include "CorrectnessTest.h"
#include "BigInteger.h"
#include <iostream>

using namespace std;

CorrectnessTest::CorrectnessTest()
{
}

CorrectnessTest::CorrectnessTest(const CorrectnessTest& orig)
{
}

CorrectnessTest::~CorrectnessTest()
{
}

void CorrectnessTest::run()
{
    cout << "[Correctnes TESTS]" << endl;
    
    testAdd();
}

void CorrectnessTest::testAdd()
{
    BigInteger a;
    BigInteger b;
    BigInteger r;
    BigInteger expected;
    
    a.initSize(2048/32),
    b.initSize(2048/32);
    r.initSize(2048/32);
    expected.initSize(2048/32);
    
    a.parseString("8768279873802716238987346287098787657656763502221946787");
    b.parseString("1231230932483459873495094398734762765654543128761987338");
    expected.parseString("9999510806286176112482440685833550423311306630983934125");
    
    BigInteger::add(&r, &a, &b);
    
    cout << "Add (standard) ";
    
    if (BigInteger::isEqual(&r, &expected))
        cout << "[OK]" << endl;
    else
        cout << "## ERROR ##" << endl;
    
}