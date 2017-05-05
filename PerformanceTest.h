/* 
 * File:   PerformanceTest.h
 * Author: dcr
 *
 * Created on April 24, 2017, 4:16 PM
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
    
private:
    void testPerformanceRange();
    void testPerformanceAdd();
    void testPerformanceSubtract();
    void testPerformanceMult();
    void testPerformanceModMult();

};

#endif	/* PERFORMANCETEST_H */

