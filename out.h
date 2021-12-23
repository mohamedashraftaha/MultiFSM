//
// Created by mohamedashraf on 10/24/21.
//
#ifndef MOHAMEDTAHA_900172754_FSM_SIMULATOR_OUT_H
#define MOHAMEDTAHA_900172754_FSM_SIMULATOR_OUT_H
#include"statesActions.h"
#include "var.h"
#include <fstream>
using namespace std;
class out: public statesActions {
private:
//    string outAction;
//    string outString;
string varName;
bool isvar;
bool iscomvar;
var* v;
var* comvar;
string fname;
mutex *mtx;
string machineName;
public:
    //Default constructor
    out();

    //constructor
    out(var*,var*,const string,const string, const string, const bool,const bool, string,mutex*, const string   );

    // virtual func inherited from stateActions
    void runAction();

    //destructor
    ~out();

};


#endif //MOHAMEDTAHA_900172754_FSM_SIMULATOR_OUT_H
