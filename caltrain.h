#include <pthread.h>

struct station {
    int emptySeats;              //empty seats on train
    int waitingPassengers;       //waiting on the station
    int onBoardPassengers;       //standing inside the train
    pthread_mutex_t trainLock;
    pthread_cond_t trainArrived;
    pthread_cond_t allPassengersSit;};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);