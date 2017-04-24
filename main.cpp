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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CorrectnessTest.h"


void printUsage()
{
    printf("Usage:\n");
    printf("  ./BigInteger [options]\n");
    printf("\n");
    printf("[options]\n");
    printf("  --help                        prints this message\n");
    printf("  --test-correctness            test the correctness of all the algorithms\n");
}


int doTestCorrectness;

void parseArgs(int argc, char* args[])
{
    for (int i=0; i < argc; i++)
    {
        if (strcmp(args[i], "--test-correctness") == 0)
            doTestCorrectness = 1;
                
    }
}

/*
 * 
 */
int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printUsage();
        return (EXIT_SUCCESS);
    }
    
    parseArgs(argc, argv);
    
    if (doTestCorrectness)
    {
        CorrectnessTest test;
        test.run();
    }
    
    return (EXIT_SUCCESS);
}

