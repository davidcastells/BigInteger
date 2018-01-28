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

void big_integer_base_powerModMont(unsigned int * r_data, const unsigned int r_base, const unsigned int r_size, 
	unsigned int* x_data, const unsigned int x_base, const unsigned int x_size, 
	unsigned int* e_data, const unsigned int e_base, const unsigned int e_size,
	unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    unsigned int radix_data[m_size+1];
    const unsigned int radix_size = m_size;
    const unsigned int radix_base = 0;

    unsigned int mprime_data[m_size+1];
    const unsigned int mprime_size = m_size;
    const unsigned int mprime_base = 0;
        
    big_integer_base_radixFromMontgomeryMod(radix_data, radix_base, radix_size, m_data, m_base, m_size);
    big_integer_base_mprimeFromMontgomeryRadix(mprime_data, mprime_base, mprime_size, m_data, m_base, m_size, radix_data, radix_base, radix_size);
    
    big_integer_base_powerModMontgomery(r_data, r_base, r_size,
            x_data, x_base, x_size,
            e_data, e_base, e_size,
            m_data, m_base, m_size,
            mprime_data, mprime_base, mprime_size,
            radix_data, radix_base, radix_size);
}

/**
 * As implemented in "Handbook of Applied Cryptography" algorithm 14.94
 * we compute r = x ^ e mod m
 * @param r
 * @param v
 * @param mod
 */
void big_integer_base_powerModMontgomery(unsigned int * r_data, const unsigned int r_base, const unsigned int r_size, 
	unsigned int* x_data, const unsigned int x_base, const unsigned int x_size, 
	unsigned int* e_data, const unsigned int e_base, const unsigned int e_size,
	unsigned int* m_data, const unsigned int m_base, const unsigned int m_size,
	unsigned int* mprime_data, const unsigned int mprime_base, const unsigned int mprime_size, 
	unsigned int* radix_data, const unsigned int radix_base, const unsigned int radix_size)
{
    //assert(big_integer_base_isLessThan(x_data, x_base, x_size, m_data, m_base, m_size));
    
    if (base_verbosity > VERBOSITY_LEVEL_POWER_MOD)
    {
        cout << "x = " << big_integer_base_toHexString(x_data, x_base, x_size) << endl;
        cout << "e = " << big_integer_base_toHexString(e_data, e_base, e_size) << endl;
        cout << "m = " << big_integer_base_toHexString(m_data, m_base, m_size) << endl;
        cout << "m' = " << big_integer_base_toHexString(mprime_data, mprime_base, mprime_size) << endl;
        cout << "radix = " << big_integer_base_toHexString(radix_data, radix_base, radix_size) << endl;
    }
    
    unsigned int radix2_data[m_size];
    const unsigned int radix2_size = m_size;
    const unsigned int radix2_base = 0;

    // a = radix mod m
    unsigned int a_data[m_size];
    unsigned int a_size = m_size;
    const unsigned int a_base = 0;

    big_integer_base_mod(a_data, a_base, a_size, radix_data, radix_base, radix_size, m_data, m_base, m_size);

    // radix2 = radix^2 mod m = a^2 mod m
    // interleaved requires that radix be less than m, so we use a
    big_integer_base_squareMod_interleaved(radix2_data, radix2_base, radix2_size,
            a_data, a_base, a_size, 
            m_data, m_base, m_size);

    if (base_verbosity > VERBOSITY_LEVEL_POWER_MOD)
    {
        cout << "a = radix mod m =  " << big_integer_base_toHexString(a_data, a_base, a_size) << endl;
        cout << "radix^2 mod m = " << big_integer_base_toHexString(radix2_data, radix2_base, radix2_size) << endl;

    }
	unsigned int xprime_data[m_size];
	const unsigned int xprime_size = m_size;
	const unsigned int xprime_base = 0;
	
        big_integer_base_mod_short(x_data, x_base, x_size, m_data, m_base, m_size);
        
	big_integer_base_multMontgomeryForm3(xprime_data, xprime_base, xprime_size,
		x_data, x_base, x_size,
		radix2_data, radix2_base, radix2_size,
		m_data, m_base, m_size,
		mprime_data, mprime_base, mprime_size);
	
    if (base_verbosity > VERBOSITY_LEVEL_POWER_MOD)
    {
        cout << "xprime = x * r^2 * R mod m = " << big_integer_base_toHexString(xprime_data, xprime_base, xprime_size) << endl;
    }	
	// a.reduceWorkingSize(m->m_size);
	// a_size = m_size;
	
	int t = big_integer_base_getLength(e_data, e_base, e_size);
	
	for (int i=t-1; i >= 0; i--)
	{
		int ei = big_integer_base_getBit(e_data, e_base, e_size, i);
		
		unsigned int temp_data[a_size];
		const unsigned int temp_size = a_size;
		const unsigned int temp_base = 0;
		
                big_integer_base_copy(temp_data, temp_base, temp_size, a_data, a_base, a_size);
		big_integer_base_multMontgomeryForm3(a_data, a_base, a_size,
			temp_data, temp_base, temp_size,
			temp_data, temp_base, temp_size,
			m_data, m_base, m_size,
			mprime_data, mprime_base, mprime_size);
		
                if (base_verbosity > VERBOSITY_LEVEL_POWER_MOD)
                {
                    cout << "a = a^2 * R^-1 mod m =  " << big_integer_base_toHexString(a_data, a_base, a_size) << endl;
                }
                
		if (ei)
		{
			big_integer_base_copy(temp_data, temp_base, temp_size, a_data, a_base, a_size);
			big_integer_base_multMontgomeryForm3(a_data, a_base, a_size,
				temp_data, temp_base, temp_size,
				xprime_data, xprime_base, xprime_size,
				m_data, m_base, m_size,
				mprime_data, mprime_base, mprime_size);
                        
                        if (base_verbosity > VERBOSITY_LEVEL_POWER_MOD)
                        {
                            cout << "a = a^2 * R^-1 mod m =  " << big_integer_base_toHexString(a_data, a_base, a_size) << endl;
                        }

		}   
	}
	
	unsigned int one_data[1];
	const unsigned int one_size = 1;
	const unsigned int one_base = 0;
	one_data[0] = 1;
	
	big_integer_base_multMontgomeryForm3(r_data, r_base, r_size,
		a_data, a_base, a_size,
		one_data, one_base, one_size,
		m_data, m_base, m_size,
		mprime_data, mprime_base, mprime_size);
        
        if (base_verbosity > VERBOSITY_LEVEL_POWER_MOD)
        {
            cout << "r = a * 1 * R^-1 mod m =  " << big_integer_base_toHexString(r_data, r_base, r_size) << endl;
        }

}