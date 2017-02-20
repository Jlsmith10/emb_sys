#include "assignment1.h"
#include "assignment2.h"
#include "workload.h"
#include "scheduler.h"
#include "governor.h"
#include <limits.h> // LLONG_MAX


#define DEBUG_WORKLOAD        0
#define DEBUG_SELECT_TASK     0
#define ENERGY_OPT_1          0
#define ENERGY_OPT_2          0
#define ENERGY_OPT_3          1

// Note: Deadline of each workload is defined in the "workloadDeadlines" variable.
// i.e., You can access the dealine of the BUTTON thread using workloadDeadlines[BUTTON]
// See also deadlines.c and workload.h

// Assignment: You need to implement the following two functions.

// learn_workloads(SharedVariable* v):
// This function is called at the start part of the program before actual scheduling
// - Parameters
// sv: The variable which is shared for every function over all threads
void learn_workloads(SharedVariable* sv) {
  // This function is executed before the scheduling simulation.
  // You need to calculate the execution time of each thread here.

  // Thread functions for workloads: 
  // thread_button, thread_twocolor, thread_temp, thread_track,
  // thread_touch, thread_rgbcolor, thread_aled, thread_buzzer

  // Tip 1. You can call each workload function here like:
  // thread_button();
  //
  long long startTime, finishTime;
  long long last_deadline;

  // First get workloads at max frequency
  set_by_max_freq();

  startTime = get_current_time_us();
  thread_button(sv);
  finishTime = get_current_time_us();
  sv->max_workloads[0] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_button time (us): %lld\n", finishTime - startTime);
  }

  startTime = get_current_time_us();
  thread_twocolor(sv);
  finishTime = get_current_time_us();
  sv->max_workloads[1] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_twocolor time (us): %lld\n", finishTime - startTime);
  }

  startTime = get_current_time_us();
  thread_temp(sv);
  finishTime = get_current_time_us();
  sv->max_workloads[2] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_temp time (us): %lld\n", finishTime - startTime); 
  }

  startTime = get_current_time_us();
  thread_track(sv);
  finishTime = get_current_time_us();
  sv->max_workloads[3] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_track time (us): %lld\n", finishTime - startTime);
  }

  startTime = get_current_time_us();
  thread_touch(sv);
  finishTime = get_current_time_us();
  sv->max_workloads[4] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_touch time (us): %lld\n", finishTime - startTime);
  }

  startTime = get_current_time_us();
  thread_rgbcolor(sv);
  finishTime = get_current_time_us();
  sv->max_workloads[5] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_rgbcolor time (us): %lld\n", finishTime - startTime);
  }

  startTime = get_current_time_us();
  thread_aled(sv);
  finishTime = get_current_time_us();
  sv->max_workloads[6] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_aled time (us): %lld \n", finishTime - startTime);
  }

  startTime = get_current_time_us();
  thread_buzzer(sv);
  finishTime = get_current_time_us();
  sv->max_workloads[7] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_buzzer time (us): %lld \n", finishTime - startTime);
  }
    
  // Then get workloads at min frequency
  set_by_min_freq();

  startTime = get_current_time_us();
  thread_button(sv);
  finishTime = get_current_time_us();
  sv->min_workloads[0] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_button time (us): %lld\n", finishTime - startTime);
  }

  startTime = get_current_time_us();
  thread_twocolor(sv);
  finishTime = get_current_time_us();
  sv->min_workloads[1] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_twocolor time (us): %lld\n", finishTime - startTime);
  }

  startTime = get_current_time_us();
  thread_temp(sv);
  finishTime = get_current_time_us();
  sv->min_workloads[2] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_temp time (us): %lld\n", finishTime - startTime); 
  }

  startTime = get_current_time_us();
  thread_track(sv);
  finishTime = get_current_time_us();
  sv->min_workloads[3] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_track time (us): %lld\n", finishTime - startTime);
  }

  startTime = get_current_time_us();
  thread_touch(sv);
  finishTime = get_current_time_us();
  sv->min_workloads[4] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_touch time (us): %lld\n", finishTime - startTime);
  }

  startTime = get_current_time_us();
  thread_rgbcolor(sv);
  finishTime = get_current_time_us();
  sv->min_workloads[5] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_rgbcolor time (us): %lld\n", finishTime - startTime);
  }

  startTime = get_current_time_us();
  thread_aled(sv);
  finishTime = get_current_time_us();
  sv->min_workloads[6] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_aled time (us): %lld \n", finishTime - startTime);
  }

  startTime = get_current_time_us();
  thread_buzzer(sv);
  finishTime = get_current_time_us();
  sv->min_workloads[7] = finishTime - startTime;

  if(DEBUG_WORKLOAD) {
    printDBG("thread_buzzer time (us): %lld \n", finishTime - startTime);
  }
  
  int i; 

  if(DEBUG_WORKLOAD) {
    for(i = 0; i < NUM_TASKS; i++) {
      printDBG("workloadDeadlines[%d] = %d\n", i, workloadDeadlines[i]);
    }
  }


}


// select_task(SharedVariable* sv, const int* aliveTasks):
// This function is called while runnning the actual scheduler
// - Parameters
// sv: The variable which is shared for every function over all threads
// aliveTasks: an array where each element indicates whether the corresponed task is alive(1) or not(0).
// idleTime: a time duration in microsecond. You can know how much time was waiting without any workload
//           (i.e., it's larger than 0 only when all threads are finished and not reache the next preiod.)
// - Return value
// TaskSelection structure which indicates the scheduled task and the CPU frequency
TaskSelection select_task(SharedVariable* sv, const int* aliveTasks, long long idleTime) {
  // This function is executed inside of the scheduling simulation.
  // You need to implement an energy-efficient EDF (Earliest Deadline First) scheduler.

  // Tip 1. You may get the current time elapsed in the scheduler here like:
  // long long curTime = get_scheduler_elapsed_time_us();

  // Also, do not make any interruptable / IO tasks in this function.
  // You can use printfDBG instead of printf.
  int i;
  long long earliest_deadline = LLONG_MAX;
  int earliest_task;

  for(i = 0; i < NUM_TASKS; i++) {
    if(aliveTasks[i] == 1) {

      // Check if we have a new earlier deadline
      if(workloadDeadlines[i] < earliest_deadline) {
        // If so, set it to the highest priority task
        earliest_deadline = workloadDeadlines[i];
        earliest_task = i;
      }
    }
  }

  long long elapsed_time = get_scheduler_elapsed_time_us();

  // The retun value can be specified like this:
  TaskSelection sel;
  
  // The thread ID which will be scheduled. i.e., 0(BUTTON) ~ 7(BUZZER)
  sel.task = earliest_task; 

  // Request the maximum frequency (if you want the minimum frequency, use 0 instead.)
  sel.freq = 1;

  if(ENERGY_OPT_1) {
    if(elapsed_time + sv->min_workloads[earliest_task] <
              workloadDeadlines[earliest_task]) {
      sel.freq = 0;
    }
    else {
      sel.freq = 1;
    }
  }

  if(ENERGY_OPT_2) {
    sel.freq = 0;
  }

  // Final energy optimization
  if(ENERGY_OPT_3) {
    // Check if task has expired or just arrived
    if(sv->elapsed_time[earliest_task] <= 0) {
      sv->elapsed_time[earliest_task] = sv->min_workloads[earliest_task];
    }

    // Check if we can still execute the task at minimum frequency before the deadline
    if(sv->elapsed_time[earliest_task] + elapsed_time < workloadDeadlines[earliest_task]) {
      sel.freq = 0;
      // Subtract 10000 (10 ms) from elapsed time because workload is based on minimum frequency
      sv->elapsed_time[earliest_task] -= 10000;
    }
    else {
      sel.freq = 1;
      
      // Subtract 20000 (20 ms) from elapsed time because workload is based on minimum frequency
      sv->elapsed_time[earliest_task] -= 20000;
    }
  }

  return sel;
}
