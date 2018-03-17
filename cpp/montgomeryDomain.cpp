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

/**
 * Converts the number (n) to its representation in the Montgomery residue system
 * 
 *  nprime = n * R mod m
 * 
 * @param r
 * @param n
 * @param R
 * @param m
 */
void BigInteger::toMontgomeryDomain(BigInteger* nprime, BigInteger* n, BigInteger* R, BigInteger* m)
{
    BigInteger::multMod(nprime, n, R, m);

}

/**
 * Converts back a number from the Montgomery domain into the normal domain
 * 
 *  n = nprime * R^(-1) mod m
 * 
 * @param n
 * @param nprime
 * @param Rinv
 * @param m
 */
void BigInteger::fromMontgomeryDomain(BigInteger* n, BigInteger* nprime, BigInteger* Rinv, BigInteger* m)
{
    BigInteger temp; 
    temp.initSize(nprime->m_size+Rinv->m_size);
    BigInteger::mult(&temp, nprime, Rinv);

    if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::fromMontgomeryDomain temp:" << temp.toHexString() << std::endl;
    if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::fromMontgomeryDomain m:" << m->toHexString() << std::endl;

    BigInteger q; 
    q.initSize(n->m_size);
    BigInteger::div_naive(&temp, m, &q, n);

    if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::fromMontgomeryDomain q:" << q.toHexString() << std::endl;
    if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::fromMontgomeryDomain n:" << n->toHexString() << std::endl;
}

/**
 * 
 * @param radix
 * @param m
 */        
void BigInteger::radixFromMontgomeryMod(BigInteger* radix, BigInteger* m)
{
    assert(radix->m_size > m->getLimbLength());
    
    int mLen = m->getLimbLength()*32;

    radix->setIntValue(1);
    radix->shiftLeft(mLen);      
}

void BigInteger::radixFromMontgomeryModBase2(BigInteger* radix, BigInteger* m)
{
    assert(radix->m_size > m->getLimbLength());
    
    int mLen = m->getLength();

    radix->setIntValue(1);
    radix->shiftLeft(mLen);      
}

void BigInteger::radixInvFromMontgomeryMod(BigInteger* radixInv, BigInteger* radix, BigInteger* mod)
{
    BigInteger radixMod(*radix);
    radixMod.mod(mod);
        
    BigInteger::inverseMod(radixInv, &radixMod, mod);         // radixInv = radix ^ (-1) mod m
}

/**
 * Computes m' in the montgomery domain, this is a value such m * m' mod radix = -1 
 * 
 * To compute it we first compute 
 *      m' = radix - (m ^ -1) mod radix
 * 
 * If number is not invertible, then mprime is zero
 * 
 * @param mprime
 * @param m
 * @param radix
 */
void BigInteger::mprimeFromMontgomeryRadix(BigInteger* mprime, BigInteger* m, BigInteger* radix)
{
    BigInteger temp;
    temp.initSize(radix->m_size);
    temp.copy(m);
    BigInteger invTemp;
    invTemp.initSize(radix->m_size);
    BigInteger::inverseMod(&invTemp, &temp, radix);    // invTemp = (m mod radix) ^ (-1) mod radix
 
    if (invTemp.isZero())
    {
        mprime->zero();
    }
    else
    {
        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::mprimeFromMontgomeryRadix m = " << m->toHexString() << std::endl;
        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::mprimeFromMontgomeryRadix m^-1 = " << invTemp.toHexString() << std::endl;

        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::mprimeFromMontgomeryRadix radix = " << radix->toHexString() << std::endl;

        mprime->copy(radix);
        mprime->subtract(&invTemp);


        if (verbosity > VERBOSITY_LEVEL_MONTGOMERY) std::cout << "BigInteger::mprimeFromMontgomeryRadix mprime = " << mprime->toHexString() << std::endl;
    }
}
