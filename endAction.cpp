//
// Created by mohamedashraf on 10/24/21.
//

#include "endAction.h"
#include <fstream>
#include <ios>

#define debug 0 //set 1 to activate
endAction::endAction() {
    if (debug)
        cout<<"endAction::endAction()\n";

}
//##########################
endAction::endAction(const string a, string out){
    ActionType =a;
    fname = out;
}
//#########################
void endAction::runAction() {
    ofstream of;
    of.open(fname, ios::app);
    of<<"############### FSM ENDED ###############\n";
    of.close();
    // fixed issue from the FSM assignment
    //exit(0);
}
//##########################

endAction::~endAction() {
    if (debug)
        cout<<"endAction::~endAction()\n";

    
}