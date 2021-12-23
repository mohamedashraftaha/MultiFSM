//
// Created by mohamedashraf on 11/1/21.
//

#ifndef MOHAMEDTAHA_900172754_FSM_SIMULATOR_ENDACTION_H
#define MOHAMEDTAHA_900172754_FSM_SIMULATOR_ENDACTION_H


#include "statesActions.h"
#include <fstream>
#include <string>

// changed name from end to endAction -> since I got the error reference to ‘end’ is ambiguous
class endAction: public statesActions{
public:
    endAction();
    endAction(const string, string);
    void runAction();
    ~endAction();

private:
string fname;
};


#endif //MOHAMEDTAHA_900172754_FSM_SIMULATOR_ENDACTION_H
