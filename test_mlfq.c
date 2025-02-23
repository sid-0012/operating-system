#include <stdio.h>
#include "aux.h"
#include "umix.h"
#include "sys.h"
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>


// Ensure that within each priority level (0 to LMAX), processes execute in FIFO order unless preempted.
// Test: Create multiple processes at the same level and verify they finish in order of creation unless quantum limits intervene.
void Test1() {
    // Test: Basic FIFO Behavior at Level 0
    DPrintf("Test 1: FIFO Behavior at L=0\n");
    DPrintf("Processes should finish in order: 1, 2, 3\n");
    if (GetSchedPolicy() != MLFQ) {
        DPrintf("ERROR: Policy is not MLFQ. Set to MLFQ in mycode3.c\n");
        Exit();
    }

    if (Fork() == 0) {
        SlowPrintf(5, "222222"); // Process 2
        DPrintf("\nProcess 2 (L=0) exiting\n");
        Exit();
    }
    if (Fork() == 0) {
        SlowPrintf(5, "333333"); // Process 3
        DPrintf("\nProcess 3 (L=0) exiting\n");
        Exit();
    }
    SlowPrintf(5, "111111"); // Process 1
    DPrintf("\nProcess 1 (L=0) exiting\n");
    Exit();
}

// Processes at higher priority levels (lower L values) should preempt those at lower levels.
// Test: Start a process at a lower level (e.g., L=1), then create a new process (starts at L=0), and check if the L=0 process runs first.
void Test2() {
    // Test: Priority Level Preemption
    DPrintf("Test 2: Higher Priority Preemption\n");
    DPrintf("Process 2 (L=0) should preempt Process 1 after process 1 printing out 11111\n");
    if (GetSchedPolicy() != MLFQ) {
        DPrintf("ERROR: Policy is not MLFQ\n");
        Exit();
    }

    // Simulate Process 1 running long enough to drop to L=1
    for (int i = 0; i < 5; i++) SlowPrintf(1, "1"); // Exceed 1 quantum at L=0
    if (Fork() == 0) {
        SlowPrintf(5, "222222"); // Process 2 starts at L=0
        DPrintf("\nProcess 2 exiting\n");
        Exit();
    }
    SlowPrintf(5, "111111"); // Process 1 continues
    DPrintf("\nProcess 1 exiting\n");
    Exit();
}

// A process should be demoted to the next level (L+1) after using 2^L quanta, unless at LMAX.
// Test: Run a process at L=0 with enough work to exceed 2^0=1 quantum, and verify it moves to L=1; repeat up to LMAX.
void Test3() {
    // Test: Quantum-Based Demotion, including stays at LMAX for 3 times and back to L=0
    DPrintf("Test 3: Quantum Demotion\n");
    DPrintf("Process 1 should drop from 0 to LMAX then back to 0\n");
    DPrintf("Add DPrintf to your code when move down\n");
    // expect:
        // Start process 1
        // Preempt process 0
        // Test 3: Quantum Demotion L=0 to L=1
        // Process 1 should drop to L=1 after 1 quantum
        // Move down process 1 to level 1
        // Move down process 1 to level 2
        // Move down process 1 to level 3
        // Move down process 1 to level 4
        // 1Move down process 1 to level 5
        // 1Move down process 1 to level 6
        // 11Move down process 1 to level 7
        // 1111Move down process 1 to level 8
        // 111111111Move down process 1 to level 9
        // 1111111111111111Move down process 1 to level 10
        // 1111111111111111111111111111111111Move down process 1 to level 11
        // 111111111111111111111111111111111111111111111111111111111111111111Move down process 1 to level 12
        // 111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111Process 1 Stay at LMAX for 1 times
        // 1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111Process 1 Stay at LMAX for 2 times
        // 1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111Process 1 Stay at LMAX for 3 times
        // process 1 stay at LMAX for 3 times, promote to L0
        // Move down process 1 to level 1
        // 1Move down process 1 to level 2
        // Move down process 1 to level 3
        // Move down process 1 to level 4
        // ......
    if (GetSchedPolicy() != MLFQ) {
        DPrintf("ERROR: Policy is not MLFQ\n");
        Exit();
    }


    for (int i = 0; i < 550; i++) SlowPrintf(5, "1");
    
    Exit();
}

void Test4() {
    // Test: Parent Preemption and Queue Head Placement
    DPrintf("Test 5: Parent Preemption and Queue Head\n");
    DPrintf("Parent at L=1 creates child (L=0), parent goes to head of L=1\n");
    if (GetSchedPolicy() != MLFQ) {
        DPrintf("ERROR: Policy is not MLFQ\n");
        Exit();
    }

    
    SlowPrintf(5, "111111"); 
    if (Fork() == 0) {
        SlowPrintf(5, "222222222222"); 
        DPrintf("\nprocess 2 exiting\n");
        Exit();
    }
    if (Fork() == 0) {
        SlowPrintf(5, "333333333333");
        DPrintf("\nProcess 3 exiting\n");
        Exit();
    }
    SlowPrintf(5, "111111"); 
    DPrintf("\nParent (process 1) exiting, should exit before other procs\n");
    Exit();
}

// Test interaction between processes at different levels with varying workloads.
// Test: Create processes that spread across all levels and verify scheduling respects priority and quanta limits.
void Test5() {
    // Test: Multiple Processes Across Levels
    DPrintf("Test 7: Multiple Processes Across Levels\n");
    DPrintf("Processes spread across levels, higher levels run first\n");
    // expect:
        // Start process 1
        // Preempt process 0
        // Test 7: Multiple Processes Across Levels
        // Processes spread across levels, higher levels run first
        // Start process 2
        // Preempt process 1
        // Start process 3
        // Preempt process 1
        // Move down process 1 to level 1
        // 2Move down process 2 to level 1
        // 3Move down process 3 to level 1
        // 1Move down process 1 to level 2
        // 2Move down process 2 to level 2
        // 3Move down process 3 to level 2
        // 11Move down process 1 to level 3
        // 22Move down process 2 to level 3
        // 33Move down process 3 to level 3
        // 1111Move down process 1 to level 4
        // 222Move down process 2 to level 4
        // 3333Move down process 3 to level 4
        // 11111111
        // Process 1 exiting
        // 22222222Move down process 2 to level 5
        // 3333333
        // Process 3 exiting

        // Process 2 exiting
    if (GetSchedPolicy() != MLFQ) {
        DPrintf("ERROR: Policy is not MLFQ\n");
        Exit();
    }

    if (Fork() == 0) {
        for (int i = 0; i < 15; i++) SlowPrintf(1, "2"); 
        DPrintf("\nProcess 2 exiting\n");
        Exit();
    }
    if (Fork() == 0) {
        for (int i = 0; i < 15; i++) SlowPrintf(1, "3"); 
        DPrintf("\nProcess 3 exiting\n");
        Exit();
    }
    for (int i = 0; i < 15; i++) SlowPrintf(1, "1"); 
    DPrintf("\nProcess 1 exiting\n");
    Exit();
}

// Ensure the scheduler handles up to MAXPROCS processes correctly across levels.
// Test: Spawn MAXPROCS processes and verify scheduling behaves as expected without crashes.
void Test6() {
    // Test: MAXPROCS Limit
    DPrintf("Test 9: MAXPROCS (%d) Processes\n", MAXPROCS);
    DPrintf("All processes should schedule correctly\n");
    //
        // Start process 1
        // Preempt process 0
        // Test 9: MAXPROCS (10) Processes
        // All processes should schedule correctly
        // Start process 2
        // Preempt process 1
        // Start process 3
        // Preempt process 1
        // Start process 4
        // Preempt process 1
        // Start process 5
        // Preempt process 1
        // Start process 6
        // Preempt process 1
        // Start process 7
        // Preempt process 1
        // Start process 8
        // Preempt process 1
        // Start process 9
        // Preempt process 1
        // Start process 10
        // Preempt process 1
        // Move down process 1 to level 1
        // 0Move down process 2 to level 1
        // 1Move down process 3 to level 1
        // Move down process 4 to level 1
        // 3Move down process 5 to level 1
        // 4Move down process 6 to level 1
        // 5Move down process 7 to level 1
        // 6Move down process 8 to level 1
        // Move down process 9 to level 1
        // 8Move down process 10 to level 1
        // Move down process 1 to level 2
        // Move down process 2 to level 2
        // Move down process 3 to level 2
        // 2Move down process 4 to level 2
        // Move down process 5 to level 2
        // Move down process 6 to level 2
        // Move down process 7 to level 2
        // 6Move down process 8 to level 2
        // 7Move down process 9 to level 2
        // 8Move down process 10 to level 2
        // PPMove down process 1 to level 3
        // 00Move down process 2 to level 3
        // 11Move down process 3 to level 3
        // 22Move down process 4 to level 3
        // 33Move down process 5 to level 3
        // 4Move down process 6 to level 3
        // 55Move down process 7 to level 3
        // 6Move down process 8 to level 3
        // 77Move down process 9 to level 3
        // 88Move down process 10 to level 3
        // PPMove down process 1 to level 4
        // 000Move down process 2 to level 4
        // 11111Move down process 3 to level 4
        // 2222Move down process 4 to level 4
        // 3333Move down process 5 to level 4
        // 4444Move down process 6 to level 4
        // 5555Move down process 7 to level 4
        // 6666Move down process 8 to level 4
        // 7777Move down process 9 to level 4
        // 88Move down process 10 to level 4
        // PPPPMove down process 1 to level 5
        // 000000Move down process 2 to level 5
        // 1111
        // Process 1 exiting
        // 22222
        // Process 2 exiting
        // 33333
        // Process 3 exiting
        // 444444
        // Process 4 exiting
        // 55555
        // Process 5 exiting
        // 66666
        // Process 6 exiting
        // 77777
        // Process 7 exiting
        // 888888
        // Process 8 exiting
        // PPPP
        // Parent exiting

        // Process 0 exiting
    if (GetSchedPolicy() != MLFQ) {
        DPrintf("ERROR: Policy is not MLFQ\n");
        Exit();
    }

    int pids[MAXPROCS];
    for (int i = 0; i < MAXPROCS; i++) {
        if ((pids[i] = Fork()) == 0) {
            SlowPrintf(1, "%d%d%d%d%d%d%d%d%d%d%d%d", i, i, i, i, i, i, i, i, i, i, i, i);
            DPrintf("\nProcess %d exiting\n", i);
            Exit();
        }
    }
    SlowPrintf(2, "PPPPPPPPPPPP"); // Parent
    DPrintf("\nParent exiting\n");
    Exit();
}

void Test7() {
    DPrintf("Test 7: At third times of LMAX and fork. Child get placed at L0 first then parent\n");
    DPrintf("Add DPrintf to your code when move down levels\n");
    DPrintf("Add #undef LMAX #define LMAX 10 to mycode3.c to avoid kernel panic! \n");
    // expect:
    // Start process 1
    // Preempt process 0
    // Test 7: At third times of LMAX and fork. Child get placed at L0 first then parent
    // Add DPrintf to your code when move down levels
    // Move down process 1 to level 1
    // Move down process 1 to level 2
    // Move down process 1 to level 3
    // Move down process 1 to level 4
    // 1Move down process 1 to level 5
    // 1Move down process 1 to level 6
    // 11Move down process 1 to level 7
    // 1111Move down process 1 to level 8
    // 11111111Move down process 1 to level 9
    // 1111111111111111Move down process 1 to level 10
    // 11111111111111111111111111111111Process 1 Stay at LMAX for 1 times
    // 11111111111111111111111111111111Process 1 Stay at LMAX for 2 times
    // 11111111111111111111111111111Start process 2
    // Preempt at third time at LMAX, move to L0
    // Preempt process 1
    // 2Move down process 2 to level 1
    // Move down process 1 to level 1
    // 2Move down process 2 to level 2
    // 11Move down process 1 to level 2
    // 22Move down process 2 to level 3
    // 1Move down process 1 to level 3
    // 222Move down process 2 to level 4
    // 1111Move down process 1 to level 4
    // 22222222Move down process 2 to level 5
    // 111111Move down process 1 to level 5
    // 222222Process 2 exits
    // 1111111111111111Move down process 1 to level 6
    // 1111111111111111111111111111Move down process 1 to level 7
    // 1111111111111111111111111111111111111111111Process 1 exits

    if (GetSchedPolicy() != MLFQ) {
        DPrintf("ERROR: Policy is not MLFQ\n");
        Exit();
    }

    // put process at the middle of the third times in LMAX
    for (int i = 0; i < 125; i++) SlowPrintf(5, "1");

    if(Fork() == 0){
        SlowPrintf(1, "222222222222222222222");
        DPrintf("Process 2 exits\n");
        Exit();
    }

    for (int i = 0; i < 100; i++) SlowPrintf(1, "1");
    DPrintf("Process 1 exits\n");
    
    Exit();
}


void Main() {
    if (GetSchedPolicy() != MLFQ) {
        DPrintf("ERROR: Set policy to MLFQ in mycode3.c\n");
        Exit();
    }

    int testnum = 7; // Change to select test (1-9)
    switch (testnum) {
        case 1: Test1(); break;
        case 2: Test2(); break;
        case 3: Test3(); break;
        case 4: Test4(); break;
        case 5: Test5(); break;
        case 6: Test6(); break;
        case 7: Test7(); break;
        // case 8: Test8(); break;
        // case 9: Test9(); break;
        default: Printf("Invalid test number\n"); Exit();
    }
}
