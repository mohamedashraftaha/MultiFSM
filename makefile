FSMtest: MFSM.h MFSM.cpp TestingFunctionality.cpp

	g++ -std=c++11 -pthread -ggdb -fno-elide-constructors -o MFSMtest var.cpp state.cpp transition.cpp out.cpp sleep.cpp add.cpp wait.cpp endAction.cpp statesActions.cpp FSM.cpp MFSM.cpp TestingFunctionality.cpp 
clean:

	rm MFSMtest
