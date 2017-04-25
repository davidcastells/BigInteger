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
    void testShiftRight();
    void testShiftLeft();
    void testRange();
    void testAdd();
    void testAddShifted();
    void testMult();
    
private:
    int stopAtFirstError;
};

#endif	/* CORRECTNESSTEST_H */

