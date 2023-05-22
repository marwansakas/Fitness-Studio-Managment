#ifndef TRAINER_H_
#define TRAINER_H_

#include "Customer.h"
typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    // constructor
    Trainer(int t_capacity);
    //copy constructor
    Trainer(Trainer &other);
    //copy assignment operator
    Trainer& operator=(const Trainer &other);
    //move constructor
    Trainer(Trainer &&other);
    //move assignment operator
    Trainer& operator=(Trainer &&other);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
    virtual ~Trainer();
private:
    int salary;
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    void copy(int t_capacity, bool t_open,int salary, const std::vector<Customer *>& t_customerList,
              const std::vector<OrderPair>& t_orderList);
    void clear();
};


#endif