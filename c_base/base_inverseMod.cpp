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

/**
 *
 * @param ret_data          ret = a ^ -1 mod b
 * @param ret_base
 * @param ret_size
 * @param a_data            number we are computing the inverse
 * @param a_base
 * @param a_size
 * @param b_data            modulo  
 * @param b_base
 * @param b_size
 */
void big_integer_base_inverseMod(unsigned int* ret_data, unsigned int ret_base, const unsigned int ret_size, 
        unsigned int* a_data, unsigned int a_base, unsigned int a_size,
        unsigned int* b_data, unsigned int b_base, unsigned int b_size)
{
    //if (extraChecks)
    {
        assert(big_integer_base_isLessThan(a_data, a_base, a_size, b_data, b_base, b_size));
//                assert(b->m_size == ret->m_size);
//                assert(a->m_size == ret->m_size);
    }



    int maxSize = ret_size;
    if (a_size > maxSize) maxSize = a_size;
    if (b_size > maxSize) maxSize = b_size;


    // int t, nt, r, nr, q, tmp;
    // if (b < 0) b = -b;   // not necessary, always positive
    // if (a < 0) a = b - (-a % b); // not necessary, always positive

    unsigned int t_data[maxSize];
    unsigned int t_base = 0;
    unsigned int t_size = maxSize;
    
    unsigned int nt_data[maxSize];
    unsigned int nt_base = 0;
    unsigned int nt_size = maxSize;
    
    unsigned int r_data[maxSize];
    unsigned int r_base = 0;
    unsigned int r_size = maxSize;
    
    unsigned int nr_data[maxSize];
    unsigned int nr_base = 0;
    unsigned int nr_size = maxSize;

    unsigned int q_data[maxSize];
    unsigned int q_base = 0;
    unsigned int q_size = maxSize;
    
    unsigned int tmp_data[maxSize];
    unsigned int tmp_base = 0;
    unsigned int tmp_size = maxSize;

    unsigned int p_data[maxSize*2];
    unsigned int p_base = 0;
    unsigned int p_size = maxSize*2;

    big_integer_base_zero(t_data, t_base, t_size);                               // t = 0;  
    big_integer_base_setIntValue(nt_data, nt_base, nt_size, 1);                  // nt = 1;  
    big_integer_base_copy(r_data, r_base, r_size, b_data, b_base, b_size);  // r = b;  
    big_integer_base_div_naive(a_data, a_base, a_size, b_data, b_base, b_size, 
            tmp_data, tmp_base, tmp_size, nr_data, nr_base, nr_size);       // nr = a % b;

    while (!big_integer_base_isZero(nr_data, nr_base, nr_size))            // while (nr != 0) 
    {
        if (base_verbosity)
        {
            cerr << "r: " << big_integer_base_toHexString(r_data, r_base, r_size) << endl;
            cerr << "nr: " << big_integer_base_toHexString(nr_data, nr_base, nr_size) << endl;
            cerr << "t: " << big_integer_base_toHexString(t_data, t_base, t_size) << endl;
            cerr << "nt: " << big_integer_base_toHexString(nt_data, nt_base, nt_size) << endl;
        }

        big_integer_base_div_naive(r_data, r_base, r_size,
                nr_data, nr_base, nr_size,
                q_data, q_base, q_size,
                tmp_data, tmp_base, tmp_size);      // q = r/nr;
        
        big_integer_base_copy(tmp_data, tmp_base, tmp_size, nt_data, nt_base, nt_size);                              // tmp = nt;
        big_integer_base_mult(p_data, p_base, p_size, q_data, q_base, q_size, nt_data, nt_base, nt_size);
        big_integer_base_copy(nt_data, nt_base, nt_size, t_data, t_base, t_size);
        big_integer_base_subtract_short(nt_data, nt_base, nt_size, p_data, p_base, p_size);                            // nt = t - q*nt;
        big_integer_base_copy(t_data, t_base, t_size, tmp_data, tmp_base, tmp_size);                               // t = tmp;

        if (base_verbosity)
        {
            cerr << "r: " << big_integer_base_toHexString(r_data, r_base, r_size) << endl;
            cerr << "nr: " << big_integer_base_toHexString(nr_data, nr_base, nr_size) << endl;
            cerr << "t: " << big_integer_base_toHexString(t_data, t_base, t_size) << endl;
            cerr << "nt: " << big_integer_base_toHexString(nt_data, nt_base, nt_size) << endl;
        }

        big_integer_base_copy(tmp_data, tmp_base, tmp_size, nr_data, nr_base, nr_size);                              // tmp = nr; 
        big_integer_base_mult(p_data, p_base, p_size, q_data, q_base, q_size, nr_data, nr_base, nr_size);
        big_integer_base_copy(nr_data, nr_base, nr_size, r_data, r_base, r_size);
        big_integer_base_subtract_short(nr_data, nr_base, nr_size, p_data, p_base, p_size);                            // nr = r - q*nr;
        big_integer_base_copy(r_data, r_base, r_size, tmp_data, tmp_base, tmp_size);
    }

    unsigned int one_data[1]={1};
    unsigned int one_base = 0;
    unsigned int one_size = 1;

    if (big_integer_base_isLessThan(one_data, one_base, one_size, r_data, r_base, r_size))     // if (r > 1) return -1;  / * No inverse * /
    {
        if (base_verbosity > VERBOSITY_LEVEL_INV_MOD)
        {
            cerr << "BigInteger::inverseMod Number not invertible " << endl;
            cerr << "BigInteger::inverseMod a: " << big_integer_base_toHexString(a_data, a_base, a_size) << endl;
            cerr << "BigInteger::inverseMod b: " << big_integer_base_toHexString(b_data, b_base, b_size) << endl;
            cerr << "BigInteger::inverseMod r: " << big_integer_base_toHexString(r_data, r_base, r_size) << endl;
        }
        //assert(false);
        big_integer_base_zero(t_data, t_base, t_size);
    }
    else if (big_integer_base_isNegative(t_data, t_base, t_size)) // if (t < 0)
    {
        big_integer_base_add_short(t_data, t_base, t_size, b_data, b_base, b_size);       //  t += b;

        if (base_verbosity) //  > VERBOSITY_LEVEL_INV_MOD) 
            cerr << "BigInteger::inverseMod t: " << big_integer_base_toHexString(t_data, t_base, t_size) << endl;
    }

    big_integer_base_copy(ret_data, ret_base, ret_size, t_data, t_base, t_size);
}