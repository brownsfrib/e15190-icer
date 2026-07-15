// C/C++ includes
#include <iostream>
#include <sstream>
#include <time.h>
#include <tuple>
#include <map>
#include <complex>
#include <vector>
#include <functional>
#include <algorithm>

// ROOT includes 
#include "TROOT.h"
#include "TApplication.h"
#include "TSystem.h" 
#include "TGraphErrors.h"
#include "TTreeReader.h"
#include "TSystem.h"
#include "TTreeReaderValue.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TH2.h"
#include "THnSparse.h"
#include "TH3.h"
#include "TF1.h"
#include "TLorentzVector.h"
#include "TRandom.h"
#include "TEnv.h"

// Framework includes 
#include "HiRACalibratedRootEvent.h"
#include "MBCalibratedRootEvent.h"

// Sam's includes (primarily for flow analysis)
#include "ame20parser.h"
#include "./Plugins/StatusMethod.h"


// Holds accumulator data, just to get rid of some definition bloat in main
// Defined per multiplicity bin!
struct RefCell{
	double sumW 		= 0.0;
	double sumW_2 		= 0.0;
	double sumW_C2		= 0.0;
	double sumW_C2_2	= 0.0;
	double Qx		= 0.0;
	double Qy		= 0.0;
	double sumM		= 0.0;
	int nEvents		= 0.0;
};

// All reference flow calculated from the microball!
struct RefTree{
	TTree* tree;
	std::string treeName;
	std::string treeDesc;

	// Write out variables
	// Standard event variables (only one)
	int microball_mult; // (M) centrality -> multiplicity mappings are used later

	// Accumulated during event analysis
	double sumW; 		// sum of weights (w) defined as w = M(M-1)
	double sumW_2; 		// sum of squared weights (exponents are separated by _ in this code
	double sumW_C2; 	// sum of weights product with reference flow (c{2})
	double sumW_C2_2; 	// sum of weights product with reference flow squared (c{2})^2
	double Qx; 		// self explanatory at this point - read the paper if you don't know
	double Qy; 		// self explanatory at this point - read the paper if you don't know
	double sumM; 		// sum of all event multiplicity (M) from microball
	int nEvents; 		// number of events processed 

	

	RefTree(TTree* tree_in) : tree(tree_in){
		tree->Branch("M", 		&microball_mult, 	"M/I");
		tree->Branch("sumW", 		&sumW, 			"sumW/D");
		tree->Branch("sumW_2", 		&sumW_2, 		"sumW_2/D");
		tree->Branch("sumW_C2", 	&sumW_C2, 		"sumW_C2/D");
		tree->Branch("sumW_C2_2", 	&sumW_C2_2, 		"sumW_C2_2/D");
		tree->Branch("Qx", 		&Qx, 			"Qx/D");
		tree->Branch("Qy", 		&Qy, 			"Qy/D");
		tree->Branch("sumM",		&sumM,			"sumM/D");
		tree->Branch("nEvents", 	&nEvents, 		"nEvents/I");
	}


};

// Holds accumulator data, just to get rid of some definition bloat in main
// Defined per multiplicity bin!
struct DifCell{
	double sumWp		= 0.0;
	double sumWp_2		= 0.0;
	double sumWp_C2p	= 0.0;
	double sumWp_C2p_2	= 0.0;
	double sumWp_W		= 0.0;
	double sumWp_W_C2_C2p	= 0.0;
	double sumPx		= 0.0;
	double sumPy		= 0.0;
	int sumMp		= 0.0;
};

// Differential flow (on rapidity and pT axes) from Hira, using microball reference data!
struct DifTree{
	TTree* tree;
	std::string treeName;
	std::string treeDesc;

	// Write out variables
	// Standard event variables
	int microball_mult; 	// microball multiplicity, necessary for calculating weights
	int A; 			// Mass number of particle
	int Z; 			// Proton number of particle
	int rapBin; 		// bin from a priori defined TAxis on the rapidity axis (per particle)
	int ptBin; 		// bin from a priori defined TAxis on the transverse momentum axis (per particle)
	double sumWp; 		// p denotes 'prime' (w') - multiplicity of POIs product with microball multiplicity
	double sumWp_2; 	// sum of squares of weights (w')^2
	double sumWp_C2p; 	// sum of w' product with <2'> defined by POIs
	double sumWp_C2p_2; 	// sum of w' product with (<2'>)^2
	double sumWp_W; 	// weight (microball) product with w'
	double sumWp_W_C2_C2p; 	// weight (microball) product with w' and <2> and <2'> (<2'> comes from POI)
	double sumPx; 		// self explanatory - POI Q vector component
	double sumPy; 		// self explanatory - POI Q vector component
	int sumMp; 		// sum of POI multiplicity

	DifTree(TTree* tree_in) : tree(tree_in){
		tree->Branch("M", 		&microball_mult, 	"M/I");
		tree->Branch("A", 		&A, 			"A/I");
		tree->Branch("Z", 		&Z, 			"Z/I");
		tree->Branch("rapBin", 		&rapBin, 		"rapBin/I");
		tree->Branch("ptBin", 		&ptBin, 		"ptBin/I");
		tree->Branch("sumWp", 		&sumWp, 		"sumWp/D");
		tree->Branch("sumWp_2",		&sumWp_2,		"sumWp_2/D");
		tree->Branch("sumWp_C2p", 	&sumWp_C2p,		"sumWp_C2p/D");
		tree->Branch("sumWp_C2p_2", 	&sumWp_C2p_2,		"sumWp_C2p_2/D");
		tree->Branch("sumWp_W", 	&sumWp_W,		"sumWp_W/D");
		tree->Branch("sumWp_W_C2_C2p", 	&sumWp_W_C2_C2p,	"sumWp_W_C2_C2p/D");
		tree->Branch("sumPx",		&sumPx,			"sumPx/D");
		tree->Branch("sumPy",		&sumPy,			"sumPy/D");
		tree->Branch("sumMp", 		&sumMp,			"sumMp/I");
	};


};



/*
TTreeCache notes: 
	- ROOT files stores data in baskets, typically a few hundred kB
	- Without a cache, each "fetch next basket" is a seperate HTTP request
	- Basically you fill the cache and heavily limit the amount of HTTP requests you make
	- Instead of hundreds of small reads, you get a few reads of larger sizes
*/

// -------------------------------------
// -------------------------------------
// Basing this study (run by run for now) off of this paper: 
// -------------------------------------
// -------------------------------------
// https://arxiv.org/pdf/nucl-ex/0110016
// -------------------------------------
// -------------------------------------

// TODO include multiple harmonics in output file
#define harmonic 1.0
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

	std::string RUN_Y_CM(gSystem->Getenv("RUN_Y_CM"));
	if (RUN_Y_CM[0] == '\0'){
		std::string message = "\033[1;31mRUN_Y_CM not set! Check ./data/runList.csv or your python submission script, it might be corrupted\033[0m\n";
		throw std::runtime_error(message);
	}
	const double y_cm = std::stod(RUN_Y_CM);

	MassEval2020 isotopeTable("/mnt/home/brow1247/Koonin/e15190/data/mass_1.mas20");
	
	TFile* file = TFile::Open(RUN_INPUT_PATH.c_str(), "READ");
	if ((!file) || (file->IsZombie())){
		std::string message = "\033[1;31mSomething lurks .... you're file " + RUN_INPUT_PATH + " cannot be opened!\033[0m\n";
		throw std::runtime_error(message);
	}
	TTree* tree = (TTree*)file->Get("E15190");
        tree->SetCacheSize(200*1024*1024);  // 200 MB
        // TDC Caching
        tree->AddBranchToCache("TDCTriggers.uBallHiRA_TRG");
        // Microball caching
        tree->AddBranchToCache("uBall.fmulti", kTRUE);
        // HiRA caching 
	tree->AddBranchToCache("HiRA.fmulti", kTRUE); 
	tree->AddBranchToCache("HiRA.fAId", kTRUE); 
	tree->AddBranchToCache("HiRA.fZId", kTRUE);
        tree->AddBranchToCache("HiRA.fKinEnergy", kTRUE);
        tree->AddBranchToCache("HiRA.fMomentum", kTRUE);
        tree->AddBranchToCache("HiRA.fPhi", kTRUE);
        tree->AddBranchToCache("HiRA.fTheta", kTRUE);
	// normally you spend the first 100 events deciding what to cache. disable this
	tree->StopCacheLearningPhase(); 

	TTreeReader reader(tree);
	TTreeReaderValue<double> MBHiraTrig(reader, "TDCTriggers.uBallHiRA_TRG");
	TTreeReaderValue<MicroballCalibratedData> microballData(reader, "uBall.");
        TTreeReaderValue<HiRACalibratedData> hiraData(reader, "HiRA.");

	std::string outputFileName = RUN_OUTPUT_DIR + "/cumulant_" + RUN_NUMBER + ".root";
	TFile* outputFile = TFile::Open(outputFileName.c_str(), "RECREATE", "", 5);
	if ((!outputFile) || (outputFile->IsZombie())){
		throw std::runtime_error("Could not open output file: " + outputFileName);
	}
	outputFile->cd();

	// -------------------------------------------------
	// --- Defining reference and differential trees ---
	// -------------------------------------------------

	std::string refTitle = "reference flow data - all microball data";
	TTree refTree_out("refTree", refTitle.c_str());
	RefTree refTree(&refTree_out);

	std::string difTitle = "differential (y and pT) flow data from HiRA";
	TTree difTree_out("difTree", difTitle.c_str());
	DifTree difTree(&difTree_out);

	// -------------------------------------------------
	// --- Defining reference and differential trees ---
	// -------------------------------------------------


	// -------------------------------------------------
	// ------------ Defining accumulators --------------
	// -------------------------------------------------

	std::map<int, RefCell> refCells;				 // keyed as :-> M
	std::map<std::tuple<int, int, int, int, int>, DifCell> difCells; // keyed as :-> A, Z, M, rapBin, ptBin

	// -------------------------------------------------
	// ------------ Defining accumulators --------------
	// -------------------------------------------------

	// -------------------------------------------------
	// ---------- Defining rapidity and pT axes --------
	// -------------------------------------------------
	const TAxis rapAxis(10, -0.2, 0.2);
	const TAxis ptAAxis(10, 50, 400);

	// -------------------------------------------------
	// ---------- Defining rapidity and pT axes --------
	// -------------------------------------------------

	while (reader.Next()){
		MicroballCalibratedData& microball = *microballData;
		HiRACalibratedData& hira	   = *hiraData;
		if (
			(microball.fmulti > 1)
			&&(hira.fmulti > 1)
		){

			// Calculate Q vector for all particles
			std::complex<double> Qn(0.0, 0.0);	

			for (int i=0; i < microball.fmulti; i++){
				Qn += std::complex<double>(
                                                std::cos(harmonic*microball.fPhi[i]),
                                                std::sin(harmonic*microball.fPhi[i])
                                                );
			}

			std::map<std::tuple<int, int, int, int, int>, std::complex<double>> pEv;
			std::map<std::tuple<int, int, int, int, int>, int> 		    mEv;

			for (int i=0; i < hira.fmulti; i++){
				const int A 		= hira.fAId[i];
				const int Z 		= hira.fZId[i];
				if (
					(A <= 0)
					||(Z <= 0)
					||(A < Z)
				){
					continue;
				}
				const double phi 	= hira.fPhi[i];
				const double theta 	= hira.fTheta[i];
				const double momenta 	= hira.fMomentum[i];
				TVector3 track(
						std::cos(phi)*std::sin(theta), 
						std::sin(phi)*std::sin(theta), 
						std::cos(theta)
					      );
				track *= momenta;
				const double ptA	= track.Perp()/(double)A; 
				const double energy 	= hira.fKinEnergy[i] + \
							  isotopeTable.Mass((A-Z), Z, true);
				double rap		= 0.5*log((energy + track.Z())/(energy - track.Z()));
				rap -= y_cm;
				int rap_Bin 		= rapAxis.FindBin(rap);
				int pt_Bin 		= ptAAxis.FindBin(ptA);

				std::tuple<int, int, int, int, int> key = 
					std::make_tuple(A, Z, microball.fmulti, rap_Bin, pt_Bin);

				pEv[key] += std::complex<double>(
						std::cos(harmonic*phi),
						std::sin(harmonic*phi));
				mEv[key] += 1;
			}

			// Now we gotta calculate some weights and shit
			const double w 		= (double)(microball.fmulti)*((double)microball.fmulti - 1.0);
			const double c2_event 	= (std::norm(Qn) - (double)microball.fmulti)/w; 

			RefCell& refCell 	= refCells[microball.fmulti];
			refCell.sumW 		+= w;
			refCell.sumW_2 		+= w*w;
			refCell.sumW_C2 	+= w*c2_event;
			refCell.sumW_C2_2 	+= w*c2_event*c2_event;
			refCell.Qx		+= std::real(Qn);
			refCell.Qy		+= std::imag(Qn);
			refCell.sumM		+= microball.fmulti;
			refCell.nEvents		+= 1;

			std::map<std::tuple<int, int, int, int, int>, std::complex<double>>::iterator it;
			for (it = pEv.begin(); it != pEv.end(); it++){
				const double wp		= (double)(mEv[it->first])*(double)(microball.fmulti);
				double const c2p_event 	= std::real(it->second*std::conj(Qn))/wp;
				DifCell& difCell 	= difCells[it->first];
				difCell.sumWp		+= wp;
				difCell.sumWp_W		+= wp*w;
				difCell.sumWp_2		+= wp*wp;
				difCell.sumWp_C2p	+= wp*c2p_event;
				difCell.sumWp_C2p_2	+= wp*c2p_event*c2p_event;
				difCell.sumWp_W_C2_C2p	+= wp*w*c2p_event*c2_event;
				difCell.sumPx		+= std::real(it->second);
				difCell.sumPy		+= std::imag(it->second);
				difCell.sumMp		+= mEv[it->first];
			}
		}
	}

	std::map<int, RefCell>::iterator refIt;
	for (refIt = refCells.begin(); refIt != refCells.end(); refIt++){
		RefCell& refCell 	= refIt->second;
		refTree.microball_mult 	= refIt->first;
		refTree.sumW		= refCell.sumW; 
		refTree.sumW_2		= refCell.sumW_2; 
		refTree.sumW_C2		= refCell.sumW_C2; 
		refTree.sumW_C2_2	= refCell.sumW_C2_2; 
		refTree.Qx		= refCell.Qx;
		refTree.Qy		= refCell.Qy;
		refTree.sumM		= refCell.sumM;
		refTree.nEvents		= refCell.nEvents;
		refTree_out.Fill();

	}

	std::map<std::tuple<int, int, int, int, int>, DifCell>::iterator difIt;
	for (difIt = difCells.begin(); difIt != difCells.end(); difIt++){
		const std::tuple<int, int, int, int, int>& key = difIt->first;
		DifCell& difCell 	= difIt->second;
		difTree.A              	= std::get<0>(key);
		difTree.Z              	= std::get<1>(key);
		difTree.microball_mult 	= std::get<2>(key);
		difTree.rapBin         	= std::get<3>(key);
		difTree.ptBin          	= std::get<4>(key);

		difTree.sumWp		= difCell.sumWp;
		difTree.sumWp_2		= difCell.sumWp_2;
		difTree.sumWp_C2p	= difCell.sumWp_C2p;
		difTree.sumWp_C2p_2	= difCell.sumWp_C2p_2;
		difTree.sumWp_W		= difCell.sumWp_W;
		difTree.sumWp_W_C2_C2p	= difCell.sumWp_W_C2_C2p;
		difTree.sumPx		= difCell.sumPx;
		difTree.sumPy		= difCell.sumPy;
		difTree.sumMp		= difCell.sumMp;
		difTree_out.Fill();
	}


	outputFile->cd();
	rapAxis.Write("rapAxis");
	ptAAxis.Write("ptAAxis");
	refTree_out.Write();
	difTree_out.Write();
	outputFile->Write("", TObject::kOverwrite);
	outputFile->Close();
	delete outputFile;

	file->Close();
	delete file;

	return 0;
}
