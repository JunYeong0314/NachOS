// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "thread.h" 
#include "synch.h"

// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

#define BUFFER_SIZE 5

Semaphore *empty;
Semaphore *full;
Semaphore *mutex;

void
Producer(int arg){
 while(true){
	empty->P();
	mutex->P();

 	printf("Producer produced an item.\n");

	mutex->V();
	full->V();
	currentThread->Yield();
 }
}

void
Consumer(int arg){
 while(true){
	full->P();
	mutex->P();

	printf("Consumer consumed an item,\n");

	mutex->V();
	empty->V();
	currentThread->Yield();
 }
}

void 
SemTest()
{
 mutex = new Semaphore("mutex", 1);
 empty = new Semaphore("empty", BUFFER_SIZE);
 full = new Semaphore("full", 0);

 Thread *t = new Thread("producer");
 t->Fork(Producer, 1);

 Thread *c = new Thread("consumer");
 c->Fork(Consumer, 1);
}

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

 Thread *t = new Thread("forked thread");
 t->Fork(SimpleThread, 1);
 SimpleThread(0);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}

