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
#include "../big_integer.h"

#include <assert.h>
#include <iostream>

using namespace std;

void big_integer_multMontgomeryForm3(big_integer* r, big_integer* x, big_integer* y, big_integer* m, big_integer* mprime)
{
    int i;
    

    //if (extraChecks)
    {
        assert(r != x);
        assert(r != y);
        assert(r != m);
        assert(r != mprime);
        assert(big_integer_isLessThan(x, m));
        assert(big_integer_isLessThan(y, m));
    }

    int sr= big_integer_getLimbLength(m);
    
    big_integer t;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(t, x->m_size + y->m_size);

    big_integer tm;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(tm, sr);

    big_integer tmm;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(tmm, tm.m_size + sr);
    
    big_integer_mult(&t, x, y);
    big_integer_multLow(&tm, &t, mprime);  // was mult low
    big_integer_mult(&tmm, &tm, m);


    if (big_integer_verbosity  > VERBOSITY_LEVEL_MULT_MONTGOMERY)
    {
        cout << "big_integer_multMontgomeryForm3 x:" << big_integer_toHexString(x) << endl;
        cout << "big_integer_multMontgomeryForm3 y:" << big_integer_toHexString(y) << endl;
        cout << "big_integer_multMontgomeryForm3 t:" << big_integer_toHexString(&t) << endl;
        cout << "big_integer_multMontgomeryForm3 m:" << big_integer_toHexString(m) << endl;
        cout << "big_integer_multMontgomeryForm3 mprime:" << big_integer_toHexString(mprime) << endl;
        cout << "big_integer_multMontgomeryForm3 tm:" << big_integer_toHexString(&tm) << endl;
        cout << "big_integer_multMontgomeryForm3 tmm:" << big_integer_toHexString(&tmm) << endl;
    }

    big_integer u;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(u, tmm.m_size);
    big_integer_add(&u, &t, &tmm);


    if (big_integer_verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) 
        cout << "big_integer_multMontgomeryForm3 u:" << big_integer_toHexString(&u) << endl;

    
    big_integer_shiftRight(r, &u, sr*32);


    if (big_integer_verbosity  > VERBOSITY_LEVEL_MULT_MONTGOMERY)
        cout << "big_integer_multMontgomeryForm3 r:" << big_integer_toHexString(r) << endl;

    if (big_integer_isLessThan(m, r))   // if (r >= m) r = r - m
    {
        big_integer_subtract_short(r, m);

        if (big_integer_verbosity  > VERBOSITY_LEVEL_MULT_MONTGOMERY) 
            cout << "big_integer_multMontgomeryForm3 r:" << big_integer_toHexString(r) << endl;

    }
}