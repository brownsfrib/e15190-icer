#ifndef PARSEDATABASE_HH 
#define PARSEDATABASE_HH

// C/C++ includes
#include <iostream>
#include <vector>
#include <map>
#include <tuple>

// ROOT includes
#include "TROOT.h"
#include "TSystem.h"
#include "TSQLServer.h"
#include "TSQLResult.h"
#include "TSQLRow.h"

class ParseDatabase{
	public:
		std::string baseOutputDir;
		TSQLServer* dataBase;

		ParseDatabase(){
			std::string basePath(gSystem->Getenv("basePath"));
			this->baseOutputDir = basePath + "/outputs";

			std::string dataBasePath(gSystem->Getenv("runLogPath"));
			std::string dataBaseConnection = "sqlite://" + dataBasePath;

			this->dataBase = TSQLServer::Connect(dataBaseConnection.c_str(), "nobody", "");
			this->SetOutputMap();
		}

		~ParseDatabase(){
			this->dataBase->Close();	
		}

	private:

		std::map<Int_t, TString> PullFromDatabase(std::string command){
		        std::map<Int_t, TString> valueMap;
		        TSQLResult* results = this->dataBase->Query(command.c_str());
		        TSQLRow* reader;
		
		        Int_t counter = 0;
		        while ((reader = results->Next())){
		                if (results->GetFieldCount() == 2){ // this should ALWAYS be true for how we've constructed the database
		                        Int_t indexValue = 0;
		                        TString value;
		                        for (Int_t i=0; i < results->GetFieldCount(); i++){
		                                if (strcmp(results->GetFieldName(i), "index") == 0){
		                                        indexValue = std::stoi(reader->GetField(i));
		                                }
		                                else{
		                                        value = (TString)reader->GetField(i);
		                                }
		                        }
		                        valueMap[counter] = value;
		                        counter++;
		                }
		        }
		        return valueMap;
		}


		std::map<Int_t, std::string> outputMap;	
		void SetOutputMap(){
		        std::map<Int_t, TString> batchNum       = PullFromDatabase("SELECT * FROM ibatch");
		        std::map<Int_t, TString> runNumMin      = PullFromDatabase("SELECT * FROM run_min");
		        std::map<Int_t, TString> runNumMax      = PullFromDatabase("SELECT * FROM run_max");
		        std::map<Int_t, TString> beams          = PullFromDatabase("SELECT * FROM beam");
		        std::map<Int_t, TString> targets        = PullFromDatabase("SELECT * FROM target");
		        std::map<Int_t, TString> beamEnergies   = PullFromDatabase("SELECT * FROM beam_energy");
		
		        for (std::map<Int_t, TString>::iterator it = runNumMin.begin(); it != runNumMin.end(); it++){
		                std::string projectile(beams[it->first]);
		                std::string target(targets[it->first]);
		                std::string beamEnergy(beamEnergies[it->first]);
		
		                std::string batch(batchNum[it->first]);
		                Int_t minRun = runNumMin[it->first].Atoi();
		                Int_t maxRun = runNumMax[it->first].Atoi();
		
		                std::string createSystem = projectile + "+" + target + "@" + beamEnergy + "MeV";
				for (unsigned int run=minRun; run <= maxRun; run++){
					std::string outputPath = this->baseOutputDir + "/" + createSystem + "/";
					outputPath += runNumMin[it->first] + "_" + runNumMax[it->first] + "/";
					this->outputMap[run] = outputPath;
				}

		        }
			return;
		}
		

	public:
		std::string GetOutputPath(int runNumber){
			return this->outputMap[runNumber];
		}

};
#endif
