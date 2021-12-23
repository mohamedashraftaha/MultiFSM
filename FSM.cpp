//
// Created by mohamedashraf on 10/24/21.
//
#include "FSM.h"
#include "out.h"
#include <ios>
#include <mutex>
#include <sstream>
#include<fstream>
#include <stdexcept>
#include <string>
#define debug 0 // set to 1 in case of debugging

FSM::FSM(){};
//##############################################
FSM::FSM(string name,vector<var>* v, mutex * m, mutex* m1) : machineName(name), commonvars(v), outWaitmtx(m), exprMutex(m1){


};
//##############################################
void FSM::parser(const string filename,const string& outFile, mutex* m,  mutex*m1) {
  // getting filename from the filename path
  // getting index of the . , then extracting substring from the start till the
  // . to get the file name
  ifstream file(filename.c_str());
  bool statesExists(false),transitionsExists(false);
  vector<string> linesVector;

  if (!file) {
    cout << "Cannot Open " << filename << endl;
    throw runtime_error("File Cannot be opened \n");
  }

  // get all the lines to make it easier to read specific block
  // for example read from the line after the one that contains states
  // until the transitions
  getLineByLine(linesVector,  file);

  if (debug)  
    for(auto x : linesVector){
      cout<<x<<endl;
    }

  int indexOfStates(0);
  for (int i(0); i < linesVector.size(); ++i) {
    string token = linesVector[i].substr(0, linesVector[i].find(' '));
    if (token == "FSM")
      {
        // FSM fsmOne then we get FSM alone and fsmOne alone
        machineName = linesVector[i].substr(linesVector[i].find(' ') + 1);
      }
    if (token == "VAR") {
      int StringStartIndex = 0;
      int StringEndIndex = linesVector[i].find(' ');
      if (debug)
        cout << linesVector[i].substr(StringStartIndex,StringEndIndex - StringStartIndex)<< endl;
      // Extracting the variables names
      StringStartIndex = linesVector[i].find(' ') + 1;
      string varsNames;
      varsNames = linesVector[i].substr(StringStartIndex, linesVector[i].size());
      parseVars(varsNames);
    }
    if (token == "States:") {
      string tempToken;
      // getting the block after states * The definition of the states *
      int j(i + 1);
      while (tempToken != "Transitions:") {
        // now we have all the states
        parseStates(linesVector[j],outFile, m,m1);
        ++j;
        tempToken = linesVector[j].substr(0, linesVector[j].find(' '));
      }
        statesExists = true;
    }
    if (token == "Transitions:") {
      if(!statesExists)
            throw runtime_error("File is corrupted: No States Exist!\n");
      string tempToken;
      int j(i + 1);
      while (j < linesVector.size()) {
        parseTransitions(linesVector[j]);
        ++j;
        tempToken = linesVector[j].substr(0, linesVector[j].find(' '));
      }
      transitionsExists = true;
    }
  }
  if(!transitionsExists)
      throw runtime_error("File is corrupted: No Transitions Exist!\n");
  file.close();
}
//#############################################
void FSM::getLineByLine(vector<string> & v, ifstream & f){

  string line;
  while (getline(f, line)) {
    // if there is an empty line in the file between states and transitions for example
    // to avoid any errors in parsing because of empty lines
      if(!line.empty()){
        v.push_back(line);
      }
  }

};
//##############################################
void FSM::parseVars(const string varString) {
  string vs = varString;
  if (debug)
    // trying a string with more than one 2 Vars to make sure that it is generic
    vs = "X, Y, Z";

  // extracting variable by variable
  // Remove all commas for easy parsing
  vs.erase(remove(vs.begin(), vs.end(), ','),vs.end());
  stringstream ss(vs);
  string singleVar;
  while (ss >> singleVar) {
    // 0 is initial value
    var x(singleVar, 0);
    variablesVec.push_back(x);
  }
}
//##############################################
void FSM::parseStates(const string stateDefinition, const string& outFile, mutex * m, mutex* m1) {
  string sd; 
  string NameOfState;
  int endIndex = stateDefinition.find(':');

  // Extracting the name of the state
  NameOfState = stateDefinition.substr(0, endIndex);

  // get actions
  string actions = stateDefinition.substr(endIndex + 2);
  string actionstemp = "";
  state s;
  vector<statesActions *> vecActions;
  s.setName(NameOfState);

  // used specifically to handle the case of the last action
  actions.append(",");

  // get states by looping over char by char until reaching a delimiter ','
  for (int i = 0; i < actions.length(); ++i) {
    if (actions[i] != ',')
      actionstemp += actions[i];
    else {
      string Type, Def;
      if (actionstemp[0] == ' ')
        actionstemp.erase(0, 1);
      // extracting type and definition
      // my logic is when there is no space between type and definition
      // then it is addition i.e X=X+1 or wait or end
      if (actionstemp.find(' ') < actionstemp.length()) {
        int endIndex = actionstemp.find_first_of(' ');
        Type = actionstemp.substr(0, endIndex);
        Def = actionstemp.substr(endIndex + 1);

        // case where we want to output the value of variable X for example
        if (Type == "out") {
          int val, idx_of_VariableName, idx_of_COMVARname; // store idx of the VAR
          string VAR_Char;
          bool is_VAR = false; // if VAR
          for (int i(0); i < variablesVec.size(); ++i) {
            if (variablesVec[i].getName() == Def) {
              idx_of_VariableName = i;
              val = variablesVec[i].getValue();
              VAR_Char = Def;
              Def = to_string(val);
              is_VAR = true;
            }
          }
        
          // if it is a variable and is not defined in variables vector i.e. is_VAR = false
          // then we are trying to output a variable that was not defined



          /* for MFSM added a checker for COMVAR as well*/
          bool is_COMVAR = false;
        

          for(int i{0}; i < commonvars->size();++i){
         
            if((*commonvars)[i].getName() == Def){
                idx_of_COMVARname = i;
                val = (*commonvars)[i].getValue();
                VAR_Char = Def;
                Def = to_string(val);
                is_COMVAR = true;
        
            }

          }


          /* if no comvar or var then it is an error*/
          if (Def[0] >= 'A' && Def[0] <='Z' && !is_VAR /*&& !is_COMVAR*/){
              string ErrString = "Error!: VAR "+Def+" is not defined in the VAR section!";
                throw runtime_error(ErrString);
          }

          // remove quotes
          Def.erase(remove(Def.begin(), Def.end(), '"'), Def.end());

          // pusshing object of type out action to vector of type actions
          // passing address of object of type var -> containint VarName,Value:
          // e.g. X,1 so that we can print the updated value
          out *outOp = new out(&(*commonvars)[idx_of_COMVARname],&variablesVec[idx_of_VariableName],\
           Type, Def,VAR_Char, is_VAR,is_COMVAR,outFile,outWaitmtx, machineName);
          vecActions.push_back(outOp);
          s.setStateActions(vecActions);
           
          // to avoid adding old output to new output
          vecActions.pop_back();
        }
        if (Type == "sleep") {
          int secs = stoi(Def);
          sleep *sleepOp = new sleep(Type, actionstemp, secs,outFile);
          vecActions.push_back(sleepOp);
          s.setStateActions(vecActions);

          // to avoid adding old output to new output
          vecActions.pop_back();
        }
      } else {
        if (actionstemp == "wait") {
          // call wait
          WaitAct.setCurrState(NameOfState);
          WaitAct.setNextState("");
          WaitAct.setGoToVal(0);

          wait *waitOp = new wait(actionstemp, &WaitAct, NameOfState,&transitionsVec,machineName,outFile, outWaitmtx );
          vecActions.push_back(waitOp);
          s.setStateActions(vecActions);
          // to avoid adding old output to new output
          vecActions.pop_back();

        }
        if (actionstemp == "end") {
          // call endAction
          endAction *endOp = new endAction(actionstemp,outFile);
          vecActions.push_back(endOp);
          s.setStateActions(vecActions);
          // to avoid adding old output to new output
          vecActions.pop_back();

        }
        if (actionstemp.find('+') <= actionstemp.size()) {
          Type = "add";
          int operand1(-1), operand2(-1);
          string actionVarOp1(""), actionVarOp2("");

          // fixed the issue of having variables with more than 1 number -> i.e
          // X=100+100
          int EqualIdx = actionstemp.find('=');
          int plusIdx = actionstemp.find('+');
          for (int i(EqualIdx + 1); i < plusIdx; ++i)
            actionVarOp1 += actionstemp[i];

          for (int i(plusIdx + 1); i < actionstemp.size(); ++i)
            actionVarOp2 += actionstemp[i];


          string resultName = "";
          resultName += actionstemp[0];
          pair<string, int> o1, o2;
          int idx_of_resultVar(0);


         
          // pusshing object of type out action to vector of type actions
          // passing address of the vector var -> containint VarName,Value: e.g.
          // X,1 so that we can update the value already in X(as an example)
          // with the other operand
          add *addOp = new add(commonvars,&variablesVec, Type, 
          actionstemp,idx_of_resultVar, actionVarOp1, actionVarOp2,outFile,resultName, m1);
          vecActions.push_back(addOp);
        
          s.setStateActions(vecActions);
          // to avoid adding old output to new output
          vecActions.pop_back();

        }
      }
      if (debug)
        cout << actionstemp << endl;
      actionstemp = "";
    }

  }

  // push the state
  statesVec.push_back(s);
}
//##############################################
void FSM::parseTransitions(const string transitionString) {
  string ts = transitionString;
  // remove all commas for easier parsing
  ts.erase(remove(ts.begin(), ts.end(), ','),ts.end());
  // remove all spaces for easier parsing
  ts.erase(remove(ts.begin(), ts.end(), ' '),ts.end());
  string currS = "";
  currS += ts[0]; // convert to string
  string nextS = "";
  nextS += ts[1];          // convert to string
  int gotoS = ts[2] - '0'; // convert to int
  transition t(currS, nextS, gotoS);
  transitionsVec.push_back(t);
}
//##############################################
void FSM::runFSM() {
  // parsing the file to get
  //                          -> 1. machine name
  //                          -> 2. Var
  //                          -> 3. states
  //                          -> 4. Transition


  // creating an output file
  string mName = machineName.substr(machineName.find('/')+1);
  string outFileName = "testfiles/outputFiles/OUT"+mName; 
  parser(machineName,outFileName, outWaitmtx,exprMutex);
  ofstream outFile;
  outFile.open(outFileName);
  transition transitionTrack; // keep track of transitions;

  transitionTrack.setCurrState(statesVec[0].getName()); // initially state A
  vector<statesActions *> sActions; // get actions in each state

  ofstream f;


  outFile << "########## FSM RUNNING ##########\n\n";
  outFile << "FSM Name: " << machineName << "\n\n";
  outFile <<"############# ACTIVE STATE: "+   statesVec[0].getName()+" ################\n";
  outFile.close();
  // infinite loop
 for (;;) {

    for (auto stateInst:statesVec) {
      if (transitionTrack.getCurrState() == stateInst.getName()) {
          sActions = stateInst.getStateActions();
        for (auto ActionsInst : sActions) {
            if (ActionsInst->getActionType() == "wait") {
              ActionsInst->runAction();
            int toWhichState;
            // now we have the value by which we know how to go to next state
            // now we loop over transitions vector to identify what is the next
            // state based on the current state and the goto value
            toWhichState = WaitAct.getGoToVal();
            for (auto transitionInst: transitionsVec) {
              if (transitionInst.getCurrState() == transitionTrack.getCurrState() && transitionInst.getGoToVal() == toWhichState) {
                string nextState = transitionInst.getNextState();
                transitionTrack.setCurrState(nextState);
                f.open(outFileName,ios::app);
                f << "\n~~~~~~~~~ Moving to the Next State: \""<< transitionTrack.getCurrState()<< "\"~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<< endl;
                f <<"############# ACTIVE STATE: "+  transitionTrack.getCurrState()+" ################\n";
                f.close();
                break;
              }
            }
            break;
          }
          ActionsInst->runAction();
          if (ActionsInst->getActionType() =="end"){
            return;
          }
        }
      }
    }
  }
}
//###################################################
FSM::~FSM() {
  if (debug)
    cout << "FSM::~FSM()";
};