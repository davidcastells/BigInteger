/* 
 * File:   PerformanceTestZZ.h
 * Author: dcr
 *
 * Created on May 10, 2017, 8:22 AM
 */

#ifndef PERFORMANCETESTZZ_H
#define	PERFORMANCETESTZZ_H

class PerformanceTestZZ {
public:
    PerformanceTestZZ();
    PerformanceTestZZ(const PerformanceTestZZ& orig);
    virtual ~PerformanceTestZZ();

public:
    void run();
    
private:
    void testPerformanceRange();
    void testPerformanceAdd();
    void testPerformanceSubtract();
    void testPerformanceMult();
    void testPerformanceModMult();
    void testPerformanceModPow();
};

#endif	/* PERFORMANCETESTZZ_H */

