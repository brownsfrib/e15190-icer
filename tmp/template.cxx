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
#include "./Plugins/StatusMethod.hh"


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


// Scratch path: /scratch/e15190/runs
//ca40ni58_140/  ca40ni64_140/  ca40sn112_140/  ca40sn124_140/  ca48ni58_140/  ca48ni64_140/  ca48sn112_140/  ca48sn124_140/
//ca40ni58_56/   ca40ni64_56/   ca40sn112_56/   ca40sn124_56/   ca48ni58_56/   ca48ni64_56/   ca48sn112_56/   ca48sn124_56/

int main (int argc, char* argv[]){
	clock_t fStartTime = clock();
	
	const size_t bins = 60; // microball max multiplicities
	TH1D multHist("multHist", "", bins, 0, bins);

	std::string histTitle = "Centrality : Total Multiplicity for run 2571";
	TH2D centMult("mult_cent", histTitle.c_str(), 
			bins, 0, 1,
			bins, 0, bins);
	centMult.GetXaxis()->SetTitle("Centrality");
	centMult.GetXaxis()->CenterTitle(true);
	centMult.GetYaxis()->SetTitle("Microball + HiRA Multiplicity");
	centMult.GetYaxis()->CenterTitle(true);


	std::string runPath = "https://data.heavyion.cc/ca40ni58_140/CalibratedData_2571.root";
	TFile* file = TFile::Open(runPath.c_str(), "READ");
	TTree* tree = (TTree*)file->Get("E15190");
	tree->SetCacheSize(200*1024*1024);  // 200 MB
	tree->AddBranchToCache("uBall.fmulti", kTRUE);
	tree->AddBranchToCache("TDCTriggers.uBallHiRA_TRG");
	tree->StopCacheLearningPhase(); // normally you spend the first 100 events deciding what to cache. disable this

	// Cache the branches you need to grab, mostly HiRA
	//tree->AddBranchToCahce("*"); // cache everything! kinda unnecessary
	//tree->AddBranchToCache("HiRA.fmulti", kTRUE);
	//tree->AddBranchToCache("HiRA.fKinEnergy", kTRUE);

	TTreeReader reader(tree);
	//TTreeReaderValue<HiRACalibratedData> hiraData(reader, "HiRA.");
	TTreeReaderValue<MicroballCalibratedData> microballData(reader, "uBall.");
	TTreeReaderValue<double> MBHiraTrig(reader, "TDCTriggers.uBallHiRA_TRG");

	// Populating multiplicity distribution for given run
	int entry = 0;
	int totalEntries = 2*reader.GetEntries(true);
	while (reader.Next()){
		MicroballCalibratedData& microball	= *microballData;
                //HiRACalibratedData& hira		= *hiraData;
		PrintStatus(entry, totalEntries, fStartTime);
		bool trigger 	= *MBHiraTrig >= 0;
		int microMult 	= microball.fmulti; 
		if (
			(trigger)
			&&(microMult > 0)
		){
			multHist.Fill(microMult);
		}
	}


	reader.Restart();
	while (reader.Next()){
		MicroballCalibratedData& microball	= *microballData;
                //HiRACalibratedData& hira			= *hiraData;
		PrintStatus(entry, totalEntries, fStartTime);
		bool trigger 	= *MBHiraTrig >= 0;
		int microMult 	= microball.fmulti; 
		if (
			(trigger)
			&&(microMult > 0)
		){
			centMult.Fill(multToCent(multHist, microMult), microMult);
		}
	}

	// Close connection to input file
	file->Close();

	TFile* outputFile = TFile::Open("/mnt/home/brow1247/Koonin/e15190/output.root", "RECREATE");
	outputFile->cd();
	centMult.SetOption("colz");
	centMult.Write();
	outputFile->Write("", TObject::kOverwrite);
	outputFile->Close();


  return 0;

}
