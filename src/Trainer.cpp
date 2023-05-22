#include "../include/Trainer.h"

// default constructor
Trainer::Trainer(int t_capacity) : salary(0),capacity(t_capacity),open(false) ,customersList(),orderList(){}
// copy constructor
Trainer::Trainer(Trainer &other): salary(other.salary),capacity(other.capacity) , open(other.open),customersList() ,orderList(){
    copy(other.capacity,other.open,other.salary,other.customersList,other.orderList);
}
// move constructor
Trainer::Trainer(Trainer &&other):salary(other.salary),capacity(other.capacity),open(other.open),customersList(),orderList(){
    size_t size = other.customersList.size();
    for(size_t i = 0 ; i < size ; i++){
        this->customersList.push_back(other.customersList[i]);
        other.customersList[i] = nullptr;
    }
    size = other.orderList.size();
    for(size_t j = 0 ; j < size ; j++){
        OrderPair temp = {other.orderList[j].first,other.orderList[j].second};
        this->orderList.push_back(temp);
    }
    other.orderList.clear();
    other.customersList.clear();
    other.open = false;
    other.capacity = 0;
}
// this function adds a customer to the trainers customers list
void Trainer::addCustomer(Customer* customer){
    if(customersList.size() == (size_t)capacity || customer == nullptr){
        return;
    }
    if (customer->toString() == customer->getName() + ",swt") {
        SweatyCustomer* toAdd = new SweatyCustomer(customer->getName(), customer->getId());
        this->customersList.push_back(toAdd);
        toAdd = nullptr;
        delete toAdd;
    } else if (customer->toString() == customer->getName() + ",chp") {
        CheapCustomer* toAdd = new CheapCustomer(customer->getName(), customer->getId());
        this->customersList.push_back(toAdd);
        toAdd = nullptr;
        delete toAdd;
    } else if (customer->toString() == customer->getName() + ",mcl") {
        HeavyMuscleCustomer* toAdd = new HeavyMuscleCustomer(customer->getName(), customer->getId());
        this->customersList.push_back(toAdd);
        toAdd = nullptr;
        delete toAdd;
    } else if (customer->toString() == customer->getName() + ",fbd") {
        FullBodyCustomer* toAdd = new FullBodyCustomer(customer->getName(),customer->getId());
        this->customersList.push_back(toAdd);
        toAdd = nullptr;
        delete toAdd;
    }
}

int Trainer::getCapacity() const {
    return capacity;
}

// searches in customer list for a customer that matches the id given, if none found it returns nullptr.
Customer* Trainer::getCustomer(int id){
    size_t size = customersList.size();
    Customer* temp;
    for(size_t i = 0 ; i < size ; i++){
        temp = customersList.at(i);
        if (temp->getId() == id){
            return temp;
        }
    }
    return nullptr;
}

// switches the trainer status from closed to open
void Trainer::openTrainer(){
    open = true;
}

// switches the trainer status from open to close and frees all his customers
void Trainer::closeTrainer(){
    open = false;
    size_t size = this->customersList.size();
    for(size_t i = 0 ; i < size ; i++){
        delete(this->customersList[i]);
    }
    this->customersList.clear();
    this->orderList.clear();
}

// checks if the trainer is open or no
bool Trainer::isOpen(){
    if(!open){
        return false;
    }
    return true;
}

// this function returns the salary for this trainer
int Trainer::getSalary(){
    return salary;
}

// this function returns a reference to the customer list
std::vector<Customer*>& Trainer::getCustomers(){
    return customersList;
}

// this function returns a reference to the orders list
std::vector<OrderPair>& Trainer::getOrders(){
    return orderList;
}
// need to implement remove a specific customer using its destructor.
void Trainer::removeCustomer(int id){
    if(!this->getCustomer(id)){
        return;
    }
    int index=0;
    size_t size = this->customersList.size();
    for(size_t i = 0; i < size ; i++){
        if(id == customersList[i]->getId()){
            delete(customersList[i]);
            customersList.erase(customersList.begin() + index);
            break;
        }
        index++;
    }
    size = this->orderList.size();
    std::vector<OrderPair> temp;
    for(size_t j = 0 ; j < size ; j++){
        if(this->orderList[j].first != id){
            temp.push_back(this->orderList[j]);
        }
        else if(this->orderList[j].first == id){
            salary -= this->orderList[j].second.getPrice();
        }
    }
    this->orderList.clear();
    if(this->customersList.empty()){
        open = false;
        return;
    }
    for(OrderPair order : temp){
        this->orderList.push_back(order);
    }
}

Trainer::~Trainer(){
    clear();
}


void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    for(int workout_id : workout_ids){
        OrderPair temp = OrderPair(customer_id,workout_options[workout_id]);
        orderList.push_back(temp);
        salary+= workout_options[workout_id].getPrice();
    }
}


void Trainer::copy(int t_capacity, bool t_open,int t_salary, const std::vector<Customer*>& t_customerList,
                   const std::vector<OrderPair>& t_orderList) {
    this->open = t_open;
    this->capacity = t_capacity;
    this->salary = t_salary;
    size_t size = t_customerList.size();
    for(size_t i = 0; i < size ; i++) {
        if (t_customerList[i]->toString() == t_customerList[i]->getName() + ",swt") {
            SweatyCustomer* toAdd = new SweatyCustomer(t_customerList[i]->getName(), t_customerList[i]->getId());
            this->customersList.push_back(toAdd);
            toAdd = nullptr;
            delete toAdd;
        } else if (t_customerList[i]->toString() == t_customerList[i]->getName() + ",chp") {
            CheapCustomer* toAdd = new CheapCustomer(t_customerList[i]->getName(), t_customerList[i]->getId());
            this->customersList.push_back(toAdd);
            toAdd = nullptr;
            delete toAdd;
        } else if (t_customerList[i]->toString() == t_customerList[i]->getName() + ",mcl") {
            HeavyMuscleCustomer* toAdd = new HeavyMuscleCustomer(t_customerList[i]->getName(), t_customerList[i]->getId());
            this->customersList.push_back(toAdd);
            toAdd = nullptr;
            delete toAdd;
        } else if (t_customerList[i]->toString() == t_customerList[i]->getName() + ",fbd") {
            FullBodyCustomer* toAdd = new FullBodyCustomer(t_customerList[i]->getName(),t_customerList[i]->getId());
            this->customersList.push_back(toAdd);
            toAdd = nullptr;
            delete toAdd;
        }
    }
    for(OrderPair order: t_orderList){
        orderList.push_back(order);
    }
}


Trainer& Trainer::operator=(const Trainer &other) {
    if(this != &other){
        clear();
        copy(other.capacity,other.open,other.salary,other.customersList,other.orderList);
    }
    return *this;
}

// move assignment operator
Trainer& Trainer::operator=(Trainer &&other) {
    if(this != &other){
        this->clear();
        this->capacity = other.capacity;
        this->open = other.open;
        this->salary = other.salary;
        size_t size = other.customersList.size();
        for(size_t i = 0 ; i < size ; i++){
            this->customersList.push_back(other.customersList[i]);
            other.customersList[i] = nullptr;
        }
        size = other.orderList.size();
        for(size_t j = 0 ; j < size ; j++){
            OrderPair temp = {other.orderList[j].first,other.orderList[j].second};
            this->orderList.push_back(temp);
        }
        other.orderList.clear();
        other.customersList.clear();
        other.open = false;
        other.capacity = 0;
    }
    return *this;
}

// destructor
void Trainer::clear() {
    size_t customer_list_size = customersList.size();
    for(size_t i = 0 ; i < customer_list_size ; i++){
        delete(customersList[i]);
        customersList[i] = nullptr;
    }
    open = false;
    capacity = 0;
    orderList.clear();
    customersList.clear();
}