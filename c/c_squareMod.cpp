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



void big_integer_squareMod_short(big_integer* x, big_integer* m)
{
    big_integer ref;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(ref, x->m_size);
    big_integer_copy(&ref, x);

    big_integer_squareMod(x, &ref, m);
}

void big_integer_squareMod(big_integer* r, big_integer* v, big_integer* m)
{
    //if (extraChecks) 
    {
        assert(r != v);
        assert(r != m);
        assert(big_integer_isLessThan(v, m));
    }

    // was multMod
    big_integer_multMod_interleaved(r, v, v, m);
}
