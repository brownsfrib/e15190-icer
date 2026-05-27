### Updates to E15190-Unified-Analysis Framework
Man it was so unified wasn't it? 
Got rid of ForwardArray, NeutronWall, and VetoWall structures
	- Maybe we can use Vetowall data later. I doubt it 
Cleaned up the code base, moved all run based parsing/cleaning/calibration to ./lib and associated subdirectories

Call ./build\_library.sh to recompile the E15190 run reader library (again, only for the Microball and HiRA)
Call ./compile.sh MyExecutable.cxx (must be .cxx!) to compile your analysis binary
	- Will be stored as ./bin/MyExecutable

Keep configuration files (E15190 library run files (.conf), ROOT environment scripts (centralities, .env) in ./config

That's it. We're on the HPCC and recompiled with the a new GECKO container
