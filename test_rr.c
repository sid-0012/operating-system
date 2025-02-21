#include <stdio.h>
#include "aux.h"
#include "umix.h"
#include "sys.h"


// Test 1: Basic interleaving with 3 processes
void Test1()
{
	DPrintf("Output expected is alternating numbers per quantum.\n");
	if (GetSchedPolicy() != ROUNDROBIN) {
		DPrintf("ERROR: Policy is not set to ROUNDROBIN. Please manually set policy to ROUNDROBIN in mycode2.c.");
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

void Test2()
{
	if (GetSchedPolicy() != ROUNDROBIN) {
		DPrintf("ERROR: Policy is not set to ROUNDROBIN. Please manually set policy to ROUNDROBIN in mycode2.c.");
		Exit();
	}
	if (Fork () == 0) {
		if (Fork () == 0) {
			SlowPrintf (5, "5555555555");		// process 5
			DPrintf("\nProcess %d exiting.\n", 5);
			Exit ();
		}
		SlowPrintf (5, "2222222222");			// process 2
		DPrintf("\nProcess %d exiting.\n", 2);
		Exit ();
	}
	if (Fork () == 0) {
		if (Fork () == 0) {
			SlowPrintf (5, "AAAAAAAAAA");			// process 10
			DPrintf("\nProcess %d exiting.\n", 10);
			Exit ();
		}
		SlowPrintf (5, "3333333333");			// process 3
		DPrintf("\nProcess %d exiting.\n", 3);
		Exit ();
	}
	if (Fork () == 0) {
		if (Fork () == 0) {
			SlowPrintf (5, "999999999");			// process 9
			DPrintf("\nProcess %d exiting.\n", 9);
			Exit ();
		}
		SlowPrintf (5, "4444444444");			// process 4
		DPrintf("\nProcess %d exiting.\n", 4);
		Exit ();
	}

	if (Fork () == 0) {
		if (Fork () == 0) {
			SlowPrintf (5, "7777777777");			// process 7
			DPrintf("\nProcess %d exiting.\n", 7);
			if (Fork () == 0) {
				SlowPrintf (5, "8888888888");			// process 8
				DPrintf("\nProcess %d exiting.\n", 8);
				Exit ();
			}
			Exit ();
		}

		SlowPrintf (5, "6666666666");			// process 6
		DPrintf("\nProcess %d exiting.\n", 6);
		Exit ();
	}
	SlowPrintf (5, "1111111111");			// process 1
	DPrintf("\nProcess %d exiting.\n", 1);
	Exit ();
}

void Test3 ()
{
	if (GetSchedPolicy() != ROUNDROBIN) {
		DPrintf("ERROR: Policy is not set to ROUNDROBIN. Please manually set policy to ROUNDROBIN in mycode2.c.");
		Exit();
	}
	if (Fork () == 0) {
		if (Fork () == 0) {
			SlowPrintf (5, "5555555555");		// process 5
			DPrintf("\nProcess %d exiting.\n", 5);
			Exit ();
		}
		SlowPrintf (5, "2222222222");			// process 2
		DPrintf("\nProcess %d exiting.\n", 2);
		Exit ();
	}
	if (Fork () == 0) {
		if (Fork () == 0) {
			SlowPrintf (5, "AAAAAAAAAA");			// process 10
			DPrintf("\nProcess %d exiting.\n", 10);
			Exit ();
		}
		SlowPrintf (5, "3333333333");			// process 3
		DPrintf("\nProcess %d exiting.\n", 3);
		Exit ();
	}
	if (Fork () == 0) {
		if (Fork () == 0) {
			SlowPrintf (5, "999999999");			// process 9
			DPrintf("\nProcess %d exiting.\n", 9);
			Exit ();
		}
		SlowPrintf (5, "4444444444");			// process 4

		if (Fork () == 0) {
			if (Fork () == 0) {
				SlowPrintf (5, "CCCCCCCCCC");			// process 12
				DPrintf("\nProcess %d exiting.\n", 12);
				Exit ();
			}
			SlowPrintf (5, "BBBBBBBBBB");			// process 11
			DPrintf("\nProcess %d exiting.\n", 11);
			Exit ();
		}
		DPrintf("\nProcess %d exiting.\n", 4);
		Exit ();
	}

	if (Fork () == 0) {
		if (Fork () == 0) {
			SlowPrintf (5, "7777777777");			// process 7
			DPrintf("\nProcess %d exiting.\n", 7);
			if (Fork () == 0) {
				SlowPrintf (5, "8888888888");			// process 8
				DPrintf("\nProcess %d exiting.\n", 8);
				Exit ();
			}
			Exit ();
		}

		SlowPrintf (5, "6666666666");			// process 6
		DPrintf("\nProcess %d exiting.\n", 6);
		Exit ();
	}
	SlowPrintf (5, "1111111111");			// process 1
	DPrintf("\nProcess %d exiting.\n", 1);
	Exit ();
}

/**********************************************************/

void Test4(){
	if (GetSchedPolicy() != ROUNDROBIN) {
		DPrintf("ERROR: Policy is not set to ROUNDROBIN. Please manually set policy to ROUNDROBIN in mycode2.c.");
		Exit();
	}

	DPrintf("Processes should finish nearly at the same time.\n");
    int p2, p3, p4;
    const int BASE_WORK = 200; // Total work units = 1000


    if ((p2 = Fork()) == 0) {   
        for(int i=0; i<BASE_WORK; i++) {
            SlowPrintf(1, "2"); // Fast print for clean output
        }
        DPrintf("\nP2 completed\n");
        Exit();
    }

    if ((p3 = Fork()) == 0) {    
        RequestCPUrate(20);
        for(int i=0; i<BASE_WORK; i++) {
            SlowPrintf(1, "3");
        }
        DPrintf("\nP3 completed\n");
        Exit();
    }

    if ((p4 = Fork()) == 0) {   
        RequestCPUrate(10);
        for(int i=0; i<BASE_WORK; i++) {
            SlowPrintf(1, "4");
        }
        DPrintf("\nP4 completed\n");
        Exit();
    }

    // Parent process 
    for(int i=0; i<BASE_WORK; i++) {
        SlowPrintf(1, "1");
    }
    DPrintf("\nP1 completed\n");

    
    Exit();
}

void Test5() {
    if (GetSchedPolicy() != ROUNDROBIN) {
        DPrintf("ERROR: Policy is not set to ROUNDROBIN\n");
        Exit();
    }

    DPrintf("Testing MAXPROCS+1 process creation and slot reuse\n");
    const int BASE_WORK = 100;
    int children[MAXPROCS + 1];

    // Create MAXPROCS-1 children (parent + children = MAXPROCS)
    for (int i = 0; i < MAXPROCS - 1; i++) {
        if ((children[i] = Fork()) == 0) {
            // Child process
            DPrintf("Child %d started\n", i+1);
            for (int j = 0; j < BASE_WORK; j++) {
                SlowPrintf(1, "%d", i+1);  // Work=1 for clear preemption
            }
            DPrintf("\nChild %d exited\n", i+1);
            Exit();
        }
    }

    // Attempt to create additional child (exceeds MAXPROCS)
    DPrintf("Attempting to create child %d (should block)\n", MAXPROCS);
    int blocked_child = Fork();
    if (blocked_child == 0) {
        DPrintf("Blocked child started\n");
        for (int j = 0; j < BASE_WORK; j++) {
            SlowPrintf(1, "X");  // Mark blocked child's output
        }
        DPrintf("\nBlocked child exited\n");
        Exit();
    } else {
        DPrintf("Parent continues after blocked fork\n");
    }

    // Parent work (keep parent active to maintain process count)
    for (int j = 0; j < BASE_WORK; j++) {
        SlowPrintf(1, "P");  // Parent's work
    }
    DPrintf("\nParent work done\n");

    // Wait for children while tracking exits
    int exited_count = 0;
    while ((exited_count = Wait(0)) > 0) {
        DPrintf("Child exited, %d remaining\n", MAXPROCS-1 - exited_count);
    }

    DPrintf("All children completed\n");
    Exit();
}

void Main()
{
    if (GetSchedPolicy() != ROUNDROBIN) {
        DPrintf("ERROR: Set policy to ROUNDROBIN in mycode3.c\n");
        Exit();
    }

    int testnum = 5;  // Change to select test (1-5)
    
    switch(testnum) {
        case 1: Test1(); break;
        case 2: Test2(); break;
        case 3: Test3(); break;
        case 4: Test4(); break;
        case 5: Test5(); break;
        default: Printf("Invalid test"); Exit();
    }
}
