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

/* 
 * File:   assertf.h
 * Author: dcr
 *
 * Created on March 16, 2018, 6:11 PM
 */

#ifndef ASSERTF_H
#define ASSERTF_H

#include <assert.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define log_error(M, ...) fprintf(stderr, "[ERROR] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define assertf(A, M, ...) if(!(A)) {log_error(M, ##__VA_ARGS__); assert(A); }



#ifdef __cplusplus
}
#endif

#endif /* ASSERTF_H */

