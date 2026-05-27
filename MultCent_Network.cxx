// C/C++ includes
#include <iostream>
#include <sstream>
#include <time.h>
#include <tuple>
#include <map>
#include <vector>
#include <functional>
#include <algorithm>

// ROOT includes 
#include <TROOT.h>
#include <TApplication.h>
#include <TSystem.h> 
#include <TTreeReader.h>
#include <TSystem.h>
#include <TTreeReaderValue.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TH2.h>
#include <THnSparse.h>
#include <TH3.h>
#include <TF1.h>
#include <TLorentzVector.h>
#include <TRandom.h>
#include <TEnv.h>

// Framework includes 
#include "HiRACalibratedRootEvent.h"
#include "MBCalibratedRootEvent.h"

// Sam's includes (primarily for flow analysis)
#include "ame20parser.h"
#include "./Plugins/StatusMethod.h"


// Matches Agnieszka's definition - no change in multiplicity -> centrality mapping
double multToCent(TH1D& hist, int mult){
	double sumAbove = 0.0;
	int bin = hist.FindBin(mult);
	for (int b=hist.GetNbinsX(); b >= bin; b--){
		sumAbove += hist.GetBinContent(b);
	}
	return sumAbove/hist.Integral(); // 0 = central, 1 = peripheral
}


/*
TTreeCache notes: 
	- ROOT files stores data in baskets, typically a few hundred kB
	- Without a cache, each "fetch next basket" is a seperate HTTP request
	- Basically you fill the cache and heavily limit the amount of HTTP requests you make
	- Instead of hundreds of small reads, you get a few reads of larger sizes
*/

int main (int argc, char* argv[]){
	// Getting environment variables - we need all of them!
	std::string RUN_NUMBER(gSystem->Getenv("RUN_NUMBER"));
	if (RUN_NUMBER[0] == '\0'){
		std::string message = "\033[1;31mRUN_NUMBER not set!\033[0m\n";
		throw std::runtime_error(message);
	}

	std::string RUN_INPUT_PATH(gSystem->Getenv("RUN_INPUT_PATH"));
	if (RUN_INPUT_PATH[0] == '\0'){
		std::string message = "\033[1;31mRUN_INPUT_PATH not set!\033[0m\n";
		throw std::runtime_error(message);
	}

	std::string RUN_OUTPUT_DIR(gSystem->Getenv("RUN_OUTPUT_DIR"));
	if (RUN_OUTPUT_DIR[0] == '\0'){
		std::string message = "\033[1;31mRUN_OUTPUT_DIR not set!\033[0m\n";
		throw std::runtime_error(message);
	}
	
	const size_t bins = 60; // microball max multiplicities
	TH1D multHist("multHist", "", bins, 0, bins);
	std::string histTitle = "Centrality : Total Multiplicity for run " + RUN_NUMBER;
	TH2D centMult("cent_mult", histTitle.c_str(), 
			bins, 0, 1,
			bins, 0, bins);
	centMult.GetXaxis()->SetTitle("Centrality");
	centMult.GetXaxis()->CenterTitle(true);
	centMult.GetYaxis()->SetTitle("Microball Multiplicity (Microball + HiRA Trigger)");
	centMult.GetYaxis()->CenterTitle(true);

	TFile* file = TFile::Open(RUN_INPUT_PATH.c_str(), "READ");
	if ((!file) || (file->IsZombie())){
		std::string message = "\033[1;31mSomething lurks .... you're file " + RUN_INPUT_PATH + " cannot be opened!\033[0m\n";
		throw std::runtime_error(message);
	}
	TTree* tree = (TTree*)file->Get("E15190");
	tree->SetCacheSize(200*1024*1024);  // 200 MB
	tree->AddBranchToCache("uBall.fmulti", kTRUE);
	tree->AddBranchToCache("TDCTriggers.uBallHiRA_TRG");
	tree->StopCacheLearningPhase(); // normally you spend the first 100 events deciding what to cache. disable this

	TTreeReader reader(tree);
	TTreeReaderValue<MicroballCalibratedData> microballData(reader, "uBall.");
	TTreeReaderValue<double> MBHiraTrig(reader, "TDCTriggers.uBallHiRA_TRG");

	// Populating multiplicity distribution for given run
	int entry = 0;
	int totalEntries = 2*reader.GetEntries(true);
	clock_t fStartTime = clock();
	while (reader.Next()){
		MicroballCalibratedData& microball    = *microballData;
		PrintStatus(entry, totalEntries, fStartTime);
		bool trigger     = *MBHiraTrig >= 0;
		int microMult     = microball.fmulti; 
		if (
			(trigger)
			&&(microMult > 0)
		){
			multHist.Fill(microMult);
		}
	}

	reader.Restart();
	while (reader.Next()){
		MicroballCalibratedData& microball    = *microballData;
		PrintStatus(entry, totalEntries, fStartTime);
		bool trigger     = *MBHiraTrig >= 0;
		int microMult     = microball.fmulti; 
		if (
			(trigger)
			&&(microMult > 0)
		){
			centMult.Fill(multToCent(multHist, microMult), microMult);
		}
	}

	// Close connection to input file
	file->Close();
	if (centMult.GetEntries() != 0){
		std::string outputFileName = RUN_OUTPUT_DIR + "/cent_mult_" + RUN_NUMBER + ".root";
		TFile* outputFile = TFile::Open(outputFileName.c_str(), "RECREATE");
		outputFile->cd();
		centMult.SetOption("colz");
		centMult.Write();
		outputFile->Write("", TObject::kOverwrite);
		outputFile->Close();
	}

	return 0;
}
