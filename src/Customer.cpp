#include "../include/Customer.h"


Customer::Customer(std::string c_name, int c_id):name(c_name) ,id(c_id){}

Customer::Customer(const Customer &other):name(other.name),id(other.id){}

int Customer::getId() const{
    return id;
}

std::string Customer::getName() const{
    return name;
}

// constructors
SweatyCustomer::SweatyCustomer(std::string name, int id): Customer(name,id){}

CheapCustomer::CheapCustomer(std::string name, int id): Customer(name,id){}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name,id){}

FullBodyCustomer::FullBodyCustomer(std::string name, int id): Customer(name,id){}

// copy constructors
SweatyCustomer::SweatyCustomer(const SweatyCustomer &other):Customer(other){}

CheapCustomer::CheapCustomer(const CheapCustomer &other):Customer(other){}

HeavyMuscleCustomer::HeavyMuscleCustomer(const HeavyMuscleCustomer &other):Customer(other){}

FullBodyCustomer::FullBodyCustomer(const FullBodyCustomer &other):Customer(other){}

// toString functions
std::string SweatyCustomer::toString() const {
    return getName() + ",swt";
}

std::string CheapCustomer::toString() const {
    return getName() + ",chp";
}

std::string HeavyMuscleCustomer::toString() const {
    return getName() + ",mcl";
}

std::string FullBodyCustomer::toString() const {
    return getName() + ",fbd";
}

// order functions
std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> customer_order;
    int size = (int)workout_options.size();
    for(int i = 0 ; i < size; i++){
        if(workout_options.at(i).getType() == CARDIO){
            customer_order.push_back(workout_options.at(i).getId());
        }
    }
    return customer_order;
}



std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> output;
    if(workout_options.empty()){
        return output;
    }
    int temp_price,temp_id;
    int min_id = workout_options.at(0).getId();
    int min_price = workout_options.at(0).getPrice();
    size_t size = workout_options.size();
    for(size_t i = 1 ; i < size ; i++){
        temp_price = workout_options.at(i).getPrice();
        temp_id = workout_options.at(i).getId();
        if(min_price == temp_price){
            if(min_id > temp_id){
                min_id = temp_id;
                min_price = temp_price;
            }
        }
        if(min_price > temp_price){
            min_id = temp_id;
            min_price = temp_price;
        }
    }
    output.push_back(min_id);
    return output;
}



std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> output;
    int anaerobic_size = 0;
    size_t size = workout_options.size();
    for(size_t i = 0 ; i < size ; i++){
        if(workout_options.at(i).getType() == ANAEROBIC){
            anaerobic_size++;
        }
    }
    if(anaerobic_size == 0){
        return output;
    }
    std::vector<std::pair<int,int>> temp;
    int id,price;
    int counter = 0;
    std::pair<int,int> pair;
    for(size_t j = 0 ; j < size ; j++){
        if(workout_options.at(j).getType() == ANAEROBIC){
            id = workout_options[j].getId();
            price = workout_options[j].getPrice();
            pair = {id,price};
            temp.push_back(pair);
            counter++;
            if(counter == anaerobic_size){
                break;
            }
        }
    }
    std::sort(temp.begin(),temp.end(),my_compare);
    for(int k = 0 ; k < anaerobic_size ; k++){
        output.push_back(temp[k].first);
    }
    return output;
}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options){
    std::pair<int,int> min_cardio = GetLeastExpensiveWorkout(workout_options,CARDIO);
    std::pair<int,int> max_mix = GetMostExpensiveWorkout(workout_options,MIXED);
    std::pair<int,int> min_anaerobic = GetLeastExpensiveWorkout(workout_options,ANAEROBIC);
    std::vector<int> output;
    if(min_cardio.second == -1 || max_mix.second == -1 || min_anaerobic.second == -1){
        return output;
    }
    output.push_back(min_cardio.first);
    output.push_back(max_mix.first);
    output.push_back(min_anaerobic.first);
    return output;
}



// this function gets id and the price of the most expensive workout of a given type of workout
std::pair<int,int> Customer::GetMostExpensiveWorkout(const std::vector<Workout> &workout_options, enum WorkoutType type){
    int maxPrice = std::numeric_limits<int>::min();
    int maxId=0;
    int size = (int)workout_options.size();
    int temp_price,temp_id;
    bool found = false;
    for(int i= 0; i < size; i++) {
        if (workout_options.at(i).getType() == type) {
            found = true;
            temp_price = workout_options.at(i).getPrice();
            temp_id = workout_options.at(i).getId();
            if (temp_price == maxPrice) {
                if (maxId > temp_id) {
                    maxId = temp_id;
                }
            }
            if (temp_price > maxPrice) {
                maxId = temp_id;
                maxPrice = temp_price;
            }
        }
    }
    std::pair<int,int> output;
    if(!found){
        output.first = -1;
        output.second = -1;
        return output;
    }
    output.first = maxId;
    output.second = maxPrice;
    return output;
}

// this function gets id and price of the least expensive workout of a given type of workout
std::pair<int,int> Customer::GetLeastExpensiveWorkout(const std::vector<Workout> &workout_options, enum WorkoutType type){
    int minPrice = std::numeric_limits<int>::max();
    int minId=0;
    int size = (int)workout_options.size();
    int temp_price,temp_id;
    bool found = false;
    for(int i= 0; i < size; i++) {
        if (workout_options.at(i).getType() == type) {
            found = true;
            temp_price = workout_options.at(i).getPrice();
            temp_id = workout_options.at(i).getId();
            if (temp_price == minPrice) {
                if (minId < temp_id) {
                    minId = temp_id;
                }
            }
            if (temp_price < minPrice) {
                minId = temp_id;
                minPrice = temp_price;
            }
        }
    }
    std::pair<int,int> output;
    if(!found){
        output.first = -1;
        output.second = -1;
        return output;
    }
    output.first = minId;
    output.second = minPrice;
    return output;
}




// this is a compare function for the usage of std::sort
int HeavyMuscleCustomer::my_compare(const std::pair<int,int>& a , const std::pair<int,int>& b){
    if(a.second > b.second)
        return -1;
    if(a.second < b.second)
        return 1;
    if(a.first < b.first)
        return -1;
    return 1;
}



// destructors ( all the destructors are empty , because this objecct does not contain any resources

Customer::~Customer() {}

SweatyCustomer::~SweatyCustomer() {}

CheapCustomer::~CheapCustomer() {}

FullBodyCustomer::~FullBodyCustomer() {}

HeavyMuscleCustomer::~HeavyMuscleCustomer() {}











