//
// Created by mohamedashraf on 10/24/21.
//

#include "add.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#define debug 0 // set 1 to activate
add::add() {
    if(debug)
        cout<<"add::add()\n";
};
//####################################
add::add(vector<var>* comvaraddr,vector<var>* addr,const string type, const string def,
const int idx,const string o1, const string o2, string out,string rn , mutex * m){
    operand1 = o1;
    operand2 = o2;
    ActionType = type;
    ActionDefinition = def;
    varVecPtr = addr;
    Resindex = idx;
    comvarVecPtr = comvaraddr;
    fname =out;
    resultName = rn;
    mtx =m;
};
//####################################
void add::runAction() {

    // with this implementation we have the opportunity to work with any num of variables
    // i.e Y= A+S+H+R+A+F
    ofstream of;
    of.open(fname,ios::app);
    int result(0);
    int oldVal(0);
    of<<"~~~ Running Addition action ~~~\n";
    of<<"Expression "<< ActionDefinition<<endl;
    int val1(-1),val2(-1);
    
    bool isCommonVar=false, resCommonVar=false;
    //vector<var> varVec = *varVecPtr;
    for(auto varInst: *varVecPtr){
        if (varInst.getName() == operand1)
            val1 = varInst.getValue();
        if (varInst.getName() == operand2)
            val2 = varInst.getValue();
    }
    for(auto varInst: *comvarVecPtr){
        if (varInst.getName() == operand1){
            isCommonVar = true;
            val1 = varInst.getValue();
        }
        if (varInst.getName() == operand2){
            isCommonVar = true;
            val2 = varInst.getValue();
        }
        if (varInst.getName() == resultName){
            resCommonVar = true;
        }

    }


    /* getting thr index of the result var if it is a local var*/
     for (int i(0); i < varVecPtr->size(); ++i) 
     {
            if (resultName == (*varVecPtr)[i].getName())
                Resindex = i;
        
        }

    /* getting thr index of the result var if it is a global var*/
    for (int i(0); i < comvarVecPtr->size(); ++i) {
            if (resultName == (*comvarVecPtr)[i].getName())
                  Resindex = i;
        }


    // }
    // to check if it was a variable or a constant
    // if a constant then the above if conditions were not visited
    // then it must be a constant so I convert it from string to int
    if(val1 ==-1) val1 =stoi(operand1);
    if(val2 ==-1) val2 =stoi(operand2);
    
    // result is common var OR there is a global variable in the expression OR BOTH
    // Y=Y+1
    // S=Y+1
    if(resCommonVar || isCommonVar){
        mtx->lock();
        result = val1+val2;

        // result is common varialbe so we set result of the common variable in the common variables vector
        if(resCommonVar){
            (*comvarVecPtr)[Resindex].setValue(result);
            of<< "result: "<<(*comvarVecPtr)[Resindex].getValue()<<endl;
        }
        // expression commmon var but not result
        // here result is not a common variable so it is a local var -> so we set result of the variable in the local var vector
        else{
            (*varVecPtr)[Resindex].setValue(result);
            of<< "result: "<<(*varVecPtr)[Resindex].getValue()<<endl;
        }
        mtx->unlock();
    }
    else {
        result = val1+val2;
        (*varVecPtr)[Resindex].setValue(result);
        of<< "result: "<<(*varVecPtr)[Resindex].getValue()<<endl;
    }
    of.close();
}
//####################################
add::~add(){
if(debug)
    cout<<"add::~add()\n";

//delete the var vector ptr
if (varVecPtr != nullptr)
{
    delete varVecPtr;
    varVecPtr = nullptr;
}
else
    printf("Nothing to delete \n");

//delete the comvar vector pointer
if (comvarVecPtr != nullptr)
{
    delete comvarVecPtr;
    comvarVecPtr = nullptr;
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


//####################################