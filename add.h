//
// Created by mohamedashraf on 10/24/21.
//

#ifndef MOHAMEDTAHA_900172754_FSM_SIMULATOR_ADD_H
#define MOHAMEDTAHA_900172754_FSM_SIMULATOR_ADD_H


#include"statesActions.h"
#include"var.h"
#include <fstream>
#include <mutex>
using namespace std;
class add: public statesActions {
private:
    string operand1;
    string operand2;
    vector<var>* varVecPtr; //address of result in the vector of type variables 
    vector<var>* comvarVecPtr; //address of result in the vector of type COMM variables 
    int Resindex; // index of the result var in the var vectors
    string fname;
    string resultName;
    mutex * mtx;
public:
    //default constructor
    add();

    // constructor
    add(vector<var>*,vector<var>*,const string, const string,const int, const string, const string, string,string, mutex*);
    
    // virtual func
    void runAction();
    
    //destructor
    ~add();
};
#endif //MOHAMEDTAHA_900172754_FSM_SIMULATOR_ADD_H
