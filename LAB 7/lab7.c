/*
    Assume that there are 10 processes in the system. Use, random function generator to generate following details (range is also mentioned)
        1. Arrival Time (0- 30)
        2. CPU Burst Time (5-25)
        3. Priority (1-5)
    Implement FCFS, SRTF, Priority and RR (with q=2, 4, 6) scheduling approaches for a given set of values and processes. Compute the CPU utilization, Average Turnaround Time, Average Waiting Time and Turnaround Time (for each process) and Waiting Time (for each process). Repeat the entire procedure 5 times and comment on the effectiveness of scheduling algorithms used.

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


struct process{
    int pid;
    int arrivalTime;
    int cpuBurstTime;
    int priority;
    int remainingTime;
    int waitingTime;
};


void printProcess(struct process* p){
    printf("---------------------------------------------------------------------------------\n");
    printf("Process Details:\n");
    printf("PID: %d\n", p->pid);
    printf("Arrival Time: %d\n", p->arrivalTime);
    printf("Priority: %d\n", p->priority);
    printf("Waiting Time: %d\n", p->waitingTime);
    printf("---------------------------------------------------------------------------------\n");
}

void printProcesses(struct process* p){
    for(int i=0; i<10; i++){
        printProcess(&p[i]);
    }
}


void generateRandomProcess(){
    FILE* fp = fopen("processes.txt", "w");
    for(int i=0; i<10; i++){
        int arrivalTime = rand() % 30;
        int cpuBurstTime = rand() % 25 + 5;
        int priority = rand() % 5 + 1;
        fprintf(fp, "%d %d %d", arrivalTime, cpuBurstTime, priority);
        if(i!=9){
            fprintf(fp, "\n");
        }
    }
    fclose(fp);
}

struct process* copyProcessArray(struct process* p){
    struct process* copy = (struct process*)malloc(10*sizeof(struct process));
    for(int i=0; i<10; i++){
        copy[i] = p[i];
    }
    return copy;
}

int compareArrivalTime(const void* a, const void* b){
    struct process* p1 = (struct process*)a;
    struct process* p2 = (struct process*)b;
    return (p1->arrivalTime - p2->arrivalTime);
}


int compareRemainingTime(const void* a, const void* b){
    struct process* p1 = (struct process*)a;
    struct process* p2 = (struct process*)b;
    return (p1->remainingTime - p2->remainingTime);
}

int comparePriority(const void* a, const void* b){
    struct process* p1 = (struct process*)a;
    struct process* p2 = (struct process*)b;
    return (p1->priority - p2->priority);
}

struct process* queue;
int front = 0;
int rear = 0;

void initializeQueue(){
    queue = (struct process*)malloc(10*sizeof(struct process));
}

void freeQueue(){
    free(queue);
}


// cmp = 0 for remaining time
// cmp = 1 for priority
// cmp = -1 for no sorting



void addToQueue(struct process* p, int cmp){
    queue[rear] = *p;
    rear++;
    if(cmp==0)
    qsort(queue, rear+1, sizeof(struct process), compareRemainingTime);
    else if(cmp==1)
    qsort(queue, rear+1, sizeof(struct process), comparePriority);
}

void removeFromQueue(int flag){
    if(rear==0){
        return;
    }
    if(flag){
        printProcess(&queue[front]);
    }
    for(int i=1; i<rear; i++){
        queue[i-1] = queue[i];
    }
    rear--;
}

void updateQueue(){
    queue[front].remainingTime--;
    for(int i=front+1; i<rear; i++){
        queue[i].waitingTime++;
    }
    if(queue[front].remainingTime==0){
        removeFromQueue(1);
    }
}

void updateQueueRR(int q){
    queue[front].remainingTime--;
    for(int i=front+1; i<rear; i++){
        queue[i].waitingTime++;
    }
    if(queue[front].remainingTime==0){
        removeFromQueue(1);
    }
    // subtracting remaining from burst time to check if the time elapsed is a multiple of q
    else if(((queue[front].cpuBurstTime-queue[front].remainingTime)%q)==0){
        struct process* p = &queue[front];
        addToQueue(p,-1);
        removeFromQueue(0);
    }
}


void FCFS(){
    FILE* fp = fopen("processes.txt", "r");
    struct process* processes = (struct process*)malloc(10*sizeof(struct process));
    for(int i=0; i<10; i++){
        fscanf(fp, "%d %d %d", &processes[i].arrivalTime, &processes[i].cpuBurstTime, &processes[i].priority);
        processes[i].pid = i+1;
        processes[i].remainingTime = processes[i].cpuBurstTime;
    }
    qsort(processes, 10, sizeof(struct process), compareArrivalTime);
    for(int i=0; i<10; i++){
        printProcess(&processes[i]);
    }
    fclose(fp);
}
void SRTF(){
    initializeQueue();
    FILE* fp = fopen("processes.txt", "r");
    struct process* processes = (struct process*)malloc(10*sizeof(struct process));
    for(int i=0; i<10; i++){
        fscanf(fp, "%d %d %d", &processes[i].arrivalTime, &processes[i].cpuBurstTime, &processes[i].priority);
        processes[i].pid = i+1;
        processes[i].remainingTime = processes[i].cpuBurstTime;
    }
    qsort(processes, 10, sizeof(struct process), compareArrivalTime);
    addToQueue(&processes[0],0);
    int time = processes[0].arrivalTime;
    int i = 1;
    while(i<10){
        if(time==processes[i].arrivalTime){
            addToQueue(&processes[i],0);
            i++;
        }
        else{
            updateQueue();
            time++;
        }
    }
    while(front!=-1 && rear!=-1){
        updateQueue();
        time++;
    }
    freeQueue();
    fclose(fp);
}


void priority(){
    initializeQueue();
    FILE* fp = fopen("processes.txt", "r");
    struct process* processes = (struct process*)malloc(10*sizeof(struct process));
    for(int i=0; i<10; i++){
        fscanf(fp, "%d %d %d", &processes[i].arrivalTime, &processes[i].cpuBurstTime, &processes[i].priority);
        processes[i].pid = i+1;
        processes[i].remainingTime = processes[i].cpuBurstTime;
    }
    qsort(processes, 10, sizeof(struct process), compareArrivalTime);
    addToQueue(&processes[0],1);
    int time = processes[0].arrivalTime;
    int i = 1;
    while(i<10){
        if(time==processes[i].arrivalTime || (front==-1 && rear==-1)){
            while(i<10 && (time==processes[i].arrivalTime || (front==-1 && rear==-1))){
                addToQueue(&processes[i],1);
                i++;
            }
        }
        else{
            updateQueue();
            time++;
        }
    }
    while(front!=-1 && rear!=-1){
        updateQueue();
        time++;
    }
    freeQueue();
    fclose(fp);
}


void RR(){
    FILE* fp = fopen("processes.txt", "r");
    struct process* processes1 = (struct process*)malloc(10*sizeof(struct process));
    for(int i=0; i<10; i++){
        fscanf(fp, "%d %d %d", &processes1[i].arrivalTime, &processes1[i].cpuBurstTime, &processes1[i].priority);
        processes1[i].pid = i+1;
        processes1[i].remainingTime = processes1[i].cpuBurstTime;
        processes1[i].waitingTime = 0;
    }
    qsort(processes1, 10, sizeof(struct process), compareArrivalTime);
    for(int q=2; q<=2; q+=2){
        struct process* processes = copyProcessArray(processes1);
        // printProcesses(processes);`
        initializeQueue();
        printf("\nRuning RR for q = %d\n", q);
        addToQueue(&processes[0],-1);
        int time = processes[0].arrivalTime;
        int i = 1;
        while(i<10){
            if(time==processes[i].arrivalTime || rear==0){
                while(i<10 && (time==processes[i].arrivalTime || rear==0)){
                    addToQueue(&processes[i],-1);
                    time = processes[i].arrivalTime;
                    i++;
                }
            }
            else{
                updateQueueRR(q);
                time++;
            }
        }
        while(rear!=0){
            updateQueueRR(q);
            time++;
        }
        freeQueue();
    }
    fclose(fp);
}

int main(){
    generateRandomProcess();
    // FCFS();
    SRTF();
    // priority();
    // RR();
    return 0;
}