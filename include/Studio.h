#ifndef STUDIO_H_
#define STUDIO_H_

#include "Trainer.h"
#include "Action.h"
#include "queue"
#include <fstream>

class BaseAction;

class Studio {
public:
    // empty constructor
    Studio();
    // default constructor
    Studio(const std::string &configFilePath);
    // main function (start the studio)
    void start();
    //destructor
    virtual ~Studio();
    //copy constructor
    Studio(Studio &other);
    //move constructor
    Studio(Studio &&other);//done
    //copy assignment operator
    Studio& operator=(const Studio &other);
    //move assignment operator
    Studio& operator=(Studio &&other);
    // getter functions
    int getNumOfTrainers() const;
    Trainer *getTrainer(int tid);
    const std::vector<BaseAction *> &getActionsLog() const;
    std::vector<Workout> &getWorkoutOptions();
private:
    bool open;
    int numOfTrainers;
    int numOfCustomers;
    std::vector<Trainer *> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction *> actionsLog;
    void splitString(std::string s, std::queue<std::string> &queue, char c);
    void copy(const Studio &other);
    void clear();
    void performCloseAll();
    void performOpenTrainer(int trainer_id , std::queue <std::string>* input);
    void performClose(int id);
    void performPrintWorkoutOptions();
    void performMoveCustomer(int src,int dst,int customer_id);
    void performPrintTrainerStatus(int id);
    void performOrder(int id);
    void performPrintActionsLog();
    void performBackupStudio();
    void performRestoreStudio();
};
#endif