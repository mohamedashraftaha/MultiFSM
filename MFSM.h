//
// Created by mohamedashraf on 11/22/21.
//

#ifndef MOHAMEDTAHA_900172754_M_FSM_SIMULATOR_MFSM_H
#define MOHAMEDTAHA_900172754_M_FSM_SIMULATOR_MFSM_H

#include"FSM.h"
#include <mutex>
#include <string>

class MFSM: public FSM
{
private:


// vector that will contain machine names
vector<string> FSMmachinesNames;


//vector of FSM machines in the mfsm file
vector<FSM> FSMmachines;


// vector of common variables
vector<var> commonVars; 

string MFSMname;



string path;

mutex out_wait_mutex, exprMutex;


public:
    // default constructor
    MFSM(){};

    // constructor to get the M_FSM file
    MFSM(string);

    // parsing function
    void parser(const string);

    // parse machines names
    void parseMachines(const string);

    // parse comvar sections
    void parseComvar(const string);

    // run machine
    void runMFSM();


    ~MFSM(){}  ;

    
};




#endif //MOHAMEDTAHA_900172754_M_FSM_SIMULATOR_MFSM_H