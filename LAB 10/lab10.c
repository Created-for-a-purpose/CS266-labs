/*

    A single-lane bridge connects the two Vermont villages of North Tunbridge and South Tunbridge.
    Farmers in the two villages use this bridge to deliver their produce to the neighboring town.
    The bridge can become deadlocked if a northbound and a southbound farmer get on the bridge at the same time.
    (Vermont farmers are stubborn and are unable to back up.)
    Using semaphores and/or mutex locks, design an algorithm in pseudocode that prevents deadlock.
    Initially, do not be concerned about starvation
    (the situation in which northbound farmers prevent southbound farmers from using the bridge, or vice versa).

    Implement your solution using POSIX synchronization. In particular,
    represent northbound and southbound farmers as separate threads.
    Once a farmer is on the bridge, the associated thread will sleep
    for a random period of time, representing traveling across the bridge.
    Design your program so that you can create several threads representing
    the northbound and southbound farmers.

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/wait.h>

void waitS(int *s)
{
    *s = *s - 1;
}

void signalS(int *s)
{
    *s = *s + 1;
}

int main()
{
    // A semaphore to lock the bridge.
    int bridge = 0;
    // A semaphore to lock the northbound queue.
    int northbound = 0;
    // A semaphore to lock the southbound queue.
    int southbound = 0;
    pid_t pid = fork();
    if (pid == 0)
    {
        // Northbound farmer
        int ct = 0;
        pid_t northboundFarmers[5];
        for (int i = 0; i < 5; i++)
        {
            northboundFarmers[i] = fork();
            if (northboundFarmers[i] == 0)
            {
                sleep(3); // Waiting for all reader processes to be created
                do
                {
                    waitS(&northbound);
                    ct++;
                    if (ct == 1)
                    {
                        waitS(&bridge);
                    }
                    signalS(&northbound);
                    printf("Northbound farmer %d is on the bridge\n", i);
                    printf("There are currently %d farmers on the bridge\n", ct);
                    sleep((rand() % 5)+2);
                    printf("Northbound farmer %d has crossed the bridge\n", i);
                    waitS(&northbound);
                    ct--;
                    if (ct == 0)
                    {
                        signalS(&bridge);
                    }
                    signalS(&northbound);

                } while (1);
                exit(0);
            }
        }
    }
    else
    {
        // Southbound farmer
        int ct = 0;
        pid_t southboundFarmers[5];
        for (int i = 0; i < 5; i++)
        {
            southboundFarmers[i] = fork();
            if (southboundFarmers[i] == 0)
            {
                sleep(3); // Waiting for all reader processes to be created
                do
                {
                    waitS(&southbound);
                    ct++;
                    if (ct == 1)
                    {
                        waitS(&bridge);
                    }
                    signalS(&southbound);
                    printf("Southbound farmer %d is on the bridge\n", i);
                    printf("There are currently %d farmers on the bridge\n", ct);
                    sleep((rand() % 5)+2);
                    printf("Southbound farmer %d has crossed the bridge\n", i);
                    waitS(&southbound);
                    ct--;
                    if (ct == 0)
                    {
                        signalS(&bridge);
                    }
                    signalS(&southbound);

                } while (1);
                exit(0);
            }
        }
    }
    return 0;
}