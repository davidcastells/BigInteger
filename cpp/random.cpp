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
#include <stdlib.h>
#include <time.h>

/**
 * Sets the integer to a random value (using all the limbs)
 */
void BigInteger::random()
{
    unsigned int lc = (unsigned int) clock();
    while (lc == (unsigned int) clock());
    
    srand((unsigned int) clock());

    
    for (int i=0; i < m_size; i++)
    {
        m_data[i] = rand();
        
        // Mingw random seems to return 16 bit numbers
        #ifdef __MINGW32__
            m_data[i] <<= 16;
            m_data[i] |= rand();
        #endif

        // Windows random seems to return 16 bit numbers also
        #ifdef WIN32
            m_data[i] <<= 16;
            m_data[i] |= rand();
        #endif
    }
}


/**
 * Creates a random number having n bits
 */
void BigInteger::random(int n)
{
    assert(n < getNumBits());
    
    random();
    int shift = getNumBits() - n;
    shiftRight(shift);    
}