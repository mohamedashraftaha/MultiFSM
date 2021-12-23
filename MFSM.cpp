//
// Created by mohamedashraf on 11/22/21.
//
#include"MFSM.h"
#include <fstream>
#include <mutex>
#include <ostream>
#include <thread>
#define debug 0 // set 1 to activate



// constructor
MFSM::MFSM(string filename): MFSMname(filename){};
//############################
void MFSM::parser(const string filename ){
    ifstream file(filename.c_str());
    vector<string>linesvector;

    if (!file) {
        cout << "Cannot Open " << filename << endl;
        throw runtime_error("File Cannot be opened \n");
    }

    // use the get LineByLine function in the FSM class, MFSM inherit from FSM
    FSM::getLineByLine(linesvector, file);
    if (debug){
        for(auto line:linesvector)
            cout<<line<<endl;}

    for (int i(0); i < linesvector.size(); ++i) {
        string token = linesvector[i].substr(0, linesvector[i].find(' '));
        if (token == "MFSM")
        {
            MFSMname = linesvector[i].substr(linesvector[i].find(' ')+1);
        }
        if (token == "COMVAR"){
            int StringStartIndex = 0;
            int StringEndIndex = linesvector[i].find(' ');
            if (debug)
                cout << linesvector[i].substr(StringStartIndex,StringEndIndex - StringStartIndex)<< endl;
            // Extracting the variables names
            StringStartIndex = linesvector[i].find(' ') + 1;
            string varsNames;
            varsNames = linesvector[i].substr(StringStartIndex, linesvector[i].size());
            parseComvar(varsNames);
        }
        if (token.find("machines") != string::npos) {
            for(int  j{i+1}; j < linesvector.size();++j)
                FSMmachinesNames.push_back(linesvector[j]);
            break;
        } 
    }
    if (debug)
    {
    cout<<"NAME: "<< MFSMname<<endl;
    cout<<"COMVAR:\n";

    for(int x{0}; x<commonVars.size();++x){
       cout<< commonVars[x].getName()<<endl;
    }
    cout<<"Machines\n";
    for(auto x: FSMmachinesNames)
        cout<<x<<endl;
    }
};
//############################
void MFSM::parseComvar(const string comVars){
    string comvars = comVars;
    if (debug)
        // trying a string with more than one 2 Vars to make sure that it is generic
        comvars = "A, B, C, D, E, F, G, H, I, J, K";

    // extracting variable by variable
    // Remove all commas for easy parsing
    comvars.erase(remove(comvars.begin(), comvars.end(), ','),comvars.end());
    stringstream ss(comvars);
    string singleVar;
    while (ss >> singleVar) {
        // 0 is initial value
        var x(singleVar,0);// = new comvar(singleVar,0);
        commonVars.push_back(x);   
    }
}
//###############################
void MFSM::runMFSM(){

    path = MFSMname.substr(0, MFSMname.find('/')+1);
    

    //parsing file name
    parser(MFSMname);

    // vector of threads 
    vector<thread> FSMthreads;
    
    int size = FSMmachinesNames.size();

    vector<string> FSMmachineNames_Fullpath(size);
    // add .fsm to FSM names to be able to open it

    cout<<"~~~ For better testing/Output Each thread will create a file with the output and user can trace any changes to the output as follows: \n 1. Open a terminal for each machine in the mfsm file path, you should open the terminals in the folder testfiles/outputFiles \n 2. use the command: tail -F OUTfsm#N.fsm , Where #N is the machine number in words, i.e One,Two,.. \n";

    cout<<"\n\n~~~~~~~~~ Starting Machines.... ~~~~~~~~~ \n\n";
    for(int i{0}; i < size;++i){
        FSMmachineNames_Fullpath[i]= path+FSMmachinesNames[i]+".fsm";
    }

    // call constructor for all machines in the mfsm file
    

    for(int i{0};i<size;++i){
        FSM machine(FSMmachineNames_Fullpath[i],&commonVars, &out_wait_mutex, &exprMutex);
        FSMmachines.push_back(machine);
    }

 
    for(int i{0}; i< size;++i){
        cout<<FSMmachines[i]<<endl;
    }

   // FSMmachines[0].runFSM();    
   //create a thread to run FSM(with fsm machine name) then push it to threads vector
    for(int i{0}; i< FSMmachinesNames.size();++i){
        FSMthreads.push_back(
             // lambda expression to execute the run fsm    
            thread( [i,this](){
                    FSMmachines[i].runFSM();}
            )
        );
    
     }  

    // join threads
    for(thread &th : FSMthreads)
        th.join();


};
