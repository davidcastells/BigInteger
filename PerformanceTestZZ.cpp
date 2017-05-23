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

#include "PerformanceTestZZ.h"
#include "PerformanceLap.h"
#include <iostream>
#include <NTL/ZZ.h>

NTL_CLIENT

using namespace std;


PerformanceTestZZ::PerformanceTestZZ()
{
}

PerformanceTestZZ::PerformanceTestZZ(const PerformanceTestZZ& orig)
{
}

PerformanceTestZZ::~PerformanceTestZZ()
{
}

void PerformanceTestZZ::run()
{
    cout << "[Performance TESTS ZZ]" << endl;
    
//    BigInteger::extraChecks = 1;

    testPerformanceFinalModPow();
   
    cout << "[Complete]" << endl;

}

/**
 * 
 */
void PerformanceTestZZ::runAll()
{
   cout << "[Performance TESTS ZZ]" << endl;
    
   testPerformanceRange();
   testPerformanceAdd(); 
   testPerformanceSubtract(); 
   testPerformanceMult();
   testPerformanceModMult();
   testPerformanceModPow();
}

void PerformanceTestZZ::testPerformanceRange()
{
    double seconds;
    cout << "RANGE)" << endl;
    cout << "Spliting two random numbers (10k times)" << endl;
    cout << "Details;\tbits;\ttime;" << endl;
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        ZZ a;
        ZZ b;
        ZZ r;
//        a.initSize(bits/32);
//        b.initSize(bits/32);
//        r.initSize(bits/32);
        RandomBits(a, bits);
        RandomBits(b, bits);
        
        lap.start();
        for (int rep=0; rep < 10000; rep++)
        {
            int cutPoint = bits/2 + 1;
            // do not know how to do range in NTL yet
//            BigInteger::range(&b, &a, bits+1, cutPoint);
//            BigInteger::range(&r, &a, cutPoint-1, 0);
        }
        seconds = lap.stop();
        
        cout << "Range;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
}

void PerformanceTestZZ::testPerformanceAdd()
{
    double seconds;
    cout << "ADD)" << endl;
    cout << "Adding two random numbers (10k times)" << endl;
    cout << "Details;\tbits;\ttime;" << endl;
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        ZZ a;
        ZZ b;
        ZZ r;
        RandomBits(a, bits);
        RandomBits(b, bits);        
        
        lap.start();
        for (int rep=0; rep < 10000; rep++)
            add(r, a, b);
        seconds = lap.stop();
        
        cout << "Add;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
}


void PerformanceTestZZ::testPerformanceSubtract()
{
    double seconds;
    cout << "SUBTRACT)" << endl;
    cout << "Subtracting two random numbers (10k times)" << endl;
    cout << "Details;\t\tbits;\ttime;" << endl;
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        ZZ a;
        ZZ b;
        ZZ r;
        RandomBits(a, bits);
        RandomBits(b, bits);        
        
        lap.start();
        for (int rep=0; rep < 10000; rep++)
            sub(r, a, b);
        seconds = lap.stop();
        
        cout << "Subtract;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
}


void PerformanceTestZZ::testPerformanceMult()
{
    double seconds;
    int numReps = 1000;
    cout << "MULTIPLY)" << endl;
    cout << "Multiplying two random numbers (" << (numReps/1000) << "k times)" << endl;
    cout << "Details;\t\tbits;\ttime;" << endl;
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        ZZ a;
        ZZ b;
        ZZ r;
        RandomBits(a, bits);
        RandomBits(b, bits);        
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            mul(r, a, b);
        seconds = lap.stop();
        
        cout << "Multiply ;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
    
}

void PerformanceTestZZ::testPerformanceModMult()
{
     
    double seconds;
    int numReps = 1000;
    cout << "MODULAR MULTIPLY)" << endl;
    cout << "Modular Multiplying two random numbers ("<< (numReps/1000)<<"k times)" << endl;
    cout << "Details;\t\tbits;\ttime;" << endl;
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
        
        ZZ a;
        ZZ b;
        ZZ m;
        ZZ r;
        RandomBits(a, bits);
        RandomBits(b, bits);        
        RandomBits(m, bits);        
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            MulMod(r, a, b, m);
        seconds = lap.stop();
        
        cout << "ZZ Modular Multiply Simple;\t" << bits << ";\t" << seconds << ";" <<endl;
    }
    
}

void PerformanceTestZZ::testPerformanceModPow()
{
     
    double seconds;
    int numReps = 10;
    cout << "MODULAR EXPONENTIATION)" << endl;
    cout << "Modular Exponentiation two random numbers ("<< (numReps)<<" times)" << endl;
    cout << "Details;\t\tbits;\ttime;" << endl;
    
    ZZ a;
    ZZ e;
    ZZ m;
    ZZ r, r2;
 
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
     
        RandomBits(a, bits);
        RandomBits(e, bits);
        
        ZZ radix;
        ZZ mprime;
        
        RandomBits(m, bits);
        
        a = a%m;
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            PowerMod(r, a, e, m);
        seconds = lap.stop();
        
        cout << "ZZ Modular Exponentiation;\t" << bits << ";\t" << seconds << ";" <<endl;

    }

}

void PerformanceTestZZ::testPerformanceFinalModPow()
{
    double seconds;
    int numReps = 100;
    int maxBits = 2048;
    cout << "MODULAR EXPONENTIATION)" << endl;
    cout << "Modular Exponentiation two random numbers ("<< (numReps)<<" times)" << endl;
    cout << "Details;\t\tbits;\ttime;" << endl;
    
   
 
    
//    for (int bits = 32; bits <= maxBits; bits *= 2)
//    {
//        PerformanceLap lap;
//     
//        a.initSize(bits/32);
//        e.initSize(bits/32);
//        m.initSize(bits/32);
//        r.initSize(bits/32);
//        r2.initSize(bits/32);
//        a.random();
//        e.random();
//        
//        BigInteger radix;
//        radix.initSize(m.m_size+1);
//        BigInteger mprime;
//        mprime.initSize(radix.m_size);        
//
//        m.random();
//        
//        lap.start();
//        for (int rep=0; rep < numReps; rep++)
//            BigInteger::powerModSlidingWindow(&r, &a, &e, &m);
//        seconds = lap.stop();
//        
//        cout << "Sliding Window Modular Exponentiation;\t" << bits << ";\t" << seconds << ";" <<endl;
//
//    }    
    
    ZZ a;
    ZZ e;
    ZZ m;
    ZZ r, r2;
 
    
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        PerformanceLap lap;
     
        RandomBits(a, bits);
        RandomBits(e, bits);
        
        ZZ radix;
        ZZ mprime;
        
        RandomBits(m, bits);
        
        a = a%m;
        
        lap.start();
        for (int rep=0; rep < numReps; rep++)
            PowerMod(r, a, e, m);
        seconds = lap.stop();
        
        cout << "ZZ Modular Exponentiation;\t" << bits << ";\t" << seconds << ";" <<endl;

    }
    
}