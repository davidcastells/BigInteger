/* 
 * File:   PerformanceLap.h
 * Author: dcr
 *
 * Created on April 14, 2016, 5:46 PM
 */

#ifndef PERFORMANCELAP_H
#define	PERFORMANCELAP_H

class PerformanceLap {
public:
    PerformanceLap();
    virtual ~PerformanceLap();
    
    void start();
    double stop();
    double lap();
    
private:
    double d0;
    double df;
};

#endif	/* PERFORMANCELAP_H */

