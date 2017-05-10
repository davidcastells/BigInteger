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
   testPerformanceModMult();
   testPerformanceModPow();
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
    int numReps = 1000;
    cout << "MULTIPLY)" << endl;
    cout << "Multiplying two random numbers (" << (numReps/1000) << "k times)" << endl;
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
        for (int rep=0; rep < numReps; rep++)
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
        for (int rep=0; rep < numReps; rep++)
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
        for (int rep=0; rep < numReps; rep++)
            BigInteger::mult_karatsubaRecursive(&r, &a, &b);
        seconds = lap.stop();
        
        cout << "Multiply Recursive Karatsuba ;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
}

void PerformanceTest::testPerformanceModMult()
{
     
    double seconds;
    int numReps = 1000;
    cout << "MODULAR MULTIPLY)" << endl;
    cout << "Modular Multiplying two random numbers ("<< (numReps/1000)<<"k times)" << endl;
    cout << "Details;\t\tbits;\ttime;" << endl;
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        BigInteger a;
        BigInteger b;
        BigInteger m;
        BigInteger r;
        a.initSize(bits/32);
        b.initSize(bits/32);
        m.initSize(bits/32);
        r.initSize(bits/32);
        a.random();
        b.random();
        m.random();
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::multMod(&r, &a, &b, &m);
        seconds = lap.stop();
        
        cout << "Modular Multiply Simple;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
    
     
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        BigInteger a;
        BigInteger b;
        BigInteger m;
        BigInteger radix;
        BigInteger r;
        a.initSize(bits/32);
        b.initSize(bits/32);
        m.initSize(bits/32);
        radix.initSize(m.m_size+1);
        r.initSize(bits/32);
        a.random();
        b.random();
        m.random();
        
        BigInteger::radixFromMontgomeryMod(&radix, &m);
        
        BigInteger radixInv(radix);
        BigInteger::inverseMod(&radixInv, &radix, &m);         // radixInv = radix ^ (-1) mod m
        
        BigInteger mprime(radix);
        BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);


        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::multMontgomeryForm(&r, &a, &b, &m, &mprime);
        seconds = lap.stop();
        
        cout << "Montgomery Modular Multiply;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        BigInteger a;
        BigInteger b;
        BigInteger m;
        BigInteger radix;
        BigInteger r;
        a.initSize(bits/32);
        b.initSize(bits/32);
        m.initSize(bits/32);
        radix.initSize(m.m_size+1);
        r.initSize(bits/32);
        a.random();
        b.random();
        m.random();
        
        BigInteger::radixFromMontgomeryMod(&radix, &m);
        
        BigInteger radixInv(radix);
        BigInteger::inverseMod(&radixInv, &radix, &m);         // radixInv = radix ^ (-1) mod m
        
        BigInteger mprime(radix);
        BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);


        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::multMontgomeryForm2(&r, &a, &b, &m, &mprime);
        seconds = lap.stop();
        
        cout << "Montgomery Modular Multiply (2);\t" << bits << ";\t" << seconds << ";" <<endl;
    }
}

void PerformanceTest::testPerformanceModPow()
{
     
    double seconds;
    int numReps = 10;
    cout << "MODULAR EXPONENTIATION)" << endl;
    cout << "Modular Exponentiation two random numbers ("<< (numReps)<<" times)" << endl;
    cout << "Details;\t\tbits;\ttime;" << endl;
    
    BigInteger a;
    BigInteger e;
    BigInteger m;
    BigInteger r, r2;
 
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
     
        a.initSize(bits/32);
        e.initSize(bits/32);
        m.initSize(bits/32);
        r.initSize(bits/32);
        r2.initSize(bits/32);
        a.random();
        e.random();
        
        BigInteger radix;
        radix.initSize(m.m_size+1);
        BigInteger mprime;
        mprime.initSize(radix.m_size);        

        m.random();
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::powerMod(&r, &a, &e, &m);
        seconds = lap.stop();
        
        cout << "Standard Modular Exponentiation;\t" << bits << ";\t" << seconds << ";" <<endl;

    }
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
     
        a.initSize(bits/32);
        e.initSize(bits/32);
        m.initSize(bits/32);
        r.initSize(bits/32);
        r2.initSize(bits/32);
        a.random();
        e.random();
        
        BigInteger radix;
        radix.initSize(m.m_size+1);
        BigInteger mprime;
        mprime.initSize(radix.m_size);        

        do
        {

            m.random();
//        cout << "m: " << m.toHexString() << endl;

            BigInteger::radixFromMontgomeryMod(&radix, &m);
        
            BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);
        
        } while (mprime.isZero());
        
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::powerModMontgomery(&r2, &a, &e, &m, &mprime, &radix);
        seconds = lap.stop();
        
        cout << "Montgomery Modular Exponentiation;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
}