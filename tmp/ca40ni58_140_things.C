// C/C++ includes
#include <iostream>
#include <sstream>
#include <time.h>
#include <tuple>
#include <map>
#include <vector>
#include <functional>
#include <algorithm>

#include "../Plugins/ame20parser.h"

void ca40ni58_140_things(){
	MassEval2020 isotopeTable("/mnt/home/brow1247/Koonin/e15190/data/mass_1.mas20");

	double beamA = 40;
	double beamZ = 20;
	double beamMass = isotopeTable.Mass((beamA-beamZ), beamZ, true);

	double targA = 58;
	double targZ = 28;
	double targMass = isotopeTable.Mass((targA-targZ), targZ, true);

	double combinedMass = beamMass + targMass;

	double beamEnergy = 140*beamA + beamMass;
	double totalEnergy = beamEnergy + targMass;

	double beamMomentum = sqrt(beamEnergy*beamEnergy - beamMass*beamMass);
	double beamBeta = beamMomentum/totalEnergy;
	double beamRapidity = 0.5*log((1.0+beamBeta)/(1.0-beamBeta));

	double cmMomentum = sqrt(totalEnergy*totalEnergy - combinedMass*combinedMass);
	double cmBeta = cmMomentum/totalEnergy;
	double cmRapidity = 0.5*log((1.0+cmBeta)/(1.0-cmBeta));

	std::cout << "cm rapidity: " << cmRapidity << '\t';
	std::cout << "beam rapidity: " << beamRapidity << '\n';


}
