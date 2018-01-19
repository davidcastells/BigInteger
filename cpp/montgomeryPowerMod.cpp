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
#include "../BigInteger.h"

#include <assert.h>
#include <iostream>

using namespace std;
        

void BigInteger::powerModMontgomery(BigInteger* r, BigInteger* x, BigInteger* e, BigInteger* m)
{
    BigInteger radix;
    radix.initSize(m->m_size+1);
    BigInteger mprime;
    mprime.initSize(radix.m_size);        
        
    BigInteger::radixFromMontgomeryMod(&radix, m);        
    BigInteger::mprimeFromMontgomeryRadix(&mprime, m, &radix);
    
    BigInteger::powerModMontgomery(r, x, e, m, &mprime, &radix);
}

/**
 * As implemented in "Handbook of Applied Cryptography" algorithm 14.94
 * we compute r = x ^ e mod m
 * @param r
 * @param x should be < mod, otherwise we fix it
 * @param mod
 */
void BigInteger::powerModMontgomery(BigInteger* r, BigInteger* x, BigInteger* e, BigInteger* m, BigInteger* mprime, BigInteger* radix)
{
    if (extraChecks)
    {
        assert(!m->isZero());
        assert(!mprime->isZero());
        assert(r != x);
        assert(r != e);
        assert(r != m);
        assert(r != mprime);
        assert(r != radix);
    }
    BigInteger radix2;
    radix2.initSize(m->m_size);
    squareMod(&radix2, radix, m);

    if (verbosity > VERBOSITY_LEVEL_POWER_MOD)
    {
        std::cout << "BigInteger::powerModMontgomery x: " << x->toHexString() << std::endl;
        std::cout << "BigInteger::powerModMontgomery e: " << e->toHexString() << std::endl;
        std::cout << "BigInteger::powerModMontgomery radix: " << radix->toHexString() << std::endl;
        std::cout << "BigInteger::powerModMontgomery m: " << m->toHexString() << std::endl;
        std::cout << "BigInteger::powerModMontgomery m': " << mprime->toHexString() << std::endl;
        std::cout << "BigInteger::powerModMontgomery radix ^2 mod m: " << radix2.toHexString() << std::endl;
    }

    BigInteger xprime;
    xprime.initSize(m->m_size);

    // make sure x < m
    x->mod(m);

    // we compute xprime = x * (radix^2 mod m) * radix ^-1 mod m 
    multMontgomeryForm3(&xprime, x, &radix2, m, mprime);

    if (verbosity > VERBOSITY_LEVEL_POWER_MOD)
        cout << "BigInteger::powerModMontgomery xprime: " << xprime.toHexString() << endl;

//            BigInteger a(*radix);
//            a.mod(m);
//            a.reduceWorkingSize(m->m_size);

    BigInteger a;
    a.initSize(m->m_size);
    BigInteger::mod_naive( radix, m, &a);

    int t = e->getLength();

    BigInteger temp(a);

    for (int i=t-1; i >= 0; i--)
    {
        int ei = e->getBit(i);

        temp.copy(&a);
        multMontgomeryForm3(&a, &temp, &temp, m, mprime);

        if (verbosity > VERBOSITY_LEVEL_POWER_MOD)
        {
            cout << "a = a^2 * R^-1 mod m =  " << a.toHexString() << endl;
        }
        
        if (ei)
        {
            temp.copy(&a);
            multMontgomeryForm3(&a, &temp, &xprime, m, mprime);
            
            if (verbosity > VERBOSITY_LEVEL_POWER_MOD)
            {
                cout << "a = x' * a * R^-1 mod m =  " << a.toHexString() << endl;
            }
        }   
    }

    BigInteger one;
    one.initSize(1);
    one.setIntValue(1);

    multMontgomeryForm3(r, &a, &one, m, mprime);
    
    if (verbosity > VERBOSITY_LEVEL_POWER_MOD)
    {
        cout << "r = a * 1 * R^-1 mod m =  " << r->toHexString() << endl;
    }
}



unsigned int bnExpModThreshTable[] = {7, 25, 81, 241, 673, 1793, 0xffffffff /*Integer.MAX_VALUE*/ }; // Sentinel

    /**
     * Returns a BigInteger whose value is x to the power of y mod z.
     * Assumes: z is odd && x < z.
     *
     * The algorithm is adapted from Colin Plumb's C library.
     *
     * The window algorithm:
     * The idea is to keep a running product of b1 = n^(high-order bits of exp)
     * and then keep appending exponent bits to it.  The following patterns
     * apply to a 3-bit window (k = 3):
     * To append   0: square
     * To append   1: square, multiply by n^1
     * To append  10: square, multiply by n^1, square
     * To append  11: square, square, multiply by n^3
     * To append 100: square, multiply by n^1, square, square
     * To append 101: square, square, square, multiply by n^5
     * To append 110: square, square, multiply by n^3, square
     * To append 111: square, square, square, multiply by n^7
     *
     * Since each pattern involves only one multiply, the longer the pattern
     * the better, except that a 0 (no multiplies) can be appended directly.
     * We precompute a table of odd powers of n, up to 2^k, and can then
     * multiply k bits of exponent at a time.  Actually, assuming random
     * exponents, there is on average one zero bit between needs to
     * multiply (1/2 of the time there's none, 1/4 of the time there's 1,
     * 1/8 of the time, there's 2, 1/32 of the time, there's 3, etc.), so
     * you have to do one multiply per k+1 bits of exponent.
     *
     * The loop walks down the exponent, squaring the result buffer as
     * it goes.  There is a wbits+1 bit lookahead buffer, buf, that is
     * filled with the upcoming exponent bits.  (What is read after the
     * end of the exponent is unimportant, but it is filled with zero here.)
     * When the most-significant bit of this buffer becomes set, i.e.
     * (buf & tblmask) != 0, we have to decide what pattern to multiply
     * by, and when to do it.  We decide, remember to do it in future
     * after a suitable number of squarings have passed (e.g. a pattern
     * of "100" in the buffer requires that we multiply by n^1 immediately;
     * a pattern of "110" calls for multiplying by n^3 after one more
     * squaring), clear the buffer, and continue.
     *
     * When we start, there is one more optimization: the result buffer
     * is implcitly one, so squaring it or multiplying by it can be
     * optimized away.  Further, if we start with a pattern like "100"
     * in the lookahead window, rather than placing n into the buffer
     * and then starting to square it, we have already computed n^2
     * to compute the odd-powers table, so we can place that into
     * the buffer and save a squaring.
     *
     * This means that if you have a k-bit window, to compute n^z,
     * where z is the high k bits of the exponent, 1/2 of the time
     * it requires no squarings.  1/4 of the time, it requires 1
     * squaring, ... 1/2^(k-1) of the time, it reqires k-2 squarings.
     * And the remaining 1/2^(k-1) of the time, the top k bits are a
     * 1 followed by k-1 0 bits, so it again only requires k-2
     * squarings, not k-1.  The average of these is 1.  Add that
     * to the one squaring we have to do to compute the table,
     * and you'll see that a k-bit window saves k-2 squarings
     * as well as reducing the multiplies.  (It actually doesn't
     * hurt in the case k = 1, either.)
     * 
     * inspired from http://grepcode.com/file_/repository.grepcode.com/java/root/jdk/openjdk/6-b14/java/math/BigInteger.java/?v=source
     */
void BigInteger::powerMod_ColinPlumb(BigInteger* r, BigInteger* x, BigInteger* exp, BigInteger* mod) 
{
    BigInteger radix(mod->m_size+1);
    BigInteger mprime(radix.m_size);
    
    BigInteger::radixFromMontgomeryMod(&radix, mod);
    BigInteger::mprimeFromMontgomeryRadix(&mprime, mod, &radix);
    
    powerMod_ColinPlumb(r, x, exp, mod, &mprime, &radix);
}

void BigInteger::powerMod_ColinPlumb(BigInteger* r, BigInteger* x, BigInteger* exp, BigInteger* mod, BigInteger* mprime, BigInteger* radix) 
{
    assert(!exp->isOne());
    assert(!x->isZero());
    assert(x->isLessThan(mod));
    assert(r->getNumBits() >= mod->getLimbLength());

//    BigInteger base = new BigInteger(*x);

    // Select an appropriate window size
    int windowBits = 0;

    while (exp->getLength() > bnExpModThreshTable[windowBits]) 
    {
        windowBits++;
    }

    // Calculate appropriate table size
    int tblmask = 1 << windowBits;

    // Allocate table for precomputed odd powers of base in Montgomery form
    BigInteger table[tblmask];
    
    for (int i=0; i < tblmask; i++)
        table[i].initSize(mod->m_size);

    // Compute the modular inverse
    
    int inv = mprime->m_data[0];

    // Convert base to Montgomery form
    //BigInteger a = base.multiply(radix); // base.shiftLeft(mod.getLimbLength() * 32);
    
    BigInteger::toMontgomeryDomain(&table[0], x, radix, mod);
//        table[0] = base.modMult(radix, mod); //  a.mod(mod);

//        System.out.println("radix=" + radix.toHexString());
//        System.out.println("base=" + base.toHexString());
//        System.out.println("mod=" + mod.toHexString());

//        System.out.println("table[0]=" + table[0].toHexString() );

        // Set b to the square of the base
//        BigInteger b = table[0].square(); 
//        System.out.println("square=" + b.toHexString() );
//        System.out.println("mod=" + mod.toHexString() );
//        b = b.montReduce(mod, inv);
    BigInteger b(mod->m_size+1);
        
    BigInteger::multMontgomeryForm3(&b, &table[0], &table[0], mod, mprime );
        
    //BigInteger b = table[0].montMult(table[0], mod, inv);
//        System.out.println("b=" + b.toHexString() );

        // Set t to high half of b
        

//        System.out.println("t=" + b.toHexString() );


        // Fill in the table with odd powers of the base
        for (int i=1; i<tblmask; i++)
        { 
//            System.out.println("t=" + t.toHexString() );
//            System.out.println("table["+(i-1)+"]=" + table[i-1].toHexString() );
//            System.out.println("prod" + t.multiply(table[i-1]).toHexString() );
//            
//            System.out.println("mod" + mod.toHexString() );

//            table[i] = t.multiply(table[i-1]).montReduce(mod, inv);
            //table[i] = b.montMult(table[i-1], mod, inv);
            BigInteger::multMontgomeryForm3(&table[i], &b, &table[i-1], mod, mprime);
            
//            System.out.println("table["+i+"]=" + table[i].toHexString() );
        }

        // Pre load the window that slides over the exponent
        
        int buf2 = 0;
        
        int nStartBit = exp->getLength()-1;
        int nStopBit = nStartBit; // nStartBit - windowBits;

        for (int i = 0; i <= windowBits; i++) 
        {
            buf2 = exp->binary_to_decimal(nStartBit, nStopBit);
            nStopBit--;
        }
        
        // @todo bitpos and eIndex should be replaced by nBit

//        buf = exp.binary_to_decimal(nStartBit, nStopBit);
        
//        System.out.println("exp=" + exp.toHexString());
  //      System.out.println("["+nStartBit+", "+nStopBit+"] = " + String.format("%08X", buf));
//        System.out.println("tbuf="+String.format("%08X", tbuf));


        // The first iteration, which is hoisted out of the main loop
        int isone = true;

        
        // take just odd exponent window values
        int multpos = nStartBit-windowBits;

        int oldStopBit = nStopBit;
        nStopBit++;
        while ((buf2 & 1) == 0)
        {
            multpos++;
            nStopBit++;
            
            buf2 = exp->binary_to_decimal(nStartBit, nStopBit);
        }
 
        if (nStopBit == nStartBit)
            isone = false;

        nStopBit = oldStopBit;
        
        int step = 0;
//        System.out.println("buf["+(step++)+"]="+buf2);
        BigInteger* mult = &table[buf2 >> 1];

//        System.out.println("select table[" + (buf>>>1) + "] -> mult = " + mult.toHexString());
//        ebits = nStartBit;
        buf2 = 0;
        
 
        
        
        // The main loop
        while(true)
        {
            nStartBit--;
            
            // Advance the window
            buf2 <<= 1;

//            System.out.println("ebits " + ebits + " buf " + buf);
            
            if (nStopBit >= 0)
            {
                buf2 |= exp->getBit(nStopBit);
                nStopBit--;
                
                
  //              int buf2 = exp.binary_to_decimal(nStartBit, nStopBit);
                
    //            System.out.println("buf2=" + buf2);
            }
            else
            {
//                System.err.println("ending");
            }

      //      System.out.println("exp=" + exp.toHexString());
        //    System.out.println("["+nStartBit+", "+nStopBit+"] = " + String.format("%08X", buf));

            // Examine the window for pending multiplies
            if ((buf2 & tblmask) != 0) 
            {
                multpos = nStartBit - windowBits;
                while ((buf2 & 1) == 0) 
                {
                    buf2 >>= 1;
                    //nStopBit--;
                    multpos++;
                }

//                System.out.println("buf["+(step++)+"]="+buf2);
                mult = &table[buf2 >> 1];

//                System.out.println("select table[" + (buf>>>1) + "] -> mult = " + mult.toHexString());
                buf2 = 0;
            }

            // Perform multiply
            if (nStartBit == multpos) 
            {
                if (isone)
                {
                    b.copy(mult);
                    isone = false;

//                    System.out.println("step["+(step++)+"] b = " + b.toHexString());
                }
                else
                {
                    //b = b.montMult(mult, mod, inv);
                    b.montgomeryMult(mult, mod, radix, mprime);
//                    System.out.println("step["+(step++)+"] mr = " + b.toHexString());
                }
            }

            // Check if done
            if (nStartBit == 0)
                break;

            // Square the input
            if (!isone) 
            {
                b.montgomerySquare(mod, radix, mprime);
//                System.out.println("step["+(step++)+"] mr = " + b.toHexString());
            }
        }

        // Convert result out of Montgomery form and return
        BigInteger::montgomeryReduction(r, &b, mod, radix, mprime->m_data[0]);
    
    }