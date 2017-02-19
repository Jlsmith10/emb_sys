// Section 2: Energy-efficient real time scheduler
// Important! DO NOT MODIFY this file. You will not submit this file.
// This file is provided for your implementation of the program procedure.
// For more details, please see the instructions in the class website.

#include <stdio.h>
#include <pthread.h>
#include <wiringPi.h>
#include <softPwm.h>

#include <stdlib.h>
#include <signal.h>

#include "workload.h"
#include "assignment1.h"
#include "assignment2.h"

#include "scheduler.h"
#include "governor.h"

#define TIME_SELECT_TASK    0

#define thread_def(NAME) \
int bExit_##NAME; \
void* thread_##NAME(void* param) { \
	SharedVariable* pV = (SharedVariable*) param; \
	workload_##NAME(); \
	body_##NAME(pV); \
	bExit_##NAME = 1; \
	return NULL; }

thread_def(button)
thread_def(twocolor)
thread_def(temp)
thread_def(track)
thread_def(touch)
thread_def(rgbcolor)
thread_def(aled)
thread_def(buzzer)

void signal_handler(int signum) {
	finish_workload();
	release_buffer(fileno(stdout));
	printf("\nProgram exits by SIGINT.\n");
	exit(0);
}

int main(int argc, char* argv[]) {
	SharedVariable v;
  long long startTime, finishTime;
  int min_freq_count = 0, max_freq_count = 0;

	int runningTimeInSec = 10;
	if (argc == 2) {
		runningTimeInSec = atoi(argv[1]);
	}

	if (wiringPiSetup() == -1) {
		printf("Failed to setup wiringPi.");
		return 1; 
	}

	// Initialize for the interfaces provided
	signal(SIGINT, signal_handler);
	init_deferred_buffer(1024); // 1MB
	init_userspace_governor();
	init_workload();

	// Initializers that you need to implement
	init_shared_variable(&v);
	init_sensors(&v);
	learn_workloads(&v);

	// Init scheduler
	int aliveTasks[NUM_TASKS];
	init_scheduler(runningTimeInSec);
	set_by_max_freq(); // reset to the max freq

	printDBG("Start Scheduling with %d threads\n", NUM_TASKS);
	TaskSelection sel;
	long long idleTime;

	while (v.bProgramExit != 1) {
		// 1. Prepare tasks
		idleTime = prepare_tasks(aliveTasks, &v);
		if (idleTime < 0)
			break;

		// 2. Select a process: You need to implement.
    if(TIME_SELECT_TASK) {
      startTime = get_current_time_us();
      
      sel = select_task(&v, aliveTasks, idleTime); 

      finishTime = get_current_time_us();
      printDBG("select task time (us): %lld \n", finishTime - startTime);
    }
    else {
      sel = select_task(&v, aliveTasks, idleTime); 
    }

		if (sel.task < 0)
			break;

    if(sel.freq == 0) {
      min_freq_count++;
    }
    else {
      max_freq_count++;
    }

		// 3. Run the selected task
		execute_task(sel);
	}

	finish_workload();
	release_buffer(fileno(stdout));
	printf("Scheduler program finished.\n");
  printf("max_freq_count = %d, min_freq_count = %d\n", min_freq_count, max_freq_count);


	return 0;
}
