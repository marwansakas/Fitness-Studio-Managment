#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include "Workout.h"
#include <vector>
#include <limits>
#include <iostream>
#include <algorithm>

class Customer{
public:
    Customer(std::string c_name, int c_id);
    Customer(const Customer &other);
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual ~Customer();
private:
    const std::string name;
    const int id;
protected:
    std::pair<int,int> GetMostExpensiveWorkout(const std::vector<Workout> &workout_options, enum WorkoutType type);
    std::pair<int,int> GetLeastExpensiveWorkout(const std::vector<Workout> &workout_options, enum WorkoutType type);
};


class SweatyCustomer : public Customer {
public:
    SweatyCustomer(std::string name, int id);
    SweatyCustomer(const SweatyCustomer &other);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    virtual ~SweatyCustomer();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    CheapCustomer(const CheapCustomer &other);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    virtual ~CheapCustomer();
private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(std::string name, int id);
    HeavyMuscleCustomer(const HeavyMuscleCustomer &other);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    virtual ~HeavyMuscleCustomer();
private:
    static int my_compare(const std::pair<int,int>& a , const std::pair<int,int>& b);
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(std::string name, int id);
    FullBodyCustomer(const FullBodyCustomer &other);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    virtual ~FullBodyCustomer();
private:
};


#endif