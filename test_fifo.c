#include <stdio.h>
#include "aux.h"
#include "umix.h"
#include "sys.h"

// Test 1: Basic fork order
void Test1()
{
    DPrintf("Expect: \n111222333\n");
    if (Fork() == 0) {        // Process 2
        SlowPrintf(6, "222");
        Exit();
    }
    if (Fork() == 0) {        // Process 3
        SlowPrintf(6, "333");
        Exit();
    }
    SlowPrintf(6, "111");  // Process 1
    Exit();
}

// Test 2: Nested forks
void Test2()
{
    DPrintf("Expect: \n111122224444\n");
    if (Fork() == 0) {        // Process 2
        if (Fork() == 0) {    // Process 4
            SlowPrintf(6, "4444");
            Exit();
        }
        SlowPrintf(6, "2222");
        Exit();
    }
    SlowPrintf(6, "1111");       // Process 1
    Exit();
}

// Test 3: Mixed with immediate exit
void Test3()
{
    DPrintf("Expect: \n11112222\n");
    if (Fork() == 0) {        // Process 2
        SlowPrintf(6, "2222");
        Exit();
    }
    if (Fork() == 0) {        // Process 3 (exits immediately)
        Exit();
    }
    SlowPrintf(6, "1111");       // Process 1
    Exit();
}

// Test 4: MAXPROCS processes
void Test4()
{
    DPrintf("Expect: \n12345678910\n");
    for (int i = 0; i < MAXPROCS+10; i++) {
        // DPrintf("i=%d\n", i);
        if (Fork() == 0) {    // Processes 2-6
            SlowPrintf(6, "%d", i+2);
            Exit();
        }
    }
    SlowPrintf(6, "1");       // Process 1
    Exit();
}

// Test 5: Interleaved forks
void Test5()
{
    DPrintf("Expect: \n12345\n");
    if (Fork() == 0) {        // Process 2
        if (Fork() == 0) {    // Process 4
            SlowPrintf(6, "4");
            Exit();
        }
        if (Fork() == 0) {    // Process 5
            SlowPrintf(6, "5");
            Exit();
        }
        SlowPrintf(6, "2");
        Exit();
    }
    if (Fork() == 0) {        // Process 3
        SlowPrintf(6, "3");
        Exit();
    }
    SlowPrintf(6, "1");       // Process 1
    Exit();
}

void Test6()
{
	DPrintf("Expect the following string:\n11111111112222222222333333333344444444445555555555\n");
	if (GetSchedPolicy() != FIFO) {
		DPrintf("ERROR: Policy is not set to FIFO. Please manually set policy to FIFO in mycode2.c.");
		Exit();
	}
	if (Fork () == 0) {
		if (Fork () == 0) {
			SlowPrintf (5, "5555555555");		// process 5
			Exit ();
		}
		SlowPrintf (5, "2222222222");			// process 2
		Exit ();
	}
	if (Fork () == 0) {
		SlowPrintf (5, "3333333333");			// process 3
		Exit ();
	}
	if (Fork () == 0) {
		SlowPrintf (5, "4444444444");			// process 4
		Exit ();
	}
	SlowPrintf (5, "1111111111");			// process 1
	Exit ();
}

void Main()
{
    int testnum = 5;  // Change this to select test (1-5)
    
    switch(testnum) {
        case 1: Test1(); break;
        case 2: Test2(); break;
        case 3: Test3(); break;
        case 4: Test4(); break;
        case 5: Test5(); break;
        case 6: Test6(); break;
        default: Printf("Invalid test"); Exit();
    }
}
