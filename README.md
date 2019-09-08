# learningWinApi
Small milestones that I reached while trying to learn Windows API



### seShutdownPrivilege_example
Every SID / token has certain privileges associated with it which aren't enabled by default. This program enables seShutdownPrivilege and uses windows API to poweroff the PC without warnings.


### list_active_processes_pid,uid,domain_info.cpp

Program returns an output like the task manager showing all processes with pid its count and the process owner info along with SID


### Run_As_Administrator.cpp

This program is pretty simple once you run it, it checks if the current program was ran as an administrator if not then the program relaunches its self as an administrator with a UAC prompt ofc.