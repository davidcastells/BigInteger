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
#include <string.h>

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
    
    BigInteger::extraChecks = 1;
    
    testIsLessThan();
    testParseNumbers();
    testShiftLeft();
    testShiftRight();
    testRange();
    testAdd();
    testAddShifted();
    testMult();
    testDiv();
    testMultMod();
    testModBase();
    testInverseMod();
    testMultMontgomeryForm();
//    testMod();

    testSquareMod();
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

    cout << "Range a[1023..512] ";
    
    BigInteger::range(&r, &a, 1023, 512);

    cout << "[OK]" << endl;

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
    cout << "Division and Modulo ";
    
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
    checkDivision("Division (3)", "3760E5AF3248DACA0000000000000001", "10000000000000000", "3760E5AF3248DACA", "1");
    

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


void CorrectnessTest::testIsLessThan()
{
    BigInteger a, b;
    a.initFromHexString("3A35825373ADDCE6");
    b.initFromHexString("000000010000000000000000");
    
    cout << "IsLessThan (a<B)? ";
    
    if (a.isLessThan(&b))
        cout << "[OK]";
    else
        cout << "### ERROR ###";
    
    cout << "IsLessThan (B<a)? ";
    
    if (b.isLessThan(&a))
        cout << "### ERROR ###";
    else
        cout << "[OK]";
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
    int bits = 1024;
    
    BigInteger a;
    BigInteger b;
    BigInteger r;
    BigInteger exp;
    
    a.initSize(bits/32),
    b.initSize(bits/32);
    r.initSize((2*bits)/32);
    exp.initSize(bits/32);
    
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
    checkResultMatchsExpected(&r, &exp);
    
    cout << "Mult (karatsuba recursive) ";
    
    BigInteger::mult_karatsubaRecursive(&r, &a, &b);
    checkResultMatchsExpected(&r, &exp);
    
}

void CorrectnessTest::testMultMod()
{
    checkMultMod("Mult Mod (same size)", "8000000000000000", "2B62E85E55F7CD1A", "45CB977B71DFED43", "A6653828E63485C");
    checkMultMod("Mult Mod ", "00000000CB1F03567C39076B", "45CB977B71DFED43", "000000010000000000000000", "00000001");
}

void CorrectnessTest::testMultMontgomeryForm()
{
    cout << "Mult Montgomery Form ";

    int bits = 64;
    
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
        
//        cout << "a: " << a.toHexString() << endl;
//        cout << "e: " << e.toHexString() << endl;

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

        BigInteger::powerMod(&r, &a, &e, &m);
        
        // cout << "r: " << r.toHexString() << endl;
        
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
