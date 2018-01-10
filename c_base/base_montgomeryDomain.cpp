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

#include "../big_integer_base.h"

#include <assert.h>
#include <iostream>

using namespace std;

void big_integer_base_mprimeFromMontgomeryRadix(unsigned int* mprime_data,
        unsigned int mprime_base,
        const unsigned int mprime_size,
        unsigned int* m_data,
        unsigned int m_base,
        const unsigned int m_size,
        unsigned int* radix_data, 
        unsigned int radix_base,
        const unsigned int radix_size)
{
    unsigned int temp_data[radix_size];
    unsigned int temp_base = 0;
    const unsigned int temp_size = radix_size;

    big_integer_base_copy(temp_data, temp_base, temp_size, m_data, m_base, m_size);

    unsigned int invTemp_data[radix_size];
    unsigned int invTemp_base = 0;
    const unsigned int invTemp_size = radix_size;

    big_integer_base_inverseMod(invTemp_data, invTemp_base, invTemp_size,
            temp_data, temp_base, temp_size,
            radix_data, radix_base, radix_size);
 
    if (big_integer_base_isZero(invTemp_data, invTemp_base, invTemp_size))
    {
        big_integer_base_zero(mprime_data, mprime_base, mprime_size);
    }
    else
    {
        if (base_verbosity) // > VERBOSITY_LEVEL_MONTGOMERY) 
            cout << "BigInteger::mprimeFromMontgomeryRadix m = " << big_integer_base_toHexString(m_data, m_base, m_size) << endl;
        if (base_verbosity) // > VERBOSITY_LEVEL_MONTGOMERY) 
            cout << "BigInteger::mprimeFromMontgomeryRadix m^-1 = " << big_integer_base_toHexString(invTemp_data, invTemp_base, invTemp_size) << endl;

        if (base_verbosity) //  > VERBOSITY_LEVEL_MONTGOMERY) 
            cout << "BigInteger::mprimeFromMontgomeryRadix radix = " << big_integer_base_toHexString(radix_data, radix_base, radix_size) << endl;

        big_integer_base_copy(mprime_data, mprime_base, mprime_size, radix_data, radix_base, radix_size);
        big_integer_base_subtract_short(mprime_data, mprime_base, mprime_size, invTemp_data, invTemp_base, invTemp_size);


        if (base_verbosity) //  > VERBOSITY_LEVEL_MONTGOMERY) 
            cout << "BigInteger::mprimeFromMontgomeryRadix mprime = " << big_integer_base_toHexString(mprime_data, mprime_base, mprime_size) << endl;
    }
}

void big_integer_base_radixFromMontgomeryMod(unsigned int* radix_data, unsigned int radix_base, unsigned int radix_size,
        unsigned int* m_data, unsigned int m_base, unsigned int m_size)
{
    assert(radix_size > big_integer_base_getLimbLength(m_data, m_base, m_size));
    
    int mLen = big_integer_base_getLimbLength(m_data, m_base, m_size)*32;

    big_integer_base_setIntValue(radix_data, radix_base, radix_size, 1);
    big_integer_base_shiftLeft_short(radix_data, radix_base, radix_size, mLen);      
}