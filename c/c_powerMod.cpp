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

void big_integer_powerMod(big_integer* r, big_integer* v, big_integer* power, big_integer* mod)
{
    //if (extraChecks)
    {
        assert(r != v);
        assert(r != power);
        assert(r != mod);
    }

    big_integer x;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(x, mod->m_size+1); // for interleaved operations
    big_integer_mod(&x, v, mod);    // ensure x is less than mod, x = v mod mod
    
    big_integer y;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(y, power->m_size);
    big_integer_copy(&y, power);


    // we use a simple square-and-multiply algorithm,
    if (big_integer_verbosity) //  > VERBOSITY_LEVEL_POWER_MOD) 
        cout << "BigInteger::powerMod v " << big_integer_toHexString(v) << endl;
    if (big_integer_verbosity) //  > VERBOSITY_LEVEL_POWER_MOD) 
        cout << "BigInteger::powerMod power " << big_integer_toHexString(power) << endl;
    if (big_integer_verbosity) //  > VERBOSITY_LEVEL_POWER_MOD) 
        cout << "BigInteger::powerMod mod " << big_integer_toHexString(mod) << endl;

    big_integer_setIntValue(r, 1);

    while(!big_integer_isZero(&y))
    {
            if (big_integer_isOdd(&y))
                big_integer_multMod_interleaved_short(r, &x, mod);

            big_integer_shiftRight_short(&y, 1);    // / 2
            big_integer_squareMod_short(&x, mod); 
    }

    if (big_integer_verbosity) //  > VERBOSITY_LEVEL_POWER_MOD) 
        cout << "BigInteger::powerMod r " << big_integer_toHexString(r) << endl;
}


void big_integer_powerMod_interleaved(big_integer* r, big_integer* v, big_integer* power, big_integer* mod)
{
    //if (extraChecks)
    {
        assert(r != v);
        assert(r != power);
        assert(r != mod);
        assert(big_integer_getNumBits(r) > big_integer_getLength(v));
        assert(big_integer_getNumBits(r) > big_integer_getLength(power));
        assert(big_integer_getNumBits(r) > big_integer_getLength(mod));
    }

    big_integer x;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(x, mod->m_size+1); // for interleaved operations
    big_integer_copy(&x, v);
    //BigInteger y(*power);   // @todo remove y

    int maxBit = big_integer_getLength(power);

    // When using interleaved we need to apply mod before
    big_integer_mod_short(&x, mod);
    // y.mod(mod);

    // we use a simple square-and-multiply algorithm,
    if (big_integer_verbosity > VERBOSITY_LEVEL_POWER_MOD) 
    {
        cout << "BigInteger::powerMod v " << big_integer_toHexString(&x) << endl;
        cout << "BigInteger::powerMod power " << big_integer_toHexString(power) << endl;
        cout << "BigInteger::powerMod mod " << big_integer_toHexString(mod) << endl;
    }
    
    big_integer_setIntValue(r, 1);

//        while(!y.isZero())
    for (int i=0; i < maxBit; i++)
    {
            //if (y.isOdd())
        if (big_integer_getBit(power, i))
        {
//                    std::cout << " r " << r->toHexString() << std::endl;
//                    std::cout << "  * x " << x.toHexString() << std::endl;
//                    std::cout << " mod mod " << mod->toHexString() << std::endl;

            big_integer_multMod_interleaved_short(r, &x, mod);

//                    std::cout << " = r " << r->toHexString() << std::endl;
        }

            // y.shiftRight(1);    // / 2
            big_integer_squareMod_interleaved_short(&x, mod); 
    }

    if (big_integer_verbosity > VERBOSITY_LEVEL_POWER_MOD) 
        cout << "BigInteger::powerMod r " << big_integer_toHexString(r) << endl;
}
