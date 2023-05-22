#include "../include/Action.h"

extern Studio* backup;
// all the default constructors
BaseAction::BaseAction() : errorMsg(""),status(ERROR),type(){}

OpenTrainer::OpenTrainer(int id, std::vector<Customer*> &customersList) : trainerId(id),customers(){
    error("Trainer does not Exist or is already open");
    this->setType(openTrainer);
    size_t size = customersList.size();
    for(size_t i = 0 ; i < size ; i++){
        if (customersList[i]->toString() == customersList[i]->getName() + ",swt") {
            SweatyCustomer* toAdd = new SweatyCustomer(*((SweatyCustomer*)customersList[i]));
            this->customers.push_back(toAdd);
            toAdd = nullptr;
        } else if (customersList[i]->toString() == customersList[i]->getName() + ",chp") {
            CheapCustomer* toAdd = new CheapCustomer(*((CheapCustomer*)customersList[i]));
            this->customers.push_back(toAdd);
            toAdd = nullptr;
        } else if (customersList[i]->toString() == customersList[i]->getName() + ",mcl") {
            HeavyMuscleCustomer* toAdd = new HeavyMuscleCustomer(*((HeavyMuscleCustomer*)customersList[i]));
            this->customers.push_back(toAdd);
            toAdd = nullptr;
        } else if (customersList[i]->toString() == customersList[i]->getName() + ",fbd") {
            FullBodyCustomer* toAdd = new FullBodyCustomer(*((FullBodyCustomer*)customersList[i]));
            this->customers.push_back(toAdd);
            toAdd = nullptr;
        }
    }
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) {
    error("cannot move customer");
    this->setType(moveCustomer);
}

Order::Order(int id) : trainerId(id) {
    error("Trainer does not exist or is not open");
    this->setType(order);
}

Close::Close(int id) : trainerId(id) {
    error("Trainer does not exist or is not open");
    this->setType(close);
}

CloseAll::CloseAll() {
    error("");
    this->setType(closeAll);
}

PrintWorkoutOptions::PrintWorkoutOptions() {
    error("");
    this->setType(printWorkoutOptions);
}

PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id) {
    error("");
    this->setType(printTrainerStatus);
}

PrintActionsLog::PrintActionsLog() {
    error("");
    this->setType(printActionsLog);
}

BackupStudio::BackupStudio() {
    error("");
    this->setType(backupStudio);
}

RestoreStudio::RestoreStudio() {
    error("");
    this->setType(restoreStudio);
}

// copy constructors
BaseAction::BaseAction(const BaseAction &other):errorMsg(other.errorMsg),status(other.status),type(other.type){}

OpenTrainer::OpenTrainer(const OpenTrainer &other): BaseAction(other) , trainerId(other.trainerId),customers() {
    size_t size = other.customers.size();
    for(size_t i = 0 ; i < size ; i++){
        if (other.customers[i]->toString() == other.customers[i]->getName() + ",swt") {
            SweatyCustomer* toAdd = new SweatyCustomer(other.customers[i]->getName(), other.customers[i]->getId());
            this->customers.push_back(toAdd);
            toAdd = nullptr;
            delete toAdd;
        } else if (other.customers[i]->toString() == other.customers[i]->getName() + ",chp") {
            CheapCustomer* toAdd = new CheapCustomer(other.customers[i]->getName(), other.customers[i]->getId());
            this->customers.push_back(toAdd);
            toAdd = nullptr;
            delete toAdd;
        } else if (other.customers[i]->toString() == other.customers[i]->getName() + ",mcl") {
            HeavyMuscleCustomer* toAdd = new HeavyMuscleCustomer(other.customers[i]->getName(), other.customers[i]->getId());
            this->customers.push_back(toAdd);
            toAdd = nullptr;
            delete toAdd;
        } else if (other.customers[i]->toString() == other.customers[i]->getName() + ",fbd") {
            FullBodyCustomer* toAdd = new FullBodyCustomer(other.customers[i]->getName(),other.customers[i]->getId());
            this->customers.push_back(toAdd);
            toAdd = nullptr;
            delete toAdd;
        }
    }
}

MoveCustomer::MoveCustomer(const MoveCustomer &other):BaseAction(other),
                                                      srcTrainer(other.srcTrainer),dstTrainer(other.dstTrainer),id(other.id){
    this->setType(moveCustomer);
}

Order::Order(const Order &other):BaseAction(other) , trainerId(other.trainerId){
    this->setType(order);
}

Close::Close(const Close &other):BaseAction(other),trainerId(other.trainerId) {
    this->setType(close);
}

CloseAll::CloseAll(const CloseAll &other):BaseAction(other){
    this->setType(closeAll);
}

PrintWorkoutOptions::PrintWorkoutOptions(const PrintWorkoutOptions &other):BaseAction(other){
    this->setType(printWorkoutOptions);
}

PrintTrainerStatus::PrintTrainerStatus(const PrintTrainerStatus &other):BaseAction(other),trainerId(other.trainerId){
    this->setType(printTrainerStatus);
}

PrintActionsLog::PrintActionsLog(const PrintActionsLog &other):BaseAction(other){
    this->setType(printActionsLog);
}

BackupStudio::BackupStudio(const BackupStudio &other):BaseAction(other){
    this->setType(backupStudio);
}

RestoreStudio::RestoreStudio(const RestoreStudio &other):BaseAction(other){
    this->setType(restoreStudio);
}

// getters functions
ActionType BaseAction::getType() const{
    return this->type;
}

ActionStatus BaseAction::getStatus() const {
    return status;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

// change status functions
void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    this->errorMsg = "Error: "+errorMsg;
    status = ERROR;
}

// set type function
void BaseAction::setType(ActionType to_set){
    this->type = to_set;
}

// act functions
void OpenTrainer::act(Studio &studio) {
    Trainer* temp = studio.getTrainer(trainerId);
    if (temp == nullptr) {
        this->error("Trainer does not Exist or is already open");
        std::cout << getErrorMsg() << std::endl;
        return;
    }
    if (temp->isOpen()) {
        this->error("Trainer does not Exist or is already open");
        std::cout << getErrorMsg() << std::endl;
        return;
    }
    temp->openTrainer();
    std::vector<Customer*>::iterator it;
    int count = 0;
    int capacity = temp->getCapacity();
    for (it = customers.begin(); it != customers.end() && count < capacity; it++) {
        temp->addCustomer(it.operator*());
        count++;
    }

    complete();
}

void MoveCustomer::act(Studio &studio) {//needs to be checked
    if (srcTrainer == dstTrainer) {
        error("cannot move customer");
        std::cout << getErrorMsg() << std::endl;
        return;
    }
    Trainer* src = studio.getTrainer(srcTrainer);
    if (src == nullptr) {
        error("cannot move customer");
        std::cout << getErrorMsg() << std::endl;
        return;
    }
    Trainer* dst = studio.getTrainer(dstTrainer);
    if (dst == nullptr) {
        error("cannot move customer");
        std::cout << getErrorMsg() << std::endl;
        return;
    }
    if (dst->getCustomers().size() == (size_t) dst->getCapacity()) {
        error("cannot move customer");
        std::cout << getErrorMsg() << std::endl;
        return;
    }
    if((!dst->isOpen()) || (!src->isOpen())){
        std::cout << getErrorMsg() << std::endl;
        error("cannot move customer");
        return;
    }
    Customer* toMove = src->getCustomer(id);
    if(toMove == nullptr){
        std::cout << getErrorMsg() << std::endl;
        error("cannot move customer");
        return;
    }
    if (toMove->toString() == toMove->getName() + ",swt") {
        SweatyCustomer *toAdd = new SweatyCustomer(toMove->getName(), id);
        dst->addCustomer(toAdd);
        dst->order(toAdd->getId(), toAdd->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
        delete toAdd;
    } else if (toMove->toString() == toMove->getName() + ",chp") {
        CheapCustomer *toAdd = new CheapCustomer(toMove->getName(), id);
        dst->addCustomer(toAdd);
        dst->order(toAdd->getId(), toAdd->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
        delete toAdd;
    } else if (toMove->toString() == toMove->getName() + ",mcl") {
        HeavyMuscleCustomer *toAdd = new HeavyMuscleCustomer(toMove->getName(), id);
        dst->addCustomer(toAdd);
        dst->order(toAdd->getId(), toAdd->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
        delete toAdd;
    } else if (toMove->toString() == toMove->getName() + ",fbd") {
        FullBodyCustomer *toAdd = new FullBodyCustomer(toMove->getName(), id);
        dst->addCustomer(toAdd);
        dst->order(toAdd->getId(), toAdd->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
        delete toAdd;
    }
    src->removeCustomer(toMove->getId());
    complete();
}

void Order::act(Studio &studio) {
    Trainer* temp = studio.getTrainer(this->trainerId);
    if (temp == nullptr) {
        delete (temp);
        this->error("Trainer does not exist or is not open");
        std::cout << getErrorMsg() << std::endl;
        return;
    }
    if (!temp->isOpen()) {
        temp = nullptr;
        delete (temp);
        this->error("Trainer does not exist or is not open");
        std::cout << getErrorMsg() << std::endl;
        return;
    }
    int customer_id;
    std::vector<Workout> workout_options = studio.getWorkoutOptions();
    std::vector<int> workoutOrder ;
    std::vector<Customer*> customers = temp->getCustomers();
    size_t num_of_customers = customers.size();
    for (size_t i = 0; i < num_of_customers; i++) {
        workoutOrder= customers[i]->order(workout_options);
        customer_id = customers[i]->getId();
        temp->order(customer_id, customers[i]->order(studio.getWorkoutOptions()),
                    studio.getWorkoutOptions());
        for(int j : workoutOrder){
            std::cout << customers[i]->getName() + " Is Doing " + workout_options[j].getName() << std::endl;
        }
    }
    for(size_t j = 0; j < num_of_customers ; j++){
        customers[j] = nullptr;
    }
    complete();
}

void Close::act(Studio &studio) {
    Trainer* temp = studio.getTrainer(this->trainerId);
    if (!temp) {
        this->error("Trainer does not exist or is not open");
        std::cout << getErrorMsg() << std::endl;
        return;
    }
    if (!temp->isOpen()) {
        this->error("Trainer does not exist or is not open");
        std::cout << getErrorMsg() << std::endl;
        return;
    }
    temp->closeTrainer();
    int salary = temp->getSalary();
    std::string output = "Trainer " + std::to_string(this->trainerId) +
                         " closed. Salary " + std::to_string(salary) + "NIS";
    std::cout << output << std::endl;
    temp = nullptr;
    complete();
}

void CloseAll::act(Studio &studio) {
    int numOfTrainers = studio.getNumOfTrainers();
    for (int i = 0; i < numOfTrainers; i++) {
        Trainer *temp = studio.getTrainer(i);
        if (temp->isOpen()) {
            Close* close = new Close(i);
            close->act(studio);
            delete close;
        }
    }
    complete();
}

void PrintWorkoutOptions::act(Studio &studio) {
    std::vector <Workout> &workoutOp = studio.getWorkoutOptions();
    for (Workout t: workoutOp) {
        std::string tType = "Mixed";
        if (t.getType() == ANAEROBIC) {
            tType = "Anaerobic";
        } else {
            if (t.getType() == CARDIO) {
                tType = "Cardio";
            }
        }
        std::cout << t.getName() + ", " + tType + ", " + std::to_string(t.getPrice()) << std::endl;
    }
    complete();
}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer* t = studio.getTrainer(trainerId);
    if (t->isOpen()) {
        std::cout << "Trainer " + std::to_string(trainerId) + " status: open" << std::endl;
        std::cout << "Customers:" << std::endl;
        std::vector < Customer * > allCustomers = t->getCustomers();
        for (Customer *customer: allCustomers) {
            std::cout << std::to_string(customer->getId()) + " " + customer->getName() << std::endl;
        }
        std::cout << "Orders:" << std::endl;
        std::vector <OrderPair> orderList = t->getOrders();
        for (OrderPair order: orderList) {
            std::cout << order.second.getName() + " " + std::to_string(order.second.getPrice()) + "NIS " +
                         std::to_string(order.first) << std::endl;
        }
        std::cout << "Current Trainer's Salary: " + std::to_string(t->getSalary()) + "NIS" << std::endl;
    } else {
        std::cout << "Trainer " + std::to_string(trainerId) + " status: closed" << std::endl;
    }
    complete();
}

void PrintActionsLog::act(Studio &studio) {
    std::vector <BaseAction*> temp = studio.getActionsLog();
    for (BaseAction* log: temp) {
        std::cout << log->toString() << std::endl;
    }
    size_t size = temp.size();
    for(size_t i = 0 ; i < size ; i++){
        temp[i] = nullptr;
    }
    complete();
}

void BackupStudio::act(Studio &studio) {
    backup = new Studio(studio);
    complete();
}

void RestoreStudio::act(Studio &studio) {
    if(backup == nullptr){
        error("No backup available");
        std::cout << getErrorMsg() << std::endl;
    }else{
        studio = *backup;
        complete();
    }
}


std::string OpenTrainer::toString() const {
    std::string log = "open " + std::to_string(trainerId);
    size_t i = 0 ;
    size_t size = customers.size();
    for (Customer* cust: customers) {
        if(i < size) {
            log += " " + cust->toString();
            i++;
        }
    }
    if (getStatus() == ERROR) {
        log += " " + getErrorMsg();
    }else{
        log += " Completed";
    }
    return log;
}

std::string MoveCustomer::toString() const {
    std::string log =
            "move " + std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) + " " + std::to_string(id);
    if (getStatus() == ERROR){
        log += " " + getErrorMsg();
    }else{
        log += " Completed";
    }
    return log;
}

std::string Order::toString() const {
    std::string log = "order " + std::to_string(trainerId);
    if (getStatus() == ERROR) {
        log += " " + getErrorMsg();
    }else{
        log += " Completed";
    }
    return log;
}

std::string Close::toString() const {
    std::string log = "close " + std::to_string(trainerId);
    if (getStatus() == ERROR) {
        log += " " + getErrorMsg();
    }else{
        log += " Completed";
    }
    return log;
}

std::string CloseAll::toString() const {
    return "closeall Completed";
}

std::string PrintWorkoutOptions::toString() const {
    return "workout_options Completed";
}

std::string PrintTrainerStatus::toString() const {
    return "status " + std::to_string(this->trainerId) + " Completed";
}

std::string PrintActionsLog::toString() const {
    return "log Completed";
}

std::string BackupStudio::toString() const {
    return "backup Completed";
}

std::string RestoreStudio::toString() const {
    if(this->getStatus() == ERROR){
        return "restore " + this->getErrorMsg();
    }
    return "restore Completed";
}

// destructors
BaseAction::~BaseAction(){}

OpenTrainer::~OpenTrainer() {
    size_t size = this->customers.size();
    for(size_t i = 0 ; i < size ; i++){
        delete(customers[i]);
    }
}

Order::~Order() {}

MoveCustomer::~MoveCustomer() {}

RestoreStudio::~RestoreStudio() {}

BackupStudio::~BackupStudio() {}

PrintActionsLog::~PrintActionsLog() {}

PrintTrainerStatus::~PrintTrainerStatus() {}

PrintWorkoutOptions::~PrintWorkoutOptions() {}

Close::~Close() {}

CloseAll::~CloseAll() {}