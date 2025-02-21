#include <stdio.h>
#include "aux.h"
#include "umix.h"
#include "sys.h"

// Test 1: Basic LIFO order TODO
void Test1()
{
    DPrintf("Expect: \n222233331111\n");
    if (Fork() == 0) {        // Process 2 (runs first)
        SlowPrintf(6, "2222");
        Exit();
    }
    if (Fork() == 0) {        // Process 3 (runs second)
        SlowPrintf(6, "3333");
        Exit();
    }
    SlowPrintf(6, "1111");  // Process 1 (runs last)
    Exit();
}

// Test 2: Nested LIFO behavior
void Test2()
{
    DPrintf("Expect: \n444422221111\n");
    if (Fork() == 0) {        // Process 2
        if (Fork() == 0) {    // Process 4 (runs first)
            SlowPrintf(6, "4444");
            Exit();
        }
        SlowPrintf(6, "2222");  // Runs second
        Exit();
    }
    SlowPrintf(6, "1111");      // Runs third
    Exit();
}

// Test 3: Immediate exit handling
void Test3()
{
    DPrintf("Expect: \n3331111\n");
    if (Fork() == 0) {        // Process 2 (exits immediately)
        Exit();
    }
    if (Fork() == 0) {        // Process 3 (runs first)
        SlowPrintf(6, "333");
        Exit();
    }
    SlowPrintf(6, "1111");  // Runs second
    Exit();
}

// Test 4: MAXPROCS boundary test
void Test4()
{
    DPrintf("Expect: \n121110987654321\n");
    for (int i = MAXPROCS; i>=0; i--) {
        if (Fork() == 0) {    
            SlowPrintf(6, "%d", i+2);  // Should run in reverse order 
            Exit();
        }
    }
    SlowPrintf(6, "1");       // Process 1 (runs last)
    Exit();
}

// Test 5: Deep nesting
void Test5()
{
    DPrintf("Expect: \n666444222555333111\n");
    if (Fork() == 0) {        // Process 2
        if (Fork() == 0) {    // Process 4
            if (Fork() == 0) { // Process 6 (runs first)
                SlowPrintf(6, "666");
                Exit();
            }
            SlowPrintf(6, "444");  // Runs second
            Exit();
        }
        SlowPrintf(6, "222");  // Runs third
        Exit();
    }
    if (Fork() == 0) {        // Process 3
        if (Fork() == 0) {    // Process 5 (runs fourth)
            SlowPrintf(6, "555");
            Exit();
        }
        SlowPrintf(6, "333");  // Runs fifth
        Exit();
    }
    SlowPrintf(6, "111");      // Runs sixth
    Exit();
}

// Test 6: Mixed active/exited processes
void Test6()
{
    DPrintf("Expect: \n555555555533333333331111111111\n");
    if (Fork() == 0) {        // Process 2 (exits)
        Exit();
    }
    if (Fork() == 0) {        // Process 3
        if (Fork() == 0) {    // Process 5 (runs first)
            SlowPrintf(6, "5555555555");
            Exit();
        }
        SlowPrintf(6, "3333333333");  // Runs second
        Exit();
    }
    if (Fork() == 0) {        // Process 4 (exits)
        Exit();
    }
    SlowPrintf(6, "1111111111");  // Runs third
    Exit();
}

void Test7()
{
	DPrintf("Expect the following string:\n55555555552222222222333333333344444444441111111111\n");
	if (GetSchedPolicy() != LIFO) {
		DPrintf("ERROR: Policy is not set to LIFO. Please manually set policy to LIFO in mycode2.c.");
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
    if (GetSchedPolicy() != LIFO) {
        DPrintf("ERROR: Set policy to LIFO in mycode3.c\n");
        Exit();
    }

    int testnum = 5;  // Change to select test (1-6)
    
    switch(testnum) {
        case 1: Test1(); break;
        case 2: Test2(); break;
        case 3: Test3(); break;
        case 4: Test4(); break;
        case 5: Test5(); break;
        case 6: Test6(); break;
        case 7: Test7(); break;
        default: Printf("Invalid test"); Exit();
    }
}
