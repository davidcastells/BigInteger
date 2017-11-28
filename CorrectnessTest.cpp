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
#include "big_integer.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
    
    testRandom();
    testIsLessThan();
    testIsLessThanC();
    testParseNumbers();
    testParseNumbersC();
    testShiftLeft();
    testShiftRight();
    testRange();
    testAdd();
    testAddC();
    testAddShifted();
    testMult();
    testMultC();
    testDiv();
    testDivC();
    testMultMod();
    testModBase();
    testInverseMod();
    testMultMontgomeryForm();
//    testMod();

    testSquareMod();
//    BigInteger::verbosity = verbosity = 3;
    testPowerMod();
    
    BigInteger::extraChecks = 0;
    
    cout << "========================================" << endl;
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


void CorrectnessTest::checkResultMatchsExpectedC(unsigned int * r_data, unsigned int r_base, unsigned int r_size,
        unsigned int * exp_data, unsigned int exp_base, unsigned int exp_size)
{
    if (big_integer_isEqual(r_data, r_base, r_size, exp_data, exp_base, exp_size))
        cout << "[OK]" << endl;
    else
    {        
        cout << "## ERROR ##" << endl;
        cout << "Expected: " << big_integer_toHexString(exp_data, exp_base, exp_size) << endl;
        cout << "Result: " << big_integer_toHexString(r_data, r_base, r_size) << endl;
        
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
    
    cout << msg;
    
    BigInteger r2;
    r2.initSize(bm.m_size);
    
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
    r2.initSize(r.m_size);
        
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

    BigInteger::powerMod(&r2, &a, &e, &m);
        
    if (verbosity)  cout << "r (std): " << r2.toHexString() << endl;

    cout << msg << " (std) " ;
    checkResultMatchsExpected(&r2, &r);
        
    // BigInteger::verbosity = 1;
    BigInteger::powerModMontgomery(&r2, &a, &e, &m, &mprime, &radix);

    if (verbosity)  cout << "r (mont): " << r2.toHexString() << endl;

    cout << msg << " (mont) " ;
    checkResultMatchsExpected(&r2, &r);
    
    BigInteger::powerModSlidingWindow(&r2, &a, &e, &m);
    
    cout << msg << " (sliding window) " ;
    checkResultMatchsExpected(&r2, &r);
    
}

void CorrectnessTest::checkDivision(const char* msg, const char* a, const char* b, const char* eq, const char* er)
{
    BigInteger ba, bb, beq, ber;
    
    ba.initFromHexString(a);
    bb.initFromHexString(b);
    beq.initFromHexString(eq);
    ber.initFromHexString(er);
    
//    cout << "beq = " << beq.toHexString() << endl;
    
    BigInteger bq(beq);
    BigInteger br(ber);
    
    BigInteger::div_naive(&ba, &bb, &bq, &br);
    
    cout << msg << "(q) ";
    checkResultMatchsExpected(&bq, &beq);
    cout << msg << "(r) ";
    checkResultMatchsExpected(&br, &ber);
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

    
    big_integer_initFromHexString(ba_data, ba_base, ba_size, a);
    big_integer_initFromHexString(bb_data, bb_base, bb_size, b);
    big_integer_initFromHexString(beq_data, beq_base, beq_size, eq);
    big_integer_initFromHexString(ber_data, ber_base, ber_size, er);
    
//    cout << "beq = " << beq.toHexString() << endl;
    
    unsigned int bq_data[2048/32];
    unsigned int bq_base = 0;
    unsigned int bq_size = 2048/32;
    
    unsigned int br_data[2048/32];
    unsigned int br_base = 0;
    unsigned int br_size = 2048/32;
    
//    BigInteger bq(beq);
//    BigInteger br(ber);
    
    big_integer_div_naive(ba_data, ba_base, ba_size,
            bb_data, bb_base, bb_size,
            bq_data, bq_base, bq_size,
            br_data, br_base, br_size);
    
    cout << msg << "(q) ";
    checkResultMatchsExpectedC(bq_data, bq_base, bq_size, beq_data, beq_base, beq_size);
    cout << msg << "(r) ";
    checkResultMatchsExpectedC(br_data, br_base, br_size, ber_data, ber_base, ber_size);
}



void CorrectnessTest::testRange()
{
    BigInteger a;
    BigInteger r;
    BigInteger exp;
    a.initSize(64/32),
    r.initSize(64/32);
    exp.initSize(64/32);
    a.parseHexString("ABCDEF9876543210");
    
    exp.parseHexString("9876543210");
    
    
    
    cout << "Range a[39..0] " ;
    BigInteger::range(&r, &a, 39, 0);
    checkResultMatchsExpected(&r, &exp);
    
    exp.parseHexString("ABCDEF");
    cout << "Range a[63..0] ";
    BigInteger::range(&r, &a, 63, 40);
    checkResultMatchsExpected(&r, &exp);
    
    a.initSize(5);
    a.parseHexString("E991945399FE030C3A91FD8E07F953A0");
    r.initSize(3);
    exp.initSize(3);
    
    cout << "Range a[159..64] ";
    exp.parseHexString("E991945399FE030C");
    BigInteger::range(&r, &a, 159, 64);
    checkResultMatchsExpected(&r, &exp);

    cout << "Range a[63..0] ";
    exp.parseHexString("3A91FD8E07F953A0");
    BigInteger::range(&r, &a, 63, 0);
    checkResultMatchsExpected(&r, &exp);

//    BigInteger::verbosity = 1;
    
    cout << "Range a[125..61] ";
    a.initFromHexString("3760E5AF3248DACA0000000000000001");
    exp.initFromHexString("00000000000001BB072D799246D650");
    r.initSize(exp.m_size);
    
    BigInteger::range(&r, &a, 125, 61);
    checkResultMatchsExpected(&r, &exp);
    
    a.initSize(32); 
    a.m_data[31] = -1;
    r.initSize(16);

    cout << "Range a[1015..506] ";
    a.initFromHexString("A5A2F020BC402A46AF7866A6D81251BC6282F3565F95B6C2A2BA2F9DBF56331F02ED5C8292104D2D03E575A240F9D8FCD3E4E5548FE4D9F9D4466B645A3515846D963E19490E86398A701E6682BB4BAF572680EFD38EC6339E2A567955723DFB7F24C3310D8818C619D79A5060C46CC8BE2456CF24EB68349170C61547B363");
    exp.initFromHexString("2968BC082F100A91ABDE19A9B604946F18A0BCD597E56DB0A8AE8BE76FD58CC7C0BB5720A484134B40F95D68903E763F34F9395523F9367E75119AD9168D4561");
    r.initSize(exp.m_size);
    BigInteger::range(&r, &a, 1015, 506);

    checkResultMatchsExpected(&r, &exp);

}

void CorrectnessTest::testAdd()
{
    BigInteger a;
    BigInteger b;
    BigInteger r;
    BigInteger exp;
    
    a.initFromHexString("000000000000000000000000");
    b.initFromHexString("00000000");
    exp.initFromHexString("000000000000000000000000");

    BigInteger::add(&r, &a, &b);
    
    cout << "Add (1) ";
    checkResultMatchsExpected(&r, &exp);
    
    BigInteger::add(&r, &b, &a);
    
    cout << "Add (2) ";
    checkResultMatchsExpected(&r, &exp);

    a.initSize(2048/32),
    b.initSize(2048/32);
    r.initSize(2048/32);
    exp.initSize(2048/32);
    
    a.parseString("8768279873802716238987346287098787657656763502221946787");
    b.parseString("1231230932483459873495094398734762765654543128761987338");
    exp.parseString("9999510806286176112482440685833550423311306630983934125");
    
    BigInteger::add(&r, &a, &b);
    
    cout << "Add (3) ";
    checkResultMatchsExpected(&r, &exp);
    
}

void CorrectnessTest::testAddC()
{
    unsigned int a_data[2048/32];
    unsigned int a_base = 0;
    unsigned int a_size = 2048/32;
    unsigned int b_data[2048/32];
    unsigned int b_base = 0;
    unsigned int b_size = 2048/32;
    unsigned int r_data[2048/32];
    unsigned int r_base = 0;
    unsigned int r_size = 2048/32;
    unsigned int exp_data[2048/32];
    unsigned int exp_base = 0;
    unsigned int exp_size = 2048/32;
    
    big_integer_initFromHexString(a_data, a_base, a_size, "000000000000000000000010");
    big_integer_initFromHexString(b_data, b_base, b_size, "00000000");
    big_integer_initFromHexString(exp_data, exp_base, exp_size, "000000000000000000000010");

    big_integer_add(r_data, r_base, r_size, a_data, a_base, a_size, b_data, b_base, b_size);
    
    cout << "AddC (1) ";
    checkResultMatchsExpectedC(r_data, r_base, r_size, exp_data, exp_base, exp_size);
    
    big_integer_add(r_data, r_base, r_size, b_data, b_base, b_size, a_data, a_base, a_size);
    
    cout << "AddC (2) ";
    checkResultMatchsExpectedC(r_data, r_base, r_size, exp_data, exp_base, exp_size);

    big_integer_parseString(a_data, a_base, a_size, "8768279873802716238987346287098787657656763502221946787");
    big_integer_parseString(b_data, b_base, b_size, "1231230932483459873495094398734762765654543128761987338");
    big_integer_parseString(exp_data, exp_base, exp_size, "9999510806286176112482440685833550423311306630983934125");
    
    big_integer_add(r_data, r_base, r_size, a_data, a_base, a_size, b_data, b_base, b_size);
    
    cout << "AddC (3) ";
    checkResultMatchsExpectedC(r_data, r_base, r_size, exp_data, exp_base, exp_size);
    
    big_integer_copy(r_data, r_base, r_size, a_data, a_base, a_size);
    big_integer_add_short(r_data, r_base, r_size, b_data, b_base, b_size);
    
    cout << "AddC (4) ";
    checkResultMatchsExpectedC(r_data, r_base, r_size, exp_data, exp_base, exp_size);
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
    checkDivision("Division (2)", "15B1742F2AFBE68D0000000000000000", "45CB977B71DFED43", "4f91587a190a3d8c", "a6653828e63485c");
    checkDivision("Division (3)", "3760E5AF3248DACA0000000000000001", "10000000000000000", "3760E5AF3248DACA", "000000000000000000001");
    
}

void CorrectnessTest::testDivC()
{
    cout << "DivisionC and ModuloC " << flush;
    
    const int bits = 512;

    unsigned int x_data[bits/32];
    unsigned int x_base = 0;
    unsigned int x_size = bits/32;
    unsigned int y_data[2*bits/32];
    unsigned int y_base = 0;
    unsigned int y_size = 2*bits/32;
    unsigned int q_data[bits/32];
    unsigned int q_base = 0;
    unsigned int q_size = bits/32;
    unsigned int r_data[bits/32];
    unsigned int r_base = 0;
    unsigned int r_size = bits/32;
    
    big_integer_random(x_data, x_base, x_size);
    big_integer_random(q_data, q_base, q_size);
    big_integer_random_bits(r_data, r_base, r_size, big_integer_getLength(x_data, x_base, x_size)-1);  // ensure r < x
    
    big_integer_mult(y_data, y_base, y_size, x_data, x_base, x_size, q_data, q_base, q_size);
    big_integer_add_short(y_data, y_base, y_size, r_data, r_base, r_size);                   // y = q*x + r

    cout << endl;
    cout << big_integer_toHexString(x_data, x_base, x_size) << " * " << endl;
    cout << big_integer_toHexString(q_data, q_base, q_size) << " = " << endl;
    cout << big_integer_toHexString(y_data, y_base, y_size) << " + " << endl;
    cout << big_integer_toHexString(r_data, r_base, r_size) << endl;
    
    unsigned int qp_data[bits/32];
    unsigned int qp_base = 0;
    unsigned int qp_size = bits/32;
    unsigned int rp_data[bits/32];
    unsigned int rp_base = 0;
    unsigned int rp_size = bits/32;

    big_integer_div_naive(y_data, y_base, y_size,
            x_data, x_base, x_size,
            qp_data, qp_base, qp_size, 
            rp_data, rp_base, rp_size);
    
    if (!big_integer_isEqual(q_data, q_base, q_size, qp_data, qp_base, qp_size))
    {
        cout << "### ERROR ###" << endl;
    }
    else if (!big_integer_isEqual(r_data, r_base, r_size, rp_data, rp_base, rp_size))
    {
        cout << "### ERROR ###" << endl;
    }
    else
        cout << "[OK]" << endl;
    
    
    checkDivisionC("Division (1)", "55B1742F2AFBE68D", "77B71DFED43", "B73F3", "13B49F796F4");
    checkDivisionC("Division (2)", "15B1742F2AFBE68D0000000000000000", "45CB977B71DFED43", "4f91587a190a3d8c", "a6653828e63485c");
    checkDivisionC("Division (3)", "3760E5AF3248DACA0000000000000001", "10000000000000000", "3760E5AF3248DACA", "000000000000000000001");
    
}

void CorrectnessTest::testInverseMod()
{
    cout << "Inverse Mod ";
    
    BigInteger m;
    m.initSize(2048/32);
    m.parseString("264564564564564564565464564611");
    //m.random();
    
    BigInteger v;
    v.initSize(2048/32);
    v.parseString("54654645645645654654654654645675467");
    v.random();
    
    BigInteger vinv;
    vinv.initSize(2048/32);
    
    BigInteger::extraChecks = false;
    BigInteger::inverseMod(&vinv, &v, &m);
    BigInteger::extraChecks = true;
    
    // Ensure vinv * v mod m = 1
    BigInteger r;
    r.initSize(2048/32);
    BigInteger::multMod(&r, &vinv, &v, &m);
    
    BigInteger one;
    one.initSize(2048/32);
    one.setIntValue(1);
    
    if (BigInteger::isEqual(&r, &one))
    {
        cout << "[OK]" << endl;
    }
    else
    {
        cout << "### ERROR ###" << endl; 
        cout << " v = " << v.toHexString() << endl;
        cout << " vinv = " << vinv.toHexString() << endl;
        cout << " r = " << r.toHexString() << endl;
    }
    
    cout << "Invalid inv mod";

    v.initFromHexString("3A35825373ADDCE6");
    m.initFromHexString("10000000000000000");
    r.initFromHexString("0");
    //BigInteger::verbosity = 10;
    BigInteger::inverseMod(&vinv, &v, &m);
    
    checkResultMatchsExpected(&vinv, &r);

}

/**
 * Test random number generator
 */
void CorrectnessTest::testRandom()
{
    BigInteger a;
    a.initSize(4);
    a.random();

    // Check that higher parts (16 most significant bits) of limbs are 
    // not always zero. This happens when compiling with MINGW
    int sum = 0;
    for (int i=0; i < a.m_size; i++)
    {
        sum += (a.m_data[i]>>16);   // sum the higher part of the random number
    }
    cout << "Random: " << a.toHexString() << " ";
    
    if (sum == 0)
    {
        cout << " ### ERROR ### higher part of limbs always zero" << endl;
        if (stopAtFirstError)
            exit(-1);
    }
    else
        cout << "[OK]" << endl;
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

void CorrectnessTest::testIsLessThanC()
{
    unsigned int a_data[2048/32];
    unsigned int a_base = 0;
    unsigned int a_size = 2048/32;
    unsigned int b_data[2048/32];
    unsigned int b_base = 0;
    unsigned int b_size = 2048/32;
    
    big_integer_initFromHexString(a_data, a_base, a_size, "3A35825373ADDCE6");
    big_integer_initFromHexString(b_data, b_base, b_size, "000000010000000000000000");
    
    
    cout << "IsLessThanC (a<B)? ";
    
    if (big_integer_isLessThan(a_data, a_base, a_size, b_data, b_base, b_size))
        cout << "[OK]" << endl;
    else
    {        
        cout << "### ERROR ###" << endl;
        exit(-1);
    }
    
    cout << "IsLessThan (B<a)? ";
    
    if (big_integer_isLessThan(b_data, b_base, b_size, a_data, a_base, a_size))
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
    checkMult("Mult (2)", "5B8B8C8C3DC2206684A28191094E98A083E1912AE56FA3", "CDACA63DD6296E991945399FE030C3A8522C3A41BF10A", "498C7CC155A6D839E94640D834D747AC3BAE38BD908E8224402FE15DC19E8AE61A63C9FADE44991A9EDDD3FCF5E");
    checkMult("Mult (3)", "8C3DC2206684A28191094E98A083E1912AE56FA3", "CDACA63DD6296E991945399FE030C3A8522C3A41BF10A", "70AC09044BC7AE32F43AD3C90A7F13D5146014E6C048AFE15DC19E8AE61A63C9FADE44991A9EDDD3FCF5E");
    checkMult("Mult (4)", "5B8B8C8C3DC2206684A28191094E98A083E1912AE56FA3", "63DD6296E991945399FE030C3A8522C3A41BF10A", "23B62213F40B7EEEF0271F670D406D5A983B3472635F64FE15DC19E8AE61A63C9FADE44991A9EDDD3FCF5E");
    checkMult("Mult (5)", "400611DB4F6B3155309A0C3941095CC408A5162E7E26490C125C070B026825AB", "16B6559525E671F730060C65018944C014CD0ABE267158635824650A026B4FA7", "5AE1F40DBAE64FF72C651F8B56BEFE2751D10E095810C76C954C9765F21ED4FF230CDC57AED2B0D9CF21C373004944E873557784628E029AACDBF6DCA09578D");
}

void CorrectnessTest::testMultC()
{
    checkMultC("MultC (1)", "5B8B8C8C3DC2206684", "51E7C8116D4A26C669FE2A", "1D4A09EF733CFB077101819A3A5356EA232BC9A8");
    checkMultC("MultC (2)", "5B8B8C8C3DC2206684A28191094E98A083E1912AE56FA3", "CDACA63DD6296E991945399FE030C3A8522C3A41BF10A", "498C7CC155A6D839E94640D834D747AC3BAE38BD908E8224402FE15DC19E8AE61A63C9FADE44991A9EDDD3FCF5E");
    checkMultC("MultC (3)", "8C3DC2206684A28191094E98A083E1912AE56FA3", "CDACA63DD6296E991945399FE030C3A8522C3A41BF10A", "70AC09044BC7AE32F43AD3C90A7F13D5146014E6C048AFE15DC19E8AE61A63C9FADE44991A9EDDD3FCF5E");
    checkMultC("MultC (4)", "5B8B8C8C3DC2206684A28191094E98A083E1912AE56FA3", "63DD6296E991945399FE030C3A8522C3A41BF10A", "23B62213F40B7EEEF0271F670D406D5A983B3472635F64FE15DC19E8AE61A63C9FADE44991A9EDDD3FCF5E");
    checkMultC("MultC (5)", "400611DB4F6B3155309A0C3941095CC408A5162E7E26490C125C070B026825AB", "16B6559525E671F730060C65018944C014CD0ABE267158635824650A026B4FA7", "5AE1F40DBAE64FF72C651F8B56BEFE2751D10E095810C76C954C9765F21ED4FF230CDC57AED2B0D9CF21C373004944E873557784628E029AACDBF6DCA09578D");
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
    unsigned int r_data[2*bits/32];
    unsigned int r_base = 0;
    unsigned int r_size = 2*bits/32;
    unsigned int exp_data[bits/32];
    unsigned int exp_base = 0;
    unsigned int exp_size = bits/32;
        
    big_integer_initFromHexString(a_data, a_base, a_size, sa);
    big_integer_initFromHexString(b_data, b_base, b_size, sb);
    big_integer_initFromHexString(exp_data, exp_base, exp_size, sexp);
    
    cout << msg << "(standard) " ;
    
//    cout << endl;
//    cout << "a = " << big_integer_toHexString(a_data, a_base, a_size) << endl;
//    cout << "b = " << big_integer_toHexString(b_data, b_base, b_size) << endl;
    
    big_integer_mult(r_data, r_base, r_size, a_data, a_base, a_size, b_data, b_base, b_size);
    checkResultMatchsExpectedC(r_data, r_base, r_size, exp_data, exp_base, exp_size);
    
}

void CorrectnessTest::testMultMod()
{
    checkMultMod("Mult Mod (same size)", "8000000000000000", "2B62E85E55F7CD1A", "45CB977B71DFED43", "A6653828E63485C");
    checkMultMod("Mult Mod ", "00000000CB1F03567C39076B", "45CB977B71DFED43", "000000010000000000000000", "00000001");
}

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
        BigInteger::inverseMod(&radixInv, &radix, &m);         // radixInv = radix ^ (-1) mod m

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
        BigInteger::extraChecks = true;

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
    
    big_integer_initFromHexString(a_data, a_base, a_size, "4f91587a190a3d8c");
    
    unsigned int b_data[bits/32];
    unsigned int b_base = 0;
    unsigned int b_size = bits/32;
    big_integer_parseString(b_data, b_base, b_size, "5733461082048707980");
    
    cout << "Check parse NumbersC ";
    checkResultMatchsExpectedC(a_data, a_base, a_size, b_data, b_base, b_size);
    
//    cout << " a = " << big_integer_toHexString(a_data, a_base, a_size) << endl;
//    cout << " b = " << big_integer_toHexString(b_data, a_base, a_size) << endl;

    const char* sa = "5B8B8C8C3DC2206684A28191094E98A083E1912AE56FA3";
    const char* ssa = "2AE56FA3 A083E191 91094E98 6684A281 8C3DC220 005B8B8C 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 ";
    
    big_integer_initFromHexString(a_data, a_base, a_size, sa);
    const char* csa = big_integer_toHexString(a_data, a_base, a_size);
    
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
    
    cout << "Shift Right 32 (same size) " ;
    checkResultMatchsExpected(&r, &expected);
    
    r.initSize(32/32);
    expected.initSize(32/32);
    expected.parseHexString("ABCDEF98");
    
    BigInteger::shiftRight(&r, &a, 32);
    cout << "Shift Right 32 (smaller size) " ;
    checkResultMatchsExpected(&r, &expected);
    
    cout << "Shift Right 63 ";
    a.initFromHexString("DD8396BCC9236B280000000000000004");
    expected.initFromHexString("00000001BB072D799246D650");
    r.initSize(expected.m_size);
    BigInteger::shiftRight(&r, &a, 63);
    checkResultMatchsExpected(&r, &expected);

    cout << "Shift Right 96  " ;
    a.initFromHexString("3A91FD8E07F953A0000000000000000000000000");
    expected.initFromHexString("3A91FD8E07F953A0");
    r.initSize(expected.m_size);
    BigInteger::shiftRight(&r, &a, 96);
    checkResultMatchsExpected(&r, &expected);
    
    cout << "Shift Right 506 ";
    a.initFromHexString("A5A2F020BC402A46AF7866A6D81251BC6282F3565F95B6C2A2BA2F9DBF56331F02ED5C8292104D2D03E575A240F9D8FCD3E4E5548FE4D9F9D4466B645A3515846D963E19490E86398A701E6682BB4BAF572680EFD38EC6339E2A567955723DFB7F24C3310D8818C619D79A5060C46CC8BE2456CF24EB68349170C61547B363");
    expected.initFromHexString("2968BC082F100A91ABDE19A9B604946F18A0BCD597E56DB0A8AE8BE76FD58CC7C0BB5720A484134B40F95D68903E763F34F9395523F9367E75119AD9168D4561");
    r.initSize(expected.m_size);
    BigInteger::shiftRight(&r, &a, 506);
    checkResultMatchsExpected(&r, &expected);

}

void CorrectnessTest::testSquareMod()
{
    BigInteger a;
    BigInteger m;
    BigInteger r;
 
    for (int bits=32; bits < 1024; bits*=2)
    {
        a.initSize(bits);
        m.initSize(bits);
        r.initSize(bits);
        a.random();
        m.random();


        cout << "Square Mod ("<< bits<<" bits) " ;

        BigInteger::multMod(&r, &a, &a, &m);
        
        a.squareMod(&m);

        checkResultMatchsExpected(&a, &r);
    }
}

void CorrectnessTest::testPowerMod()
{
    checkPowerMod("PowerMod (1)", "2E70041164DB2E1F67F4258C3ED452B2489C045C57952DEB3A1740240B326EB5234E39D61D3D3E912E73719319674F9657460BC44A7627A76DA97978136944F5",
            "3A2956A20BE47DC03B3165E777DF7DE503431FC3042C6A64413D1C0873F436AE79FF7D9073B77F342DDF71BF59E73792636F005572C136FE33715778136C6EF1",
            "74C673764F100BE55B6027B2151655E3288528C3632218C164FF67807FBE1E8C2B6F5032241E425F2AFB729B1C653F7D203947293C3603AF105A2D74137D40DF",
            "53843786EA27F4A23F3636910AA846B2D3AE258CDFC8AD43197A7A1904DD213155589B25D578DC9B478A5E218397BD5560D19A2781E0820073DBCFD654128F70");
    checkPowerMod("PowerMod (2)", "39054AD14F8573D335E363401280522B328735171CC458F40BDA7E053A28119C3D052DFE36176BCE30EE62C3682F07A438F073B10C191E0F44DB39C920A95385", 
            "44BD1D62768E43740920239C4B8B7D5E6D2E507D495C156D130059E922EA599513B571B80C922C70305A62EF28AE6FA04519684234632D660AA417C920AC7D81",
            "50766FF31D9713155C5D63F80496289027D56BE475F351E61A2735CE0BAC218E6A653572630D6D122FC6631C692E579C51415CD35CAE3CBC506C75C820AF277D",
            "22424FE41686F0EFBE62BA53979420C606D5C334D1EDEDA95FB44E3039BF99A3D0D402519CBAD661E2AC33907AE81EE49FE3CEE923670A669FEC903B739559FF");
    
    BigInteger a;
    BigInteger e;
    BigInteger m;
    BigInteger r, r2;
 
    for (int bits=32; bits < 1024; bits*=2)
    {
        a.initSize(bits/32);
        e.initSize(bits/32);
        m.initSize(bits/32);
        r.initSize(bits/32);
        r2.initSize(bits/32);
        a.random();
        e.random();
        
        if (verbosity) cout << "a: " << a.toHexString() << endl;
        if (verbosity) cout << "e: " << e.toHexString() << endl;

        BigInteger radix;
        radix.initSize(m.m_size+1);
        BigInteger mprime;
        mprime.initSize(radix.m_size);        

        do
        {

            m.random();
            if (verbosity) cout << "m: " << m.toHexString() << endl;

            BigInteger::radixFromMontgomeryMod(&radix, &m);
        
            BigInteger::mprimeFromMontgomeryRadix(&mprime, &m, &radix);
        
        } while (mprime.isZero());

        BigInteger::powerMod(&r, &a, &e, &m);
        
        if (verbosity)  cout << "r: " << r.toHexString() << endl;
        
        // BigInteger::verbosity = 1;
        BigInteger::powerModMontgomery(&r2, &a, &e, &m, &mprime, &radix);

        cout << "Power Mod Montgomery ("<< bits<<" bits) " ;

        checkResultMatchsExpected(&r2, &r);
    }
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
