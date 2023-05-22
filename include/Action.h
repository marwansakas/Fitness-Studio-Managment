#ifndef ACTION_H_
#define ACTION_H_

#include <iostream>
#include "Trainer.h"
#include "Studio.h"

enum ActionStatus{
    COMPLETED, ERROR
};

enum ActionType{
    openTrainer, order, moveCustomer, close, closeAll, printWorkoutOptions,
    printTrainerStatus, backupStudio, printActionsLog, restoreStudio
};

//Forward declaration
class Studio;

class BaseAction{
public:
    BaseAction();
    BaseAction(const BaseAction &other);
    ActionStatus getStatus() const;
    ActionType getType() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
    virtual ~BaseAction();
protected:
    void complete();
    void error(std::string errorMsg);
    void setType(ActionType to_set);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
    ActionType type;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer*> &customersList);
    OpenTrainer(const OpenTrainer &other);
    void act(Studio &studio);
    std::string toString() const;
    ~OpenTrainer();
private:
    const int trainerId;
    std::vector<Customer*> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    Order(const Order &other);
    void act(Studio &studio);
    std::string toString() const;
    ~Order() override;
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    MoveCustomer(const MoveCustomer &other);
    void act(Studio &studio);
    std::string toString() const;
    ~MoveCustomer() override;
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    Close(const Close &other);
    void act(Studio &studio);
    std::string toString() const;
    ~Close() override;
private:
    const int trainerId;

};


class CloseAll : public BaseAction {
public:
    CloseAll();
    CloseAll(const CloseAll &other);
    void act(Studio &studio);
    std::string toString() const;
    ~CloseAll() override;
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    PrintWorkoutOptions(const PrintWorkoutOptions &other);
    void act(Studio &studio);
    std::string toString() const;
    ~PrintWorkoutOptions() override;
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    PrintTrainerStatus(const PrintTrainerStatus &other);
    void act(Studio &studio);
    std::string toString() const;
    ~PrintTrainerStatus() override;
private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    PrintActionsLog(const PrintActionsLog &other);
    void act(Studio &studio);
    std::string toString() const;
    ~PrintActionsLog() override;
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    BackupStudio(const BackupStudio &other);
    void act(Studio &studio);
    std::string toString() const;
    ~BackupStudio() override;
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    RestoreStudio(const RestoreStudio &other);
    void act(Studio &studio);
    std::string toString() const;
    ~RestoreStudio() override;

};

#endif