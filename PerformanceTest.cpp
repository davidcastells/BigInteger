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
#include "big_integer_base.h"
#include "big_integer.h"

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
    testPerformanceAddC(); 
    testPerformanceAddCBase(); 
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
    
    int maxBits  = 4096*2;
    
    for (int bits = 32; bits <= maxBits; bits *= 2)
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

void PerformanceTest::testPerformanceAddCBase()
{
    double seconds;
    cout << "ADD)" << endl;
    cout << "Adding two random numbers (10k times)" << endl;
    cout << "Details;\tbits;\ttime;" << endl;
    
    int maxBits = 4096*2;
    
    unsigned int ba_data[maxBits/32];
    unsigned int ba_base = 0;
    unsigned int ba_size = maxBits/32;
    unsigned int bb_data[maxBits/32];
    unsigned int bb_base = 0;
    unsigned int bb_size = maxBits/32;
    unsigned int br_data[maxBits/32];
    unsigned int br_base = 0;
    unsigned int br_size = maxBits/32;
    
    for (int bits = 32; bits <= maxBits; bits *= 2)
    {
        PerformanceLap lap;
        
        ba_size = bits/32; 
        bb_size = bits/32;
        br_size = bits/32;
        big_integer_base_initSize(ba_data, ba_base, ba_size, bits/32); 
        big_integer_base_initSize(bb_data, bb_base, bb_size, bits/32); 
        big_integer_base_initSize(br_data, br_base, br_size, bits/32);
        big_integer_base_random(ba_data, ba_base, ba_size);
        big_integer_base_random(bb_data, bb_base, bb_size);
        
        lap.start();
        for (int rep=0; rep < 10000; rep++)
            big_integer_base_add(br_data, br_base, br_size,
                    ba_data, ba_base, ba_size, 
                    bb_data, bb_base, bb_size);
        seconds = lap.stop();
        
        cout << "AddCBase;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
}

void PerformanceTest::testPerformanceAddC()
{
    double seconds;
    cout << "ADD)" << endl;
    cout << "Adding two random numbers (10k times)" << endl;
    cout << "Details;\tbits;\ttime;" << endl;
    
    int maxBits = 4096*2;
    
    unsigned int ba_data[maxBits/32];
    unsigned int ba_size = maxBits/32;
    unsigned int bb_data[maxBits/32];
    unsigned int bb_size = maxBits/32;
    unsigned int br_data[maxBits/32];
    unsigned int br_size = maxBits/32;
    
    for (int bits = 32; bits <= maxBits; bits *= 2)
    {
        PerformanceLap lap;
        
        big_integer a;
        big_integer b;
        big_integer r;
        big_integer_init(&a, ba_data, bits/32); 
        big_integer_init(&b, bb_data, bits/32); 
        big_integer_init(&r, br_data, bits/32); 
        big_integer_base_random(a.m_data, 0, a.m_size);
        big_integer_base_random(b.m_data, 0, b.m_size);
        
        lap.start();
        for (int rep=0; rep < 10000; rep++)
            big_integer_add(&r, &a, &b);
        seconds = lap.stop();
        
        cout << "AddC;\t" << bits << ";\t" << seconds << ";" <<endl;
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
    int maxBits = 1024;
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
        
        // to avoid crashing montgomeryMult
        a.mod(&m);  
        b.mod(&m);
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::multMod(&r, &a, &b, &m);
        seconds = lap.stop();
        
        cout << "Modular Multiply Simple (c++);\t" << bits << ";\t" << seconds << ";" <<endl;
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            big_integer_base_multMod(r.m_data, 0, r.m_size, 
                a.m_data, 0, a.m_size,
                b.m_data, 0, b.m_size,
                m.m_data, 0, m.m_size);
        seconds = lap.stop();
        
        cout << "Modular Multiply Simple (base);\t" << bits << ";\t" << seconds << ";" <<endl;

    }
    
    
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
        r.initSize(bits/32+1);
        a.random();
        b.random();
        m.random();
        
        // to avoid crashing montgomeryMult
        a.mod(&m);  
        b.mod(&m);
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::multMod_interleaved(&r, &a, &b, &m);
        seconds = lap.stop();
        
        cout << "Modular Multiply Interleaved;\t" << bits << ";\t" << seconds << ";" <<endl;
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
        r.initSize(bits/32+1);
        a.random();
        b.random();
        
        do {m.random();} while (!m.isOdd());
        
        BigInteger::radixFromMontgomeryMod(&radix, &m);
        
//        BigInteger radixInv(radix);
//        BigInteger::inverseMod(&radixInv, &radix, &m);         // radixInv = radix ^ (-1) mod m
        
        BigInteger mprime(radix);
        BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);

        // to avoid crashing montgomeryMult
        a.mod(&m);  
        b.mod(&m);

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
        
//        BigInteger radixInv(radix);
//        BigInteger::inverseMod(&radixInv, &radix, &m);         // radixInv = radix ^ (-1) mod m
        
        BigInteger mprime(radix);
        BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);

        // to avoid crashing montgomeryMult
        a.mod(&m);  
        b.mod(&m);

        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::multMontgomeryForm2(&r, &a, &b, &m, &mprime);
        seconds = lap.stop();
        
        cout << "Montgomery Modular Multiply (2);\t" << bits << ";\t" << seconds << ";" <<endl;
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
        
//        BigInteger radixInv(radix);
//        BigInteger::inverseMod(&radixInv, &radix, &m);         // radixInv = radix ^ (-1) mod m
        
        BigInteger mprime(radix);
        BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);

        // to avoid crashing montgomeryMult
        a.mod(&m);  
        b.mod(&m);

        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::multMontgomeryForm3(&r, &a, &b, &m, &mprime);
        seconds = lap.stop();
        
        cout << "Montgomery Modular Multiply (3);\t" << bits << ";\t" << seconds << ";" <<endl;
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
        r.initSize(bits/32+1);
        a.random();
        b.random();
        m.random();
        
        BigInteger::radixFromMontgomeryMod(&radix, &m);
        
//        BigInteger radixInv(radix);
//        BigInteger::inverseMod(&radixInv, &radix, &m);         // radixInv = radix ^ (-1) mod m
        
        BigInteger mprime(radix);
        BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);

        // to avoid crashing montgomeryMult
        a.mod(&m);  
        b.mod(&m);

        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::montgomeryMult(&r, &a, &b, &m, &radix, mprime.m_data[0]);
        seconds = lap.stop();
        
        cout << "Montgomery Modular Multiply 1 limb mprime ;\t" << bits << ";\t" << seconds << ";" <<endl;
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
        r.initSize(bits/32+1);
        a.random();
        b.random();
        m.random();
        
        BigInteger::radixFromMontgomeryMod(&radix, &m);
        
//        BigInteger radixInv(radix);
//        BigInteger::inverseMod(&radixInv, &radix, &m);         // radixInv = radix ^ (-1) mod m
        
        BigInteger mprime(radix);
        BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);

        // to avoid crashing montgomeryMult
        a.mod(&m);  
        b.mod(&m);

        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::montgomeryMultBase2(&r, &a, &b, &m);
        seconds = lap.stop();
        
        cout << "Montgomery Modular Multiply base 2 ;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
}

void PerformanceTest::testPerformanceModPow()
{
    double seconds;
    double maxSeconds = 5;
    int numReps = 10;
    int maxBits = 4096;
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
        a.mod(&m);
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::powerModSlidingWindow(&r, &a, &e, &m);
        seconds = lap.stop();
        
        cout << "Modular Exponentiation (Sliding Window);\t" << bits << ";\t" << seconds << ";" <<endl;

        if (seconds > maxSeconds)
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
        
        a.mod(&m);
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::powerModMontgomery(&r2, &a, &e, &m, &mprime, &radix);
        seconds = lap.stop();
        
        cout << "Modular Exponentiation (Montgomery);\t" << bits << ";\t" << seconds << ";" <<endl;
        
        if (seconds > maxSeconds)
            bits = maxBits;
    }
    
    for (int bits = 32; bits <= maxBits; bits *= 2)
    {
        PerformanceLap lap;
     
        a.initSize(bits/32+1);
        e.initSize(bits/32+1);
        m.initSize(bits/32);
        r.initSize(bits/32+1);
        r2.initSize(bits/32+1);
        a.random();
        e.random();
        
        BigInteger radix;
        radix.initSize(m.m_size+1);
//        BigInteger mprime;
//        mprime.initSize(radix.m_size);        

        do
        {

            m.random();
//        cout << "m: " << m.toHexString() << endl;

            BigInteger::radixFromMontgomeryModBase2(&radix, &m);
        
            //BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);
        
        } while (!m.isOdd());
        
        a.mod(&m);
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::powerModMontgomeryBase2(&r2, &a, &e, &m, &radix);
        seconds = lap.stop();
        
        cout << "Modular Exponentiation (Montgomery Base2);\t" << bits << ";\t" << seconds << ";" <<endl;
        
        if (seconds > maxSeconds)
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
        a.mod(&m);
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::powerMod(&r, &a, &e, &m);
        seconds = lap.stop();
        
        cout << "Modular Exponentiation (Standard);\t" << bits << ";\t" << seconds << ";" <<endl;
        
        if (seconds > maxSeconds)
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
        
        a.mod(&m);
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::powerMod_ColinPlumb(&r2, &a, &e, &m); // , &mprime, &radix);
        seconds = lap.stop();
        
        cout << "Modular Exponentiation (Colin Plumb);\t" << bits << ";\t" << seconds << ";" <<endl;
        
        if (seconds > maxSeconds)
            bits = maxBits;
    }
}

void PerformanceTest::testPerformanceFinalModPow()
{
    double seconds;
    double maxSeconds = 10;
    
    int numReps = 100;
    int maxBits = 2048;
    
    cout << "MODULAR EXPONENTIATION)" << endl;
    cout << "Modular Exponentiation two random numbers ("<< (numReps)<<" times)" << endl;
    cout << "Details;\t\tbits;\ttime;" << endl;
    
    BigInteger a;
    BigInteger e;
    BigInteger m;
    BigInteger r, r2;
 
    
    for (int bits = 32; bits <= 256; bits *= 2)
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

        if (seconds > maxSeconds)
            bits = maxBits;
    }

    for (int bits = 32; bits <= 256; bits *= 2)
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

        if (seconds > maxSeconds)
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
        
        if (seconds > maxSeconds)
            bits = maxBits;
    }
    
    for (int bits = 32; bits <= maxBits; bits *= 2)
    {
        PerformanceLap lap;
     
        a.initSize(bits/32+1);
        e.initSize(bits/32+1);
        m.initSize(bits/32);
        r.initSize(bits/32+1);
        r2.initSize(bits/32+1);
        a.random();
        e.random();
        
        BigInteger radix;
        radix.initSize(m.m_size+1);
//        BigInteger mprime;
//        mprime.initSize(radix.m_size);        

        do
        {

            m.random();
//        cout << "m: " << m.toHexString() << endl;

            BigInteger::radixFromMontgomeryMod(&radix, &m);
        
            //BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);
        
        } while (!m.isOdd());
        
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::powerModMontgomeryBase2(&r2, &a, &e, &m, &radix);
        seconds = lap.stop();
        
        cout << "Montgomery Modular Exponentiation (Base 2);\t" << bits << ";\t" << seconds << ";" <<endl;
        
        if (seconds > maxSeconds)
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
        
        a.mod(&m);
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            BigInteger::powerMod_ColinPlumb(&r2, &a, &e, &m, &mprime, &radix);
        seconds = lap.stop();
        
        cout << "Modular Exponentiation (Colin Plumb);\t" << bits << ";\t" << seconds << ";" <<endl;
        
        if (seconds > maxSeconds)
            bits = maxBits;
    }
    
}