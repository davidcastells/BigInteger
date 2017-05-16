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

#ifndef PERFORMANCETEST_H
#define	PERFORMANCETEST_H

class PerformanceTest {
public:
    PerformanceTest();
    PerformanceTest(const PerformanceTest& orig);
    virtual ~PerformanceTest();

public:
    void run();
    void runAll();
    
private:
    void testPerformanceRange();
    void testPerformanceAdd();
    void testPerformanceSubtract();
    void testPerformanceMult();
    void testPerformanceModMult();
    void testPerformanceModPow();
    void testPerformanceFinalModPow();


public:
    int verbosity;
};

#endif	/* PERFORMANCETEST_H */

