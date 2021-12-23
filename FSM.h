//
// Created by mohamedashraf on 10/24/21.
//

#ifndef MOHAMEDTAHA_900172754_FSM_SIMULATOR_FSM_H
#define MOHAMEDTAHA_900172754_FSM_SIMULATOR_FSM_H
// for GNU C++11 + compilers
#include<bits/stdc++.h>
#include<iostream>
#include <mutex>
#include<vector>
#include<fstream>
#include<sstream>
#include<algorithm>
#include "state.h"
#include "var.h"
#include "transition.h"
#include "statesActions.h"
#include"out.h"
#include"sleep.h"
#include"add.h"
#include"wait.h"
#include "endAction.h"
#include "add.h"
class FSM {
private:
    vector<transition> transitionsVec;
    vector<state> statesVec;
    vector<var> variablesVec;
    string machineName;
    transition WaitAct; // for the wait action to set the next state
    vector<var>* commonvars;
    mutex * outWaitmtx;
    mutex* exprMutex;

public:
    int gotoNextTransition;
    //Default Constructor
    FSM();

    // constructor
    FSM(string, vector<var>*, mutex * , mutex*);

    // making the input const as a promise to the compiler that the inputted parameter not to be changed
    // parsing function
    void parser(const string,const string&, mutex *, mutex*);

    // get line by line as vectors 
    void getLineByLine(vector<string>&, ifstream&);

    //parse VAR
    void parseVars(const string);

    //parse States
    void parseStates(const string,const string&, mutex* , mutex*);

    //parse Transitions
    void parseTransitions(const string);

    // run the FSM
    void runFSM();

    // destructor
    ~FSM();

};


#endif //MOHAMEDTAHA_900172754_FSM_SIMULATOR_FSM_H
