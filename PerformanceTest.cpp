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
    
    BigInteger::extraChecks = 1;

    testPerformanceFinalModPow();
   
    cout << "[Complete]" << endl;

}

void PerformanceTest::runAll()
{
    cout << "[Performance TESTS]" << endl;
    
    BigInteger::extraChecks = 1;

    testPerformanceRange();
    testPerformanceAdd(); 
    testPerformanceSubtract(); 
    testPerformanceMult();
    testPerformanceModMult();
    testPerformanceModPow();
   
    cout << "[Complete]" << endl;

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
            BigInteger::range(&b, &a, bits-1, cutPoint);
            BigInteger::range(&r, &a, cutPoint-1, 0);
        }
        seconds = lap.stop();
        
        cout << "Range;\t" << bits << ";\t" << seconds << ";" <<endl;
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
        r.initSize((2*bits)/32);
        a.random();
        b.random();
        
        if (verbosity > 1)
        {
            cout << "a: " << a.toHexString() << endl;
            cout << "b: " << b.toHexString() << endl;
        }
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::mult_naive(&r, &a, &b);
        seconds = lap.stop();
        
        cout << "Multiply Naive;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        BigInteger a;
        BigInteger b;
        BigInteger r;
        a.initSize(bits/32);
        b.initSize(bits/32);
        r.initSize((2*bits)/32);
        a.random();
        b.random();
        
        if (verbosity > 1)
        {
            cout << "a: " << a.toHexString() << endl;
            cout << "b: " << b.toHexString() << endl;
        }
        
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
        r.initSize((2*bits)/32);
        a.random();
        b.random();
        
        if (verbosity > 1)
        {
            cout << "a: " << a.toHexString() << endl;
            cout << "b: " << b.toHexString() << endl;
        }
        
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
        r.initSize((2*bits)/32);
        a.random();
        b.random();
        
        if (verbosity > 1)
        {
            cout << "a: " << a.toHexString() << endl;
            cout << "b: " << b.toHexString() << endl;
        }
        
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
    int numReps = 100;
    int maxBits = 512;
    cout << "MODULAR MULTIPLY)" << endl;
    cout << "Modular Multiplying two random numbers ("<< (numReps/1000)<<"k times)" << endl;
    cout << "Details;\t\tbits;\ttime;" << endl;
    
    for (int bits = 32; bits <= maxBits; bits *= 2)
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
    int maxBits = 512;
    cout << "MODULAR EXPONENTIATION)" << endl;
    cout << "Modular Exponentiation two random numbers ("<< (numReps)<<" times)" << endl;
    cout << "Details;\t\tbits;\ttime;" << endl;
    
    BigInteger a;
    BigInteger e;
    BigInteger m;
    BigInteger r, r2;
 
    for (int bits = 32; bits <= maxBits; bits *= 2)
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
            BigInteger::powerModSlidingWindow(&r, &a, &e, &m);
        seconds = lap.stop();
        
        cout << "Sliding Window Modular Exponentiation;\t" << bits << ";\t" << seconds << ";" <<endl;

        if (seconds > 10)
            bits = maxBits;
    }    
    
    
    for (int bits = 32; bits <= maxBits; bits *= 2)
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
        
        if (seconds > 10)
            bits = maxBits;
    }
    
    for (int bits = 32; bits <= maxBits; bits *= 2)
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
        
        if (seconds > 10)
            bits = maxBits;

    }
    

}

void PerformanceTest::testPerformanceFinalModPow()
{
    double seconds;
    int numReps = 100;
    int maxBits = 2048;
    cout << "MODULAR EXPONENTIATION)" << endl;
    cout << "Modular Exponentiation two random numbers ("<< (numReps)<<" times)" << endl;
    cout << "Details;\t\tbits;\ttime;" << endl;
    
    BigInteger a;
    BigInteger e;
    BigInteger m;
    BigInteger r, r2;
 
    
    for (int bits = 32; bits <= maxBits; bits *= 2)
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
            BigInteger::powerModSlidingWindow(&r, &a, &e, &m);
        seconds = lap.stop();
        
        cout << "Sliding Window Modular Exponentiation;\t" << bits << ";\t" << seconds << ";" <<endl;

        if (seconds > 10)
            bits = maxBits;
    }

for (int bits = 32; bits <= maxBits; bits *= 2)
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

        m.random(bits-1);
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::powerMod_interleaved(&r, &a, &e, &m);
        seconds = lap.stop();
        
        cout << "Interleaved Modular Exponentiation;\t" << bits << ";\t" << seconds << ";" <<endl;

        if (seconds > 10)
            bits = maxBits;
    }    
    
    for (int bits = 32; bits <= maxBits; bits *= 2)
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
        
        if (seconds > 10)
            bits = maxBits;
    }
    
}