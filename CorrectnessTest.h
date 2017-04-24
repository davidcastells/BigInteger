/* 
 * File:   CorrectnessTest.h
 * Author: dcr
 *
 * Created on April 24, 2017, 11:42 AM
 */

#ifndef CORRECTNESSTEST_H
#define	CORRECTNESSTEST_H

class CorrectnessTest {
public:
    CorrectnessTest();
    CorrectnessTest(const CorrectnessTest& orig);
    virtual ~CorrectnessTest();
    
public:
    void run();
    void testAdd();
    
private:

};

#endif	/* CORRECTNESSTEST_H */

