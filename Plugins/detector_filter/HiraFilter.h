#ifndef HIRAFILTER_H
#define HIRAFILTER_H

// C/C++ includes
#include <iostream>
#include <tuple>
#include <map>
#include <fstream>
#include <sstream>

// ROOT includes
#include "TSystem.h"

// Filter "arbitrary" data based on E15190 position/energy calibrations
// Punch through energies (calibrated experiment date, 2018) can be found here:
//	- https://groups.frib.msu.edu/hira/15190-14030/files/HiRAPunchThroughSummary.pdf
// "Punch through energy" means the maximum energy of a particle species the detector can resolve
// Silicon/CsI detectors were source calibrated (daughters of U232 before experiment) 
// Telescopes (Silicon + CsI setup) were gain matched from two body Rutherford scattering Ca40 (56/140 MeV/u) on a CH2 (plastic) target

class HiraFilter{
	public:
		// null c-dtor
		HiraFilter(std::string angleCalibration_in){
			if (gSystem->AccessPathName(angleCalibration_in.c_str())){
				std::string message = "\033[1;31mERROR: HiraFilter.h constructor - file " + \
						       angleCalibration_in + \
						       " does not exist.\033[0m\n";
				throw std::runtime_error(message);
			}
			std::cout << "\033[3;33mNote! HiraFilter theta/phi filter operates only in radians\033[0m\n";
			angleCalibration = angleCalibration_in;

			// Construct theta/phi vectors 
			// Default to radians
			readAngleCalibration();
		
		}

		~HiraFilter(){}


	// Testers
	public:
		// Determine if a particle's theta/phi is mapped to a HiRA theta/phi epsilon-delta style
		// I would default epsilon for HiRA is half a degree (generous)
		bool isMeasuredThetaPhi(
                                const float& theta,
                                const float& phi,
                                double epsilon
                ){
                        for (unsigned int i=0; i < labAngles.size(); i++){
                                float labTheta  = std::get<0>(labAngles[i]);
                                float labPhi    = std::get<1>(labAngles[i]);
                                bool isTheta    = std::fabs(labTheta - theta) <= epsilon;
                                bool isPhi      = std::fabs(labPhi - phi) <= epsilon;
                                if (
                                        (isTheta)
                                        &&(isPhi)
                                ){
                                        return true;
                                }
                        }
                        return false;
                }

		// Non-inclusive upper bound kinetic energy checker
                bool isMeasuredKinetic(
				const int& A,
				const int& Z,
				const float& labKinetic){
			if (!isMeasuredAZ(A, Z)){
				return false;
			}
                        float minPunch = minEnergy.at(std::make_tuple(A, Z));
                        float maxPunch = maxEnergy.at(std::make_tuple(A, Z));
                        if (
                                (labKinetic >= minPunch)
                                &&(labKinetic < maxPunch)
                        ){
                                return true;
                        }
                        return false;
                }

		// Checks if the A/Z passed is something measured by HiRA
                // Experiment specific, no need to get fancy
                bool isMeasuredAZ(const int& A, const int& Z){
                        for (unsigned int i=0; i < viableAZ.size(); i++){
                                if (
                                        (A == std::get<0>(viableAZ[i]))
                                        &&(Z == std::get<1>(viableAZ[i]))
                                ){
                                        return true;
                                }
                        }
                        return false;
                }

		
	private:
		// Calibration files
		// Theta/phi file with angles given in RADIANS
		// Let me say that again since I've wasted months of my life on this difference
		// The angles, in the calibration file, are in RADIANS
		std::string angleCalibration;

		// Ordering of lab angles is theta, phi
		std::vector<std::tuple<float, float>> labAngles;

		void readAngleCalibration(){
			std::ifstream angleFile(angleCalibration);
			std::string line;
			while (std::getline(angleFile, line)){
				std::stringstream ss(line);
				std::string token;
				bool firstLine = true;
				float theta;
				float phi;
				while (std::getline(ss, token, ',')){
					if (firstLine){
						theta = std::stof(token);
						firstLine = false;
					}
					else{
						phi = std::stof(token);
					}
				}
				labAngles.emplace_back(std::make_pair(theta, phi));
			}
			angleFile.close();
		}



		// Setting particle species measured by HiRA (safety)
		// the sunset is really pretty right now hehe this is fun
		static const inline std::vector<std::tuple<int, int>> viableAZ{
			{1, 1}, // proton
			{2, 1}, // deuteron
			{3, 1}, // triton
			{3, 2}, // He3
			{4, 2} // alpha
		};

		// Silicon (DSSD) punch through energies (lab frame)
		static const inline std::map<std::tuple<int, int>, float> minEnergy{
			{{1, 1}, 15.4273},
			{{2, 1}, 20.753},
			{{3, 1}, 24.630},
			{{3, 2}, 54.5826},
			{{4, 2}, 62.01368}
		};


		// CsI punch through energies (lab frame)
		static const inline std::map<std::tuple<int, int>, float> maxEnergy{
			{{1, 1}, 198.485},
			{{2, 1}, 263.650},
			{{3, 1}, 312.444},
			{{3, 2}, 709.306},
			{{4, 2}, 797.840}
		};

};
#endif
