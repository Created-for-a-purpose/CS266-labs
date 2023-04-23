/**
 * @file lab5.c
 * @brief Lab 5: System Calls
 * @details
 * 1. Write a C program to create the process using fork() and display the parent and child process ID using getpid() and getppid() system calls, respectively.
 * 2. Write a C program to demonstrate the execl() system call for (1) display the content of directory (command: ls)(2) display the process tree (command: pstree).
 * 3. Write a C program to demonstrate wait and sleep system calls return the pid of the child that terminated.
 * 4. Write a C program to create the multiple processes using fork() and display the process IDs and their parent process IDs in hierarchy.
 * 5. Write a C program to display the directory content using readdir system call.
 * 6. Write a c program to interrupt and terminate the current process using signal handlers. (use SIGINT i.e., Ctrl-C to interrupt and Ctrl-\to kill the process)
 * Also, run the codes shared in the class and practice with the various system calls in LINUX OS.
 * @author Aniruddh Modi
 * @date 2023-04-23
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<dirent.h>
#include<signal.h>



void question1(){
    /*
        Fork returns 0 if it is the child process and the child process ID if it is the parent process.
    */
    pid_t pid = fork();
    if(pid == 0){
        printf("Child process ID: %d\n", getpid());
        printf("Parent process ID: %d\n", getppid());
    }
    else{
        printf("Parent process ID: %d\n", getpid());
        printf("Child process ID: %d\n", pid);
    }
}

void question2(){
    /*
        execl() takes the path of the executable file as the first argument and the arguments to the executable file as the second argument.
    */
    if(fork() == 0){
        printf("The execl command called ls!\n");
        execl("/bin/ls", "ls", NULL);
    }
    else{
        wait(NULL);
        printf("The execl command called pstree!\n");
        execl("/usr/bin/pstree", "pstree", NULL);
    }
}

void question3(){
    /*
        wait() returns the process ID of the child that terminated.
    */
    if(fork() == 0){
        printf("Child process ID: %d\n", getpid());
        printf("Parent process ID: %d\n", getppid());
    }
    else{
        printf("Parent process ID: %d\n", getpid());
        printf("Child process ID: %d\n", wait(NULL));
    }
}

void question4(){
    /*
        Fork returns 0 if it is the child process and the child process ID if it is the parent process.
    */
    pid_t pid = fork();
    if(pid == 0){
        pid_t pid1 = fork();
        if(pid1 == 0){
            printf("Child of Child1 process ID: %d\n", getpid());
            printf("Child1 process ID: %d\n", getppid());
        }
        else{
            wait(NULL);
            printf("Child1 process ID: %d\n", getpid());
            printf("Child of Child1 process ID: %d\n", pid1);
        }
    }
    else{
        wait(NULL);
        pid_t pid2 = fork();
        if(pid2 == 0){
            printf("Child2 process ID: %d\n", getpid());
            printf("Parent process ID: %d\n", getppid());
        }
        else{
            wait(NULL);
            printf("Parent process ID: %d\n", getpid());
            printf("Child1 process ID: %d\n", pid);
            printf("Child2 process ID: %d\n", pid2);
        }
    }
}

void question5(){
    
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
}
void handleInterupt(){
    printf("\nInterupt occured\n");
    exit(0);
}

void handleQuit(){
    printf("\nBye Bye!!\n");
    exit(0);
}

void question6(){
    signal(SIGINT, handleInterupt);
    signal(SIGQUIT, handleQuit);
    while(1);
}

int main(){

    // question1();
    // question2();
    // question3();
    // question4();
    // question5();
    question6();

    return 0;
}