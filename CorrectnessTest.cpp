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

#include "CorrectnessTest.h"
#include "BigInteger.h"
#include "big_integer_base.h"
#include "big_integer_array.h"
#include "big_integer_apint.h"
#include "big_integer.h"
#include "assertf.h"
#include "big_integer_apint_radix.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <iostream>

using namespace std;



CorrectnessTest::CorrectnessTest()
{
    stopAtFirstError = 1;
    verbosity = 6;
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
    
    BigInteger::extraChecks = 1;

    const char* msg = "PowerMod (1)";
    const char* sa = "2E70041164DB2E1";
    const char* se = "23041164DB2E1";
    const char* sm = "55CE28B1";
    const char* sexp = "1D68E58E";
    
    {
        BigInteger a;
        BigInteger e;
        BigInteger m;
        BigInteger r, r2;

        a.initFromHexString(sa);
        e.initFromHexString(se);
        m.initFromHexString(sm);
        r.initFromHexString(sexp);
        r2.initSize(m.m_size+1);

    //    verbosity = 1;
        if (verbosity) cout << "a: " << a.toHexString() << endl;
        if (verbosity) cout << "e: " << e.toHexString() << endl;

        BigInteger radix;
        radix.initSize(m.m_size+1);
        BigInteger mprime;
        mprime.initSize(radix.m_size);        

        if (verbosity) cout << "m: " << m.toHexString() << endl;

        BigInteger::radixFromMontgomeryMod(&radix, &m);        
        BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);

        assert(!mprime.isZero());


        //BigInteger::verbosity = VERBOSITY_LEVEL_POWER_MOD+1;
        BigInteger::powerModMontgomery(&r2, &a, &e, &m, &mprime, &radix);

        if (verbosity)  cout << "r (mont): " << r2.toHexString() << endl;

        cout << msg << " (mont) " ;
        checkResultMatchsExpected(&r2, &r);
    }
    msg = "PowerModC (1)";
    
    unsigned int a_data[2048/32];
    unsigned int a_base = 0;
    unsigned int a_size = 2048/32;
    unsigned int e_data[2048/32];
    unsigned int e_base = 0;
    unsigned int e_size = 2048/32;
    unsigned int m_data[2048/32];
    unsigned int m_base = 0;
    unsigned int m_size = 2048/32;
    unsigned int exp_data[2048/32];
    unsigned int exp_base = 0;
    unsigned int exp_size = 2048/32;

    unsigned int r_data[2048/32];
    unsigned int r_base = 0;
    unsigned int r_size = 2048/32;
    
    unsigned int r2_data[2048/32];
    unsigned int r2_base = 0;
    unsigned int r2_size = 2048/32;
    
    big_integer_base_initFromHexString(a_data, a_base, a_size, sa);
    big_integer_base_initFromHexString(e_data, e_base, e_size, se);
    big_integer_base_initFromHexString(m_data, m_base, m_size, sm);
    big_integer_base_initFromHexString(exp_data, exp_base, exp_size, sexp);

        
    if (base_verbosity) cout << "a: " << big_integer_base_toHexString(a_data, a_base, a_size) << endl;
    if (base_verbosity) cout << "e: " << big_integer_base_toHexString(e_data, e_base, e_size) << endl;
    if (base_verbosity) cout << "m: " << big_integer_base_toHexString(m_data, m_base, m_size) << endl;
    
//    base_verbosity = VERBOSITY_LEVEL_POWER_MOD+1;

    big_integer r2;
    big_integer_init(&r2, r2_data, r2_size);
    big_integer a;
    big_integer_init(&a, a_data, a_size);
    big_integer e;
    big_integer_init(&e, e_data, e_size);
    big_integer m;
    big_integer_init(&m, m_data, m_size);
    
    
    unsigned int mprime_data[2048/32];
    unsigned int mprime_base = 0;
    unsigned int mprime_size = 2048/32;

    unsigned int radix_data[2048/32];
    unsigned int radix_base = 0;
    unsigned int radix_size = 2048/32;

    big_integer_base_radixFromMontgomeryMod(radix_data, radix_base, radix_size, m_data, m_base, m_size);
    
    big_integer_base_mprimeFromMontgomeryRadix(mprime_data, mprime_base, mprime_size, 
            m_data, m_base, m_size,
            radix_data, radix_base, radix_size);

    big_integer_base_powerModMontgomery(r2_data, r2_base, r2_size,
            a_data, a_base, a_size,
            e_data, e_base, e_size,
            m_data, m_base, m_size,
            mprime_data, mprime_base, mprime_size,
            radix_data, radix_base, radix_size);

    cout << msg << " (c base mont) " ;
    checkResultMatchsExpectedCBase(r2_data, r2_base, r2_size, exp_data, exp_base, exp_size);
    
    {
        big_integer r2;
        big_integer a;
        big_integer e;
        big_integer m;
        big_integer mprime;
        big_integer radix;

        big_integer_init(&r2, r2_data, r2_size);
        big_integer_init(&a, a_data, a_size);
        big_integer_init(&e, e_data, e_size);
        big_integer_init(&m, m_data, m_size);
        big_integer_init(&mprime, mprime_data, mprime_size);
        big_integer_init(&radix, radix_data, radix_size);

        big_integer_base_initFromHexString(a_data, a_base, a_size, sa);
        
//        big_integer_verbosity = VERBOSITY_LEVEL_POWER_MOD +1;
        big_integer_powerModMontgomery(&r2, &a, &e, &m, &mprime, &radix);
    
        cout << msg << " (c mont) " ;
        checkResultMatchsExpectedCBase(r2.m_data, 0, r2.m_size, exp_data, exp_base, exp_size);    
    }
    
    testGetLength();
    testRandom();
    testRandomC();
    testIsBiggerThan();
    testIsLessThan();
    testIsLessThanC();
    testIsLessThanEqual();
    testIsNegative();
    testParseNumbers();
    testParseNumbersC();
    testShiftLeft();
    testShiftRight();
    testRange();
    testAdd();
    testAddC();
    testAddShifted();
    testSubtract();
    testSubtractC();
    testMult();
    testMultC();
    testDiv();
    testDivC();
    testMod();
    testModC();
    testMultMod();
    testMultModC();
    testModBase();
    testInverseMod();
    testSquareMod();
    testSquareModC();

    testMontgomeryDomain();
    testMontgomeryReduction();
    testMontgomeryMult();
    testMultMontgomeryForm();

    testPowerMod();
    testPowerModC();
    
    BigInteger::extraChecks = 0;
    
    cout << "========================================" << endl;
}

void CorrectnessTest::checkResultMatchsExpected(int r, int expected)
{
    if (r == expected)
        cout << "[OK]" << endl;
    else
    {        
        cout << "## ERROR ##" << endl;

        cout << "Expected: " << expected << endl;
        cout << "Result: " << r << endl;
        
        if (stopAtFirstError)
            exit(-1);
    }
    
}


void CorrectnessTest::checkResultMatchsExpected(BigInteger* r, BigInteger* expected)
{
    if (BigInteger::isEqual(r, expected))
        cout << "[OK]" << endl;
    else
    {        
        cout << "## ERROR ##" << endl;

        cout << "Expected: " << expected->toHexString() << endl;
        cout << "Result: " << r->toHexString() << endl;
        
        if (stopAtFirstError)
            exit(-1);
    }
    
}

void CorrectnessTest::checkResultMatchsExpectedApintRadix(limbs_radix_array r, unsigned int * exp_data, unsigned int exp_size)
{
    limbs_radix_array e;
    big_integer_apint_radix_initfromLimbArray(e, exp_data, exp_size);
    
    if (big_integer_apint_radix_isEqual(r, e))
        cout << "[OK]" << endl;
    else
    {
        cout << "## ERROR ##" << endl;
        cout << "Expected: " << big_integer_apint_radix_toHexString(e) << endl;
        cout << "Result:   " << big_integer_apint_radix_toHexString(r) << endl;
        if (stopAtFirstError)
            exit(-1);
    }
}

void CorrectnessTest::checkResultMatchsExpectedArray(limbs_array r, limbs_array exp)
{
    checkResultMatchsExpectedCBase(r, 0, NUM_BIG_INTEGER_ARRAY_LIMBS, exp, 0, NUM_BIG_INTEGER_ARRAY_LIMBS);
}

void CorrectnessTest::checkResultMatchsExpectedCBase(unsigned int * r_data, unsigned int r_base, unsigned int r_size,
        unsigned int * exp_data, unsigned int exp_base, unsigned int exp_size)
{
    if (big_integer_base_base_isEqual(r_data, r_base, r_size, exp_data, exp_base, exp_size))
        cout << "[OK]" << endl;
    else
    {        
        cout << "## ERROR ##" << endl;
        cout << "Expected: " << big_integer_base_toHexString(exp_data, exp_base, exp_size) << endl;
        cout << "Result: " << big_integer_base_toHexString(r_data, r_base, r_size) << endl;
        
        if (stopAtFirstError)
            exit(-1);
    }
    
}



void CorrectnessTest::checkMultMod(const char* msg, const char* a, const char* b, const char* m, const char* exp )
{
    BigInteger ba, bb, bm, be;
    
    ba.initFromHexString(a);
    bb.initFromHexString(b);
    bm.initFromHexString(m);
    be.initFromHexString(exp);
    
    cout << msg << " (std) ";
    
    BigInteger r2;
    r2.initSize(bm.m_size+1);       // mult mod interleaved requires a bigger r
    
//    BigInteger::verbosity = 1;
    BigInteger::multMod(&r2, &ba, &bb, &bm);
    
    if (BigInteger::verbosity)
    {
        cout << " ba = " << ba.toHexString() << endl;
        cout << " bb = " << bb.toHexString() << endl;
        cout << " bm = " << bm.toHexString() << endl;
        cout << " r2 = " << r2.toHexString() << endl;
    }
    
    checkResultMatchsExpected(&r2, &be);
    
    cout << msg << " (interleaved) ";
    BigInteger::multMod_interleaved(&r2, &ba, &bb, &bm);
    checkResultMatchsExpected(&r2, &be);
    
    cout << msg << " (montgomery) ";
    BigInteger bradix(bm.m_size+1);
    BigInteger bradixInv(bm.m_size+1);
    BigInteger bmprime(bm.m_size);
    
    BigInteger::radixFromMontgomeryMod(&bradix, &bm);
    BigInteger::radixInvFromMontgomeryMod(&bradixInv, &bradix, &bm);
    BigInteger::mprimeFromMontgomeryRadix(&bmprime, &bm, &bradix);
    BigInteger baprime(bm.m_size);
    BigInteger bbprime(bm.m_size);
    BigInteger brprime(bm.m_size+1);
    BigInteger::toMontgomeryDomain(&baprime, &ba, &bradix, &bm);
    BigInteger::toMontgomeryDomain(&bbprime, &bb, &bradix, &bm);
    BigInteger::montgomeryMult(&brprime, &baprime, &bbprime, &bm, &bradix, bmprime.m_data[0]);
    BigInteger::fromMontgomeryDomain(&r2, &brprime, &bradixInv, &bm);
    checkResultMatchsExpected(&r2, &be);
}

void CorrectnessTest::checkMultModC(const char* msg, const char* sa, const char* sb, const char* sm, const char* sexp )
{
    const int bits = 2048;
    const int limbs = bits/32;
    const int index = 4;
    const int base = 1*limbs;
    
    unsigned int ba_data[index*limbs];
    unsigned int ba_base = base;
    unsigned int ba_size = limbs;
    unsigned int bb_data[index*limbs];
    unsigned int bb_base = base;
    unsigned int bb_size = limbs;
    unsigned int bm_data[index*limbs];
    unsigned int bm_base = base;
    unsigned int bm_size = limbs;
    unsigned int br_data[index*limbs];
    unsigned int br_base = base;
    unsigned int br_size = limbs;
    unsigned int exp_data[index*limbs];
    unsigned int exp_base = base;
    unsigned int exp_size = limbs;
    
    big_integer_base_initFromHexString(ba_data, ba_base, ba_size, sa);
    big_integer_base_initFromHexString(bb_data, bb_base, bb_size, sb);
    big_integer_base_initFromHexString(bm_data, bm_base, bm_size, sm);
    big_integer_base_initFromHexString(exp_data, exp_base, exp_size, sexp);
        
    cout << msg << " (std) ";
    
    
//    BigInteger::verbosity = 1;
    big_integer_base_multMod(br_data, br_base, br_size,
            ba_data, ba_base, ba_size,
            bb_data, bb_base, bb_size,
            bm_data, bm_base, bm_size);
    
//    if (BigInteger::verbosity)
//    {
//        cout << " ba = " << ba.toHexString() << endl;
//        cout << " bb = " << bb.toHexString() << endl;
//        cout << " bm = " << bm.toHexString() << endl;
//        cout << " r2 = " << r2.toHexString() << endl;
//    }
    
    checkResultMatchsExpectedCBase(br_data, br_base, br_size, exp_data, exp_base, exp_size);
    
    cout << msg << " (interleaved) ";
    big_integer_base_multMod_interleaved(br_data, br_base, br_size,
            ba_data, ba_base, ba_size,
            bb_data, bb_base, bb_size,
            bm_data, bm_base, bm_size);
    
    checkResultMatchsExpectedCBase(br_data, br_base, br_size, exp_data, exp_base, exp_size);

    
    cout << msg << " (array interleaved) ";
    unsigned int br[NUM_BIG_INTEGER_ARRAY_LIMBS];
    unsigned int ba[NUM_BIG_INTEGER_ARRAY_LIMBS];
    unsigned int bb[NUM_BIG_INTEGER_ARRAY_LIMBS];
    unsigned int bm[NUM_BIG_INTEGER_ARRAY_LIMBS];
    
    big_integer_array_init(ba, &ba_data[ba_base], ba_size);
    big_integer_array_init(bb, &bb_data[bb_base], bb_size);
    big_integer_array_init(bm, &bm_data[bm_base], bm_size);
    
    big_integer_array_multMod_interleaved(br, ba, bb, bm);
    
    checkResultMatchsExpectedCBase(br, 0, NUM_BIG_INTEGER_ARRAY_LIMBS, exp_data, exp_base, exp_size);
    
    limbs_radix_array bar_a;
    limbs_radix_array bar_b;
    limbs_radix_array bar_m;
    limbs_radix_array bar_r;
    
    big_integer_apint_radix_initfromLimbArray(bar_a, &ba_data[ba_base], ba_size);
    big_integer_apint_radix_initfromLimbArray(bar_b, &bb_data[ba_base], bb_size);
    big_integer_apint_radix_initfromLimbArray(bar_m, &bm_data[ba_base], bm_size);
    
    big_integer_apint_radix_multMod_interleaved(bar_r, bar_a, bar_b, bar_m);

    cout << msg << " (apint radix) ";    
    checkResultMatchsExpectedApintRadix(bar_r, &exp_data[exp_base], exp_size);

}

void CorrectnessTest::checkPowerMod(const char* msg, const char* sa, const char* se, const char* sm, const char* ser)
{
    BigInteger a;
    BigInteger e;
    BigInteger m;
    BigInteger r, r2;
 
    a.initFromHexString(sa);
    e.initFromHexString(se);
    m.initFromHexString(sm);
    r.initFromHexString(ser);
    r2.initSize(m.m_size+1);
        
//    verbosity = 1;
    if (verbosity) cout << "a: " << a.toHexString() << endl;
    if (verbosity) cout << "e: " << e.toHexString() << endl;

    BigInteger radix;
    radix.initSize(m.m_size+1);
    BigInteger mprime;
    mprime.initSize(radix.m_size);        

    if (verbosity) cout << "m: " << m.toHexString() << endl;

    BigInteger::radixFromMontgomeryMod(&radix, &m);        
    BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);
        
    assert(!mprime.isZero());

    BigInteger::powerMod_interleaved(&r2, &a, &e, &m);
        
    if (verbosity)  cout << "r (std): " << r2.toHexString() << endl;

    cout << msg << " (std) " ;
    checkResultMatchsExpected(&r2, &r);
            
//    BigInteger::verbosity = 3;
    BigInteger::powerModMontgomery(&r2, &a, &e, &m, &mprime, &radix);

    if (verbosity)  cout << "r (mont): " << r2.toHexString() << endl;

    cout << msg << " (mont) " ;
    checkResultMatchsExpected(&r2, &r);
    
    BigInteger::powerModMontgomery(&r2, &a, &e, &m);

    if (verbosity)  cout << "r (mont): " << r2.toHexString() << endl;

    cout << msg << " (mont simple) " ;
    checkResultMatchsExpected(&r2, &r);
    
    
    BigInteger::powerModSlidingWindow(&r2, &a, &e, &m);
    
    cout << msg << " (sliding window) " ;
    checkResultMatchsExpected(&r2, &r);
    
    
    BigInteger::powerMod_ColinPlumb(&r2, &a, &e, &m);
    
    cout << msg << " (colin plumb) " ;
    checkResultMatchsExpected(&r2, &r);
    
    
    BigInteger::powerModMontgomeryBase2(&r2, &a, &e, &m);
    
    cout << msg << " (montgomery base 2) " ;
    checkResultMatchsExpected(&r2, &r);
}

void CorrectnessTest::checkPowerModC(const char* msg, const char* sa, const char* se, const char* sm, const char* sexp)
{
    unsigned int a_data[2048/32];
    unsigned int a_base = 0;
    unsigned int a_size = 2048/32;
    unsigned int e_data[2048/32];
    unsigned int e_base = 0;
    unsigned int e_size = 2048/32;
    unsigned int m_data[2048/32];
    unsigned int m_base = 0;
    unsigned int m_size = 2048/32;
    unsigned int exp_data[2048/32];
    unsigned int exp_base = 0;
    unsigned int exp_size = 2048/32;

    unsigned int r_data[2048/32];
    unsigned int r_base = 0;
    unsigned int r_size = 2048/32;
    
    unsigned int r2_data[2048/32];
    unsigned int r2_base = 0;
    unsigned int r2_size = 2048/32;
    
    big_integer_base_initFromHexString(a_data, a_base, a_size, sa);
    big_integer_base_initFromHexString(e_data, e_base, e_size, se);
    big_integer_base_initFromHexString(m_data, m_base, m_size, sm);
    big_integer_base_initFromHexString(exp_data, exp_base, exp_size, sexp);

        
//    verbosity = 1;
    if (base_verbosity) cout << "a: " << big_integer_base_toHexString(a_data, a_base, a_size) << endl;
    if (base_verbosity) cout << "e: " << big_integer_base_toHexString(e_data, e_base, e_size) << endl;

//    unsigned int radix_data[2048/32];
//    unsigned int radix_base = 0;
//    unsigned int radix_size = 2048/32;
//    unsigned int mprime_data[2048/32];
//    unsigned int mprime_base = 0;
//    unsigned int mprime_size = 2048/32;
//    
//    
    if (base_verbosity) cout << "m: " << big_integer_base_toHexString(m_data, m_base, m_size) << endl;
//
//    big_integer_radixFromMontgomeryMod(&radix, &m);        
//    BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);
//        
//    assert(!mprime.isZero());

    big_integer_base_powerMod_interleaved(r2_data, r2_base, r2_size,
            a_data, a_base, a_size,
            e_data, e_base, e_size,
            m_data, m_base, m_size);
        
    if (base_verbosity)  cout << "r (base interleaved): " << big_integer_base_toHexString(r2_data, r2_base, r2_size) << endl;

    cout << msg << " (base interleaved) " ;
    checkResultMatchsExpectedCBase(r2_data, r2_base, r2_size, exp_data, exp_base, exp_size);

    big_integer r2;
    big_integer_init(&r2, r2_data, r2_size);
    big_integer a;
    big_integer_init(&a, a_data, a_size);
    big_integer e;
    big_integer_init(&e, e_data, e_size);
    big_integer m;
    big_integer_init(&m, m_data, m_size);
    
    big_integer_powerMod(&r2, &a, &e, &m);
    
    cout << msg << " (interleaved) " ;
    checkResultMatchsExpectedCBase(r2.m_data, 0, r2.m_size, exp_data, exp_base, exp_size);

    unsigned int mprime_data[2048/32];
    unsigned int mprime_base = 0;
    unsigned int mprime_size = 2048/32;

    unsigned int radix_data[2048/32];
    unsigned int radix_base = 0;
    unsigned int radix_size = 2048/32;

    big_integer_base_radixFromMontgomeryMod(radix_data, radix_base, radix_size, m_data, m_base, m_size);
    
    big_integer_base_mprimeFromMontgomeryRadix(mprime_data, mprime_base, mprime_size, 
            m_data, m_base, m_size,
            radix_data, radix_base, radix_size);

    //base_verbosity = 7;
    
    big_integer_base_powerModMontgomery(r2_data, r2_base, r2_size,
            a_data, a_base, a_size,
            e_data, e_base, e_size,
            m_data, m_base, m_size,
            mprime_data, mprime_base, mprime_size,
            radix_data, radix_base, radix_size);
//
//    if (verbosity)  cout << "r (mont): " << r2.toHexString() << endl;
//
    cout << msg << " (base mont) " ;
    checkResultMatchsExpectedCBase(r2_data, r2_base, r2_size, exp_data, exp_base, exp_size);

//    
//    BigInteger::powerModSlidingWindow(&r2, &a, &e, &m);
//    
//    cout << msg << " (sliding window) " ;
//    checkResultMatchsExpected(&r2, &r);
    
    big_integer_init(&r2, r2_data, r2_size);
    big_integer_init(&a, a_data, a_size);
    big_integer_init(&e, e_data, e_size);
    big_integer_init(&m, m_data, m_size);
    big_integer mprime;
    big_integer radix;
    big_integer_init(&mprime, mprime_data, mprime_size);
    big_integer_init(&radix, radix_data, radix_size);
    
    big_integer_powerModMontgomery(&r2, &a, &e, &m, &mprime, &radix);
    
    cout << msg << " (mont) " ;
    checkResultMatchsExpectedCBase(r2.m_data, 0, r2.m_size, exp_data, exp_base, exp_size);

    big_integer_powerModMont(&r2, &a, &e, &m);
    
    cout << msg << " (mont without mprime&radix) " ;
    checkResultMatchsExpectedCBase(r2.m_data, 0, r2.m_size, exp_data, exp_base, exp_size);
    
    unsigned int r3[NUM_BIG_INTEGER_ARRAY_LIMBS];
    unsigned int a3[NUM_BIG_INTEGER_ARRAY_LIMBS];
    unsigned int e3[NUM_BIG_INTEGER_ARRAY_LIMBS];
    unsigned int m3[NUM_BIG_INTEGER_ARRAY_LIMBS];
    
    big_integer_array_init(a3, a_data, a_size);
    big_integer_array_init(e3, e_data, e_size);
    big_integer_array_init(m3, m_data, m_size);
    
    big_integer_array_powerModMontgomery(r3, a3, e3, m3);
    
    cout << msg << " (mont array) " ;
    checkResultMatchsExpectedCBase(r3, 0, NUM_BIG_INTEGER_ARRAY_LIMBS, exp_data, exp_base, exp_size);

    big_integer_array_init(a3, a_data, a_size);
    big_integer_array_init(e3, e_data, e_size);
    big_integer_array_init(m3, m_data, m_size);
    
    big_integer_array_powerModMontgomeryBase2_noradix(r3, a3, e3, m3);
    
    cout << msg << " (mont array base2) " ;
    checkResultMatchsExpectedCBase(r3, 0, NUM_BIG_INTEGER_ARRAY_LIMBS, exp_data, exp_base, exp_size);
    
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> apa;
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> ape;
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> apm;
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> apr;
    
    big_integer_base_copy(apa.m_data, 0, NUM_BIG_INTEGER_APINT_LIMBS, a_data, 0, a_size);
    big_integer_base_copy(ape.m_data, 0, NUM_BIG_INTEGER_APINT_LIMBS, e_data, 0, e_size);
    big_integer_base_copy(apm.m_data, 0, NUM_BIG_INTEGER_APINT_LIMBS, m_data, 0, m_size);

    apr = big_integer_apint_powerModMontgomeryBase2_noradix(apa, ape, apm);
    
    cout << msg << " (mont apint base2) " ;
    checkResultMatchsExpectedCBase(apr.m_data, 0, NUM_BIG_INTEGER_APINT_LIMBS, exp_data, exp_base, exp_size);
    
}

void CorrectnessTest::checkDivision(const char* msg, const char* sa, const char* sb, const char* seq, const char* ser)
{
    BigInteger ba, bb, beq, ber;
    
    ba.initFromHexString(sa);
    bb.initFromHexString(sb);
    beq.initFromHexString(seq);
    ber.initFromHexString(ser);
    
//    cout << "beq = " << beq.toHexString() << endl;
    
    BigInteger bq(beq);
    BigInteger br(ber);
    
    BigInteger::div_naive(&ba, &bb, &bq, &br);
    
    cout << msg << " q (std) ";
    checkResultMatchsExpected(&bq, &beq);
    cout << msg << " r (std) ";
    checkResultMatchsExpected(&br, &ber);
    
    limbs_array la_a;
    limbs_array la_b;
    limbs_array la_q;
    limbs_array la_r;
    limbs_array la_eq;
    limbs_array la_er;
    
    big_integer_array_initFromHexString(la_a, sa);
    big_integer_array_initFromHexString(la_b, sb);
    big_integer_array_initFromHexString(la_eq, seq);
    big_integer_array_initFromHexString(la_er, ser);
    
    big_integer_array_div_naive(la_a, la_b, la_q, la_r);
    
    cout << msg << " q (array) ";
    checkResultMatchsExpectedArray(la_q, la_eq);
    cout << msg << " r (array) ";
    checkResultMatchsExpectedArray(la_r, la_er);
    
    
    limbs_radix_array lar_a;
    limbs_radix_array lar_b;
    limbs_radix_array lar_q;
    limbs_radix_array lar_r;
    
    big_integer_apint_radix_initfromLimbArray(lar_a, la_a, NUM_BIG_INTEGER_ARRAY_LIMBS);
    big_integer_apint_radix_initfromLimbArray(lar_b, la_b, NUM_BIG_INTEGER_ARRAY_LIMBS);
    
    big_integer_apint_radix_div_naive(lar_a, lar_b, lar_q, lar_r);
    
    cout << msg << " q (apint radix) ";
    checkResultMatchsExpectedApintRadix(lar_q, la_eq, NUM_BIG_INTEGER_ARRAY_LIMBS);
    cout << msg << " r (apint radix) ";
    checkResultMatchsExpectedApintRadix(lar_r, la_er, NUM_BIG_INTEGER_ARRAY_LIMBS);
}

void CorrectnessTest::checkDivisionC(const char* msg, const char* a, const char* b, const char* eq, const char* er)
{
    unsigned int ba_data[2048/32];
    unsigned int ba_base = 0;
    unsigned int ba_size = 2048/32;
    unsigned int bb_data[2048/32];
    unsigned int bb_base = 0;
    unsigned int bb_size = 2048/32;
    unsigned int beq_data[2048/32];
    unsigned int beq_base = 0;
    unsigned int beq_size = 2048/32;
    unsigned int ber_data[2048/32];
    unsigned int ber_base = 0;
    unsigned int ber_size = 2048/32;

    
    big_integer_base_initFromHexString(ba_data, ba_base, ba_size, a);
    big_integer_base_initFromHexString(bb_data, bb_base, bb_size, b);
    big_integer_base_initFromHexString(beq_data, beq_base, beq_size, eq);
    big_integer_base_initFromHexString(ber_data, ber_base, ber_size, er);
    
//    cout << "beq = " << beq.toHexString() << endl;
    
    unsigned int bq_data[2048/32];
    unsigned int bq_base = 0;
    unsigned int bq_size = 2048/32;
    
    unsigned int br_data[2048/32];
    unsigned int br_base = 0;
    unsigned int br_size = 2048/32;
    
//    BigInteger bq(beq);
//    BigInteger br(ber);
    
    big_integer_base_div_naive(ba_data, ba_base, ba_size,
            bb_data, bb_base, bb_size,
            bq_data, bq_base, bq_size,
            br_data, br_base, br_size);
    
    cout << msg << "base (q) ";
    checkResultMatchsExpectedCBase(bq_data, bq_base, bq_size, beq_data, beq_base, beq_size);
    cout << msg << "base (r) ";
    checkResultMatchsExpectedCBase(br_data, br_base, br_size, ber_data, ber_base, ber_size);
    
    
    big_integer ba;
    big_integer bb;
    big_integer bq;
    big_integer br;
    big_integer beq;
    big_integer ber;
    
    big_integer_init(&ba, ba_data, ba_size);
    big_integer_init(&bb, bb_data, bb_size);
    big_integer_init(&bq, bq_data, bq_size);
    big_integer_init(&br, br_data, br_size);
    big_integer_init(&beq, beq_data, beq_size);
    big_integer_init(&ber, ber_data, ber_size);
    
    big_integer_div_naive(&ba, &bb, &bq, &br);
    
    cout << msg << "(q) ";
    checkResultMatchsExpectedCBase(bq.m_data, 0, bq.m_size, beq.m_data, 0, beq.m_size);
    cout << msg << "(r) ";
    checkResultMatchsExpectedCBase(br.m_data, 0, br.m_size, ber.m_data, 0, ber.m_size);
}



void CorrectnessTest::testRange()
{    
    checkRange("Range a[39..0] ", "ABCDEF9876543210", 39, 0, "9876543210");
    checkRange("Range a[63..40] ", "E991945399FE030C3A91FD8E07F953A0", 63, 40, "3A91FD");
    checkRange("Range a[159..64] ", "E991945399FE030C3A91FD8E07F953A0", 127, 64,"E991945399FE030C");
    checkRange("Range a[63..0] ", "E991945399FE030C3A91FD8E07F953A0", 63, 0,"3A91FD8E07F953A0");
    checkRange("Range a[125..61] ", "3760E5AF3248DACA0000000000000001", 125, 61, "00000000000001BB072D799246D650");
    checkRange("Range a[1015..506] ", 
            "A5A2F020BC402A46AF7866A6D81251BC6282F3565F95B6C2A2BA2F9DBF56331F02ED5C8292104D2D03E575A240F9D8FCD3E4E5548FE4D9F9D4466B645A3515846D963E19490E86398A701E6682BB4BAF572680EFD38EC6339E2A567955723DFB7F24C3310D8818C619D79A5060C46CC8BE2456CF24EB68349170C61547B363",
            1015, 506,
            "2968BC082F100A91ABDE19A9B604946F18A0BCD597E56DB0A8AE8BE76FD58CC7C0BB5720A484134B40F95D68903E763F34F9395523F9367E75119AD9168D4561");
}

void CorrectnessTest::testAdd()
{
    BigInteger a;
    BigInteger b;
    BigInteger r;
    BigInteger exp;
    
    checkAdd("Add (1)", "000000000000000000000000", "00000000", "000000000000000000000000");
    checkAdd("Add (2)", "00000000", "000000000000000000000000", "000000000000000000000000");
    checkAdd("Add (3)", "30000000200000000", "5000000010000000F", "8000000030000000F");
    checkAdd("Add (4)", "87682798738027162389873", "12312309324834598734950", "99994AA1A5C85B6FAABE1C3");
    checkAdd("Add (5)", "8768279873802716238987346287098787657656763502221946787", "1231230932483459873495094398734762765654543128761987338", "99994AA1A5C85B6FAABE1C3DA61F7CCEE9DBCCAACA662A9832CDABF");    
}

void CorrectnessTest::testAddC()
{
    const int bits = 2048;
    const int limbs = bits/32;
    const int index = 4;
    const int base = 1*limbs;
    
    unsigned int a_data[index*limbs];
    unsigned int a_base = base;
    unsigned int a_size = limbs;
    unsigned int b_data[index*limbs];
    unsigned int b_base = base;
    unsigned int b_size = limbs;
    unsigned int r_data[index*limbs];
    unsigned int r_base = base;
    unsigned int r_size = limbs;
    unsigned int exp_data[index*limbs];
    unsigned int exp_base = base;
    unsigned int exp_size = limbs;
    
    big_integer_base_initFromHexString(a_data, a_base, a_size, "000000000000000000000010");
    big_integer_base_initFromHexString(b_data, b_base, b_size, "00000000");
    big_integer_base_initFromHexString(exp_data, exp_base, exp_size, "000000000000000000000010");

    big_integer_base_add(r_data, r_base, r_size, a_data, a_base, a_size, b_data, b_base, b_size);
    
    cout << "AddC (1) ";
    checkResultMatchsExpectedCBase(r_data, r_base, r_size, exp_data, exp_base, exp_size);
    
    big_integer_base_add(r_data, r_base, r_size, b_data, b_base, b_size, a_data, a_base, a_size);
    
    cout << "AddC (2) ";
    checkResultMatchsExpectedCBase(r_data, r_base, r_size, exp_data, exp_base, exp_size);

    big_integer_base_parseString(a_data, a_base, a_size, "8768279873802716238987346287098787657656763502221946787");
    big_integer_base_parseString(b_data, b_base, b_size, "1231230932483459873495094398734762765654543128761987338");
    big_integer_base_parseString(exp_data, exp_base, exp_size, "9999510806286176112482440685833550423311306630983934125");
    
    big_integer_base_add(r_data, r_base, r_size, a_data, a_base, a_size, b_data, b_base, b_size);
    
    cout << "AddC (3) ";
    checkResultMatchsExpectedCBase(r_data, r_base, r_size, exp_data, exp_base, exp_size);
    
    big_integer_base_copy(r_data, r_base, r_size, a_data, a_base, a_size);
    big_integer_base_add_short(r_data, r_base, r_size, b_data, b_base, b_size);
    
    cout << "AddC (4) ";
    checkResultMatchsExpectedCBase(r_data, r_base, r_size, exp_data, exp_base, exp_size);
}

void CorrectnessTest::testSubtract()
{
    checkSubtract("Subtract (1) ", "1DFA3AFCF64353A34027", "FBB5742CF", "1DFA3AFCF633984BFD58");
}

void CorrectnessTest::testSubtractC()
{
    checkSubtractC("SubtractC (1) ", "1DFA3AFCF64353A34027", "FBB5742CF", "1DFA3AFCF633984BFD58");
}


void CorrectnessTest::checkSubtract(const char* msg, const char* sa, const char* sb, const char* sexp)
{
    BigInteger a;
    BigInteger b;
    BigInteger r;
    BigInteger exp;
    
    a.initFromHexString(sa);
    b.initFromHexString(sb);
    r.initSize(a.m_size);
    exp.initFromHexString(sexp);
    
    cout << msg ;
    
    BigInteger::subtract(&r, &a, &b);
    
    checkResultMatchsExpected(&r, &exp);
}
    
void CorrectnessTest::checkSubtractC(const char* msg, const char* sa, const char* sb, const char* sexp)
{
    const int bits = 2048;
    const int limbs = bits/32;
    const int index = 4;
    const int base = 1*limbs;
    
    unsigned int a_data[index*limbs];
    unsigned int a_base = base;
    unsigned int a_size = limbs;
    unsigned int b_data[index*limbs];
    unsigned int b_base = base;
    unsigned int b_size = limbs;
    unsigned int r_data[index*limbs];
    unsigned int r_base = base;
    unsigned int r_size = limbs;
    unsigned int exp_data[index*limbs];
    unsigned int exp_base = base;
    unsigned int exp_size = limbs;
    
    big_integer_base_initFromHexString(a_data, a_base, a_size, sa);
    big_integer_base_initFromHexString(b_data, b_base, b_size, sb);
    big_integer_base_initFromHexString(exp_data, exp_base, exp_size, sexp);

    big_integer_base_subtract(r_data, r_base, r_size, a_data, a_base, a_size, b_data, b_base, b_size);
    
    cout << msg << " (std) ";
    checkResultMatchsExpectedCBase(r_data, r_base, r_size, exp_data, exp_base, exp_size);
    
    big_integer_base_copy(r_data, r_base, r_size, a_data, a_base, a_size);
    big_integer_base_subtract_short(r_data, r_base, r_size, b_data, b_base, b_size);
    
    cout << msg << " (short) ";
    checkResultMatchsExpectedCBase(r_data, r_base, r_size, exp_data, exp_base, exp_size);
    
    limbs_array la_a;
    limbs_array la_b;
    limbs_array la_r;

    cout << msg << " (array) ";
    big_integer_array_init(la_a, &a_data[a_base], a_size);
    big_integer_array_init(la_b, &b_data[b_base], b_size);

    big_integer_array_subtract(la_r, la_a, la_b);
    checkResultMatchsExpectedCBase(la_r, 0, NUM_BIG_INTEGER_ARRAY_LIMBS, exp_data, exp_base, exp_size);
    
    cout << msg << " (array short) ";
    big_integer_array_copy(la_r, la_a);
    big_integer_array_subtract_short(la_r, la_b);
    checkResultMatchsExpectedCBase(la_r, 0, NUM_BIG_INTEGER_ARRAY_LIMBS, exp_data, exp_base, exp_size);
    
    cout << msg << " (apint radix) ";
    limbs_radix_array lra_a;
    limbs_radix_array lra_b;
    limbs_radix_array lra_r;
    
    big_integer_apint_radix_initfromLimbArray(lra_a, &a_data[a_base], a_size);
    big_integer_apint_radix_initfromLimbArray(lra_b, &b_data[b_base], b_size);
 
    big_integer_apint_radix_subtract(lra_r, lra_a, lra_b);
    checkResultMatchsExpectedApintRadix(lra_r, &exp_data[exp_base], exp_size);
    
    cout << msg << " (apint radix short) ";
    big_integer_apint_radix_copy(lra_r, lra_a);
    big_integer_apint_radix_subtract_short(lra_r, lra_b);
    
    checkResultMatchsExpectedApintRadix(lra_r, &exp_data[exp_base], exp_size);

}

/*
 *  z2 (5) = 5D89046E2103349A4BE7F542BB2B7720    00000000
  z1 (5) = A9E2ADA64B7DE037C3FD063AF92C4E94    00000000
 */

void CorrectnessTest::testAddShifted()
{
    checkAddShifted("AddShifted (1) ", "A9E2ADA64B7DE037C3FD063AF92C4E94", "5D89046E2103349A4BE7F542BB2B7720", 64, "5D89046E2103349AF5CAA2E906A95757C3FD063AF92C4E94");
    checkAddShifted("AddShifted (2) ", "3E59A0D247B5578487953590E3CB13C134C510C90537E3", "498C7CC155A6D839E9463CF29ACA2330E635F0443D3", 96, "498C7CC155A6D839E94640D834D747AC3BAE38BD90890E3CB13C134C510C90537E3");
}

void CorrectnessTest::checkAddShifted(const char* msg, const char* sa, const char* sb, int shift, const char* sexp)
{
    BigInteger a;
    BigInteger b;
    BigInteger r;
    BigInteger exp;
    
    a.initFromHexString("A9E2ADA64B7DE037C3FD063AF92C4E94");
    b.initFromHexString("5D89046E2103349A4BE7F542BB2B7720");
    exp.initFromHexString("5D89046E2103349AF5CAA2E906A95757C3FD063AF92C4E94");
    r.initSize(exp.m_size);
    
    BigInteger::addShifted(&r, &a, &b, 64);
    
    cout << msg;
    
    checkResultMatchsExpected(&r, &exp);
    
}



/**
 * Test division
 * First we compute y = q * x + r, assuming that mult and add are correct
 * 
 * Then, we do 
 *      q' = y / x
 *      r' = y % x
 * 
 * ...and check q'=q , r'=r
 */
void CorrectnessTest::testDiv()
{
    cout << "Division and Modulo " << flush;
    
    int bits = 512;
    BigInteger y;
    BigInteger x;
    BigInteger q;
    BigInteger r;
    
    x.initSize(bits/32),
    y.initSize(2*bits/32);
    r.initSize(bits/32);
    q.initSize(bits/32);
    
    x.random();
    q.random();
    r.random(x.getLength()-1);  // ensure r < x
    
    BigInteger::mult(&y, &x, &q);
    y.add(&r);                   // y = q*x + r
    
    BigInteger qprime(q);
    BigInteger rprime(r);
    
    BigInteger::div_naive(&y, &x, &qprime, &rprime);
    
    if (!BigInteger::isEqual(&q, &qprime))
    {
        cout << "### ERROR ###" << endl;
    }
    else if (!BigInteger::isEqual(&r, &rprime))
    {
        cout << "### ERROR ###" << endl;
    }
    else
        cout << "[OK]" << endl;
    
    
    checkDivision("Division (1)", "55B1742F2AFBE68D", "77B71DFED43", "B73F3", "13B49F796F4");
    checkDivision("Division (2)", "1DFA3AFCF64353A34027", "FBB5742CF", "1E7D100ACF5", "2693D3C0C");
    checkDivision("Division (2)", "15B1742F2AFBE68D0000000000000000", "45CB977B71DFED43", "4f91587a190a3d8c", "a6653828e63485c");
    checkDivision("Division (3)", "3760E5AF3248DACA0000000000000001", "10000000000000000", "3760E5AF3248DACA", "000000000000000000001");
    
}

void CorrectnessTest::testDivC()
{
    cout << "DivisionC and ModuloC " << flush;
    
    const int bits = 128;
    const int index = 4;
    const int base = 1*bits/32;

    unsigned int x_data[index*bits/32];
    unsigned int x_base = base;
    unsigned int x_size = bits/32;
    unsigned int y_data[index*bits/32];
    unsigned int y_base = base;
    unsigned int y_size = bits/32;
    unsigned int q_data[index*bits/32];
    unsigned int q_base = base;
    unsigned int q_size = bits/32;
    unsigned int r_data[index*bits/32];
    unsigned int r_base = base;
    unsigned int r_size = bits/32;
    
    big_integer_base_random_bits(x_data, x_base, x_size, bits/2);
    big_integer_base_random_bits(q_data, q_base, q_size, bits/2);
    big_integer_base_random_bits(r_data, r_base, r_size, big_integer_base_getLength(x_data, x_base, x_size)-1);  // ensure r < x
    
    if (verbosity)
    {
        cout << endl;
        cout << " x = " << big_integer_base_toHexString(x_data, x_base, x_size) << " * " << endl;
        cout << " q = " << big_integer_base_toHexString(q_data, q_base, q_size) << " = " << endl;
    }
    
    big_integer_base_mult(y_data, y_base, y_size, x_data, x_base, x_size, q_data, q_base, q_size);

    if (verbosity)  
        cout << " y(1) = " <<big_integer_base_toHexString(y_data, y_base, y_size) << "+" << endl;
    
    big_integer_base_add_short(y_data, y_base, y_size, r_data, r_base, r_size);                   // y = q*x + r
    
    if (verbosity)
    {
        cout << " r = " <<big_integer_base_toHexString(r_data, r_base, r_size) << " = " << endl;
        cout << " y = " <<big_integer_base_toHexString(y_data, y_base, y_size) << endl;
    }
    
    unsigned int qp_data[index*bits/32];
    unsigned int qp_base = base;
    unsigned int qp_size = bits/32;
    unsigned int rp_data[index*bits/32];
    unsigned int rp_base = base;
    unsigned int rp_size = bits/32;

    big_integer_base_div_naive(y_data, y_base, y_size,
            x_data, x_base, x_size,
            qp_data, qp_base, qp_size, 
            rp_data, rp_base, rp_size);
    
    if (!big_integer_base_base_isEqual(q_data, q_base, q_size, qp_data, qp_base, qp_size))
    {
        cout << "### ERROR ### q" << endl;
        cout << " q' = " <<big_integer_base_toHexString(qp_data, qp_base, qp_size) << endl;
        cout << " r' = " <<big_integer_base_toHexString(rp_data, rp_base, rp_size) << endl;
        exit(-1);
    }
    else if (!big_integer_base_base_isEqual(r_data, r_base, r_size, rp_data, rp_base, rp_size))
    {
        cout << "### ERROR ### r" << endl;
        cout << " q' = " <<big_integer_base_toHexString(qp_data, qp_base, qp_size) << endl;
        cout << " r' = " <<big_integer_base_toHexString(rp_data, rp_base, rp_size) << endl;
        exit(-1);
    }
    else
        cout << "[OK]" << endl;
    
    
    checkDivisionC("DivisionC (1)", "1DFA3AFCF64353A34027", "FBB5742CF", "1E7D100ACF5", "2693D3C0C");
    checkDivisionC("DivisionC (1)", "55B1742F2AFBE68D", "77B71DFED43", "B73F3", "13B49F796F4");
    checkDivisionC("DivisionC (2)", "15B1742F2AFBE68D0000000000000000", "45CB977B71DFED43", "4f91587a190a3d8c", "a6653828e63485c");
    checkDivisionC("DivisionC (3)", "3760E5AF3248DACA0000000000000001", "10000000000000000", "3760E5AF3248DACA", "000000000000000000001");
    
}

void CorrectnessTest::testMod()
{
    cout << "Modulo " << flush;
    
    int bits = 512;
    BigInteger y;
    BigInteger x;
    BigInteger q;
    BigInteger r;
    
    x.initSize(bits/32),
    y.initSize(2*bits/32);
    r.initSize(bits/32);
    q.initSize(bits/32);
    
    x.random();
    q.random();
    r.random(x.getLength()-1);  // ensure r < x
    
    BigInteger::mult(&y, &x, &q);
    y.add(&r);                   // y = q*x + r
    
    BigInteger rprime(r);
    
    BigInteger::mod_naive(&y, &x, &rprime);
    
    if (!BigInteger::isEqual(&r, &rprime))
    {
        cout << "### ERROR ###" << endl;
    }
    else
        cout << "[OK]" << endl;
}

void CorrectnessTest::testModC()
{
    cout << "ModuloC " << flush;
    
    const int bits = 128;
    const int index = 4;
    const int base = 1*bits/32;

    unsigned int x_data[index*bits/32];
    unsigned int x_base = base;
    unsigned int x_size = bits/32;
    unsigned int y_data[index*bits/32];
    unsigned int y_base = base;
    unsigned int y_size = bits/32;
    unsigned int q_data[index*bits/32];
    unsigned int q_base = base;
    unsigned int q_size = bits/32;
    unsigned int r_data[index*bits/32];
    unsigned int r_base = base;
    unsigned int r_size = bits/32;
    
    big_integer_base_random_bits(x_data, x_base, x_size, bits/2);
    big_integer_base_random_bits(q_data, q_base, q_size, bits/2);
    big_integer_base_random_bits(r_data, r_base, r_size, big_integer_base_getLength(x_data, x_base, x_size)-1);  // ensure r < x
    
//    cout << endl;
//    cout << " x = " << big_integer_toHexString(x_data, x_base, x_size) << " * " << endl;
//    cout << " q = " << big_integer_toHexString(q_data, q_base, q_size) << " = " << endl;
    
    big_integer_base_mult(y_data, y_base, y_size, x_data, x_base, x_size, q_data, q_base, q_size);
//    cout << " y(1) = " <<big_integer_toHexString(y_data, y_base, y_size) << "+" << endl;
    
    big_integer_base_add_short(y_data, y_base, y_size, r_data, r_base, r_size);                   // y = q*x + r
    
//    cout << " r = " <<big_integer_toHexString(r_data, r_base, r_size) << " = " << endl;
//    cout << " y = " <<big_integer_toHexString(y_data, y_base, y_size) << endl;
    
    
    unsigned int rp_data[index*bits/32];
    unsigned int rp_base = base;
    unsigned int rp_size = bits/32;

    big_integer_base_mod_naive(y_data, y_base, y_size,
            x_data, x_base, x_size,
            rp_data, rp_base, rp_size);
    
    if (!big_integer_base_base_isEqual(r_data, r_base, r_size, rp_data, rp_base, rp_size))
    {
        cout << "### ERROR ### r" << endl;
        cout << " r' = " <<big_integer_base_toHexString(rp_data, rp_base, rp_size) << endl;
        exit(-1);
    }
    else
        cout << "[OK]" << endl;
    
    
}

void CorrectnessTest::checkInverseMod(const char* msg, const char* sa, const char* sm, const char* sexp)
{
    
    
    BigInteger a;
    a.initFromHexString(sa);
    
    BigInteger m;
    m.initFromHexString(sm);
    
    BigInteger exp;
    exp.initFromHexString(sexp);
    
    BigInteger ainv;
    ainv.initSize(exp.m_size);
    BigInteger::inverseMod(&ainv, &a, &m);
    
    cout << msg << " (std) ";
    checkResultMatchsExpected(&ainv, &exp);
    
    limbs_array la_a;
    limbs_array la_m;
    limbs_array la_exp;
    limbs_array la_r;
    
    big_integer_array_initFromHexString(la_a, sa);
    big_integer_array_initFromHexString(la_m, sm);
    big_integer_array_initFromHexString(la_exp, sexp);
    
    big_integer_array_inverseMod(la_r, la_a, la_m);
    
    cout << msg << " (array) ";
    checkResultMatchsExpectedArray(la_r, la_exp);
    
    limbs_radix_array lra_a;
    limbs_radix_array lra_m;
    limbs_radix_array lra_r;
    
    big_integer_apint_radix_initfromLimbArray(lra_a, la_a, NUM_BIG_INTEGER_ARRAY_LIMBS);
    big_integer_apint_radix_initfromLimbArray(lra_m, la_m, NUM_BIG_INTEGER_ARRAY_LIMBS);
    
    big_integer_apint_radix_inverseMod(lra_r, lra_a, lra_m);
    
    cout << msg << " (apint radix) ";
    checkResultMatchsExpectedApintRadix(lra_r, la_exp, NUM_BIG_INTEGER_ARRAY_LIMBS);
}

void CorrectnessTest::testInverseMod()
{
    checkInverseMod("Inverse Mod (1)", "35DA44F3", "42E6AC98", "427217EB");
    checkInverseMod("Inverse Mod (2)", "5463AE9E", "55CE28B1", "31D11B59");
    checkInverseMod("Inverse Mod (3)", "264564564564564564565464564611", "54654645645645654654654654645675467", "432FE85ECB07F46945F7ACFFADEB7CDD3DA");
    checkInverseMod("Inverse Mod (4)", "67F4258D", "10000000000000000", "D838C7E79076A545");
}

void CorrectnessTest::checkGetLength(const char* msg, const char* sa, int exp)
{
    BigInteger a;
    a.initFromHexString(sa);
    
    cout << msg << " (std) ";
    int len = a.getLength();
    
    checkResultMatchsExpected(len, exp);
    
    limbs_array la;
    
    big_integer_array_initFromHexString(la, sa);
    
    len = big_integer_array_getLength(la);
    
    cout << msg << " (array) ";
    checkResultMatchsExpected(len, exp);
    
    limbs_radix_array lra;
    
    big_integer_apint_radix_initfromLimbArray(lra, la, NUM_BIG_INTEGER_ARRAY_LIMBS);
    
//    cout << "lra = " << big_integer_apint_radix_toHexString(lra) << endl;
    len = big_integer_apint_radix_getLength(lra);
    
    cout << msg << " (apint radix) ";
    checkResultMatchsExpected(len, exp);
}

void CorrectnessTest::testGetLength()
{
    checkGetLength(" Len (1) ", "0", 0);
    checkGetLength(" Len (2) ", "1", 1);
    checkGetLength(" Len (3) ", "10000", 17);
    checkGetLength(" Len (4) ", "10000000000", 41);
    checkGetLength(" Len (5) ", "1DFA3AFCF64353A34027", 77);
    checkGetLength(" Len (6) ", "10000000000000000000000", 89);
}

/**
 * Test random number generator
 */
void CorrectnessTest::testRandom()
{
    BigInteger a;
    a.initSize(4);

    a.random();
    checkRandomValid(&a, a.getNumBits());
    a.random(32);
    checkRandomValid(&a, 32);
}

/**
 * Check that higher parts (16 most significant bits) of limbs are 
 *  not always zero. This happens when compiling with MINGW
 * @param a
 */
void CorrectnessTest::checkRandomValid(BigInteger* a, int expBits)
{
    int sum = 0;
    for (int i=0; i < a->m_size; i++)
    {
        sum += (a->m_data[i]>>16);   // sum the higher part of the random number
    }
    cout << "Random: " << a->toHexString() << " ";
    
    if (sum == 0)
    {
        cout << " ### ERROR ### higher part of limbs always zero" << endl;
        if (stopAtFirstError)
            exit(-1);
    }
    else
        cout << "[OK]" << endl;    
}

void CorrectnessTest::testRandomC()
{
    unsigned int a_data[4];
    unsigned int a_size = 4;
    unsigned int a_base = 0;
    
    big_integer_base_random(a_data, a_base, a_size);
    checkRandomValidC(a_data, a_base, a_size, 4*32);
    
    big_integer_base_random_bits(a_data, a_base, a_size, 32);
    checkRandomValidC(a_data, a_base, a_size, 32);
}

void CorrectnessTest::checkRandomValidC(unsigned int* a_data, unsigned int a_base, unsigned int a_size, int expBits)
{
    // Check that higher parts (16 most significant bits) of limbs are 
    // not always zero. This happens when compiling with MINGW
    int sum = 0;
    for (int i=0; i < a_size; i++)
    {
        sum += (a_data[i]>>16);   // sum the higher part of the random number
    }
    cout << "Random: " << big_integer_base_toHexString(a_data, a_base, a_size) << " ";
    
    if (sum == 0)
    {
        cout << " ### ERROR ### higher part of limbs always zero" << endl;
        if (stopAtFirstError)
            exit(-1);
    }
    else
        cout << "[OK]" << endl;

}

void CorrectnessTest::testIsLessThanEqual()
{
    checkIsLessThanEqual("<= (1)", "3A35825373ADDCE6", "000000010000000000000000", 1);
    checkIsLessThanEqual("<= (2)", "000000010000000000000000", "3A35825373ADDCE6", 0);
    checkIsLessThanEqual("<= (3)", "10000000000000000", "1458B19C9567F1F24", 1);
    checkIsLessThanEqual("<= (4)", "1458B19C9567F1F24", "10000000000000000", 0);
    checkIsLessThanEqual("<= (5)", "10000000000000000", "1A2C58CE4AB3F8F92", 1);
    checkIsLessThanEqual("<= (6)", "1A2C58CE4AB3F8F92", "10000000000000000", 0);
}

void CorrectnessTest::checkIsLessThanEqual(const char* msg, const char* sa, const char* sb, int exp)
{
    BigInteger a, b;
    a.initFromHexString(sa);
    b.initFromHexString(sb);
    
    cout << msg << " (std) " ;
    
    checkResultMatchsExpected(a.isLessThanEqual(&b), exp);

    
    limbs_array la_a;
    limbs_array la_b;
    
    big_integer_array_initFromHexString(la_a, sa);
    big_integer_array_initFromHexString(la_b, sb);
    
    cout << msg << " (array) " ;
    
    checkResultMatchsExpected(big_integer_array_isLessThanEqual(la_a, la_b), exp);
    
    limbs_radix_array lra_a;
    limbs_radix_array lra_b;
    
    big_integer_apint_radix_initfromLimbArray(lra_a, la_a, NUM_BIG_INTEGER_ARRAY_LIMBS);
    big_integer_apint_radix_initfromLimbArray(lra_b, la_b, NUM_BIG_INTEGER_ARRAY_LIMBS);

    cout << msg << " (apint radix) " ;
    
    checkResultMatchsExpected(big_integer_apint_radix_isLessThanEqual(lra_a, lra_b), exp);
    
}

void CorrectnessTest::testIsLessThan()
{
    BigInteger a, b;
    a.initFromHexString("3A35825373ADDCE6");
    b.initFromHexString("000000010000000000000000");
    
    cout << "IsLessThan (a<B)? ";
    
    if (a.isLessThan(&b))
        cout << "[OK]" << endl;
    else
        cout << "### ERROR ###" << endl;
    
    cout << "IsLessThan (B<a)? ";
    
    if (b.isLessThan(&a))
        cout << "### ERROR ###" << endl;
    else
        cout << "[OK]" << endl;
}

void CorrectnessTest::testIsBiggerThan()
{
    BigInteger a, b;
    a.initFromHexString("3A35825373ADDCE6");
    b.initFromHexString("000000010000000000000000");
    
    cout << "IsBiggerThan (B>a)? ";
    
    if (b.isBiggerThan(&a))
        cout << "[OK]" << endl;
    else
    {
        cout << "### ERROR ###" << endl;
        exit(-1);
    }
    
    cout << "IsBiggerThan (A>b)? ";
    
    if (a.isBiggerThan(&b))
    {
        cout << "### ERROR ###" << endl;
        exit(-1);
    }
    else
        cout << "[OK]" << endl;
}

void CorrectnessTest::testIsNegative()
{
    limbs_radix_array one;
    limbs_radix_array zero;
    limbs_radix_array neg;
    
    big_integer_apint_radix_setIntValue(one, 1);
    big_integer_apint_radix_setIntValue(zero, 0);
    
    big_integer_apint_radix_subtract(neg, zero, one);
    
    cout << "neg: " << big_integer_apint_radix_toHexString(neg) << endl;
    
    int isNeg = big_integer_apint_radix_isNegative(neg);
    
    cout << "Neg (apint radix) " ;
    checkResultMatchsExpected(isNeg, 1);
}

void CorrectnessTest::testIsLessThanC()
{
    const int bits = 2048;
    const int limbs = bits/32;
    const int index = 4;
    const int base = 1*limbs;
    const int arraySize = index * limbs;
    
    unsigned int a_data[arraySize];
    unsigned int a_base = base;
    unsigned int a_size = limbs;
    unsigned int b_data[arraySize];
    unsigned int b_base = base;
    unsigned int b_size = limbs;
    
    big_integer_base_initFromHexString(a_data, a_base, a_size, "3A35825373ADDCE6");
    big_integer_base_initFromHexString(b_data, b_base, b_size, "000000010000000000000000");
    
    
    cout << "IsLessThanC (a<B)? ";
    
    if (big_integer_base_isLessThan(a_data, a_base, a_size, b_data, b_base, b_size))
        cout << "[OK]" << endl;
    else
    {        
        cout << "### ERROR ###" << endl;
        exit(-1);
    }
    
    cout << "IsLessThan (B<a)? ";
    
    if (big_integer_base_isLessThan(b_data, b_base, b_size, a_data, a_base, a_size))
    {
        cout << "### ERROR ###" << endl;
        exit(-1);
    }
    else
        cout << "[OK]" << endl;
}

/**
 * We are working with a number system which is based on 2^32 numbers (limbs)
 * So, being b = 2^32, we try to prove that the operations with this numbers 
 * are actually equivalent to operations modulo b
 */
void CorrectnessTest::testModBase()
{
    cout << "Modulo based sum  ";
    
    // verify (x + y ) mod b = x mod b + y mod b
    unsigned int x = 0xF8938345;
    unsigned int y = 0xF8938345;
    unsigned int r = x + y;
    
    BigInteger bx;
    BigInteger by;
    BigInteger br;
    BigInteger bir;
    bx.initSize(3);
    by.initSize(3);
    br.initSize(3);
    bir.initSize(3);
    bx.parseHexString("F8938345");
    by.parseHexString("F8938345");
    BigInteger bb;
    bb.initSize(3);
    bb.parseHexString("100000000");
    
    BigInteger::add(&br, &bx, &by);
    br.mod(&bb);
    
    bir.setIntValue(r);
    
    if (BigInteger::isEqual(&br, &bir))
        cout << "[OK]" << endl; 
    else
    {
        cout << "### ERROR ###" << endl; 
        cout << " bir = " << bir.toHexString() << endl;
        cout << " br = " << br.toHexString() << endl;
    }
    
    
    cout << "Modulo based mult  ";
    
    r = x * y;
    
    br.initSize(bx.m_size + by.m_size);
    BigInteger::mult(&br, &bx, &by);
    br.mod(&bb);
    bir.setIntValue(r);
    
    if (BigInteger::isEqual(&br, &bir))
        cout << "[OK]" << endl; 
    else
    {
        cout << "### ERROR ###" << endl; 
        cout << " bir = " << bir.toHexString() << endl;
        cout << " br = " << br.toHexString() << endl;
    }
}

/**
 * Test the multiplication 
 */
void CorrectnessTest::testMult()
{
    checkMult("Mult (1)", "5B8B8C8C3DC2206684", "51E7C8116D4A26C669FE2A", "1D4A09EF733CFB077101819A3A5356EA232BC9A8");
    checkMult("Mult (1)", "16C45AF2414BDD05", "4B98092B71927474", "6B90C0BBD3F7C446BA6020638606A44");
    checkMult("Mult (2)", "5B8B8C8C3DC2206684A28191094E98A083E1912AE56FA3", "CDACA63DD6296E991945399FE030C3A8522C3A41BF10A", "498C7CC155A6D839E94640D834D747AC3BAE38BD908E8224402FE15DC19E8AE61A63C9FADE44991A9EDDD3FCF5E");
    checkMult("Mult (3)", "8C3DC2206684A28191094E98A083E1912AE56FA3", "CDACA63DD6296E991945399FE030C3A8522C3A41BF10A", "70AC09044BC7AE32F43AD3C90A7F13D5146014E6C048AFE15DC19E8AE61A63C9FADE44991A9EDDD3FCF5E");
    checkMult("Mult (4)", "5B8B8C8C3DC2206684A28191094E98A083E1912AE56FA3", "63DD6296E991945399FE030C3A8522C3A41BF10A", "23B62213F40B7EEEF0271F670D406D5A983B3472635F64FE15DC19E8AE61A63C9FADE44991A9EDDD3FCF5E");
    checkMult("Mult (5)", "400611DB4F6B3155309A0C3941095CC408A5162E7E26490C125C070B026825AB", "16B6559525E671F730060C65018944C014CD0ABE267158635824650A026B4FA7", "5AE1F40DBAE64FF72C651F8B56BEFE2751D10E095810C76C954C9765F21ED4FF230CDC57AED2B0D9CF21C373004944E873557784628E029AACDBF6DCA09578D");
}

void CorrectnessTest::testMultC()
{
    checkMultC("MultC (1)", "5B8B8C8C3DC2206684", "51E7C8116D4A26C669FE2A", "1D4A09EF733CFB077101819A3A5356EA232BC9A8");
    checkMultC("MultC (2)", "16C45AF2414BDD05", "4B98092B71927474", "6B90C0BBD3F7C446BA6020638606A44");
    checkMultC("MultC (3)", "5B8B8C8C3DC2206684A28191094E98A083E1912AE56FA3", "CDACA63DD6296E991945399FE030C3A8522C3A41BF10A", "498C7CC155A6D839E94640D834D747AC3BAE38BD908E8224402FE15DC19E8AE61A63C9FADE44991A9EDDD3FCF5E");
    checkMultC("MultC (4)", "8C3DC2206684A28191094E98A083E1912AE56FA3", "CDACA63DD6296E991945399FE030C3A8522C3A41BF10A", "70AC09044BC7AE32F43AD3C90A7F13D5146014E6C048AFE15DC19E8AE61A63C9FADE44991A9EDDD3FCF5E");
    checkMultC("MultC (5)", "5B8B8C8C3DC2206684A28191094E98A083E1912AE56FA3", "63DD6296E991945399FE030C3A8522C3A41BF10A", "23B62213F40B7EEEF0271F670D406D5A983B3472635F64FE15DC19E8AE61A63C9FADE44991A9EDDD3FCF5E");
    checkMultC("MultC (6)", "400611DB4F6B3155309A0C3941095CC408A5162E7E26490C125C070B026825AB", "16B6559525E671F730060C65018944C014CD0ABE267158635824650A026B4FA7", "5AE1F40DBAE64FF72C651F8B56BEFE2751D10E095810C76C954C9765F21ED4FF230CDC57AED2B0D9CF21C373004944E873557784628E029AACDBF6DCA09578D");
}

void CorrectnessTest::checkRange(const char* msg, const char* sa, int upper, int lower, const char* sexp)
{
    BigInteger a;
    BigInteger r;
    BigInteger exp;
    a.initFromHexString(sa);
    exp.initFromHexString(sexp);
    r.initSize(exp.m_size);
    
    cout << "Range a[" << to_string(upper) << ".." << to_string(lower) << "] " ;
    BigInteger::range(&r, &a, upper, lower);
    checkResultMatchsExpected(&r, &exp);    
    
    unsigned int a_data[a.m_size];
    unsigned int a_base = 0;
    unsigned int a_size = a.m_size;
    
    unsigned int r_data[r.m_size];
    unsigned int r_base = 0;
    unsigned int r_size = r.m_size;
    
    unsigned int exp_data[exp.m_size];
    unsigned int exp_base = 0;
    unsigned int exp_size = exp.m_size;

    big_integer_base_initFromHexString(a_data, a_base, a_size, sa);
    big_integer_base_initFromHexString(exp_data, exp_base, exp_size, sexp);

    cout << "Range BaseC a[" << to_string(upper) << ".." << to_string(lower) << "] " ;
    
    big_integer_base_range(r_data, r_base, r_size, a_data, a_base, a_size, upper, lower);

    checkResultMatchsExpectedCBase(r_data, r_base, r_size, exp_data, exp_base, exp_size);
    
    big_integer ba;
    big_integer br;
    
    big_integer_init(&ba, a_data, a_size);
    big_integer_init(&br, r_data, r_size);
    
    cout << "Range C a[" << to_string(upper) << ".." << to_string(lower) << "] " ;
    big_integer_range(&br, &ba, upper, lower);
        
    checkResultMatchsExpectedCBase(br.m_data, 0, br.m_size, exp_data, exp_base, exp_size);
    
    limbs_array la_a;
    limbs_array la_r;
    limbs_array la_exp;
    
    big_integer_array_initFromHexString(la_a, sa);
    big_integer_array_initFromHexString(la_exp, sexp);
    
    big_integer_array_range(la_r, la_a, upper, lower);
    
    cout << "Range array a[" << to_string(upper) << ".." << to_string(lower) << "] " ;
    checkResultMatchsExpectedArray(la_r, la_exp);
    
    limbs_radix_array lar_a;
    limbs_radix_array lar_r;
    
    big_integer_apint_radix_initfromLimbArray(lar_a, la_a, NUM_BIG_INTEGER_ARRAY_LIMBS);
    
    big_integer_apint_radix_range(lar_r, lar_a, upper, lower);
    
    cout << "Range apint radix a[" << to_string(upper) << ".." << to_string(lower) << "] " ;
    checkResultMatchsExpectedApintRadix(lar_r, la_exp, NUM_BIG_INTEGER_ARRAY_LIMBS);
}

void CorrectnessTest::checkAdd(const char* msg, const char* sa, const char* sb, const char* sexp)
{
    BigInteger a;
    BigInteger b;
    BigInteger r;
    BigInteger exp;
    
    a.initFromHexString(sa);
    b.initFromHexString(sb);
    exp.initFromHexString(sexp);
    r.initSize(exp.m_size);
    
    
    BigInteger::add(&r, &a, &b);
    
    cout << msg << "(standard) ";
    checkResultMatchsExpected(&r, &exp);
    
    unsigned int aa[NUM_BIG_INTEGER_ARRAY_LIMBS];
    unsigned int ab[NUM_BIG_INTEGER_ARRAY_LIMBS];
    unsigned int ar[NUM_BIG_INTEGER_ARRAY_LIMBS];
    
    big_integer_array_init(aa, a.m_data, a.m_size);
    big_integer_array_init(ab, b.m_data, b.m_size);
    
    big_integer_array_add(ar, aa, ab);
    
    cout << msg << "(array) ";
    checkResultMatchsExpectedCBase(ar, 0, NUM_BIG_INTEGER_ARRAY_LIMBS, exp.m_data, 0, exp.m_size);
    
    /// 
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> apa;
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> apb;
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> apr;
    
    big_integer_base_copy(apa.m_data, 0, NUM_BIG_INTEGER_APINT_LIMBS, a.m_data, 0, a.m_size);
    big_integer_base_copy(apb.m_data, 0, NUM_BIG_INTEGER_APINT_LIMBS, b.m_data, 0, b.m_size);
    
    apr = apa + apb;
    
    cout << msg << "(apint) ";
    checkResultMatchsExpectedCBase(apr.m_data, 0, NUM_BIG_INTEGER_APINT_LIMBS, exp.m_data, 0, exp.m_size);
    
    
    limbs_radix_array lra_a;
    limbs_radix_array lra_b;
    limbs_radix_array lra_r;
    
    big_integer_apint_radix_initfromLimbArray(lra_a, a.m_data, a.m_size);
    big_integer_apint_radix_initfromLimbArray(lra_b, b.m_data, b.m_size);
    
    big_integer_apint_radix_add(lra_r, lra_a, lra_b);
    
    cout << msg << "(apint radix) ";
    checkResultMatchsExpectedApintRadix(lra_r, exp.m_data, exp.m_size);
    
}

void CorrectnessTest::checkMult(const char* msg, const char* sa, const char* sb, const char* sexp)
{
    int bits = 1024;
    
    BigInteger a;
    BigInteger b;
    BigInteger r;
    BigInteger exp;
    
    a.initFromHexString(sa);
    b.initFromHexString(sb);
    r.initSize(a.m_size + b.m_size);
    exp.initFromHexString(sexp);
    
    cout << msg << "(standard) ";
    
    BigInteger::mult(&r, &a, &b);
    checkResultMatchsExpected(&r, &exp);
    
    cout << msg << "(naive) ";
    
    BigInteger::mult_naive(&r, &a, &b);
    checkResultMatchsExpected(&r, &exp);
    
    cout << msg << "(karatsuba) ";
    
    BigInteger::mult_karatsuba(&r, &a, &b);
    checkResultMatchsExpected(&r, &exp);
    
    cout << msg << "(karatsuba recursive) ";
//    BigInteger::verbosity = 5;
    
    BigInteger::mult_karatsubaRecursive(&r, &a, &b);
    checkResultMatchsExpected(&r, &exp);
    
}

void CorrectnessTest::checkMultC(const char* msg, const char* sa, const char* sb, const char* sexp)
{
    int bits = 1024;

    unsigned int a_data[bits/32];
    unsigned int a_base = 0;
    unsigned int a_size = bits/32;
    unsigned int b_data[bits/32];
    unsigned int b_base = 0;
    unsigned int b_size = bits/32;
    unsigned int r_data[bits/32];
    unsigned int r_base = 0;
    unsigned int r_size = bits/32;
    unsigned int exp_data[bits/32];
    unsigned int exp_base = 0;
    unsigned int exp_size = bits/32;
        
    big_integer_base_initFromHexString(a_data, a_base, a_size, sa);
    big_integer_base_initFromHexString(b_data, b_base, b_size, sb);
    big_integer_base_initFromHexString(exp_data, exp_base, exp_size, sexp);
    
    cout << msg << "(standard) " ;
    
//    cout << endl;
//    cout << "a = " << big_integer_toHexString(a_data, a_base, a_size) << endl;
//    cout << "b = " << big_integer_toHexString(b_data, b_base, b_size) << endl;
    
    big_integer_base_mult(r_data, r_base, r_size, a_data, a_base, a_size, b_data, b_base, b_size);
    checkResultMatchsExpectedCBase(r_data, r_base, r_size, exp_data, exp_base, exp_size);
    
    
    limbs_array bia_a;
    limbs_array bia_b;
    limbs_array bia_r;
    
    big_integer_array_initFromHexString(bia_a, sa);
    big_integer_array_initFromHexString(bia_b, sb);
    
    big_integer_array_mult(bia_r, bia_a, bia_b);
    
    cout << msg << "(array) " ;
    checkResultMatchsExpectedCBase(bia_r, 0, NUM_BIG_INTEGER_ARRAY_LIMBS, exp_data, exp_base, exp_size);
    
    limbs_radix_array bar_a;
    limbs_radix_array bar_b;
    limbs_radix_array bar_r;
    big_integer_apint_radix_initfromLimbArray(bar_a, a_data, a_size);
    big_integer_apint_radix_initfromLimbArray(bar_b, b_data, b_size);
    
    big_integer_apint_radix_mult(bar_r, bar_a, bar_b);
    
    cout << msg << "(apint radix) " ;
    checkResultMatchsExpectedApintRadix(bar_r, exp_data, exp_size);
}

void CorrectnessTest::testMultMod()
{
    checkMultMod("Mult Mod (same size) ", "3A3468848E2012BD", "2B62E85E55F7CD1A", "45CB977B71DFED43", "A6653828E63485C");
    checkMultMod("Mult Mod ", "00000000CB1F03567C39076B", "45CB977B71DFED43", "000000010000000000000001", "C89F1A50CDB72538");
}

void CorrectnessTest::testMultModC()
{
    checkMultModC("Mult Mod C (1) ", "3A3468848E2012BD", "2B62E85E55F7CD1A", "45CB977B71DFED43", "A6653828E63485C");
    checkMultModC("Mult Mod C (2)", "00000000CB1F03567C39076B", "45CB977B71DFED43", "000000010000000000000000", "00000001");
}

void CorrectnessTest::checkMontgomeryMultBase2(const char* msg, const char* sx, const char* sy, const char* sm, const char* sexp)
{
    BigInteger x, y, m, exp, r;
    x.initFromHexString(sx);
    y.initFromHexString(sy);
    m.initFromHexString(sm);
    exp.initFromHexString(sexp);
    
    r.initSize(m.m_size+1);
    
    cout << msg << " (own base 2)";
    BigInteger::montgomeryMultBase2(&r, &x, &y, &m);
    checkResultMatchsExpected(&r, &exp);
    
    unsigned int ar[NUM_BIG_INTEGER_ARRAY_LIMBS];
    unsigned int ax[NUM_BIG_INTEGER_ARRAY_LIMBS];
    unsigned int ay[NUM_BIG_INTEGER_ARRAY_LIMBS];
    unsigned int am[NUM_BIG_INTEGER_ARRAY_LIMBS];
    
    big_integer_array_init(ax, x.m_data, x.m_size);
    big_integer_array_init(ay, y.m_data, y.m_size);
    big_integer_array_init(am, m.m_data, m.m_size);
    
    cout << msg << " (array) ";
    big_integer_array_montgomeryMultBase2(ar, ax, ay, am);
    
    checkResultMatchsExpectedCBase(ar, 0, NUM_BIG_INTEGER_ARRAY_LIMBS, exp.m_data, 0, exp.m_size);
}

/**
 * 
 * @param msg
 * @param sx
 * @param sy
 * @param sm
 * @param sexp
 */
void CorrectnessTest::checkMontgomeryMult(const char* msg, const char* sx, const char* sy, 
        /*const char* sradix, */
        const char* sm, /*const char* smprime,*/ const char* sexp)
{
    
    BigInteger x, y, radix, m, mprime, exp, r;
    x.initFromHexString(sx);
    y.initFromHexString(sy);
    //radix.initFromHexString(sradix);
    m.initFromHexString(sm);
    
    radix.initSize(m.m_size + 1);
    mprime.initSize(m.m_size);
    //mprime.initFromHexString(smprime);
    exp.initFromHexString(sexp);
    
    BigInteger::radixFromMontgomeryMod(&radix, &m);
    BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);
    
    r.initSize(m.m_size+1);
    
    // make sure x < m
    x.mod(&m);
    y.mod(&m);
    
    
    cout << msg << " (own)";
//    verbosity = 5;
    BigInteger::montgomeryMult(&r, &x, &y, &m, &radix, mprime.m_data[0]);
    checkResultMatchsExpected(&r, &exp);
    
   
    
//    cout << msg << " (own2)";            
//    BigInteger::multMontgomeryForm(&r, &x, &y, &m, &mprime);
//    checkResultMatchsExpected(&r, &exp);
    
                
    cout << msg << " (v2) ";
    r.initSize(exp.m_size);
    BigInteger::multMontgomeryForm2(&r, &x, &y, &m, &mprime);
    checkResultMatchsExpected(&r, &exp);
    
//    BigInteger::verbosity = 4;
    cout << msg << " (v3) ";
    r.initSize(exp.m_size);
    BigInteger::multMontgomeryForm3(&r, &x, &y, &m, &mprime);
    checkResultMatchsExpected(&r, &exp);
    
//    base_verbosity = 5;
    big_integer_base_multMontgomeryForm3(r.m_data, 0, r.m_size,
            x.m_data, 0, x.m_size, 
            y.m_data, 0, y.m_size,
            m.m_data, 0, m.m_size,
            mprime.m_data, 0, mprime.m_size);
    cout << msg << " (base v3) ";
    checkResultMatchsExpectedCBase(r.m_data, 0, r.m_size, exp.m_data, 0, exp.m_size);

    
    limbs_array lx;
    limbs_array ly;
    limbs_array lm;
    limbs_array lmprime;
    limbs_array lr;
    big_integer_array_init(lx, x.m_data, x.m_size);
    big_integer_array_init(ly, y.m_data, y.m_size);
    big_integer_array_init(lm, m.m_data, m.m_size);
    big_integer_array_init(lmprime, mprime.m_data, mprime.m_size);
    
//    big_integer_array_verbosity = 5;
    big_integer_array_multMontgomeryForm3(lr, lx, ly, lm, lmprime);
    
    cout << msg << " (array v3) ";
    checkResultMatchsExpectedCBase(lr, 0, NUM_BIG_INTEGER_ARRAY_LIMBS, exp.m_data, 0, exp.m_size);
    
    
}

void CorrectnessTest::checkMontgomeryMultRadix(const char* msg, const char* sx, const char* sy, 
        /*const char* sradix, */
        const char* sm, /*const char* smprime,*/ const char* sexp)
{
    BigInteger x, y, radix, m, mprime, exp, r;
    x.initFromHexString(sx);
    y.initFromHexString(sy);
    //radix.initFromHexString(sradix);
    m.initFromHexString(sm);
    
    
    exp.initFromHexString(sexp);
    
    x.mod(&m);
    y.mod(&m);
    
    limbs_radix_array lra_x;
    limbs_radix_array lra_y;
    limbs_radix_array lra_m;
    limbs_radix_array lra_radix;
    limbs_radix_array lra_mprime;
    limbs_radix_array lra_r;
    big_integer_apint_radix_initfromLimbArray(lra_x, x.m_data, x.m_size);
    big_integer_apint_radix_initfromLimbArray(lra_y, y.m_data, y.m_size);
    big_integer_apint_radix_initfromLimbArray(lra_m, m.m_data, m.m_size);
    

    big_integer_apint_radix_radixFromMontgomeryMod(lra_radix, lra_m);
    
//    cout << "radix = " << big_integer_apint_radix_toHexString(lra_radix) << endl;
//    cout << "m = " << big_integer_apint_radix_toHexString(lra_m) << endl;
    
    big_integer_apint_radix_mprimeFromMontgomeryRadix(lra_mprime, lra_m, lra_radix);
    
//    cout << "mprime = " << big_integer_apint_radix_toHexString(lra_mprime) << endl;
    
    big_integer_apint_radix_multMontgomeryForm3(lra_r, lra_x, lra_y, lra_m, lra_mprime);
    
    cout << msg << " (apint radix v3) ";
    checkResultMatchsExpectedApintRadix(lra_r, exp.m_data, exp.m_size);
}

void CorrectnessTest::checkMontgomeryReduction(const char* msg, const char* sx,
        const char* sradix, const char* sm, const char* sexp)
{
    cout << msg ;
    
    BigInteger x;
    BigInteger radix;
    BigInteger m;
    BigInteger exp;
    x.initFromHexString(sx);
    radix.initFromHexString(sradix);
    m.initFromHexString(sm);
    exp.initFromHexString(sexp);
    
    BigInteger r(exp);
    
    BigInteger mprime(m);
    BigInteger::inverseMod(&mprime, &m, &radix);
    
    BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);
    
    BigInteger::montgomeryReduction(&r, &x, &m, &radix, mprime.m_data[0]);
    
    checkResultMatchsExpected(&r, &exp);
}

void CorrectnessTest::testMontgomeryMult()
{
    checkMontgomeryMult("Montgomery Mult (1)", "9099454", "8DC4DD61", "67F4258D", "2BBFCB53");
    checkMontgomeryMult("Montgomery Mult (2)", "7", "3", "11", "4");
    checkMontgomeryMult("Montgomery Mult (3)", "79", "13", "8D", "4C");
    checkMontgomeryMult("Montgomery Mult (4)", "454", "361", "8D", "4C");
    checkMontgomeryMult("Montgomery Mult (5)", "109099454", "18DC4DD60", "167F4258D",  "194AE914");
    checkMontgomeryMult("Montgomery Mult (6)", "109099454", "18DC4DD61", "167F4258D", "11771625B");
    checkMontgomeryMult("Montgomery Mult (7)", "2E70041164DB2E1", "170D6629", "55CE28B1", "ACA3D7A");
        checkMontgomeryMult("Montgomery Mult (9)", "0000000002E70041164DB2E1",  "0000000000000000170D6629",  "000000000000000055CE28B1",  "00000000000000000ACA3D7A");
    checkMontgomeryMult("Montgomery Mult (10)", "10D48FD309099454", "130D53B78DC4DD61", "136944F567F4258D", "649F7D66E457166");
    checkMontgomeryMult("Montgomery Mult (11)", "10D48FD3090994548ADE1F35", "130D53B78DC4DD61D5083F6A",  "136944F567F4258D3ED452B3", "63DE248A25BE0DC6583E290");
    checkMontgomeryMult("Montgomery Mult (12)", "10D48FD2090994548ADE1F35ECE7B10D", "130D53B78DC4DD61D5083F6A189DC74B",  "136944F567F4258D3ED452B297952DEB", "466230212C34EE9A6B265ED39DAEBB8");
    
    checkMontgomeryMultRadix("Montgomery Mult Radix (1)", "9099454", "8DC4DD61", "67F4258D", "1315B3E2");
    checkMontgomeryMultRadix("Montgomery Mult Radix (2)", "7", "3", "11", "4");
    checkMontgomeryMultRadix("Montgomery Mult Radix (3)", "79", "13", "8D", "D");
    checkMontgomeryMultRadix("Montgomery Mult Radix (4)", "454", "361", "8D", "D");
    checkMontgomeryMultRadix("Montgomery Mult Radix (5)", "109099454", "18DC4DD60", "167F4258D",  "194AE914");
    checkMontgomeryMultRadix("Montgomery Mult Radix (6)", "109099454", "18DC4DD61", "167F4258D", "11771625B");
    checkMontgomeryMultRadix("Montgomery Mult Radix (7)", "2E70041164DB2E1", "170D6629", "55CE28B1", "2AEB896C");
    checkMontgomeryMultRadix("Montgomery Mult Radix (9)", "0000000002E70041164DB2E1",  "0000000000000000170D6629",  "000000000000000055CE28B1",  "000000000000000002AEB896C");
    checkMontgomeryMultRadix("Montgomery Mult Radix (10)", "10D48FD309099454", "130D53B78DC4DD61", "136944F567F4258D", "649F7D66E457166");
    checkMontgomeryMultRadix("Montgomery Mult Radix (11)", "10D48FD3090994548ADE1F35", "130D53B78DC4DD61D5083F6A",  "136944F567F4258D3ED452B3", "C85FCF32203C89684837979");
    checkMontgomeryMultRadix("Montgomery Mult Radix (12)", "10D48FD2090994548ADE1F35ECE7B10D", "130D53B78DC4DD61D5083F6A189DC74B",  "136944F567F4258D3ED452B297952DEB", "466230212C34EE9A6B265ED39DAEBB8");
    
    checkMontgomeryMultBase2("Montgomery Mult Base 2 (1)", "7", "3", "11", "F");
    checkMontgomeryMultBase2("Montgomery Mult Base 2 (2)", "9099454", "B0D3A7B",  "67F4258D", "3140844B");
//    checkMontgomeryMultBase2("Montgomery Mult (3)", "79", "13", "100000000", "8D", "32D63DBB", "4C");
//    checkMontgomeryMultBase2("Montgomery Mult (4)", "454", "361", "100000000", "8D", "32D63DBB", "4C");
//    checkMontgomeryMultBase2("Montgomery Mult (5)", "109099454", "18DC4DD60", "10000000000000000", "167F4258D", "8E153CB16F895ABB", "194AE914");
//    checkMontgomeryMultBase2("Montgomery Mult (6)", "109099454", "18DC4DD61", "10000000000000000", "167F4258D", "8E153CB16F895ABB", "11771625B");
//    checkMontgomeryMultBase2("Montgomery Mult (7)", "2E70041164DB2E1", "170D6629", "100000000", "55CE28B1", "94A0DFAF", "ACA3D7A");
//    checkMontgomeryMultBase2("Montgomery Mult (8)", "2E70041164DB2E1", "170D6629", "100000000", "55CE28B1", "00000094A0DFAF", "ACA3D7A");
//    checkMontgomeryMultBase2("Montgomery Mult (9)", "0000000002E70041164DB2E1",  "0000000000000000170D6629", "000000000000000100000000",  "000000000000000055CE28B1", "000000000000000094A0DFAF", "00000000000000000ACA3D7A");
//    checkMontgomeryMultBase2("Montgomery Mult (10)", "10D48FD309099454", "130D53B78DC4DD61", "10000000000000000", "136944F567F4258D", "76B42856F895ABB","649F7D66E457166");
//    checkMontgomeryMultBase2("Montgomery Mult (11)", "10D48FD3090994548ADE1F35", "130D53B78DC4DD61D5083F6A", "1000000000000000000000000", "136944F567F4258D3ED452B3", "78AEC30ADF7B4530041B5385","63DE248A25BE0DC6583E290");
//    checkMontgomeryMultBase2("Montgomery Mult (12)", "10D48FD2090994548ADE1F35ECE7B10D", "130D53B78DC4DD61D5083F6A189DC74B", "100000000000000000000000000000000", "136944F567F4258D3ED452B297952DEB", "17D298F192A6366B5E1CB63D81F56D3D","466230212C34EE9A6B265ED39DAEBB8");
}

void CorrectnessTest::testMontgomeryDomain()
{
    limbs_radix_array lra_m;
    limbs_radix_array lra_radix;
    limbs_radix_array lra_mprime;
    
    BigInteger m;
    m.initFromHexString("67F4258D");
    
    BigInteger exp_radix;
    exp_radix.initFromHexString("10000000000000000");
    
    big_integer_apint_radix_initfromLimbArray(lra_m, m.m_data, m.m_size);
    big_integer_apint_radix_radixFromMontgomeryMod(lra_radix, lra_m);
    
    cout << " radix from mod (apint radix) ";
    checkResultMatchsExpectedApintRadix(lra_radix, exp_radix.m_data, exp_radix.m_size);
    
    BigInteger exp_mprime;
    exp_mprime.initFromHexString("27C738186F895ABB");
    
    //big_integer_apint_radix_verbosity = VERBOSITY_LEVEL_MONTGOMERY + 1;
    big_integer_apint_radix_mprimeFromMontgomeryRadix(lra_mprime, lra_m, lra_radix);
    
    cout << " mprime from radix & mod (apint radix) ";
    checkResultMatchsExpectedApintRadix(lra_mprime, exp_mprime.m_data, exp_mprime.m_size);
}

void CorrectnessTest::testMontgomeryReduction()
{
    checkMontgomeryReduction("Montgomery Reduction (1)", "124DCA37", "100000000", "443A5BB3", "15F96733");
    checkMontgomeryReduction("Montgomery Reduction (2)", "2E70041164DB2E1", "100000000", "55CE28B1", "4822EAE4");
    checkMontgomeryReduction("Montgomery Reduction (3)", "090D8865", "100000000", "67C76889", "34B94381");
    
    for (int bits = 32; bits <= 2048; bits *= 2)
    {
        cout << "Montgomery Reduction " << to_string(bits) << " bits " ;
        BigInteger a;
        a.initSize(bits/32);
        a.random();

        BigInteger m;
        m.initSize(bits/32);

        do
        m.random(); while (!m.isOdd());


        BigInteger radix;
        radix.initSize(bits/32+1);
        radix.setIntValue(1);
        radix.shiftLeft(bits);


        BigInteger mprime;
        mprime.initSize(m.m_size);

//        cout << " m = " << m.toHexString() << endl; 
//        cout << " radixSmall = " << radixSmall.toHexString() << endl; 

        // BigInteger::inverseMod(&mprime, &m, &radixSmall);

        BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);
//        cout << " mprime= " << mprime.toHexString() << endl; 
        //cout << " radix = " << radix.toHexString() << endl; 

        BigInteger r(a);
        BigInteger radixInv(radix);
        BigInteger radixM(radix);
        radixM.mod(&m);

        BigInteger::inverseMod(&radixInv, &radixM, &m);

//        cout << " radix = " << radix.toHexString() << endl; 
//        cout << " radix^-1 mod m= " << radixInv.toHexString() << endl; 
//        cout << " m= " << m.toHexString() << endl; 


        BigInteger::montgomeryReduction(&r, &a, &m, &radix, mprime.m_data[0]);

//        cout << " a' = " << r.toHexString() << endl;

        BigInteger na(a);
        BigInteger::multMod(&na, &a, &radixInv, &m);

//        cout << " a = " << a.toHexString() << endl;
//        cout << " exp a'= a * R^1 mod m= " << na.toHexString() << endl;
        
        checkResultMatchsExpected(&r, &na);
    }
}

/**
 * We check the montgomery form
 */
void CorrectnessTest::testMultMontgomeryForm()
{     
    for (int bits = 32; bits <= 4096; bits *= 2)
    {
        cout << "Mult Montgomery Form (" << bits << ") ";

        BigInteger m;
        m.initSize(bits/32);

        BigInteger radix;
        radix.initSize(m.m_size+1);

        BigInteger temp;
        temp.initSize(radix.m_size);

        //temp.mod(&radix);            // temp = m mod radix

        BigInteger invTemp(radix);
        do
        {
            m.random();
            BigInteger::radixFromMontgomeryMod(&radix, &m);
            temp.copy(&m);
            
            assert(!radix.isZero());
            
            if (verbosity)
                cout << " radix = " << radix.toHexString() << endl; 

            BigInteger::inverseMod(&invTemp, &temp, &radix);    // invTemp = (m mod radix) ^ (-1) mod radix
        } while (invTemp.isZero());                             // until we get a valid radix^-1

        // check m^-1 * m mod radix = 1
        BigInteger check(invTemp);
        check.multMod(&m, &radix);

        if (!check.isOne())
        {
            cout << "### ERROR ### inverse mod not valid " << endl;

            cout << " radix = " << radix.toHexString() << endl;
            cout << " m = " << m.toHexString() << endl;
            cout << " m mod radix = " << temp.toHexString() << endl;
            cout << " m mod radix ^-1 mod radix = " << invTemp.toHexString() << endl;
            cout << " (m^-1 mod radix) * m mod radix= " << check.toHexString() << endl;

            if (stopAtFirstError)
                exit(-1);
        }

        BigInteger mprime(radix);
        mprime.subtract(&invTemp);
    //    np=radix-inverseModInt(modInt(n,radix),radix);

        //BigInteger
        BigInteger radixInv(radix);
        //BigInteger radixm(radix);
        //radixm.mod(&m);
//        verbosity = 1;
        if (verbosity)
        {
            cout << "radix=" << radix.toHexString() << endl;
            cout << "m=" << m.toHexString() << endl;
        }
        
        BigInteger radixMod(radix);
        radixMod.mod(&m);
        
        BigInteger::inverseMod(&radixInv, &radixMod, &m);         // radixInv = radix ^ (-1) mod m

        BigInteger check2(radixInv);
        check2.multMod(&radix, &m);
        if (!check2.isOne())
        {
            cout << "### ERROR ###" << endl;
            cout << " radix = " << radix.toHexString() << endl;
            cout << " radixInv = " << radixInv.toHexString() << endl;
            cout << " m = " << m.toHexString() << endl;
            cout << " check2 = " << check2.toHexString() << endl;
            if (stopAtFirstError)
                exit(-1);
        }

        BigInteger rprime(m);
        BigInteger r2prime;
        r2prime.initSize(m.m_size+1);
        BigInteger r(m);
        BigInteger x(m);
        BigInteger y(m);
        x.random();
        y.random();

        BigInteger xprime(x);
        BigInteger yprime(y);

        BigInteger::toMontgomeryDomain(&xprime, &x, &radix, &m);
        BigInteger::toMontgomeryDomain(&yprime, &y, &radix, &m);

        BigInteger::extraChecks = false;
        BigInteger::multMontgomeryForm2(&rprime, &xprime, &yprime, &m, &mprime);
        BigInteger::montgomeryMult(&r2prime, &xprime, &yprime, &m, &radix, mprime.m_data[0]);
        BigInteger::extraChecks = true;

        if (!BigInteger::isEqual(&rprime, &r2prime))
        {
            cerr << "## ERROR ##" << endl;
            cerr << "rprime = " << rprime.toHexString() << endl;
            cerr << "r2prime = " << r2prime.toHexString() << endl;
            //assert(false);
        }
        BigInteger::fromMontgomeryDomain(&r, &rprime, &radixInv, &m);

        BigInteger r2(r);
        BigInteger::multMod(&r2, &x, &y, &m);

        if (BigInteger::isEqual(&r, &r2))
        {
            cout << "[OK]" << endl;
        }
        else
        {
            cout << "### ERROR ###" << endl;
            cout << "radix:" << radix.toHexString() << endl;
            cout << "radixInv:" << radixInv.toHexString() << endl;
            cout << "x:" << x.toHexString() << endl;
            cout << "y:" << y.toHexString() << endl;
            cout << "xprime:" << xprime.toHexString() << endl;
            cout << "yprime:" << yprime.toHexString() << endl;
            cout << "m:" << m.toHexString() << endl;
            cout << "mprime:" << mprime.toHexString() << endl;
            cout << "rprime:" << rprime.toHexString() << endl;
            cout << "r:" << r.toHexString() << endl;
            cout << "r2:" << r2.toHexString() << endl;
        }
    }
}

void CorrectnessTest::testParseNumbers()
{
//    BigInteger::verbosity = true;
    BigInteger a;
    a.initFromHexString("4f91587a190a3d8c");
    
    BigInteger b(a);
    b.parseString("5733461082048707980");
    
    cout << "Check parse Numbers ";
    checkResultMatchsExpected(&a, &b);
}

void CorrectnessTest::testParseNumbersC()
{
//    BigInteger::verbosity = true;
    const int bits = 512;
    unsigned int a_data[bits/32];
    unsigned int a_base = 0;
    unsigned int a_size = bits/32;
    
    big_integer_base_initFromHexString(a_data, a_base, a_size, "4f91587a190a3d8c");
    
    unsigned int b_data[bits/32];
    unsigned int b_base = 0;
    unsigned int b_size = bits/32;
    big_integer_base_parseString(b_data, b_base, b_size, "5733461082048707980");
    
    cout << "Check parse NumbersC ";
    checkResultMatchsExpectedCBase(a_data, a_base, a_size, b_data, b_base, b_size);
    
//    cout << " a = " << big_integer_toHexString(a_data, a_base, a_size) << endl;
//    cout << " b = " << big_integer_toHexString(b_data, a_base, a_size) << endl;

    const char* sa = "5B8B8C8C3DC2206684A28191094E98A083E1912AE56FA3";
    const char* ssa = "2AE56FA3 A083E191 91094E98 6684A281 8C3DC220 005B8B8C 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 ";
    
    big_integer_base_initFromHexString(a_data, a_base, a_size, sa);
    const char* csa = big_integer_base_toHexString(a_data, a_base, a_size);
    
    if (strcmp(ssa, csa) != 0)
    {
        cerr << "## ERROR ## strings are different" << endl;
        cerr << " ssa = \"" << ssa << "\"" << endl;
        cerr << " csa = \"" << csa << "\"" << endl;
        exit(-1);
    }
}


/**
 * 
 */
void CorrectnessTest::testShiftLeft()
{
    checkShiftLeft("Shift Left (1)" , "ABCDEF9876543210", 32 , "ABCDEF987654321000000000");
    checkShiftLeft("Shift Left (2)" , "00000000ABCDEF9876543210", 32 , "ABCDEF987654321000000000");
    checkShiftLeft("Shift Left (3)" , "00000000ABCDEF9876543210", 64 , "ABCDEF98765432100000000000000000");
    checkShiftLeft("Shift Left (4)", "1EDABB5798CEECDAFAE870C24", 1, "3DB576AF319DD9B5F5D0E1848");

}

void CorrectnessTest::checkShiftLeft(const char* msg, const char* sa, int bits, const char* sexp)
{
    BigInteger a;
    BigInteger r;
    BigInteger expected;
    a.initFromHexString(sa);
    expected.initFromHexString(sexp);
    r.initSize(expected.m_size);
    
    BigInteger::shiftLeft(&r, &a, bits);
    
    cout << msg << " (std) ";
    checkResultMatchsExpected(&r, &expected);

    limbs_array la_a;
    limbs_array la_r;
    limbs_array la_exp;
    
    big_integer_array_initFromHexString(la_a, sa);
    big_integer_array_initFromHexString(la_exp, sexp);

    big_integer_array_shiftLeft(la_r, la_a, bits);
    
    cout << msg << " (array) ";
    checkResultMatchsExpectedArray(la_r, la_exp);
    
    limbs_radix_array lr_a;
    limbs_radix_array lr_r;

    big_integer_apint_radix_initfromLimbArray(lr_a, la_a, NUM_BIG_INTEGER_ARRAY_LIMBS);
    
    big_integer_apint_radix_shiftLeft(lr_r, lr_a, bits);

    
    cout << msg << " (apint radix) ";
    checkResultMatchsExpectedApintRadix(lr_r, la_exp, NUM_BIG_INTEGER_ARRAY_LIMBS);

}

void CorrectnessTest::checkShiftRight(const char* msg, const char* sa, int bits, const char* sexp)
{
    BigInteger a;
    BigInteger r;
    BigInteger expected;
    a.initFromHexString(sa);
    r.initSize(a.getNumBits()/32);
    expected.initFromHexString(sexp);
    
    BigInteger::shiftRight(&r, &a, bits);
    
    cout << msg ;
    checkResultMatchsExpected(&r, &expected);

}
/**
 * 
 */
void CorrectnessTest::testShiftRight()
{
    checkShiftRight("Shift Right 32 (same size) ", "ABCDEF9876543210", 32, "ABCDEF98");
    checkShiftRight("Shift Right 63 ", "DD8396BCC9236B280000000000000004", 63, "00000001BB072D799246D650");
    checkShiftRight("Shift Right 96 ", "3A91FD8E07F953A0000000000000000000000000", 96, "3A91FD8E07F953A0");
    checkShiftRight("Shift Right 506 ", "A5A2F020BC402A46AF7866A6D81251BC6282F3565F95B6C2A2BA2F9DBF56331F02ED5C8292104D2D03E575A240F9D8FCD3E4E5548FE4D9F9D4466B645A3515846D963E19490E86398A701E6682BB4BAF572680EFD38EC6339E2A567955723DFB7F24C3310D8818C619D79A5060C46CC8BE2456CF24EB68349170C61547B363",
            506, "2968BC082F100A91ABDE19A9B604946F18A0BCD597E56DB0A8AE8BE76FD58CC7C0BB5720A484134B40F95D68903E763F34F9395523F9367E75119AD9168D4561");
    
}

void CorrectnessTest::checkSquareMod(const char* msg, const char* sa, const char* sm, const char* sexp)
{
    cout << msg ;

    BigInteger a;
    BigInteger m;
    BigInteger r;
    BigInteger exp;
    
    a.initSize((unsigned int) (strlen(sa)+7/8)+1);
    a.parseHexString(sa);
    m.initFromHexString(sm);
    exp.initFromHexString(sexp);
    
    r.initSize(m.getNumBits()/32+1);
    
    a.squareMod(&m);

    checkResultMatchsExpected(&a, &exp);    
}

void CorrectnessTest::testSquareMod()
{
    BigInteger a;
    BigInteger m;
    BigInteger r;
 
    checkSquareMod("SquareMod (1)", "6AED2278", "70E6AC98", "3A079890");
    checkSquareMod("SquareMod (1)", "6AED227828246725", "70E6AC982D43A9FA", "4859F46B3B11B1F7");
    checkSquareMod("SquareMod (3)", "3A1740240B326EB5234E39D61D3D3E912E73719319674F9657460BC44A7627A76DA97978136944F5", 
            "64FF67807FBE1E8C2B6F5032241E425F2AFB729B1C653F7D203947293C3603AF105A2D74137D40DF",
            "3BE2447FDC117220741755CC8841D0B22A249FFA2B113CE1FDF7B6D332DDD2BE93AECBFA4F1AD9FD");
    
    for (int bits=32; bits < 4096; bits*=2)
    {
        a.initSize(bits/32);
        m.initSize(bits/32);
        r.initSize(bits/32);
        a.random(bits-1);
        m.random(bits-1);


        cout << "Square Mod ("<< bits<<" bits) " ;

        //verbosity = 1;
        if (verbosity) cout << "a: " << a.toHexString() << endl;
        if (verbosity) cout << "m: " << m.toHexString() << endl;
        
        BigInteger::multMod(&r, &a, &a, &m);
        
        a.squareMod(&m);

        checkResultMatchsExpected(&a, &r);
    }
}

void CorrectnessTest::testSquareModC()
{
    
//    BigInteger a;
//    BigInteger m;
//    BigInteger r;
 
//    checkSquareModC("SquareMod (1)", "6AED2278", "70E6AC98", "3A079890");
//    checkSquareModC("SquareMod (1)", "6AED227828246725", "70E6AC982D43A9FA", "4859F46B3B11B1F7");
    
    for (int bits=32; bits < 4096; bits*=2)
    {
        
    const int limbs = bits/32;
    const int index = 4;
    const int base = 1*limbs;
    
    unsigned int a_data[index*limbs];
    unsigned int a_base = base;
    unsigned int a_size = limbs;
    unsigned int m_data[index*limbs];
    unsigned int m_base = base;
    unsigned int m_size = limbs;
    unsigned int r_data[index*limbs];
    unsigned int r_base = base;
    unsigned int r_size = limbs;
    unsigned int exp_data[index*limbs];
    unsigned int exp_base = base;
    unsigned int exp_size = limbs;
    
    big_integer_base_random_bits(a_data, a_base, a_size, bits-1);
    big_integer_base_random_bits(m_data, m_base, m_size, bits-1);

        cout << "Square Mod C ("<< bits<<" bits) " ;

        //verbosity = 1;
//        if (verbosity) cout << "a: " << a.toHexString() << endl;
//        if (verbosity) cout << "m: " << m.toHexString() << endl;
        
        big_integer_base_multMod(r_data, r_base, r_size,
                a_data, a_base, a_size,
                a_data, a_base, a_size, 
                m_data, m_base, m_size);
        
        big_integer_base_squareMod_short(a_data, a_base, a_size, m_data, m_base, m_size);

        checkResultMatchsExpectedCBase(a_data, a_base, a_size, r_data, r_base, r_size);
    }
}

void CorrectnessTest::testPowerModC()
{
    checkPowerModC("PowerModC (1)", "2E70041164DB2E1", "23041164DB2E1", "55CE28B1", "1D68E58E");
    checkPowerModC("PowerModC (2)", "2E7004114A7627A76DA97978136944F5", "3ED452B2489C045C2E70041164DB2E1F", "67F4258D3ED452B297952DEB", "2704D2FC0A8413A6C989C345");
    checkPowerModC("PowerModC (3)", "64DB2E1F2E7004114A7627A76DA97978136944F5", "6DA979783ED452B2489C045C2E70041164DB2E1F", "136944F567F4258D3ED452B297952DEB", "73162639794173336F1FFA5DFCDCE3B");
    checkPowerModC("PowerModC (4)", "3A1740240B326EB5234E39D61D3D3E912E73719319674F9657460BC44A7627A76DA97978136944F5", 
            "413D1C0873F436AE79FF7D9073B77F342DDF71BF59E73792636F005572C136FE33715778136C6EF1", "64FF67807FBE1E8C2B6F5032241E425F2AFB729B1C653F7D203947293C3603AF105A2D74137D40DF",
            "57F1F631F6D2EAD5E4D1269AAFD57555359B6E4EABF1CEDBD300F512A48016D0018CE3E16C7EB079");
    
    checkPowerModC("PowerModC (5)", "2E70041164DB2E1F67F4258C3ED452B2489C045C57952DEB3A1740240B326EB5234E39D61D3D3E912E73719319674F9657460BC44A7627A76DA97978136944F5",
            "3A2956A20BE47DC03B3165E777DF7DE503431FC3042C6A64413D1C0873F436AE79FF7D9073B77F342DDF71BF59E73792636F005572C136FE33715778136C6EF1",
            "74C673764F100BE55B6027B2151655E3288528C3632218C164FF67807FBE1E8C2B6F5032241E425F2AFB729B1C653F7D203947293C3603AF105A2D74137D40DF",
            "53843786EA27F4A23F3636910AA846B2D3AE258CDFC8AD43197A7A1904DD213155589B25D578DC9B478A5E218397BD5560D19A2781E0820073DBCFD654128F70");
    checkPowerModC("PowerModC (6)", "39054AD14F8573D335E363401280522B328735171CC458F40BDA7E053A28119C3D052DFE36176BCE30EE62C3682F07A438F073B10C191E0F44DB39C920A95385", 
            "44BD1D62768E43740920239C4B8B7D5E6D2E507D495C156D130059E922EA599513B571B80C922C70305A62EF28AE6FA04519684234632D660AA417C920AC7D81",
            "50766FF31D9713155C5D63F80496289027D56BE475F351E61A2735CE0BAC218E6A653572630D6D122FC6631C692E579C51415CD35CAE3CBC506C75C820AF277D",
            "22424FE41686F0EFBE62BA53979420C606D5C334D1EDEDA95FB44E3039BF99A3D0D402519CBAD661E2AC33907AE81EE49FE3CEE923670A669FEC903B739559FF");

}

void CorrectnessTest::testPowerMod()
{
    checkPowerMod("PowerMod (1)", "2E70041164DB2E1", "23041164DB2E1", "55CE28B1", "1D68E58E");
    checkPowerMod("PowerMod (2)", "2E7004114A7627A76DA97978136944F5", "3ED452B2489C045C2E70041164DB2E1F", "67F4258D3ED452B297952DEB", "2704D2FC0A8413A6C989C345");
    checkPowerMod("PowerMod (3)", "64DB2E1F2E7004114A7627A76DA97978136944F5", "6DA979783ED452B2489C045C2E70041164DB2E1F", "136944F567F4258D3ED452B297952DEB", "73162639794173336F1FFA5DFCDCE3B");
    checkPowerMod("PowerMod (4)", "3A1740240B326EB5234E39D61D3D3E912E73719319674F9657460BC44A7627A76DA97978136944F5", 
            "413D1C0873F436AE79FF7D9073B77F342DDF71BF59E73792636F005572C136FE33715778136C6EF1", "64FF67807FBE1E8C2B6F5032241E425F2AFB729B1C653F7D203947293C3603AF105A2D74137D40DF",
            "57F1F631F6D2EAD5E4D1269AAFD57555359B6E4EABF1CEDBD300F512A48016D0018CE3E16C7EB079");
    
    checkPowerMod("PowerMod (5)", "2E70041164DB2E1F67F4258C3ED452B2489C045C57952DEB3A1740240B326EB5234E39D61D3D3E912E73719319674F9657460BC44A7627A76DA97978136944F5",
            "3A2956A20BE47DC03B3165E777DF7DE503431FC3042C6A64413D1C0873F436AE79FF7D9073B77F342DDF71BF59E73792636F005572C136FE33715778136C6EF1",
            "74C673764F100BE55B6027B2151655E3288528C3632218C164FF67807FBE1E8C2B6F5032241E425F2AFB729B1C653F7D203947293C3603AF105A2D74137D40DF",
            "53843786EA27F4A23F3636910AA846B2D3AE258CDFC8AD43197A7A1904DD213155589B25D578DC9B478A5E218397BD5560D19A2781E0820073DBCFD654128F70");
    checkPowerMod("PowerMod (6)", "39054AD14F8573D335E363401280522B328735171CC458F40BDA7E053A28119C3D052DFE36176BCE30EE62C3682F07A438F073B10C191E0F44DB39C920A95385", 
            "44BD1D62768E43740920239C4B8B7D5E6D2E507D495C156D130059E922EA599513B571B80C922C70305A62EF28AE6FA04519684234632D660AA417C920AC7D81",
            "50766FF31D9713155C5D63F80496289027D56BE475F351E61A2735CE0BAC218E6A653572630D6D122FC6631C692E579C51415CD35CAE3CBC506C75C820AF277D",
            "22424FE41686F0EFBE62BA53979420C606D5C334D1EDEDA95FB44E3039BF99A3D0D402519CBAD661E2AC33907AE81EE49FE3CEE923670A669FEC903B739559FF");
}

//function inverseModInt(x,n) {
//  var a=1,b=0,t;
//  for (;;) {
//    if (x==1) return a;
//    if (x==0) return 0;
//    b-=a*Math.floor(n/x);
//    n%=x;
//
//    if (n==1) return b; //to avoid negatives, change this b to n-b, and each -= to +=
//    if (n==0) return 0;
//    a-=b*Math.floor(x/n);
//    x%=n;
//  }
//}
