# Final-Project-377
My final project for CS 377, which extends Project 3 (Scheduling). This project implements the MLFQ scheduler.
#377 Final Project ReadMe File
##Video Link: https://drive.google.com/file/d/1v1W5jkUb6yBIrostGtJ7X4kv0ceBQxQ1/view
##Slides Link: https://docs.google.com/presentation/d/1Qtic4YGLnigy0BTK6euiqdpucnykFz9oH4aOWN55dYg/edit?usp=sharing
##Description of the Project:

This project extends Project 3, Scheduling, that we were assigned in class. It does so by adding the MLFQ (Multi-Level Feedback Queue) scheduling algorithm to the project. MLFQ is a scheduling algorithm that attempts to optimize both turnaround time (as in Round Robin) and response time, to induce interactivity with the user. It uses queues to rank running processes in order of their priority, with (in my project) Queue 1 being the highest priority and Queue 4 being the lowest priority. In my project, the MLFQ scheduler takes in a workload, which is nothing but a priority queue of available processes arranged in order of arrival time, and returns a list of the processes in order of their completion time. It makes use of Round Robin scheduling.

##MLFQ in Detail:

MLFQ , or Multi-Level Feedback Queue, is a scheduling algorithm that attempts to remedy both turnaround time and response time, to make the scheduler extremely efficient. Remedying turnaround time is achieved by running shorter jobs first, and remedying response time helps jobs that are waiting for interaction/user input. 

MLFQ attempts to optimize the aforementioned factors without prior knowledge of the durations of the jobs/processes it will receive, and how interactive they are with the CPU. It 
essentially learns from its past and uses MLFQ attempts to optimize the aforementioned factors without prior knowledge of the durations of the jobs/processes it will receive, and
how interactive they are with the CPU. It essentially learns from its past and uses that information to decrease or increase a process’s priority as it sees fit.
I have used 4 queues. The basic structure of the program is as follows:
	- A process is unloaded from the workload and placed in the queue with the highest priority, i.e., queue 1, if its arrival time is before or at the current time.
	- There is a loop that loops for the duration of one time slice, which is 5ms in my project.
  	- The highest queue that contains at least one job is selected as “curQueue”. If no queue contains any jobs, we would break out; however, this situation is unlikely as the main loop would not have been entered.
 	- If the queue contains more than one job, they are sent to the round robin method, as all the jobs in that queue have equal priority.
   - If not, there are 2 possibilities for the jobs.
   - If the job has interrupts, and those interrupts lie within the remaining allotted CPU time the job has, then it is run until it has to relinquish the CPU.
   - Else, it is a fairly straightforward process; the job is run for the entire duration of its time slice and then a new time slice begins.
