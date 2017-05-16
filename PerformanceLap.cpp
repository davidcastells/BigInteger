/* 
 * File:   PerformanceLap.cpp
 * Author: dcr
 * 
 * Created on April 14, 2016, 5:46 PM
 */

#include "PerformanceLap.h"

#include <stdlib.h>

#ifdef WIN32
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

struct timeval tnow;

/* time in seconds */

double PerformanceLap::dtime()
{
#ifdef WIN32
    	LARGE_INTEGER val;
	BOOL bRet = QueryPerformanceCounter((LARGE_INTEGER*)&val);

	double ret = val.QuadPart;

	ret = ret / m_freq.QuadPart;

    return ret;
#else
    double q;

    gettimeofday(&tnow, NULL);
    q = (double)tnow.tv_sec + (double)tnow.tv_usec * 1.0e-6;

    return q;
#endif
}

PerformanceLap::PerformanceLap() 
{
#ifdef WIN32
    BOOL bRet = QueryPerformanceFrequency((LARGE_INTEGER*)&m_freq);
#endif
    
    start();
}


PerformanceLap::~PerformanceLap() {
}


void PerformanceLap::start()
{
    d0 = dtime();
}

double PerformanceLap::stop()
{
    df = dtime();
    
    return (df-d0);
}

double PerformanceLap::lap()
{
    if (df < d0)
        stop();
    
    return (df-d0);
}