#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> // open function

int num_processes = 0;

typedef struct {
    int arrival_time;
    int processing_time;
} Process;

void printResult(double mean_turnaround){
    printf("mean turnaround = %lf",mean_turnaround);
}

double calc_fcfs_mean_turnaround(Process* processes, int num_processes) {
    // Calculate average turnaround time for FCFS scheduling
    double total_turnaround_time = 0;
    for (int i = 0; i < num_processes; i++) {
        total_turnaround_time += processes[i].processing_time;
        if (i > 0) {
            total_turnaround_time += processes[i].arrival_time - processes[i-1].processing_time;
        }
    }
    double mean_turnaround = total_turnaround_time / num_processes;
    printResult(mean_turnaround);
    return mean_turnaround;
}

double calc_lcfs_np_mean_turnaround(Process* processes, int num_processes) {
    // Calculate average turnaround time for LCFS (not preemptive) scheduling
    double total_turnaround_time = 0;
    for (int i = num_processes - 1; i >= 0; i--) {
        total_turnaround_time += processes[i].processing_time;
        if (i < num_processes - 1) {
            total_turnaround_time += processes[i+1].arrival_time - processes[i].processing_time;
        }
    }
    double mean_turnaround = total_turnaround_time / num_processes;
    printResult(mean_turnaround);
    return mean_turnaround;
}

double calc_lcfs_p_mean_turnaround(Process* processes, int num_processes) {
    // Calculate average turnaround time for LCFS (preemptive) scheduling
    double total_turnaround_time = 0;
    Process* remaining_processes = malloc(num_processes * sizeof(Process));
    for (int i = 0; i < num_processes; i++) {
        remaining_processes[i] = processes[i];
    }
    int num_remaining_processes = num_processes;
    while (num_remaining_processes > 0) {
        // Choose process with greatest processing time
        int max_processing_time_index = 0;
        for (int i = 1; i < num_remaining_processes; i++) {
            if (remaining_processes[i].processing_time > remaining_processes[max_processing_time_index].processing_time) {
                max_processing_time_index = i;
            }
        }
        Process chosen_process = remaining_processes[max_processing_time_index];
        total_turnaround_time += chosen_process.processing_time;
        // Remove chosen process from remaining processes
        for (int i = max_processing_time_index; i < num_remaining_processes - 1; i++) {
            remaining_processes[i] = remaining_processes[i+1];
        }
        num_remaining_processes--;
    }
    free(remaining_processes);
    double mean_turnaround = total_turnaround_time / num_processes;
    printResult(mean_turnaround);
    return mean_turnaround;
}

double calc_rr_mean_turnaround(Process* processes, int num_processes) {
    // Calculate average turnaround time for Round Robin scheduling with time quantum of 2 seconds
    double total_turnaround_time = 0;
    Process* remaining_processes = malloc(num_processes * sizeof(Process));
    for (int i = 0; i < num_processes; i++) {
        remaining_processes[i] = processes[i];
    }
    int num_remaining_processes = num_processes;
    while (num_remaining_processes > 0) {
        // Run each process for 2 seconds at a time
        Process current_process = remaining_processes[0];
        total_turnaround_time += current_process.processing_time;
        current_process.processing_time -= 2;
        // Update remaining processing times for other processes
        for (int i = 1; i < num_remaining_processes; i++) {
            remaining_processes[i].processing_time -= 2;
        }
        // Add process back to end of list if it has not finished
        if (current_process.processing_time > 0) {
            remaining_processes[num_remaining_processes] = current_process;
            num_remaining_processes++;
        }
        // Remove current process from list
        for (int i = 0; i < num_remaining_processes - 1; i++) {
            remaining_processes[i] = remaining_processes[i+1];
        }
        num_remaining_processes--;
    }
    free(remaining_processes);
    double mean_turnaround = total_turnaround_time / num_processes;
    printResult(mean_turnaround);
    return mean_turnaround;
}

double calc_sjf_mean_turnaround(Process* processes, int num_processes) {
    // Calculate average turnaround time for SJF (preemptive) scheduling
    double total_turnaround_time = 0;
    Process* remaining_processes = malloc(num_processes * sizeof(Process));
    for (int i = 0; i < num_processes; i++) {
        remaining_processes[i] = processes[i];
    }
    int num_remaining_processes = num_processes;
    while (num_remaining_processes > 0) {
        // Choose process with shortest processing time
        int min_processing_time_index = 0;
        for (int i = 1; i < num_remaining_processes; i++) {
            if (remaining_processes[i].processing_time < remaining_processes[min_processing_time_index].processing_time) {
                min_processing_time_index = i;
            }
        }
        Process chosen_process = remaining_processes[min_processing_time_index];
        total_turnaround_time += chosen_process.processing_time;
        // Remove chosen process from remaining processes
        for (int i = min_processing_time_index; i < num_remaining_processes - 1; i++) {
            remaining_processes[i] = remaining_processes[i+1];
        }
        num_remaining_processes--;
    }
    free(remaining_processes);
    double mean_turnaround = total_turnaround_time / num_processes;
    printResult(mean_turnaround);
    return mean_turnaround;
}

int getLine(char buffer, int buf_size, int file){
     char line[256];
    if(read(file,buffer,buf_size)>0){
        write(file,buffer,buf_size);
        return 1;
    }
    else{
        printf("coldunt read line");
        return 0;
     }
}

int main(int argc, char **argv){
    char* fileName;
    int fd;
    char line[256];
    if(argc > 1){
         fileName = argv[1]; /* should check that argc > 1 */
    }
    else{
        printf("no file wad added");
        exit(-1);
    }
    fd = open(fileName, "r"); /* should check the result */
    if(fd< 0){
        perror("cant open file");
    }
    else{
         if(getLine(line, sizeof(line), fd)>0){
            num_processes = atoi(line);
            printf("num of proccesses is: %d", num_processes);
         }
         else{
            printf("no proccesses were found");
         }

    close(fd);
    }
    exit(0);
}

