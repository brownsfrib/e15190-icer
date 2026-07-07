#ifndef MICROBALLFILTER_H
#define MICROBALLFILTER_H

// C/C++ includes
#include <iostream>
#include <tuple>
#include <map>
#include <fstream>
#include <sstream>

// ROOT includes
#include "TSystem.h"

// Filter "arbitrary" data based on E15190 position/energy calibrations
class MicroballFilter{
	public:
		// null c-dtor
		MicroballFilter(std::string angleCalibration_in){
			if (gSystem->AccessPathName(angleCalibration_in.c_str())){
				std::string message = "\033[1;31mERROR: MicroballFilter.h constructor - file " + \
						       angleCalibration_in + \
						       " does not exist.\033[0m\n";
				throw std::runtime_error(message);
			}
			std::cout << "\033[3;33mNote! MicroballFilter theta/phi filter operates only in radians\033[0m\n";
			angleCalibration = angleCalibration_in;

			// Construct theta/phi vectors 
			// Default to radians
			readAngleCalibration();
		
		}

		~MicroballFilter(){}


	// Testers
	public:
		// Check if a theta/phi value is within a microball crystal
		bool isMeasuredThetaPhi(
                                const float& theta,
                                const float& phi
                ){
                        for (unsigned int i=0; i < labAngles.size(); i++){
                                const float min_theta = std::get<0>(labAngles[i]);
                                const float max_theta = std::get<1>(labAngles[i]);
                                const float min_phi   = std::get<2>(labAngles[i]);
                                const float max_phi   = std::get<3>(labAngles[i]);

                                bool isTheta    = (min_theta <= theta) && (theta <= max_theta);
                                bool isPhi	= (min_phi <= phi) && (phi <= max_phi);
                                if (
                                        (isTheta)
                                        &&(isPhi)
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

		// Ordering of lab angles is min_theta, max_theta, min_phi, max_phi
		std::vector<std::tuple<float, float, float, float>> labAngles;

		void readAngleCalibration(){
			std::ifstream angleFile(angleCalibration);
			std::string line;
			while (std::getline(angleFile, line)){
				std::stringstream ss(line);
				std::string token;
				int counter = 0;
				float min_theta;
				float max_theta;
				float min_phi;
				float max_phi;
				while (std::getline(ss, token, ',')){
					switch(counter){
						case 0:
							min_theta = std::stof(token);
							break;
						case 1:
							max_theta = std::stof(token);
							break;
						case 2:
							min_phi = std::stof(token);
							break;
						case 3:
							max_phi = std::stof(token);
							break;
					}
					counter++;
				}
				labAngles.emplace_back(std::make_tuple(min_theta, max_theta, min_phi, max_phi));
			}
			angleFile.close();
		}

};
#endif
