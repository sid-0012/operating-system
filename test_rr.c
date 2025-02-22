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

// Test 5: Slot reuse after exit
void Test5()
{
    DPrintf("Test 5: Slot reuse after process exit\n");
    DPrintf("Expect new child to start after an exit\n");
    if (GetSchedPolicy() != ROUNDROBIN) {
        DPrintf("ERROR: Policy is not set to ROUNDROBIN\n");
        Exit();
    }
    const int BASE_WORK = 50;
    int children[MAXPROCS];

    // Create MAXPROCS-1 children + parent
    for (int i = 0; i < MAXPROCS - 2; i++) {
        if ((children[i] = Fork()) == 0) {
            int proc_num = i + 2;
            DPrintf("Child %d started\n", proc_num);
            int start = Gettime();
            for (int j = 0; j < BASE_WORK; j++) {
                SlowPrintf(1, "%d", proc_num % 10);
            }
            int end = Gettime();
            DPrintf("\nChild %d completed in %d ms\n", proc_num, end - start);
            Exit();
        }
    }

	if(Fork()==0){
		DPrintf("Last child exit immediately after creates\n");
		Exit();
	}

    // Parent with short work to exit early
    DPrintf("Parent (Process 1) started\n");
    int start = Gettime();
    for (int j = 0; j < BASE_WORK / 2; j++) {
        SlowPrintf(1, "1");
    }
    int end = Gettime();
    DPrintf("\nParent completed in %d ms\n", end - start);

    // Fork new child immediately after parent’s work
    int new_child = Fork();
    if (new_child == 0) {
        DPrintf("New child (Process %d) started\n", MAXPROCS + 1);
        int new_start = Gettime();
        for (int j = 0; j < BASE_WORK; j++) {
            SlowPrintf(1, "X");
        }
        int new_end = Gettime();
        DPrintf("\nNew child completed in %d ms\n", new_end - new_start);
        Exit();
    } else if (new_child == -1) {
        DPrintf("ERROR: New child fork failed unexpectedly\n");
    } else {
        DPrintf("New child created with PID %d\n", new_child);
    }

    // Parent exits immediately after forking new child
    Exit();
}

// Test 6: Exceeding MAXPROCS before exit
void Test6()
{
    DPrintf("Test 6: Attempt to exceed MAXPROCS before exit\n");
    DPrintf("Expect fork to fail until a process exits\n");
    if (GetSchedPolicy() != ROUNDROBIN) {
        DPrintf("ERROR: Policy is not set to ROUNDROBIN\n");
        Exit();
    }
    const int BASE_WORK = 50;
    int children[MAXPROCS];

    // Fill MAXPROCS
    for (int i = 0; i < MAXPROCS - 1; i++) {
        if ((children[i] = Fork()) == 0) {
            int proc_num = i + 2;
            DPrintf("Child %d started\n", proc_num);
            int start = Gettime();
            for (int j = 0; j < BASE_WORK; j++) {
                SlowPrintf(1, "%d", proc_num % 10);
            }
            int end = Gettime();
            DPrintf("\nChild %d completed in %d ms\n", proc_num, end - start);
            Exit();
        }
    }



    // Try to exceed MAXPROCS
    int extra_child = Fork();
    if (extra_child == 0) {
        DPrintf("ERROR: Extra child started unexpectedly\n");
        Exit();
    } else if (extra_child == -1) {
        DPrintf("Extra fork failed as expected\n");
    } else {
        DPrintf("ERROR: Extra fork succeeded unexpectedly\n");
        Wait(extra_child);
    }

    
    Exit();
}


void Main()
{
    if (GetSchedPolicy() != ROUNDROBIN) {
        DPrintf("ERROR: Set policy to ROUNDROBIN in mycode3.c\n");
        Exit();
    }

    int testnum = 6;  // Change to select test (1-5)
    
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
