/* 
 * File:   PerformanceLap.cpp
 * Author: dcr
 * 
 * Created on April 14, 2016, 5:46 PM
 */

#include "PerformanceLap.h"

#include <stdlib.h>
#include <sys/time.h>

struct timeval tnow;

/* time in seconds */
double dtime()
{
    double q;

    gettimeofday(&tnow, NULL);
    q = (double)tnow.tv_sec + (double)tnow.tv_usec * 1.0e-6;

    return q;
}

PerformanceLap::PerformanceLap() 
{
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