#include <stdio.h>
#include "aux.h"
#include "umix.h"
#include "sys.h"
#include <stdlib.h>
#include <sys/types.h>  // Added for kill()
#include <signal.h>     // Added for kill()

void Test1()
{
	DPrintf("Process should exit in order: 1, 2, 3, 4\n");
	if (GetSchedPolicy() != PROPORTIONAL) {
		DPrintf("ERROR: Policy is not set to PROPORTIONAL. Please manually set policy to PROPORTIONAL in mycode2.c.");
		Exit();
	}
	if (Fork () == 0) {

		if (Fork () == 0) {

			RequestCPUrate (10);			// process 4
			SlowPrintf (5, "4444444444");
			DPrintf("\nProcess %d exiting.\n", 4);
			Exit ();
		}

		RequestCPUrate (30);				// process 2
		SlowPrintf (5, "2222222222");
		DPrintf("\nProcess %d exiting.\n", 2);
		Exit ();
	}

	if (Fork () == 0) {

		RequestCPUrate (20);				// process 3
		SlowPrintf (5, "3333333333");
		DPrintf("\nProcess %d exiting.\n", 3);
		Exit ();
	}

	RequestCPUrate (40);					// process 1
	SlowPrintf (5, "1111111111");
	DPrintf("\nProcess %d exiting.\n", 1);
	Exit ();
}

void Test2()
{
	DPrintf("Processes all have default CPU allocation, so they should end around the same time.\n");
	DPrintf("This should behave like round robin.\n");
	if (GetSchedPolicy() != PROPORTIONAL) {
		DPrintf("ERROR: Policy is not set to PROPORTIONAL. Please manually set policy to PROPORTIONAL in mycode2.c.");
		Exit();
	}
	if (Fork () == 0) {

		if (Fork () == 0) {

			SlowPrintf (5, "4444444444");
			DPrintf("\nProcess %d exiting.\n", 4);
			Exit ();
		}

		SlowPrintf (5, "2222222222");
		DPrintf("\nProcess %d exiting.\n", 2);
		Exit ();
	}

	if (Fork () == 0) {

		SlowPrintf (5, "3333333333");
		DPrintf("\nProcess %d exiting.\n", 3);
		Exit ();
	}

	SlowPrintf (5, "1111111111");
	DPrintf("\nProcess %d exiting.\n", 1);
	Exit ();
}

void Test3()
{
	DPrintf("Process should exit in order: 1, {2, 3}, 4\n");
	DPrintf("Process 1 should finish first and process 4 should finish last. \n");
	if (GetSchedPolicy() != PROPORTIONAL) {
		DPrintf("ERROR: Policy is not set to PROPORTIONAL. Please manually set policy to PROPORTIONAL in mycode2.c.");
		Exit();
	}
	if (Fork () == 0) {

		if (Fork () == 0) {

			RequestCPUrate (1);			// process 4
			SlowPrintf (5, "4444444444");
			DPrintf("\nProcess %d exiting.\n", 4);
			Exit ();
		}

		SlowPrintf (5, "2222222222");
		DPrintf("\nProcess %d exiting.\n", 2);
		Exit ();
	}

	if (Fork () == 0) {

		SlowPrintf (5, "3333333333");
		DPrintf("\nProcess %d exiting.\n", 3);
		Exit ();
	}

	RequestCPUrate (40);					// process 1
	SlowPrintf (5, "1111111111");
	DPrintf("\nProcess %d exiting.\n", 1);
	Exit ();
}

void Test4()
{
	DPrintf("Process should exit in order: 4, 1, {2, 3}\n");
	if (GetSchedPolicy() != PROPORTIONAL) {
		DPrintf("ERROR: Policy is not set to PROPORTIONAL. Please manually set policy to PROPORTIONAL in mycode2.c.");
		Exit();
	}
	if (Fork () == 0) {

		if (Fork () == 0) {

			RequestCPUrate (30);			// process 4
			SlowPrintf (5, "444");
			RequestCPUrate(60);
			SlowPrintf (5, "4444444");
			DPrintf("\nProcess %d exiting.\n", 4);
			Exit ();
		}

		SlowPrintf (5, "2222222222");
		DPrintf("\nProcess %d exiting.\n", 2);
		Exit ();
	}

	if (Fork () == 0) {

		SlowPrintf (5, "3333333333");
		DPrintf("\nProcess %d exiting.\n", 3);
		Exit ();
	}

	RequestCPUrate (40);					// process 1
	SlowPrintf (5, "1111111111");
	DPrintf("\nProcess %d exiting.\n", 1);
	Exit ();
}

void Test5()
{
	DPrintf("Process should exit in order: 1, 2, 3, 4\n");
	DPrintf("This test is checking to see if requesting more than 100%% doesn't change the runtime.\n");
	if (GetSchedPolicy() != PROPORTIONAL) {
		DPrintf("ERROR: Policy is not set to PROPORTIONAL. Please manually set policy to PROPORTIONAL in mycode2.c.");
		Exit();
	}
	if (Fork () == 0) {

		if (Fork () == 0) {

			RequestCPUrate (10);			// process 4
			SlowPrintf (5, "44");
			RequestCPUrate(50);
			SlowPrintf (5, "44444444");
			DPrintf("\nProcess %d exiting.\n", 4);
			Exit ();
		}

		RequestCPUrate (30);				// process 2
		SlowPrintf (5, "2222222222");
		DPrintf("\nProcess %d exiting.\n", 2);
		Exit ();
	}

	if (Fork () == 0) {

		RequestCPUrate (20);				// process 3
		SlowPrintf (5, "3333333333");
		DPrintf("\nProcess %d exiting.\n", 3);
		Exit ();
	}

	RequestCPUrate (40);					// process 1
	SlowPrintf (5, "1111111111");
	DPrintf("\nProcess %d exiting.\n", 1);
	Exit ();
}

void Test6() // TODO
{
	DPrintf("Process 2 should finish first.\n");
	if (GetSchedPolicy() != PROPORTIONAL) {
		DPrintf("ERROR: Policy is not set to PROPORTIONAL. Please manually set policy to PROPORTIONAL in mycode2.c.");
		Exit();
	}
	if (Fork () == 0) {
		RequestCPUrate(99);
		SlowPrintf(5, "2222222222");

		DPrintf("\nProcess %d exiting.\n", 2);
		Exit ();
	}
	if (Fork () == 0) {
		RequestCPUrate(0);
		SlowPrintf(5, "3333333333");
		DPrintf("\nProcess %d exiting.\n", 3);
		Exit ();
	}

	if (Fork () == 0) {
		RequestCPUrate(0);
		SlowPrintf(5, "4444444444");
		DPrintf("\nProcess %d exiting.\n", 4);
		Exit ();
	}

	RequestCPUrate(0);
	SlowPrintf (5, "1111111111");
	DPrintf("\nProcess %d exiting.\n", 1);
	Exit ();
}

void Test7()
{
	DPrintf("Proc 2 should finish before Proc 1, and your program shouldn't crash or allow 1 to finish first.\n");
	DPrintf("This tests to make sure your pass value doesn't overflow.\n");
	if (GetSchedPolicy() != PROPORTIONAL) {
		DPrintf("ERROR: Policy is not set to PROPORTIONAL. Please manually set policy to PROPORTIONAL in mycode2.c.");
		Exit();
	}
	if (Fork() == 0) {
		RequestCPUrate(95);
		SlowPrintf(5, "222222222222222222");
		DPrintf("\nProcess %d exiting.\n", 2);
		Exit ();
	}
	RequestCPUrate (1);					// process 1
	SlowPrintf (9, "111111111");
	DPrintf("\nProcess %d exiting.\n", 1);
	Exit ();
}

/* ******************************************************* */

// Sum of 100% exactly: All processes get their exact requested rates.
void Test8() {
    DPrintf("Proportional Test (40:30:20:10 ratio)\n");
	DPrintf("Processes should finish nearly at the same time. 0.1 percent of error is permitted\n");
    int p2, p3, p4;
    const int BASE_WORK = 1000; // Total work units = 1000

    // Create child processes with work proportional to CPU share
    if ((p2 = Fork()) == 0) {    // 30% → 300 units
        RequestCPUrate(30);
        for(int i=0; i<BASE_WORK*0.3; i++) {
            SlowPrintf(1, "2"); // Fast print for clean output
        }
        DPrintf("\nP2 (30%%) completed\n");
        Exit();
    }

    if ((p3 = Fork()) == 0) {    // 20% → 200 units
        RequestCPUrate(20);
        for(int i=0; i<BASE_WORK*0.2; i++) {
            SlowPrintf(1, "3");
        }
        DPrintf("\nP3 (20%%) completed\n");
        Exit();
    }

    if ((p4 = Fork()) == 0) {    // 10% → 100 units
        RequestCPUrate(10);
        for(int i=0; i<BASE_WORK*0.1; i++) {
            SlowPrintf(1, "4");
        }
        DPrintf("\nP4 (10%%) completed\n");
        Exit();
    }

    // Parent process (40% → 400 units)
    RequestCPUrate(40);
    for(int i=0; i<BASE_WORK*0.4; i++) {
        SlowPrintf(1, "1");
    }
    DPrintf("\nP1 (40%%) completed\n");

    
    Exit();
}

// Sum exceeds 100%: A new request that pushes the total over 100 should fail.
void Test9() {
    DPrintf("Proportional Test (40:30:20:20 ratio)\n");
	DPrintf("Process 4 requesting 20 should fail, only still get 10 of non-requesting CPU time\n");
	DPrintf("Processes should finish nearly at the same time. 0.1 percent of error is permitted\n");
    int p2, p3, p4;
    const int BASE_WORK = 1000; // Total work units = 1000

    // Create child processes with work proportional to CPU share
    if ((p2 = Fork()) == 0) {    // 30% → 300 units
        RequestCPUrate(30);
        for(int i=0; i<BASE_WORK*0.3; i++) {
            SlowPrintf(1, "2"); // Fast print for clean output
        }
        DPrintf("\nP2 (30%%) completed\n");
        Exit();
    }

    if ((p3 = Fork()) == 0) {    // 20% → 200 units
        RequestCPUrate(20);
        for(int i=0; i<BASE_WORK*0.2; i++) {
            SlowPrintf(1, "3");
        }
        DPrintf("\nP3 (20%%) completed\n");
        Exit();
    }

    if ((p4 = Fork()) == 0) {    // 10% → 100 units
        RequestCPUrate(20);
        for(int i=0; i<BASE_WORK*0.1; i++) {
            SlowPrintf(1, "4");
        }
        DPrintf("\nP4 (non req with 10%%) completed\n");
        Exit();
    }

    // Parent process (40% → 400 units)
    RequestCPUrate(40);
    for(int i=0; i<BASE_WORK*0.4; i++) {
        SlowPrintf(1, "1");
    }
    DPrintf("\nP1 (40%%) completed\n");

    
    Exit();
}

int ProcExists(int pid) {
    // Check if process exists using kill signal 0
    return (kill(pid, 0) == 0);
}

// Zero requests (n=0): A process releasing its allocation, which should redistribute the freed percentage to others.
void Test10() {
    DPrintf("\nTesting Rate Release Redistribution\n");
    DPrintf("Initial: P1(60%%), P2(40%%)\n");
    DPrintf("After P2 releases 40%%, P3 should get 40%% share\n");

    int p2, p3, p4;
    const int BASE_WORK = 1000;

    // Process 1: 60% allocation
    if ((p2 = Fork()) == 0) {
        RequestCPUrate(60);
        for(int i=0; i<BASE_WORK*0.6; i++) {
            SlowPrintf(1, "1");
        }
        DPrintf("\nP1 (60%%) completed\n");
        Exit();
    }

    // Process 2: 40% initially, then release
    if ((p3 = Fork()) == 0) {
        RequestCPUrate(40);
        for(int i=0; i<10; i++) Yield(GetPID());
        RequestCPUrate(0); // Release allocation
        DPrintf("P2 released 40%% allocation\n");
        Exit();
    }

    // Wait for P2 to release (using ProcExists)
    while(ProcExists(p3)) Yield(GetPID());

    // Process 3: Non-requesting (should get 40%)
    if ((p4 = Fork()) == 0) {
        for(int i=0; i<BASE_WORK*0.4; i++) {
            SlowPrintf(1, "3");
        }
        DPrintf("\nP3 (40%% share) completed\n");
        Exit();
    }
}

// All processes request 0%
void Test11() {
    DPrintf("\nTesting All Processes Request 0%%\n");
    DPrintf("4 processes (parent + 3 children) should get equal 25%% shares\n");

    int p1, p2, p3;
    const int BASE_WORK = 1000;
    int start = Gettime();

    // Parent process requests 0%
    RequestCPUrate(0);

    // Create 3 child processes requesting 0%
    if ((p1 = Fork()) == 0) {
        RequestCPUrate(0);
        for(int i=0; i<BASE_WORK/4; i++) SlowPrintf(1, "1");
        DPrintf("\nC1 finished @%d", Gettime() - start);
        Exit();
    }
    if ((p2 = Fork()) == 0) {
        RequestCPUrate(0);
        for(int i=0; i<BASE_WORK/4; i++) SlowPrintf(1, "2");
        DPrintf("\nC2 finished @%d", Gettime() - start);
        Exit();
    }
    if ((p3 = Fork()) == 0) {
        RequestCPUrate(0);
        for(int i=0; i<BASE_WORK/4; i++) SlowPrintf(1, "3");
        DPrintf("\nC3 finished @%d", Gettime() - start);
        Exit();
    }

    // Parent work (should get 25% share)
    for(int i=0; i<BASE_WORK/4; i++) SlowPrintf(1, "P");
    DPrintf("\nParent finished @%d", Gettime() - start);
}

// Single process with 100%: Only one process requests 100%, others are non-requesting.
void Test12() {
	DPrintf("Testing non-requesting processes get a chance to run at least once every (k * 100) quantums\n");
	DPrintf("There are 3 non-requesting processes.\n");
	DPrintf("Inspect whether each runs 3 times (or above) in the 900 quantum period when process 1 is running\n");

    int p2, p3, p4;
    const int BASE_WORK = 900; // Total work units = 1000

    // Create child processes with work proportional to CPU share
    if ((p2 = Fork()) == 0) {   
        for(int i=0; i<5; i++) {
            SlowPrintf(1, "2"); // Fast print for clean output
        }
        DPrintf("\nP2 (non req 0%%) completed\n");
        Exit();
    }

    if ((p3 = Fork()) == 0) {    // non req
        for(int i=0; i<5; i++) {
            SlowPrintf(1, "3");
        }
        DPrintf("\nP3 (non req 0%%) completed\n");
        Exit();
    }

    if ((p4 = Fork()) == 0) { 
        for(int i=0; i<5; i++) {
            SlowPrintf(1, "4");
        }
        DPrintf("\nP4 (non req 0%%) completed\n");
        Exit();
    }

    // Parent process (40% → 400 units)
    RequestCPUrate(100);
    for(int i=0; i<BASE_WORK; i++) {
        SlowPrintf(1, "1");
    }
    DPrintf("\nP1 (100%%) completed\n");

    
    Exit();
}

void Test13() {
    DPrintf("\nTesting Non-Requesting CPU Distribution\n");
    DPrintf("40%% + 30%% + 20%% requests = 90%% claimed, 10%% split 3 ways\n");

    const int BASE_WORK = 1000;
    int children[5];
    int remaining_cpu = 100 - (40 + 30 + 20);
    int non_req_work = (remaining_cpu * BASE_WORK) / 100; // 100 units total

    // Parent requests 40%
    RequestCPUrate(40);

    // Create requesting processes
    if ((children[0] = Fork()) == 0) { // 30% = 300 units
        RequestCPUrate(30);
        for(int i=0; i<(30*BASE_WORK)/100; i++) SlowPrintf(1, "1");
        DPrintf("\n30%% requester completed\n");
        Exit();
    }
    if ((children[1] = Fork()) == 0) { // 20% = 200 units
        RequestCPUrate(20);
        for(int i=0; i<(20*BASE_WORK)/100; i++) SlowPrintf(1, "2");
        DPrintf("\n20%% requester completed\n");
        Exit();
    }

    // Create 3 non-requesting processes (3.33% each = 33 units)
    for(int j=2; j<5; j++) {
        if ((children[j] = Fork()) == 0) {
            for(int i=0; i<non_req_work/3; i++) SlowPrintf(1, "%d", j);
            DPrintf("\nNon-req %d completed\n", j-1);
            Exit();
        }
    }

    // Parent work (40% = 400 units)
    for(int i=0; i<(40*BASE_WORK)/100; i++) SlowPrintf(1, "P");
    DPrintf("\nParent 40%% completed");
}

// Process exit: When a process exits, its allocation is freed. Test if the remaining processes adjust correctly.
void Test14() {
    DPrintf("\nTesting Process Exit Allocation Recovery\n");
    DPrintf("When process exits, freed CPU should redistribute\n");

    const int BASE_WORK = 1000;
    int child1, child2, child3;

    // Initial setup: 60% requested
    RequestCPUrate(30); // Parent keeps 30%

    // Child1 requests 30% (will exit early)
    if ((child1 = Fork()) == 0) {
        RequestCPUrate(30);
        for(int i=0; i<300; i++) { // 30% work
            if(i == 100) { // Exit after partial completion
                DPrintf("\nChild1 (30%%) exiting");
                Exit();
            }
            SlowPrintf(1, "1");
        }
        Exit();
    }

    // Child2 requests 40% (will complete normally)
    if ((child2 = Fork()) == 0) {
        RequestCPUrate(40);
        for(int i=0; i<400; i++) SlowPrintf(1, "2");
        DPrintf("\nChild2 (40%%) completed");
        Exit();
    }

    // Child3 (non-requesting) - should get increased share after Child1 exits
    if ((child3 = Fork()) == 0) {
        // Work split: 300 before exit + 700 after
        for(int i=0; i<200; i++) SlowPrintf(1, "3"); 
        DPrintf("\nChild3 (non req 30%%) completed");
        Exit();
    }

    // Parent work (30%)
    for(int i=0; i<300; i++) SlowPrintf(1, "P");
    DPrintf("\nParent completed");
}

// Invalid requests (n <0 or >100): Should return -1 and have no effect.
void Test15() {
    DPrintf("\nTesting Invalid CPU Requests\n");
    DPrintf("Negative/over-100%% requests should fail\n");
    DPrintf("N and O should show minimally when P and V are running\n");

    int child1, child2, child3;
    const int BASE_WORK = 1000;

    // Parent takes 50% initially
    RequestCPUrate(50);

    // Test 1: Negative request
    if ((child1 = Fork()) == 0) {
        int rc = RequestCPUrate(-20);
        if(rc != -1) {
            DPrintf("\nFAIL: Negative request accepted");
            Exit();
        }
        for(int i=0; i<100; i++) SlowPrintf(1, "N"); // Should run minimally
        DPrintf("\nNegative test completed");
        Exit();
    }

    // Test 2: Over-100% request
    if ((child2 = Fork()) == 0) {
        int rc = RequestCPUrate(150);
        if(rc != -1) {
            DPrintf("\nFAIL: Over-100 request accepted");
            Exit();
        }
        for(int i=0; i<100; i++) SlowPrintf(1, "O"); // Should run minimally
        DPrintf("\nOver-100 test completed");
        Exit();
    }

    // Test 3: Valid request after invalid (should get 50% remaining)
    if ((child3 = Fork()) == 0) {
        int rc = RequestCPUrate(50);
        if(rc != 0) {
            DPrintf("\nFAIL: Valid request failed");
            Exit();
        }
        for(int i=0; i<500; i++) SlowPrintf(1, "V"); // 50% of BASE_WORK
        DPrintf("\nValid 50%% completed");
        Exit();
    }

    // Parent work (50% = 500 units)
    for(int i=0; i<500; i++) SlowPrintf(1, "P");
    DPrintf("\nParent 50%% completed");
}

// Re-requesting rates: A process changes its rate, which could affect the sum and redistribution.
void Test16() {
    DPrintf("\nTesting Rate Change Redistribution\n");
    DPrintf("Processes adjusting rates should trigger reallocation\n");

    const int BASE_WORK = 1000;
    int child1, child2, non_req;

    // Initial allocation: 30% (parent) + 30% (child1) + 30% (child2) = 90%
    RequestCPUrate(30);
    
    // Child1: Will increase from 30% → 50%
    if ((child1 = Fork()) == 0) {
        RequestCPUrate(30);
        for(int i=0; i<300; i++) SlowPrintf(1, "A"); // Phase1: 30%
        
        // Increase rate (30 → 50)
        if(RequestCPUrate(50) != 0) {
            DPrintf("\nFAIL: Rate increase failed");
            Exit();
        }
        for(int i=0; i<500; i++) SlowPrintf(1, "a"); // Phase2: 50%
        DPrintf("\nChild1 50%% completed");
        Exit();
    }

    // Child2: Will decrease from 30% → 10%
    if ((child2 = Fork()) == 0) {
        RequestCPUrate(30);
        for(int i=0; i<300; i++) SlowPrintf(1, "B"); // Phase1: 30%
        
        // Decrease rate (30 → 10)
        if(RequestCPUrate(10) != 0) {
            DPrintf("\nFAIL: Rate decrease failed");
            Exit();
        }
        for(int i=0; i<100; i++) SlowPrintf(1, "b"); // Phase2: 10%
        DPrintf("\nChild2 10%% completed");
        Exit();
    }

    // Non-requesting process: Should adapt to changing available CPU
    if ((non_req = Fork()) == 0) {
        // Phase1: 10% available (100 - 30-30-30 = 10)
        for(int i=0; i<100; i++) SlowPrintf(1, "N");
        
        for(int i=0; i<100; i++) SlowPrintf(1, "n"); // Phase2: Still 10%
        Exit();
    }

    // Parent work (30%)
    for(int i=0; i<600; i++) SlowPrintf(1, "P");
    DPrintf("\nParent 30%% completed");
}

void Test17() {
    DPrintf("\nTesting Maximum Processes (%d)\n", MAXPROCS);
    DPrintf("Mix of requesting/non-requesting processes\n");

    const int BASE_WORK = 1000;
    int children[MAXPROCS];
    int requested_rates = 0;
    const int REQ_RATE = 10; // Each requester gets 10%
    
    // Parent remains at 0% to not affect child allocations
    RequestCPUrate(0);

    // Create MAXPROCS children
    for(int i = 0; i < MAXPROCS; i++) {
        if ((children[i] = Fork()) == 0) {
            if(i < MAXPROCS/2) {  // First half request rates
                if(RequestCPUrate(REQ_RATE) != 0) {
                    DPrintf("\nFAIL: Process %d rate request", i);
                    Exit();
                }
                int work = (REQ_RATE * BASE_WORK) / 100;
                for(int j=0; j<work; j++) SlowPrintf(1, "%d", i);
                DPrintf("\nRequester %d done", i);
            } else {  // Second half are non-requesting
                // Calculate expected share: (100 - total_requested)/non_req_count
                int non_req_count = MAXPROCS - (MAXPROCS/2);
                int work = ((100 - (MAXPROCS/2 * REQ_RATE)) * BASE_WORK) / (100 * non_req_count);
                for(int j=0; j<work; j++) SlowPrintf(1, "N");
                DPrintf("\nNon-req %d done", i);
            }
            Exit();
        }
    }
}

// Test18: pass value overflow
void Test18() {
    DPrintf("\nTesting Pass Value Overflow\n");
    DPrintf("*CHANGE YOUR BASE TO A LARGE ENOUGH VALUE like 100000000\n");
    DPrintf("*Add reset message to your code for inspection\n");
    DPrintf("Two processes at 50%% each, running long enough to overflow pass\n");
    DPrintf("Expect reset message and continued 50:50 allocation\n");
    

    const int QUANTA_TO_OVERFLOW = 150000; // Adjust to exceed PASS_THRESHOLD with stride=20
    int child;

    if ((child = Fork()) == 0) {
        // Child process: 50% CPU
        RequestCPUrate(50);
        int start = Gettime();
        for (int i = 0; i < QUANTA_TO_OVERFLOW; i++) {
            SlowPrintf(1, "2"); // Minimal delay to count quanta
        }
        int end = Gettime();
        DPrintf("\nChild (50%%) completed after %d ms\n", end - start);
        Exit();
    }

    // Parent process: 50% CPU
    RequestCPUrate(50);
    int start = Gettime();
    for (int i = 0; i < QUANTA_TO_OVERFLOW; i++) {
        SlowPrintf(1, "1");
    }
    int end = Gettime();
    DPrintf("\nParent (50%%) completed after %d ms\n", end - start);
    Exit();
}

void Main()
{
    if (GetSchedPolicy() != PROPORTIONAL) {
        DPrintf("ERROR: Set policy to PROPORTIONAL in mycode3.c\n");
        Exit();
    }

    int testnum = 18;  // Change to select test 
    
    switch(testnum) {
        case 1: Test1(); break;
        case 2: Test2(); break;
        case 3: Test3(); break;
        case 4: Test4(); break;
        case 5: Test5(); break;
    		case 6: Test6(); break;
    		case 7: Test7(); break;
    		case 8: Test8(); break;
    		case 9: Test9(); break;
    		case 10: Test10(); break;
    		case 11: Test11(); break;
    		case 12: Test12(); break;
    		case 13: Test13(); break;
        case 14: Test14(); break;
        case 15: Test15(); break;
        case 16: Test16(); break;
        case 17: Test17(); break;
        case 18: Test18(); break;
        default: Printf("Invalid test"); Exit();
    }
}
