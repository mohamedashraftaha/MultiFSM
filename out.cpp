//
// Created by mohamedashraf on 10/24/21.
//
#include "out.h"
#include <fstream>
#include <ios>
#include <mutex>
#include <string>
#define debug 0
out::out() {
    if (debug){
        cout<<"out::out()\n";
    }
}
//###################################
out::out(var*comvtemp,var* vtemp,const string action, const string output,
 const string vn,const  bool isv, const bool iscv, string out, mutex *m, const string name) {
    ActionDefinition = output;
    ActionType= action;
    varName = vn;
    isvar = isv;
    v = vtemp;
    iscomvar = iscv;
    comvar = comvtemp;
    fname = out;
    mtx = m;
    machineName = name;
};
//###################################
void out::runAction() {
    ofstream of;
    of.open(fname,ios::app);
    //if (debug)
    of <<"~~~ Running OUT action ~~~"<<endl;
    if(isvar) {
        of << "Action: " << ActionType << " VAR " << varName << endl;
        of<<"Output: "<<v->getValue()<<endl;
        mtx->lock();
        cout <<"\""<<machineName<<"\" : " << " Action: " << ActionType << " VAR " << varName <<" : Output: "<<v->getValue()<<endl;
        mtx->unlock();
    }

    /*added condition to check for comvar*/
    else if(iscomvar){
        mtx->lock();
        of << "Action: " << ActionType << " COMVAR " << varName << endl;
        of<<"Output: "<<comvar->getValue()<<endl;
        cout <<"\""<<machineName<<"\" : " <<" Action: " << ActionType << " COMVAR " << varName << " Value: "<<comvar->getValue() <<endl<< endl;
        mtx->unlock();
        }
    else {
    
        of << "Action: " << ActionType << " " << "\"" << ActionDefinition << "\"" << endl;
        of << "Output: " << ActionDefinition << endl;
        
        /*restrict Access to terminal*/
        mtx->lock();
        cout <<"\""<<machineName<<"\" : " <<" Action: " << ActionType << " statement:  "<<ActionDefinition << endl<<endl;;
        mtx->unlock();
    }
    of.close();
};
//##################################
out::~out() {
    if (debug)
        cout<<"out::~out()\n";
    
//delete the var  ptr
if (v != nullptr)
{
    delete v;
    v = nullptr;
}
else
    printf("Nothing to delete \n");

//delete the comvar  pointer
if (comvar != nullptr)
{
    delete comvar;
    comvar = nullptr;
}
else
    printf("Nothing to delete \n");

// delete the mutex pointer
if (mtx != nullptr)
{
    delete mtx;
    mtx = nullptr;
}
else
    printf("Nothing to delete \n");
};