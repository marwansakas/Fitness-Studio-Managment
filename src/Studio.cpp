#include "../include/Studio.h"

// empty constructor
Studio::Studio() : open(false), numOfTrainers(0),numOfCustomers(0), trainers(), workout_options(), actionsLog() {}

// default constructor
Studio::Studio(const std::string &configFilePath) : open(false), numOfTrainers(0),numOfCustomers(0), trainers(), workout_options(),
                                                    actionsLog() {
    std::ifstream inFile;
    inFile.open(configFilePath);
    if (!inFile) {
        std::cerr << "Unable to open file datafile.txt";
        exit(1);
    }
    std::string line;
    int count = 0;
    int currentID = 0;
    while (getline(inFile, line)) {
        if (!(line.empty()) && line.at(0) != '#') {
            if (count == 0) {
                numOfTrainers = std::stoi(line);
            } else {
                if (count == 1) {
                    std::queue <std::string> capacities;
                    splitString(line, capacities, ',');
                    while (!capacities.empty()) {
                        trainers.push_back(new Trainer(std::stoi(capacities.front())));
                        capacities.pop();
                    }
                } else {
                    std::queue <std::string> workouts;
                    splitString(line, workouts, ',');
                    std::string name = workouts.front();
                    workouts.pop();
                    std::string type = workouts.front();
                    workouts.pop();
                    int price = std::stoi(workouts.front());
                    workouts.pop();
                    if (type == " Anaerobic"){
                        workout_options.emplace_back(currentID, name, price, ANAEROBIC);
                    }
                    else if (type == " Mixed") {
                        workout_options.emplace_back(currentID, name, price, MIXED);
                    } else if(type==" Cardio")
                        workout_options.emplace_back(currentID, name, price, CARDIO);
                    else{
                        // this line shouldn't be reached
                        exit(1);
                    }
                    currentID++;
                }
            }
            count++;
        }
    }
    inFile.close();
}

// copy constructor
Studio::Studio(Studio &other) : open(other.open), numOfTrainers(other.numOfTrainers),
                                numOfCustomers(other.numOfCustomers), trainers(),workout_options(), actionsLog() {
    this->copy(other);
}

// move constructor
Studio::Studio(Studio &&other): open(other.open) , numOfTrainers(other.numOfTrainers),numOfCustomers(other.numOfCustomers)
        , trainers() , workout_options() , actionsLog() {
    size_t size = other.trainers.size();
    for(size_t i = 0 ; i < size ; i++){
        this->trainers.push_back(other.trainers[i]);
        other.trainers[i] = nullptr;
    }
    size = other.actionsLog.size();
    for(size_t j = 0 ; j < size ; j++){
        this->actionsLog.push_back(other.actionsLog[j]);
        other.actionsLog[j] = nullptr;
    }
    size = other.workout_options.size();
    for(size_t k = 0 ; k < size ; k++){
        this->workout_options.push_back(other.workout_options[k]);
    }
}

// assignment operator
Studio& Studio::operator=(const Studio &other) {
    if(this == &other){
        return *this;
    }
    this->clear();
    this->open = other.open;
    this->numOfTrainers = other.numOfTrainers;
    this->copy(other);
    return *this;
}

// move assignment operator
Studio& Studio::operator=(Studio &&other){
    if(this == &other){
        return *this;
    }
    this->clear();
    this->numOfTrainers = other.numOfTrainers;
    this->numOfCustomers = other.numOfCustomers;
    this->open = other.open;
    size_t size = other.trainers.size();
    for(size_t i = 0 ; i < size ; i++){
        this->trainers.push_back(other.trainers[i]);
        other.trainers[i] = nullptr;
    }
    size = other.actionsLog.size();
    for(size_t j = 0 ; j < size ; j++){
        this->actionsLog.push_back(other.actionsLog[j]);
        other.actionsLog[j] = nullptr;
    }
    size = other.workout_options.size();
    for(size_t k = 0 ; k < size ; k++){
        this->workout_options.push_back(other.workout_options[k]);
    }
    return *this;
}

// destructor
Studio::~Studio() {
    this->clear();
}

// the main function ( start the studio )
void Studio::start() {
    std::cout << "studio is now open" << std::endl;
    std::queue <std::string>* input = new std::queue<std::string>();
    std::string line;
    this->open = true;
    while (open) {
        getline(std::cin, line);
        splitString(line, *input, ' ');
        if (input->front() == "closeall") {
            input->pop();
            performCloseAll();
        } else if (input->front() == "open") {
            input->pop();
            int trainerID = std::stoi(input->front());
            input->pop();
            performOpenTrainer(trainerID,input);
        } else if (input->front() == "order") {
            input->pop();
            int id = std::stoi(input->front());
            input->pop();
            performOrder(id);
        } else if (input->front() == "move") {
            input->pop();
            int src = std::stoi(input->front());
            input->pop();
            int dst = std::stoi(input->front());
            input->pop();
            int customer_id = std::stoi(input->front());
            input->pop();
            performMoveCustomer(src,dst,customer_id);
        } else if (input->front() == "close"){
            input->pop();
            int id = std::stoi(input->front());
            input->pop();
            performClose(id);
        } else if (input->front() == "workout_options") {
            input->pop();
            performPrintWorkoutOptions();
        } else if (input->front() == "status") {
            input->pop();
            int trainer_id = std::stoi(input->front());
            input->pop();
            performPrintTrainerStatus(trainer_id);
        } else if (input->front() == "log") {
            input->pop();
            performPrintActionsLog();
        } else if (input->front() == "backup") {
            input->pop();
            performBackupStudio();
        } else if (input->front() == "restore") {
            input->pop();
            performRestoreStudio();
        }
    }
    delete(input);
}

// getters functions
const std::vector<BaseAction*> &Studio::getActionsLog() const {
    return actionsLog;
}

std::vector<Workout>& Studio::getWorkoutOptions() {
    return workout_options;
}

Trainer* Studio::getTrainer(int tid) {
    if (tid >= this->numOfTrainers) {
        return nullptr;
    }
    return trainers[tid];
}

int Studio::getNumOfTrainers() const {
    return numOfTrainers;
}

// each function describes the action to be performed, these functions are called by the start function
// to perform the requested action provided by the user
void Studio::performOpenTrainer(int trainer_id , std::queue <std::string>* input){
    std::vector <Customer*> allCustomers;
    int trainer_capacity=-1;
    if(trainer_id <numOfTrainers && trainer_id>=0)
        trainer_capacity = this->getTrainer(trainer_id)->getCapacity();
    while ((!input->empty()) && (trainer_capacity > 0)) {
        std::queue <std::string> customer_details;
        splitString(input->front(), customer_details, ',');
        input->pop();
        std::string name = customer_details.front();
        customer_details.pop();
        std::string type = customer_details.front();
        customer_details.pop();
        if (type == "swt") {
            SweatyCustomer* customer = new SweatyCustomer(name,numOfCustomers);
            allCustomers.push_back(customer);
            customer = nullptr;
            numOfCustomers++;
        } else if (type == "fbd") {
            FullBodyCustomer* customer = new FullBodyCustomer(name,numOfCustomers);
            allCustomers.push_back(customer);
            customer = nullptr;
            numOfCustomers++;
        } else if (type == "chp") {
            CheapCustomer* customer = new CheapCustomer(name,numOfCustomers);
            allCustomers.push_back(customer);
            customer = nullptr;
            numOfCustomers++;
        } else if (type == "mcl") {
            HeavyMuscleCustomer* customer = new HeavyMuscleCustomer(name,numOfCustomers);
            allCustomers.push_back(customer);
            customer = nullptr;
            numOfCustomers++;
        }
        trainer_capacity--;
    }
    while(!input->empty()){
        input->pop();
    }
    OpenTrainer* action = new OpenTrainer(trainer_id, allCustomers);
    action->act(*this);
    actionsLog.push_back(action);
    action = nullptr;
    size_t size = allCustomers.size();
    // this should happen nonetheless but to make sure that unexpected things happen
    for(size_t i = 0 ; i < size ; i++){
        delete(allCustomers.at(i));
    }
    allCustomers.clear();
    delete action;
}

void Studio::performClose(int id){
    Close* action = new Close(id);
    action->act(*this);
    actionsLog.push_back(action);
    action = nullptr;
    delete action;
}

void Studio::performCloseAll(){
    CloseAll* action = new CloseAll();
    action->act(*this);
    open = false;
    delete action;
}

void Studio::performPrintWorkoutOptions(){
    PrintWorkoutOptions* action = new PrintWorkoutOptions();
    action->act(*this);
    actionsLog.push_back(action);
    action = nullptr;
    delete action;
}

void Studio::performMoveCustomer(int src,int dst,int customer_id){
    MoveCustomer* action = new MoveCustomer(src, dst, customer_id);
    action->act(*this);
    actionsLog.push_back(action);
    action = nullptr;
    delete action;
}

void Studio::performPrintTrainerStatus(int id){
    PrintTrainerStatus* action = new PrintTrainerStatus(id);
    action->act(*this);
    actionsLog.push_back(action);
    action = nullptr;
    delete action;
}

void Studio::performOrder(int id){
    Order* action = new Order(id);
    action->act(*this);
    actionsLog.push_back(action);
    action = nullptr;
    delete action;
}

void Studio::performPrintActionsLog(){
    PrintActionsLog *action = new PrintActionsLog();
    action->act(*this);
    actionsLog.push_back(action);
    action = nullptr;
    delete action;
}

void Studio::performBackupStudio(){
    BackupStudio* action = new BackupStudio();
    action->act(*this);
    actionsLog.push_back(action);
    action = nullptr;
    delete action;
}

void Studio::performRestoreStudio(){
    RestoreStudio* action = new RestoreStudio();
    action->act(*this);
    actionsLog.push_back(action);
    action = nullptr;
    delete action;
}

// these are clear and copy functions implemented to prevent code duplications among constructors and destructors
void Studio::clear() {
    size_t  size = this->trainers.size();
    for(size_t i = 0; i < size; i++){
        delete(trainers[i]);
    }
    size = this->actionsLog.size();
    for(size_t j = 0 ; j < size; j++){
        delete(actionsLog[j]);
    }
    trainers.clear();
    workout_options.clear();
    actionsLog.clear();
    numOfTrainers = 0;
    numOfCustomers = 0;
    open = false;
}

void Studio::copy(const Studio &other){
    if(this == &other){
        return;
    }
    this->clear();
    this->open = other.open;
    this->numOfTrainers = other.numOfTrainers;
    this->numOfCustomers = other.numOfCustomers;
    size_t size = other.workout_options.size();
    ActionType type;
    for(int i = 0; i < other.numOfTrainers; i++){
        Trainer* insert = new Trainer(*(other.trainers[i]));
        this->trainers.push_back(insert);
        insert = nullptr;
        delete insert;
    }
    for(size_t j = 0; j < size; j++){
        Workout insert = Workout(other.workout_options[j]);
        this->workout_options.push_back(insert);
    }
    size = other.actionsLog.size();
    for(size_t k = 0 ; k < size; k++){
        type = other.actionsLog[k]->getType();
        switch (type) {
            case openTrainer:{
                OpenTrainer* insert = new OpenTrainer(*((OpenTrainer*)other.actionsLog[k]));
                this->actionsLog.push_back(insert);
                insert = nullptr;
                delete insert;
                break;
            }
            case order:{
                Order* insert = new Order(*((Order*)other.actionsLog[k]));
                this->actionsLog.push_back(insert);
                insert = nullptr;
                delete insert;
                break;
            }
            case moveCustomer:{
                MoveCustomer* insert = new MoveCustomer(*((MoveCustomer*)other.actionsLog[k]));
                this->actionsLog.push_back(insert);
                insert = nullptr;
                delete insert;
                break;
            }
            case close:{
                Close* insert = new Close(*((Close*)other.actionsLog[k]));
                this->actionsLog.push_back(insert);
                insert = nullptr;
                delete insert;
                break;
            }
            case closeAll:{
                CloseAll* insert = new CloseAll(*((CloseAll*)other.actionsLog[k]));
                this->actionsLog.push_back(insert);
                insert = nullptr;
                delete insert;
                break;
            }
            case printWorkoutOptions:{
                PrintWorkoutOptions* insert = new PrintWorkoutOptions(*((PrintWorkoutOptions*)other.actionsLog[k]));
                this->actionsLog.push_back(insert);
                insert = nullptr;
                delete insert;
                break;
            }
            case printTrainerStatus:{
                PrintTrainerStatus* insert = new PrintTrainerStatus(*((PrintTrainerStatus*)other.actionsLog[k]));
                this->actionsLog.push_back(insert);
                insert = nullptr;
                delete insert;
                break;
            }
            case backupStudio:{
                BackupStudio* insert = new BackupStudio(*((BackupStudio*)other.actionsLog[k]));
                this->actionsLog.push_back(insert);
                insert = nullptr;
                delete insert;
                break;
            }
            case printActionsLog:{
                PrintActionsLog* insert = new PrintActionsLog(*((PrintActionsLog*)other.actionsLog[k]));
                this->actionsLog.push_back(insert);
                insert = nullptr;
                delete insert;
                break;
            }
            case restoreStudio:{
                RestoreStudio* insert = new RestoreStudio(*((RestoreStudio*)other.actionsLog[k]));
                this->actionsLog.push_back(insert);
                insert = nullptr;
                delete insert;
            }
        }
    }
}

// this function splits a given string (s) on the basis of a given char ( c ) , and puts the sub-strings in a queue
void Studio::splitString(std::string s, std::queue <std::string> &queue, char c) {
    std::string temp;
    for (char i: s) {
        if (i == c) {
            queue.push(temp);
            temp = "";
        } else {
            temp.push_back(i);
        }
    }
    queue.push(temp);
}