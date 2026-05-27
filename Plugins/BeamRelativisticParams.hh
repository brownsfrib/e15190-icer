// Class to fetch the kinetic data for the CM system
// No driver, just a striaght include.
// I hate this framework but I don't want to fix it lol

#ifndef BEAMRELATIVISTICPARAMS_HH
#define BEAMRELATIVISTICPARAMS_HH 
// C/C++ includes
#include <iostream>
#include <vector>

// ROOT includes
#include "TSystem.h"

// Project specific includes
#include "ame20parser.h"
#include "HTRunInfo.h"


class BeamRelativisticParams{
	private:
		MassEval2020* IsotopeTable;
		double beta;
		double lorentz;

		// For rapidity scaling
		double projectileRapidity;
		double targetRapidity;
		double frameRapidity;

	public:
		BeamRelativisticParams(HTRunInfo* runInfo){
			std::string beam(runInfo->GetBeam());
			std::string target(runInfo->GetTarget());
        		const char* beamEnergyChar      = runInfo->GetBeamEnergy(); // MeV !! REMEMBER ME
        		double beamEnergy               = std::stod(beamEnergyChar);

        		// Creation of mass table, checking if beam/target profiles fit the analysis we want to do

        		this->IsotopeTable = new MassEval2020(PROJECT_ROOT "/data/mass_1.mas20");
        		std::pair<int, int> beamAZ = IsotopeTable->GetIsotopeNZGeant(beam);
			int beamN = std::get<0>(beamAZ);
			int beamZ = std::get<1>(beamAZ);
        		double beamA = (double)beamN + (double)beamZ;
			double beamMass = IsotopeTable->Mass(beamN, beamZ, true);

        		std::pair<int, int> targetAZ = IsotopeTable->GetIsotopeNZGeant(target);
			int targetN = std::get<0>(targetAZ);
			int targetZ = std::get<1>(targetAZ);
        		double targetA = (double)targetN + (double)targetZ;
			double targetMass = IsotopeTable->Mass(targetN, targetZ, true);
			std::pair<double, double> lorentzBeta = this->GetParameters(beamMass, beamA*beamEnergy, targetMass);

			this->lorentz 	= std::get<0>(lorentzBeta);
			this->beta 	= std::get<1>(lorentzBeta);
		}

		std::pair<double, double> GetParameters(double beamMass, double beamKinetic, double targetMass){
			double totalEnergy      = beamKinetic + beamMass + targetMass;
			double totalMomentum    = sqrt(std::pow((totalEnergy), 2.0) - std::pow(beamMass + targetMass, 2.0));

			double frameBeta 	= totalMomentum/totalEnergy;
			double frameLorentz     = 1.0/sqrt(1.0 - std::pow(frameBeta, 2.0));
			std::pair<double, double> returnPair = std::make_pair(frameLorentz, frameBeta);

			// Calculating/saving frame rapidity
			this->frameRapidity 	= 0.5*log((1.0+frameBeta)/(1.0-frameBeta));

			// Calculating projectile rapidity
			double beamMomentum 	= sqrt(std::pow((beamKinetic + beamMass), 2.0) - std::pow(beamMass, 2.0)); 
			double beamBeta		= beamMomentum/totalEnergy;
			this->projectileRapidity= 0.5*log((1.0+beamBeta)/(1.0-beamBeta));

			// Calculating target rapidity
			double targetMomentum 	= fabs(totalMomentum - beamMomentum); // Conservation laws bbbbb
			double targetKinetic	= sqrt(std::pow(targetMomentum, 2.0) + std::pow(targetMass, 2.0)) - targetMass;
			double targetBeta	= targetMomentum/totalEnergy;
			this->targetRapidity	= 0.5*log((1.0+targetBeta)/(1.0-targetBeta));

			return returnPair;
		}

		~BeamRelativisticParams(){
			delete this->IsotopeTable;
		}

		double GetSystemBeta(){
			return this->beta;
		}

		double GetSystemLorentz(){
			return this->lorentz;
		}


		double GetProjectileRapidity(){
			return this->projectileRapidity;
		}

		double GetTargetRapidity(){
			return this->targetRapidity;
		}

		double GetFrameRapidity(){
			return this->frameRapidity;
		}
};
#endif
