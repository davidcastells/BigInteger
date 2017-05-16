/* 
 * File:   PerformanceLap.h
 * Author: dcr
 *
 * Created on April 14, 2016, 5:46 PM
 */

#ifndef PERFORMANCELAP_H
#define	PERFORMANCELAP_H

#ifdef WIN32
    #include <windows.h>
#endif

class PerformanceLap {
public:
    PerformanceLap();
    virtual ~PerformanceLap();
    
    void start();
    double stop();
    double lap();
    
private:
    double dtime();

    double d0;
    double df;
    
#ifdef WIN32
    LARGE_INTEGER m_freq;
#endif
};

#endif	/* PERFORMANCELAP_H */

