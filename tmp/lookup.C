// C/C++ includes
#include <iostream>
#include <vector>
#include <fstream>
#include <map>

// Project includes
#include "../Plugins/ame20parser.h"

MassEval2020 isotopeTable("/mnt/home/brow1247/Koonin/e15190/data/mass_1.mas20");

const double getCMrapidity(
		const int projA, 
		const int projZ, 
		const int targA, 
		const int targZ, 
		const double energy_u
		){

	double beamMass = isotopeTable.Mass((projA-projZ), projZ, true); // MeV/c^2
	double targMass = isotopeTable.Mass((targA-targZ), targZ, true); // MeV/c^2
	double combinedMass = beamMass + targMass;
	double totalEnergy = energy_u*(double)projA + combinedMass;
	double cmMomentum = sqrt(totalEnergy*totalEnergy - combinedMass*combinedMass);
	double cmBeta = cmMomentum/totalEnergy;

	return 0.5*log((1.0+cmBeta)/(1.0-cmBeta)); 
}


void lookup(){
	std::map<std::string, std::pair<int, int>> index = {
		{"ca40", std::make_pair(40, 20)},
		{"ca48", std::make_pair(48, 20)},
		{"ni58", std::make_pair(58, 28)},
		{"ni64", std::make_pair(64, 28)},
		{"sn112", std::make_pair(112, 50)},
		{"sn124", std::make_pair(124, 50)},
	};

	std::vector<std::string> projectiles = {"ca40", "ca48"};
	std::vector<std::string> targets = {"ni58", "ni64", "sn112", "sn124"};
	std::vector<double> energies = {56.0, 140.0};
	for (int j=0; j < energies.size(); j++){
		const double energy_u = energies[j];

		for (int i=0; i < projectiles.size(); i++){
			const int projA = std::get<0>(index[projectiles[i]]);
			const int projZ = std::get<1>(index[projectiles[i]]);

			for (int k=0; k < targets.size(); k++){
				const int targA = std::get<0>(index[targets[k]]);
				const int targZ = std::get<1>(index[targets[k]]);

				const double cmRapidity = getCMrapidity(projA, projZ, targA, targZ, energy_u);
				std::cout << projectiles[i] << '\t';
				std::cout << targets[k] << '\t';
				std::cout << energies[j] << '\t';
				std::cout << cmRapidity << '\n';
			}
		}
	}

}
