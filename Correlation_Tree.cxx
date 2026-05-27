// C/C++ includes
#include <iostream>
#include <sstream>
#include <time.h>
#include <tuple>
#include <map>
#include <vector>
#include <deque>
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
#include "ame20parser.h"
#include "./Plugins/StatusMethod.h"

#define WINDOWSIZE 20

struct PairTreeBuffer{
	int multi; // microball multiplicity
        float qRel, qLong, qPerp, pairSum, kT, yPair;
        int A1, Z1, A2, Z2;

        void connect(TTree& tree){
		tree.Branch("multi",	  &multi,      "multi/I");
                tree.Branch("qRel",       &qRel,       "qRel/F");
                tree.Branch("qLong",      &qLong,      "qLong/F");
                tree.Branch("qPerp",      &qPerp,      "qPerp/F");
                tree.Branch("pairSum",    &pairSum,    "pairSum/F");
                tree.Branch("kT",         &kT,         "kT/F");
                tree.Branch("yPair",      &yPair,      "yPair/F");
                tree.Branch("A1",         &A1,         "A1/I");
                tree.Branch("Z1",         &Z1,         "Z1/I");
                tree.Branch("A2",         &A2,         "A2/I");
                tree.Branch("Z2",         &Z2,         "Z2/I");
        }

        void set(
                        const std::pair<int,int>& nz1, const std::pair<int,int>& nz2,
                        double qR, double qL, double qP, double pSum,
                        double kt, double yp, int mbMult)
        {
                int a1 = nz1.first + nz1.second; 
                int a2 = nz2.first + nz2.second;
		int z1 = nz1.second;
		int z2 = nz2.second;
                // lower A first, then lower Z
                if (
                        (a2 < a1)
                        || (
                                (a2 == a1)
                                &&(z2 < z1)
                        )
                ){
                        std::swap(a1, a2);
                        std::swap(z1, z2);
                }

                A1              = a1;
                Z1              = z1;
                A2              = a2;
                Z2              = z2;
                qRel            = static_cast<float>(qR);
                qLong           = static_cast<float>(qL);
                qPerp           = static_cast<float>(qP);
                pairSum         = static_cast<float>(pSum);
                kT              = static_cast<float>(kt);
                yPair           = static_cast<float>(yp);
		multi		= mbMult;
        }
};


// One event worth of particles, used for sliding-window mixing
struct EventBuffer {
	std::vector<TVector3> momenta;
	std::vector<double> energy;
	std::vector<std::pair<int,int>> NZ;   // (N, Z) per particle
	int multi;                            // microball multiplicity at the event
	
	// Populate from the current event's vectors
	void fill(
			const std::vector<TVector3>& mom,
			const std::vector<double>& en,
			const std::vector<std::pair<int,int>>& nz,
			int mbMulti)
	{
		momenta = mom;
		energy  = en;
		NZ      = nz;
		multi   = mbMulti;
	}
	
	size_t size() const{
		return momenta.size();
	}
};

static void mixWithWindow(
		const EventBuffer& current,
		const std::deque<EventBuffer>& window,
		PairTreeBuffer& mixBuf,
		TTree& mixTree)
{
	for (const EventBuffer& past : window){
		// For each particle in current, pair with each particle in past
		for (size_t i=0; i < current.size(); i++){
			for (size_t j=0; j < past.size(); j++){
			
				TLorentzVector p1(current.momenta[i], current.energy[i]);
				TLorentzVector p2(past.momenta[j],    past.energy[j]);
				TLorentzVector pairLV = p1 + p2;
				double yPair = pairLV.Rapidity();
				double kT    = 0.5*pairLV.Perp();
				
				TVector3 pairSum  = current.momenta[i] + past.momenta[j];
				TVector3 boostVec = pairSum*(1.0/(current.energy[i] + past.energy[j]));
				p1.Boost(-boostVec);
				p2.Boost(-boostVec);
				
				TVector3 poyntingRel = 0.5*(p1.Vect() - p2.Vect());
				double qRel  = poyntingRel.Mag();
				double qLong = poyntingRel.Z();
				double qPerp = poyntingRel.Perp();
				
				mixBuf.set(current.NZ[i], past.NZ[j],
				qRel, qLong, qPerp, pairSum.Mag(), kT, yPair,
				current.multi);

				mixTree.Fill();
			}
		}
	}
}

int main (int argc, char* argv[]){
	MassEval2020 isotopeTable("/mnt/home/brow1247/Koonin/e15190/data/mass_1.mas20");

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

        tree->StopCacheLearningPhase(); // normally you spend the first 100 events deciding what to cache. disable this

        TTreeReader reader(tree);
        TTreeReaderValue<double> MBHiraTrig(reader, "TDCTriggers.uBallHiRA_TRG");
        TTreeReaderValue<MicroballCalibratedData> microballData(reader, "uBall.");
        TTreeReaderValue<HiRACalibratedData> hiraData(reader, "HiRA.");


	// Constructing output file
	std::string outputFileName = RUN_OUTPUT_DIR + "/correlations_" + RUN_NUMBER + ".root";
	TFile* outputFile = TFile::Open(outputFileName.c_str(), "RECREATE", "", 5);
	if (!outputFile || outputFile->IsZombie()) {
		throw std::runtime_error("Could not open output file: " + outputFileName);
	}
	outputFile->cd();


	// Output TTree definition
	std::string corTreeName = "Correlated data - run " + RUN_NUMBER;
	TTree corTree("cor", corTreeName.c_str());
	corTree.SetAutoFlush(-30000000);
	PairTreeBuffer corBuf;
	corBuf.connect(corTree);

	std::string mixTreeName = "Mixed data - run " + RUN_NUMBER;
	TTree mixTree("mix", mixTreeName.c_str());
	mixTree.SetAutoFlush(-30000000);
	PairTreeBuffer mixBuf;
	mixBuf.connect(mixTree);

	// For event mixing!
	std::deque<EventBuffer> window;
	// Populating the event mixing window before calculating correlations, so we don't lose the first 20 events
	clock_t fStartTime = clock();
	/*
	while (reader.Next()){
		MicroballCalibratedData &Microball 	= *microballData;
		HiRACalibratedData &HiRA 		= *hiraData;

		if (
			(Microball.fmulti > 0)
			&&(HiRA.fmulti > 0)
		){
			// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Correlation analysis %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

			std::vector<TVector3> momenta;
			std::vector<std::pair<int, int>> NZ;
			std::vector<double> energy;

			for (int i=0; i < HiRA.fmulti; i++){
				int A = HiRA.fAId[i];
				int Z = HiRA.fZId[i];
				double particleEnergy = HiRA.fKinEnergy[i] + isotopeTable.Mass((A-Z), Z, true);
				double momentum = HiRA.fMomentum[i];
				double theta	= HiRA.fTheta[i];
				double phi	= HiRA.fPhi[i];
				double pXi 	= momentum*std::cos(phi)*std::sin(theta);
				double pYi 	= momentum*std::sin(phi)*std::sin(theta);
				double pZi 	= momentum*std::cos(theta);
				TVector3 vec(pXi, pYi, pZi);

				if (
					(momentum >= 0)
					&& (A > 0)
					&& (Z > 0)
				){
					momenta.push_back(vec);
					energy.push_back(particleEnergy);
					NZ.push_back(std::make_pair(A-Z, Z));
				}
			}

			// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Event mixing %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			EventBuffer current;
			current.fill(momenta, energy, NZ, Microball.fmulti);
			if (window.size() < WINDOWSIZE){
				window.push_back(current);
			}
			else{
				break;
			}
		}
	}
	*/

	int entry = 0;
	int totalEntries = reader.GetEntries(true);
	reader.Restart();
	while(reader.Next()){
		PrintStatus(entry, totalEntries, fStartTime);

		MicroballCalibratedData &Microball 	= *microballData;
		HiRACalibratedData &HiRA 		= *hiraData;

		if (
			(Microball.fmulti > 0)
			&&(HiRA.fmulti > 0)
		){
			// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Correlation analysis %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

			std::vector<TVector3> momenta;
			std::vector<std::pair<int, int>> NZ;
			std::vector<double> energy;

			for (int i=0; i < HiRA.fmulti; i++){
				int A = HiRA.fAId[i];
				int Z = HiRA.fZId[i];
				double particleEnergy = HiRA.fKinEnergy[i] + isotopeTable.Mass((A-Z), Z, true);
				double momentum = HiRA.fMomentum[i];
				double theta	= HiRA.fTheta[i];
				double phi	= HiRA.fPhi[i];
				double pXi 	= momentum*std::cos(phi)*std::sin(theta);
				double pYi 	= momentum*std::sin(phi)*std::sin(theta);
				double pZi 	= momentum*std::cos(theta);
				TVector3 vec(pXi, pYi, pZi);

				if (
					(momentum >= 0)
					&& (A > 0)
					&& (Z > 0)
				){
					momenta.push_back(vec);
					energy.push_back(particleEnergy);
					NZ.push_back(std::make_pair(A-Z, Z));
				}
			}

			if (momenta.size() >= 2){
				for (int i=0; i < (int)momenta.size(); i++){
					for (int j=i+1; j < (int)momenta.size(); j++){

						// Pair four-vector in lab frame (before boosting)
						TLorentzVector p1(momenta[i], energy[i]);
						TLorentzVector p2(momenta[j], energy[j]);
						TLorentzVector pairLV = p1 + p2;

						float yPair = pairLV.Rapidity();
						float kT    = 0.5 * pairLV.Perp();

						// Boost into pair rest frame
						TVector3 pairSum = (momenta[i] + momenta[j]);
						TVector3 boostVec = pairSum*(1.0/(energy[i] + energy[j]));

						p1.Boost(-boostVec);
						p2.Boost(-boostVec);

						TVector3 p1Rel = p1.Vect();
						TVector3 p2Rel = p2.Vect();

						TVector3 poyntingRel 	= 0.5*(p1Rel - p2Rel);
						float qRel 		= poyntingRel.Mag();
						float qLong 		= poyntingRel.Z();
						float qPerp		= poyntingRel.Perp();

						corBuf.set(
								NZ[i], NZ[j], 
								qRel, qLong, 
								qPerp, pairSum.Mag(), 
								kT, yPair, 
								Microball.fmulti
						);

						corTree.Fill();
					}
				}
			}


			// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Event mixing %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			EventBuffer current;
			current.fill(momenta, energy, NZ, Microball.fmulti);

			if (window.size() >= WINDOWSIZE){
				mixWithWindow(current, window, mixBuf, mixTree);
				window.pop_front();
			}
			window.push_back(current);
		}
	}


	outputFile->cd();
	corTree.Write();
	mixTree.Write();
	outputFile->Write("", TObject::kOverwrite);
	outputFile->Close();
	delete outputFile;

	// Closing input file
	file->Close();
	delete file;

	return 0;

}
