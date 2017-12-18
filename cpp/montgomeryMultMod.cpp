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

void BigInteger::montgomeryMult(BigInteger* y, BigInteger* m, BigInteger* radix, unsigned int mprime)
{
    BigInteger ref(*this);
    BigInteger::montgomeryMult(this, &ref, y, m, radix, mprime);
}

void BigInteger::montgomeryMult(BigInteger* y, BigInteger* m, BigInteger* radix, BigInteger* mprime)
{
    BigInteger ref(*this);
    BigInteger::multMontgomeryForm3(this, &ref, y, m, mprime);
}

void BigInteger::montgomerySquare(BigInteger* m, BigInteger* radix, unsigned int mprime)
{
    BigInteger ref(*this);
    BigInteger::montgomeryMult(this, &ref, &ref, m, radix, mprime);
}

void BigInteger::montgomerySquare(BigInteger* m, BigInteger* radix, BigInteger* mprime)
{
    BigInteger ref(*this);
    BigInteger::multMontgomeryForm3(this, &ref, &ref, m, mprime);
}

/**
 * Algorithm 14.36 from Handbook from applied cryptography
 * Compute montgomery multiplication
 * base b= 2^32
 * 
 * @param r the result r = x * y * R^-1 mod m
 * @param x input value, such that < m
 * @param y input value, such that < m
 * @param m modulo
 * @param radix a value (2^32)^n such that it is bigger than m
 * @param mprime is a value such that m' = -m ^-1 mod b
 *      so m'*m mod b = should be -1
 *      notice that m' is < 2^32
 */
void BigInteger::montgomeryMult(BigInteger* r, BigInteger* x, BigInteger* y, BigInteger* m, BigInteger* radix, unsigned int mprime)
{
    if (extraChecks)
    {
        assert(r != x);
        assert(r != y);
        assert(r != m);
        assert(r != radix);
        assert(x->isLessThan(m));
        assert(y->isLessThan(m));
        assert(r->m_size > m->getLimbLength());
    }
    
    r->zero();
    
    int n = m->getLimbLength();
    
    // initialize variables used in the loop
    BigInteger u;
    u.initSize(4);
    
    BigInteger t2;
    t2.initSize(y->m_size+1);
    
    BigInteger t3;
    t3.initSize(m->m_size+1);
        
    for (int i=0; i < n; i++)
    {
        // ui = (a0 + xi*y0 ) * m' mod b
//        u.setIntValue(x->m_data[i]);
//        u.mult(y->m_data[0]);
//        u.add(r->m_data[0]);
//        u.mult(mprime);
//        
////        cout << " a0 = " << to_hex_string(r->m_data[0]) << endl;
////        cout << " y0 = " << to_hex_string(y->m_data[0]) << endl;
////        cout << " xi = " << to_hex_string(x->m_data[i]) << endl;
////        cout << " m' = " << to_hex_string(mprime) << endl;
////        cout << " u = " << u.toHexString() << endl;
//        
//        unsigned int ui = u.m_data[0];
        unsigned int ui = x->m_data[i];
        ui *= y->m_data[0];
        ui += r->m_data[0];
        ui *= mprime;        
        
        //unsigned long long ui = (r->m_data[0] + x->m_data[i] * y->m_data[0]) * mprime;
        //ui &= 0xFFFFFFFF;
        
//        cout << " r = " << r->toHexString() << endl;
        
        // A = (A  + xi * y + ui * m) / b
        BigInteger::mult(&t2, y, x->m_data[i]);
        
//        cout << " y = " << y->toHexString() << endl;
//        cout << " * xi = " << to_hex_string(x->m_data[i]) << endl;
//        cout << " = " << t2.toHexString() << endl;
        
        BigInteger::mult(&t3, m, ui);
        
//        cout << " ui = " << to_hex_string(ui) << endl;
//        cout << " m = " << m->toHexString() << endl;
//        cout << " * ui = " << t3.toHexString() << endl;
        
        r->add(&t2);
//        cout << " r = " << r->toHexString() << endl;
        
        r->add(&t3);
//        cout << " r = " << r->toHexString() << endl;

        r->shiftRight(32);   // /b
        
//        cout << " r = " << r->toHexString() << endl;

    }
    
    if (!r->isLessThan(m))
    {
        r->subtract(m);
//        cout << " r = " << r->toHexString() << endl;
    }
}


/**
 * Inspired in mmul from https://github.com/adamwalker/mmul/blob/master/crypto.c
 * 
 * @param r result 
 * @param x
 * @param y
 * @param m
 * @param mprime
 */
void BigInteger::multMontgomeryForm3(BigInteger* r, BigInteger* x, BigInteger* y, BigInteger* m, BigInteger* mprime)
{
    int i;

    if (extraChecks)
    {
        assert(r != x);
        assert(r != y);
        assert(r != m);
        assert(r != mprime);
        assert(x->isLessThan(m));
        assert(y->isLessThan(m));
    }
    
    BigInteger t(x->m_size + y->m_size);
    BigInteger tm(m->m_size);
    BigInteger tmm(tm.m_size + m->m_size);
    
    BigInteger::mult(&t, x, y);
    BigInteger::multLow(&tm, &t, mprime);  // was mult low
    BigInteger::mult(&tmm, &tm, m);


    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY)
    {
        std::cout << "BigInteger::multMontgomeryForm2 t:" << t.toHexString() << std::endl;
        std::cout << "BigInteger::multMontgomeryForm2 m:" << m->toHexString() << std::endl;
        std::cout << "BigInteger::multMontgomeryForm2 mprime:" << mprime->toHexString() << std::endl;
        std::cout << "BigInteger::multMontgomeryForm2 tm:" << tm.toHexString() << std::endl;
        std::cout << "BigInteger::multMontgomeryForm2 tmm:" << tmm.toHexString() << std::endl;
    }

    BigInteger u;
    u.initSize(tmm.m_size);
    BigInteger::add(&u, &t, &tmm);


    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm2 u:" << u.toHexString() << std::endl;

    BigInteger::shiftRight(r, &u, m->m_size*32);


    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm2 r:" << r->toHexString() << std::endl;

    if (m->isLessThan(r))   // if (r >= m) r = r - m
    {
        r->subtract(m);

        if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm2 r:" << r->toHexString() << std::endl;

    }
}

/**
 * As implemented in "Handbook of Applied Cryptography" algorithm 14.36
 * we compute r = x * y * R^(-1) mod m
 * 
 * b is the base of our number system (in our case 2^32)
 * n is the number of limbs
 * R = b ^n, the R , so that R > m
 * 
 * @param r         this is the return value, r = x*y*R^(−1) mod m.
 * @param x
 * @param y
 * @param m         modulo value
 * @param mprime    m prime is defined as m'= -m^(-1) mod b, where b is the base
 */
void BigInteger::multMontgomeryForm(BigInteger* r, BigInteger* x, BigInteger* y, BigInteger* m, BigInteger* mprime)
{
    assert(r->m_size >= (m->m_size-1));

    r->zero();

    BigInteger temp;
    BigInteger temp2;
    temp2.initSize(m->m_size+1);
    BigInteger temp3;
    temp3.initSize(temp2.m_size+r->m_size);
    BigInteger temp4;
    temp4.initSize(x->m_size+2);

    BigInteger bxi;
    bxi.initSize(1);
    temp.initSize(y->m_size+bxi.m_size);

    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  x:" << x->toHexString() << std::endl;
    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  y:" << y->toHexString() << std::endl;
    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  m:" << m->toHexString() << std::endl;
    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  mprime:" << mprime->toHexString() << std::endl;

    for (int i=0; i < r->m_size; i++)
    {
        // u[i] = (r[0] + x[i]*y[0]) * mprime mod b
        //      applying (i*j) mod n  = ((i mod n) * ( j mod n)) mod n ...
        // u[i] = ((r[0] + x[i]*y[0]) mod b) * (mprime mod b) mod b
        //      mprime mod b is mprime[0] because b is 2^32 (a limb)
        //      and applying  (i+j) mod n =  ((i mod n) + (j mod n)) mod n;
        // (a[0] + x[i]*y[0]) mod b = a[0] mod b + (x[i] * y[0]) mod b
        //      and because mod b is basically automatically done by using limbs...
        // r[i] = (a[0] + x[i] * y[0]) * mprime[0]
        unsigned int ui = (r->m_data[0] + x->m_data[i] * y->m_data[0]) * mprime->m_data[0];

        // R = (R + x[i]*y + u[i]*m)/b
        bxi.setIntValue(x->m_data[i]);

        BigInteger::mult(&temp, y, &bxi);         // temp = y*x[i]

        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  temp:" << temp.toHexString() << std::endl;
        
        BigInteger bui;
        bui.initSize(1);
        bui.setIntValue(ui);

        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  ui:" << bui.toHexString() << std::endl;
        
        BigInteger::mult(&temp2, m, &bui);        // temp2 = m*u[i]

        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  temp2:" << temp2.toHexString() << std::endl;
        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  r:" << r->toHexString() << std::endl;
        
        BigInteger::add(&temp3, r, &temp2);         // temp3 = r + m*u[i]
        
        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  temp3:" << temp3.toHexString() << std::endl;
        
        BigInteger::add(&temp4, &temp3, &temp);     // temp4 = r + m*u[i] + y*x[i]
        
        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  temp4:" << temp4.toHexString() << std::endl;
        
        //BigInteger::shiftRight(r, &temp4, 32);      // r = temp4 / b
        BigInteger::range(r, &temp4, r->m_size*32, 32);
        
        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm  r:" << r->toHexString() << std::endl;
    }
}

/**
 * Inspired in mmul from https://github.com/adamwalker/mmul/blob/master/crypto.c
 * 
 * @param r result 
 * @param x
 * @param y
 * @param m
 * @param mprime
 */
void BigInteger::multMontgomeryForm2(BigInteger* r, BigInteger* x, BigInteger* y, BigInteger* m, BigInteger* mprime)
{
    int i;

    if (extraChecks)
    {
        assert(r != x);
        assert(r != y);
        assert(r != m);
        assert(r != mprime);
        assert(x->isLessThan(m));
        assert(y->isLessThan(m));
    }
    
//    std::cout << "BigInteger::multMontgomeryForm2 x:" << x->toHexString() << std::endl;
//    std::cout << "BigInteger::multMontgomeryForm2 y:" << y->toHexString() << std::endl;
//    std::cout << "BigInteger::multMontgomeryForm2 m:" << m->toHexString() << std::endl;
//    std::cout << "BigInteger::multMontgomeryForm2 m':" << mprime->toHexString() << std::endl;
        
    
    BigInteger t;
    t.initSize(x->m_size + y->m_size);
    BigInteger pretm;
    pretm.initSize(t.m_size);
    
    BigInteger tm;
    tm.initSize(m->m_size);

    BigInteger tmm;
    tmm.initSize(tm.m_size + m->m_size);
    
    BigInteger::mult(&t, x, y);
    BigInteger::multLow(&pretm, &t, mprime);  // was mult low
    tm.copy(&pretm);
    BigInteger::mult(&tmm, &tm, m);


    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY)
    {
        std::cout << "BigInteger::multMontgomeryForm2 t:" << t.toHexString() << std::endl;
        std::cout << "BigInteger::multMontgomeryForm2 m:" << m->toHexString() << std::endl;
        std::cout << "BigInteger::multMontgomeryForm2 mprime:" << mprime->toHexString() << std::endl;
        std::cout << "BigInteger::multMontgomeryForm2 pretm:" << pretm.toHexString() << std::endl;
        std::cout << "BigInteger::multMontgomeryForm2 tm:" << tm.toHexString() << std::endl;
        std::cout << "BigInteger::multMontgomeryForm2 tmm:" << tmm.toHexString() << std::endl;
    }

    BigInteger u;
    u.initSize(tmm.m_size);
    BigInteger::add(&u, &t, &tmm);


    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm2 u:" << u.toHexString() << std::endl;

    BigInteger::shiftRight(r, &u, m->m_size*32);


    if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm2 r:" << r->toHexString() << std::endl;

    if (m->isLessThan(r))   // if (r >= m) r = r - m
    {
        r->subtract(m);

        if (verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) std::cout << "BigInteger::multMontgomeryForm2 r:" << r->toHexString() << std::endl;

    }
}

