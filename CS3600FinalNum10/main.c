/* 
 * File:   main.c
 * Author: Alex Hollis
 *
 * Created on December 12, 2013, 9:48 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdbool.h>

/*
 * 
 */

sem_t ticketLock; // semaphore lock for tickets left.
void *sell_tickets(void *arg);
int numTickets;
int main(int argc, char** argv) {
    
    int agents; int tickets; int res;
        
    printf("Total number of tickets: ");
    scanf("%d",&tickets);
    printf("\nTotal number of agents: ");
    scanf("%d",&agents);
    printf("\n");
    numTickets = tickets; // Set initial number of tickets.
    pthread_t agent_thread[agents];
    sem_init(&ticketLock,0,1);
    int i=0;
    void *thread_result;
    for(i=0; i< agents; i++)
    {
      printf("In main: Initiating an agent with ID %d",i);  
      res = pthread_create(&(agent_thread[i]), NULL, sell_tickets, (void *)&i);
      sleep(1); 
    }
    for(i=agents -1; i >= 0; i--) {
        
        res = pthread_join(agent_thread[i],&thread_result);
        
    }
    printf("ALL Tickets are sold\n");
    sem_destroy(&ticketLock);
    return (EXIT_SUCCESS);
}

void *sell_tickets(void *arg) {
    
    int agentID = *(int *)arg;
    int numSold=0;
    bool done = false;
    
    while(!done){
        
    sem_wait(&ticketLock);
    if(numTickets == 0){
        
        done = true;
    }else {
        
        numTickets--;
        numSold++;
        printf("\nAgent %d sold one (%d left)\n",agentID,numTickets);
        }
    sleep(1);
    sem_post(&ticketLock);
    }
    
    printf("Agent %d noticed all tickets sold! (I sold %d myself)\n",agentID,numSold);
    pthread_exit(NULL);
}

