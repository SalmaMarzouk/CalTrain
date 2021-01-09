#include <pthread.h>
#include "caltrain.h"



void station_init(struct station *station)
{
    station->emptySeats = 0;
    station->waitingPassengers = 0;
    station->onBoardPassengers = 0;
    pthread_mutex_init(&station->trainLock, NULL);
    pthread_cond_init(&station->trainArrived, NULL);
    pthread_cond_init(&station->allPassengersSit, NULL);}

//When a train arrives in the station and has opened its doors, it invokes this function,
// returns when the train is full , or there is no passengers waiting on the station
void station_load_train(struct station *station, int count)
{
    pthread_mutex_lock(&station->trainLock);
    station->emptySeats = count;    //inside the critical section, so no other train thread can change it at the same time
    //while used to recheck the condition after receiving leave signal
    while(station->emptySeats > 0 && station->waitingPassengers > 0){
        pthread_cond_broadcast(&station->trainArrived);      //used instead of signal to inform several waiting passengers
        pthread_cond_wait(&station->allPassengersSit , &station->trainLock);  //waiting signal to leave the station
    }
    station->emptySeats = 0;            //the train left, no seats for passengers
    pthread_mutex_unlock(&station->trainLock);
}

//When a passenger robot arrives in a station, it first invokes this function,
//return when the train is in the station, and there are enough seats
void station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->trainLock);
    station->waitingPassengers++;   //inside critical section, so no other passenger thread changes the values at the same time
    //there is no empty seats, wait for another train
    while(station->onBoardPassengers == station->emptySeats){
        pthread_cond_wait(&station->trainArrived , &station->trainLock);
    }
    //update station data before unlocking the critical section, no other thread deals with prev. values
    station->onBoardPassengers++;
    station->waitingPassengers--;
    pthread_mutex_unlock(&station->trainLock);
}

// Once the passenger is seated, it will call this function to let the train know that it’s on board.
void station_on_board(struct station *station)
{
    pthread_mutex_lock(&station->trainLock);
    // change data inside the critical section, so no other passenger thread changes it at the same time
    station->onBoardPassengers--;
    station->emptySeats--;
    pthread_mutex_unlock(&station->trainLock);
    //outside the critical section. no problem occurs if the values changed before check
    if((station->emptySeats == 0) || ((station->onBoardPassengers == 0)&& station->waitingPassengers == 0)){
        pthread_cond_signal(&station->allPassengersSit);
    }

}
