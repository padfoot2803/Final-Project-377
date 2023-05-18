#include <scheduling.h>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#define time_slice 5 //the quantum time slice allotted to each process
using namespace std;

pqueue_arrival read_workload(string filename) {
  pqueue_arrival workload;
  ifstream infile(filename);
  string line;
  while (getline(infile, line)) {
    istringstream iss(line);
    int arrival, duration;
    if (!(iss >> arrival >> duration)) {
      break;
    } 
    Process p = {arrival, 0, duration, 0};
    workload.push(p);
  }
  infile.close();
  return workload;
}

void show_workload(pqueue_arrival workload) {
  pqueue_arrival xs = workload;
  cout << "Workload:" << endl;
  while (!xs.empty()) {
    Process p = xs.top();
    cout << '\t' << p.arrival << ' ' << p.duration << endl;
    xs.pop();
  }
}

void show_processes(list<Process> processes) {
  list<Process> xs = processes;
  cout << "Processes:" << endl;
  while (!xs.empty()) {
    Process p = xs.front();
    cout << "\tarrival=" << p.arrival << ", duration=" << p.duration
         << ", first_run=" << p.first_run << ", completion=" << p.completion
         << endl;
    xs.pop_front();
  }
}

list<Process> mlfq (pqueue_arrival workload) {
  std::queue<Process> q1; //highest priority
  std::queue<Process> q2;
  std::queue<Process> q3;
  std::queue<Process> q4;//lowest priority
  std::queue<Process> curQueue;
  Process curProc;
  int interruptTime = -1;
  int curProcNum;
  int timePassed;
  
  std::queue<Process> arrayOfQueues [4] = {q1, q2, q3, q4};
  list <Process> complete; 
  pqueue_duration procsArrived; 
  int current_time = 0;
  while(!workload.empty() || !q1.empty() || !q2.empty()|| !q3.empty() || !q4.empty()) {
    Process proc; 

    while(true){    
      if(workload.empty()){
        break;
      }
      // get all procs arrived on or before @current_time - duration ordered into @procsArrived
      proc = workload.top(); 
      if(proc.arrival <= current_time){                             
        q1.push(proc);
        workload.pop(); //we get the process at the top of the workload, which has to have arrived before or at the current time!
        break; 
      }
      else{
        break;
      }
    }
    //now, a loop that runs for the duration of 1 time slice = 5ms
    while(true){
        int arrLen = sizeof(arrayOfQueues)/sizeof(arrayOfQueues[0]);
        int curQueueLen = sizeof(curQueue)/sizeof(curQueue.front());
        //this loop runs for the duration of 1 time slice
        for(int i = 0; i < arrLen; ++i){
                if(!arrayOfQueues[i].empty()){
                        curQueue = arrayOfQueues[i];
                        curProcNum = i + 1;
                        break;  
                }
        }
        if(curQueueLen == 1){
                curProc = curQueue.front();
                curQueue.pop();
                if(curProc.first_run == -1){
                        curProc.first_run = current_time; //if the process has obtained the CPU for the first time, we update its first run time
                }
                timePassed = curProc.duration - curProc.durationLeft;
                //we start executing a time slice. to do this, we check whether the process has interrupts within 1 time slice
                for(int i = 0; i < 3; ++i){
int placeHolder = curProc.interrupts[i];//getting the next available interrupt of the current process 
                        if(placeHolder != -1){//if there exists 1, we store it in a variable and set it to -1 so that next time, the next interrupt will be accessed
                                interruptTime = placeHolder;
                                curProc.interrupts[i] = -1;
                                break;
                        }
                        //need to set interrupt to -1 so that next time, next interrupt will be used
                } 
                //now, we have the interrupt time, if it exists
                //we now proceed with the existing interrupt within the time slice
                if(interruptTime > timePassed && interruptTime - timePassed <= 5 && interruptTime != -1){
                        //the interrupt time is within the current time! the interrupt is going to take place now! 
                        if(curProc.allottedTime <= (interruptTime-timePassed)){
                                curProc.durationLeft -= curProc.allottedTime;
                                curProc.allottedTime = time_slice; 
                                //demote the process!
                                if(curProcNum != 4){
                                //if the process is already in the lowest priority queue, it remains there. If it isn't, it gets demoted by one level.
                                        arrayOfQueues[curProcNum].push(curProc);
                                }
                        }
                        else{
                                curProc.allottedTime -= (interruptTime - timePassed);
                                curProc.durationLeft -= (interruptTime - timePassed);
                }
                //if the process had no (future) interrupts, we proceed as follows
                else{
                        //just edit its duration by subtracting time slice from it
                        if(curProc.duration <= time_slice){
                                //if the process is shorter than/exactly 5ms/has less than/equal to 5ms left to execute)
                                current_time += curProc.duration;
                                curProc.duration = 0;
                                curProc.completion = current_time;
                                complete.push_back(proc);
                        }
                        else{
                                //if the process has more than the remaining time to execute, and doesn't have any interrupts in this time slice, we let it execute for the duration of the entire time sli>
                                current_time += time_slice;
                                curProc.duration -= time_slice;
                                //then, we demote the process!
                                if(curProcNum != 4){
                                //if the process is already in the lowest priority queue, it remains there. If it isn't, it gets demoted by one level.
                                        arrayOfQueues[curProcNum].push(curProc);
                                }
                        }
                } 
        }
        else{
                //round robin!
        }
		}
    while(!procsArrived.empty()){
      workload.push(procsArrived.top());
      procsArrived.pop();
    }
  }

}
