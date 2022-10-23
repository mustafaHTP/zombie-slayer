/**
 * @file zombieSlayer.c
 * @author Mustafa Hatipoğlu
 *         18120205010
 * @brief Zombie Slayer Simulator
 * @version 0.1
 * @date 2022-04-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <unistd.h> /* for sleep in Linux */
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define NUM_OF_SLAYER 3
#define LIMIT_OF_DOORMAN 100

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; 

int zombie_counter = 0; /*holds number of zombies in the room*/
int zombie_killed_counter = 0; /*holds number of zombies killed*/

/**
 * @brief Keeps track of number of zombies entered
 * 
 */
void zombieEntered(){
    ++zombie_counter;
}

/**
 * @brief Keeps track of number of zombies killed
 * 
 */
void zombiesKilled(){
    ++zombie_killed_counter;
    --zombie_counter;
}

/**
 * @brief Returns true, If the number of zombies in the room
 * are greater than or equal to 100
 * 
 */
int tooManyZombiesInTheRoom(){
    if(zombie_counter >= 100)
        return 1;
    return 0;    
}

/**
 * @brief Returns true, If more than 100 zombies 
 * have been killed
 * 
 */
int killed100Zombies(){
    if(zombie_killed_counter >= 100)
        return 1;
    return 0;    
}

/**
 * @brief Returns true, If there are at least 
 * one zombie in the room 
 * 
 */
int zombiesExist(){
    if(zombie_counter >= 1)
        return 1;
    return 0;    
}

/**
 * @brief Returns the number of zombies killed
 * 
 */
int getKilledCount(){
    return zombie_killed_counter;
}

/**
 * @brief Returns the number of zombies in the room
 * 
 */
int getInTheRoomCount(){
    return zombie_counter;
}

/**
 * @brief Each doorman thread has a 50% chance of allowing a zombie 
 * to enter every 2ms. When the zombie enters, you need to update 
 * the zombie_counter by calling the necessary functions. 
 * If there are too many zombies, that is 100 or more, 
 * the doorman thread stops running.
 * 
 * @return void* number of zombies that doorman allow enter 
 */
void* doorMan(void* tid){

    int* result = malloc(sizeof(int));
    if(result == NULL){
        perror("malloc: ");
        exit(EXIT_FAILURE);
    }

    int enter_counter = 0;
    while(!(tooManyZombiesInTheRoom() || killed100Zombies())){
        int openDoorOrNot = rand() % 2; /*%50  probability*/
        if(openDoorOrNot){
            pthread_mutex_lock(&m);
            zombieEntered();
            pthread_mutex_unlock(&m);
            ++enter_counter;
        }
        sleep(0.002);
    }

    *result = enter_counter;
    return (void*)result;
}

/**
 * @brief Slayer thread kills a zombie every 2ms. 
 * It updates the number of zombie_killed_counter and zombie_counter 
 * by calling the appropriate functions.
 * If there are too many zombies, 
 * this thread ends its work.
 * 
 * @return void* number of zombies that slayers has killed
 */
void* slayer(void* tid){

    int* result = malloc(sizeof(int));
    if(result == NULL){
        perror("malloc: ");
        exit(EXIT_FAILURE);
    }

    int score = 0;
    while(!(tooManyZombiesInTheRoom() || killed100Zombies())){
        if(zombiesExist()){
            pthread_mutex_lock(&m);
            zombiesKilled();
            pthread_mutex_unlock(&m);
            ++score;
        }
        sleep(0.002);
    }

    *result = score;
    return (void*)result;
}

int getDoormanNumberFromArgs(char* arg){
    char copyOfArg[LIMIT_OF_DOORMAN];
    strcpy(copyOfArg, arg);
    int numOfDoorman = atoi(copyOfArg);

    return numOfDoorman;
}
/**
 * @brief This function creates n Doorman threads
 * (n command line argument) and 3 slayer threads. 
 * When the execution of all threads is finished,
 * whether 100 zombies were killed or not 
 * and it prints how many zombies each slayer has killed
 * 
 */

int main(int argc, char* args[]){
    srand(time(NULL)); /*for doorman probability*/
    pthread_t slayers[NUM_OF_SLAYER];

    /*get number of doorman from command line*/
    /*first check if the argument is given*/
    if(argc != 2){
        printf("Error: %s\n", strerror(EINVAL));
        printf("Only 1 argument can be given.\n");
        printf("Example: ./zombieSlayer 10\n");
        return -EINVAL;
    }

    int numOfDoorman = getDoormanNumberFromArgs(args[1]);
    const int NUM_OF_DOORMAN = numOfDoorman;
    pthread_t* doormans = malloc(sizeof(pthread_t) * NUM_OF_DOORMAN);


    /*init slayers*/
    for (int i = 0; i < NUM_OF_SLAYER; i++){
        int r = pthread_create(&slayers[i], NULL, slayer, NULL);
        if(r != 0){
            perror("pthread_create: ");
            exit(EXIT_FAILURE);
        }
    }
    
    /*init doormans*/
    for (int i = 0; i < NUM_OF_DOORMAN ; i++){
        int r = pthread_create(&doormans[i], NULL, doorMan, NULL);
        if(r != 0){
            perror("pthread_create: ");
            exit(EXIT_FAILURE);
        }
    }

    /*wait slayers*/
    int kills[NUM_OF_SLAYER];
    int* kill;
    for (int i = 0; i < NUM_OF_SLAYER; i++)
    {
        pthread_join(slayers[i], (void**)&kill);
        printf("slayer %d kills %d zombie\n", i, *kill);
        free(kill);
    }
    
    /*wait doormans*/
    int enters[NUM_OF_DOORMAN];
    int* enter;
    for (int i = 0; i < NUM_OF_DOORMAN; i++)
    {
        pthread_join(doormans[i], (void**)&enter);
        printf("doorman %d allows enter %d zombie\n", i, *enter);
        free(enter);
    }

    free(doormans);
    pthread_mutex_destroy(&m);

    printf("zombies in the room : %d\n", zombie_counter);
    printf("zombies get killed : %d\n", zombie_killed_counter);
    /*CHECK SUCCESS OR FAILURES FOR SLAYERS*/
    if(tooManyZombiesInTheRoom()){
        printf("Everbody died...:( \n");
    }
    if(killed100Zombies()){
        printf("Everbody is secure... :) \n");
    }

    return 0;
}