/* 
 * File:   PerformanceTest.cpp
 * Author: dcr
 * 
 * Created on April 24, 2017, 4:16 PM
 */

#include "PerformanceTest.h"
#include "PerformanceLap.h"
#include "BigInteger.h"
#include <iostream>

using namespace std;

PerformanceTest::PerformanceTest()
{
}

PerformanceTest::PerformanceTest(const PerformanceTest& orig)
{
}

PerformanceTest::~PerformanceTest()
{
}

/**
 * 
 */
void PerformanceTest::run()
{
   cout << "[Performance TESTS]" << endl;
    
   testPerformanceRange();
   testPerformanceAdd(); 
   testPerformanceSubtract(); 
   testPerformanceMult();
}

void PerformanceTest::testPerformanceRange()
{
    double seconds;
    cout << "RANGE)" << endl;
    cout << "Spliting two random numbers (10k times)" << endl;
    cout << "Details;\tbits;\ttime;" << endl;
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        BigInteger a;
        BigInteger b;
        BigInteger r;
        a.initSize(bits/32);
        b.initSize(bits/32);
        r.initSize(bits/32);
        a.random();
        b.random();
        
        lap.start();
        for (int rep=0; rep < 10000; rep++)
        {
            int cutPoint = bits/2 + 1;
            BigInteger::range(&b, &a, bits+1, cutPoint);
            BigInteger::range(&r, &a, cutPoint-1, 0);
        }
        seconds = lap.stop();
        
        cout << "Add;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
}

void PerformanceTest::testPerformanceAdd()
{
    double seconds;
    cout << "ADD)" << endl;
    cout << "Adding two random numbers (10k times)" << endl;
    cout << "Details;\tbits;\ttime;" << endl;
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        BigInteger a;
        BigInteger b;
        BigInteger r;
        a.initSize(bits/32);
        b.initSize(bits/32);
        r.initSize(bits/32);
        a.random();
        b.random();
        
        lap.start();
        for (int rep=0; rep < 10000; rep++)
            BigInteger::add(&r, &a, &b);
        seconds = lap.stop();
        
        cout << "Add;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
}


void PerformanceTest::testPerformanceSubtract()
{
    double seconds;
    cout << "SUBTRACT)" << endl;
    cout << "Subtracting two random numbers (10k times)" << endl;
    cout << "Details;\t\tbits;\ttime;" << endl;
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        BigInteger a;
        BigInteger b;
        BigInteger r;
        a.initSize(bits/32);
        b.initSize(bits/32);
        r.initSize(bits/32);
        a.random();
        b.random();
        
        lap.start();
        for (int rep=0; rep < 10000; rep++)
            BigInteger::subtract(&r, &a, &b);
        seconds = lap.stop();
        
        cout << "Subtract;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
}


void PerformanceTest::testPerformanceMult()
{
    double seconds;
    cout << "MULTIPLY)" << endl;
    cout << "Multiplying two random numbers (10k times)" << endl;
    cout << "Details;\t\tbits;\ttime;" << endl;
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        BigInteger a;
        BigInteger b;
        BigInteger r;
        a.initSize(bits/32);
        b.initSize(bits/32);
        r.initSize(bits/32);
        a.random();
        b.random();
        
        lap.start();
        for (int rep=0; rep < 10000; rep++)
            BigInteger::mult(&r, &a, &b);
        seconds = lap.stop();
        
        cout << "Multiply Simple;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        BigInteger a;
        BigInteger b;
        BigInteger r;
        a.initSize(bits/32);
        b.initSize(bits/32);
        r.initSize(bits/32);
        a.random();
        b.random();
        
        lap.start();
        for (int rep=0; rep < 10000; rep++)
            BigInteger::mult_karatsuba(&r, &a, &b);
        seconds = lap.stop();
        
        cout << "Multiply Karatsuba;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        BigInteger a;
        BigInteger b;
        BigInteger r;
        a.initSize(bits/32);
        b.initSize(bits/32);
        r.initSize(bits/32);
        a.random();
        b.random();
        
        lap.start();
        for (int rep=0; rep < 10000; rep++)
            BigInteger::mult_karatsubaRecursive(&r, &a, &b);
        seconds = lap.stop();
        
        cout << "Multiply Recursive Karatsuba ;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
}

