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
#include <TAxis.h>
#include <TLorentzVector.h>
#include <TRandom.h>
#include <TEnv.h>

// Framework includes 
#include "HiRACalibratedRootEvent.h"
#include "MBCalibratedRootEvent.h"

// Sam's includes (primarily for flow analysis)
#include "ame20parser.h"
#include "./Plugins/StatusMethod.h"

/*
TTreeCache notes: 
	- ROOT files stores data in baskets, typically a few hundred kB
	- Without a cache, each "fetch next basket" is a seperate HTTP request
	- Basically you fill the cache and heavily limit the amount of HTTP requests you make
	- Instead of hundreds of small reads, you get a few reads of larger sizes
*/

int main (int argc, char* argv[]){
	
	MassEval2020 isotopeTable("/mnt/home/brow1247/Koonin/e15190/data/mass_1.mas20");

	const size_t bins = 500; // microball max multiplicities
	TH2D phaseSpace("phaseSpace", "y : p_{T}/A",
			bins, -0.4, 0.4,
			bins, 0, 1000);
	phaseSpace.GetXaxis()->SetTitle("y_{lab} - y_{cm}");
	phaseSpace.GetXaxis()->CenterTitle(true);
	phaseSpace.GetYaxis()->SetTitle("p_{T}/A");
	phaseSpace.GetYaxis()->CenterTitle(true);

	const std::string network_file = "https://data.heavyion.cc/ca40ni58_140/CalibratedData_2632.root";
	TFile* file = TFile::Open(network_file.c_str(), "READ");
	if ((!file) || (file->IsZombie())){
		std::string message = "\033[1;31mSomething lurks .... your file cannot be opened!\033[0m\n";
		throw std::runtime_error(message);
	}
	std::cout << "\033[1;32mSuccessfully opened " << network_file << "!\033[0m\n";
	TTree* tree = (TTree*)file->Get("E15190");
	tree->SetCacheSize(200*1024*1024);  // 200 MB
	// HiRA caching
	tree->AddBranchToCache("HiRA.fmulti", kTRUE);
        tree->AddBranchToCache("HiRA.fAId", kTRUE);
        tree->AddBranchToCache("HiRA.fZId", kTRUE);
        tree->AddBranchToCache("HiRA.fKinEnergy", kTRUE);
        tree->AddBranchToCache("HiRA.fMomentum", kTRUE);
        tree->AddBranchToCache("HiRA.fPhi", kTRUE);
        tree->AddBranchToCache("HiRA.fTheta", kTRUE);
	// Trigger caching
	tree->AddBranchToCache("TDCTriggers.uBallHiRA_TRG");
	tree->StopCacheLearningPhase(); // normally you spend the first 100 events deciding what to cache. disable this

	TTreeReader reader(tree);
	TTreeReaderValue<double> trigger(reader, "TDCTriggers.uBallHiRA_TRG");
	TTreeReaderValue<HiRACalibratedData> hiraData(reader, "HiRA.");

	const float y_cm = 0.348685850975481;
	const float y_beam = 0.222337430585781;

	// Populating multiplicity distribution for given run
	int entry = 0;
	int totalEntries = reader.GetEntries(true);
	clock_t fStartTime = clock();
	while (reader.Next()){
		PrintStatus(entry, totalEntries, fStartTime);
		HiRACalibratedData& hira = *hiraData;
		for (int i=0; i < hira.fmulti; i++){
			int A = hira.fAId[i];
			int Z = hira.fZId[i];
			if (
				(hira.fKinEnergy[i] != -9999)
			){

				double particleEnergy   = hira.fKinEnergy[i] + \
							  isotopeTable.Mass((A-Z), Z, true); // MeV
				double momentum         = hira.fMomentum[i];
				double theta            = hira.fTheta[i];
				double phi              = hira.fPhi[i];
				double pXi              = momentum*std::cos(phi)*std::sin(theta);
				double pYi              = momentum*std::sin(phi)*std::sin(theta);
				double pZi              = momentum*std::cos(theta);
				TVector3 vec(pXi, pYi, pZi);
				
				double rapidity= 0.5*log((particleEnergy + pZi)/(particleEnergy - pZi));
				double pT      = sqrt(pow(pXi, 2.0) + pow(pYi, 2.0));

				rapidity -= y_cm;
				//rapidity /= y_beam;
				phaseSpace.Fill(rapidity, pT/(double)A);
			}

		}
	}

	TApplication app("app", &argc, argv);
	TCanvas can("can", "", 800, 600);
	can.SetGrid();
	phaseSpace.Draw("colz");
	can.Update();
	app.Run();

	return 0;
}
