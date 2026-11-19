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

#define WINDOWSIZE 20        // events in each mixing window
#define MIX_MULTI_WIDTH 2    // mixing classes: events are mixed only with events whose Microball
                             // multiplicity falls in the same class (multi / MIX_MULTI_WIDTH)

// Where a particle hit HiRA. Two particles that share a telescope's front strip, back strip or CsI
// crystal cannot both be reconstructed in one event (Tam thesis sec. 6.1.3), so such pairs are absent
// from the same-event data and must be vetoed in the mixed data too. The veto is applied to both.
struct DetectorHit {
	int tel, stripf, stripb, csi;
};

static bool SharesDetector(const DetectorHit& a, const DetectorHit& b){
	if (a.tel != b.tel) return false;
	return (a.stripf == b.stripf)
		|| (a.stripb == b.stripb)
		|| ((a.csi >= 0) && (a.csi == b.csi));   // csi < 0: no CsI hit (stopped in the DSSSD)
}

struct PairTreeBuffer{
	int multi; // microball multiplicity
	// qOut - points along the pair's transverse momentum (the direction the pair is flying, seen from above the beam)
	// qSide - perpendicular to both the pair's direction and the beam
	// q* are in the pair rest frame. yPair, rap1, rap2, pT1, pT2, kT and pairSum are LAB frame
	// (no boost to the CM; shift by y_cm, data/y_cm.py in e15190-icer, to compare systems/energies).
        double qRel, qLong, qOut, qSide, qPerp, pairSum, kT, yPair, rap1, rap2, pT1, pT2;
        int A1, Z1, A2, Z2;

        void connect(TTree& tree){
		tree.Branch("multi",	  &multi,      "multi/I");
                tree.Branch("qRel",       &qRel,       "qRel/D");
                tree.Branch("qLong",      &qLong,      "qLong/D");
                tree.Branch("qPerp",      &qPerp,      "qPerp/D");
		tree.Branch("qOut",	  &qOut,       "qOut/D");
		tree.Branch("qSide",	  &qSide,      "qSide/D");
                tree.Branch("pairSum",    &pairSum,    "pairSum/D");
                tree.Branch("kT",         &kT,         "kT/D");
                tree.Branch("yPair",      &yPair,      "yPair/D");
                tree.Branch("A1",         &A1,         "A1/I");
                tree.Branch("Z1",         &Z1,         "Z1/I");
                tree.Branch("A2",         &A2,         "A2/I");
                tree.Branch("Z2",         &Z2,         "Z2/I");
		tree.Branch("rap1",	  &rap1,       "rap1/D");
		tree.Branch("rap2",	  &rap2,       "rap2/D");
		tree.Branch("pT1",	  &pT1,        "pT1/D");
		tree.Branch("pT2",	  &pT2,        "pT2/D");
        }

        void set(
                        const std::pair<int,int>& nz1, const std::pair<int,int>& nz2,
                        double qR, double qL, double qP, double pSum,
			double qO, double qS,
                        double kt, double yp, int mbMult,
			double r1, double r2, double pt1, double pt2)
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
			std::swap(r1, r2);
			std::swap(pt1, pt2);
			qL = -qL;
			qO = -qO;
			qS = -qS;
                }

		multi		= mbMult;
                qRel            = qR;
                qLong           = qL;
                qPerp           = qP;
		qOut		= qO;
		qSide		= qS;
                pairSum         = pSum;
                kT              = kt;
                yPair           = yp;
                A1              = a1;
                Z1              = z1;
                A2              = a2;
                Z2              = z2;
		rap1		= r1;
		rap2		= r2;
		pT1		= pt1;
		pT2		= pt2;
        }
};


// One event worth of particles, used for sliding-window mixing
struct EventBuffer {
	std::vector<TVector3> momenta;
	std::vector<double> energy;
	std::vector<std::pair<int,int>> NZ;   // (N, Z) per particle
	std::vector<DetectorHit> hits;        // detector elements per particle (for the veto)
	int multi;                            // microball multiplicity at the event
	
	// Populate from the current event's vectors
	void fill(
			const std::vector<TVector3>& mom,
			const std::vector<double>& en,
			const std::vector<std::pair<int,int>>& nz,
			const std::vector<DetectorHit>& hit,
			int mbMulti)
	{
		momenta = mom;
		energy  = en;
		NZ      = nz;
		hits    = hit;
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
		TTree& mixTree,
		long& vetoed)
{
	for (const EventBuffer& past : window){
		// For each particle in current, pair with each particle in past
		for (size_t i=0; i < current.size(); i++){
			double pT1  = current.momenta[i].Perp();
			double rap1 = 0.5*log(
					(current.energy[i] + current.momenta[i].Z())/(current.energy[i] - current.momenta[i].Z()));
			for (size_t j=0; j < past.size(); j++){
				if (SharesDetector(current.hits[i], past.hits[j])){
					vetoed++;
					continue;
				}
				double pT2  = past.momenta[j].Perp();
				double rap2 = 0.5*log((past.energy[j] + past.momenta[j].Z())/(past.energy[j] - past.momenta[j].Z()));
				
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

				// out = along the pair's transverse momentum, side = perpendicular to it and to the beam
				TVector3 outAxis(pairSum.X(), pairSum.Y(), 0.0);
				outAxis = outAxis.Unit();                          // (0,0,0) if pT_pair = 0
				TVector3 sideAxis = TVector3(0, 0, 1).Cross(outAxis);
				double qOut  = poyntingRel.Dot(outAxis);
				double qSide = poyntingRel.Dot(sideAxis);

				mixBuf.set(
						current.NZ[i], past.NZ[j],
						qRel, qLong, qPerp, pairSum.Mag(), 
						qOut, qSide,
						kT, yPair,current.multi,
						rap1, rap2, pT1, pT2
					);

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
        tree->AddBranchToCache("HiRA.fnumtel", kTRUE);
        tree->AddBranchToCache("HiRA.fnumstripf", kTRUE);
        tree->AddBranchToCache("HiRA.fnumstripb", kTRUE);
        tree->AddBranchToCache("HiRA.fnumcsi", kTRUE);

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
	std::string corTreeName = "Correlated data - run " + RUN_NUMBER + " - rap, yPair, pT, kT in lab frame; detector veto";
	TTree corTree("cor", corTreeName.c_str());
	corTree.SetAutoFlush(-30000000);
	PairTreeBuffer corBuf;
	corBuf.connect(corTree);

	std::string mixTreeName = "Mixed data - run " + RUN_NUMBER + " - rap, yPair, pT, kT in lab frame; detector veto; mixed within multiplicity classes";
	TTree mixTree("mix", mixTreeName.c_str());
	mixTree.SetAutoFlush(-30000000);
	PairTreeBuffer mixBuf;
	mixBuf.connect(mixTree);

	// For event mixing: one window per multiplicity class
	std::map<int, std::deque<EventBuffer>> windows;
	long sameVetoed = 0, mixedVetoed = 0;

	// Populating the event mixing window before calculating correlations, so we don't lose the first 20 events
	clock_t fStartTime = clock();
	int entry = 0;
	int totalEntries = reader.GetEntries(true);
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
			std::vector<DetectorHit> hits;

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
					hits.push_back({HiRA.fnumtel[i], HiRA.fnumstripf[i], HiRA.fnumstripb[i], HiRA.fnumcsi[i]});
				}
			}

			if (momenta.size() >= 2){
				for (int i=0; i < (int)momenta.size(); i++){
					double pT1  = momenta[i].Perp();
					double rap1 = 0.5*log((energy[i] + momenta[i].Z())/(energy[i] - momenta[i].Z()));
					for (int j=i+1; j < (int)momenta.size(); j++){
						// Same veto as the mixed pairs (should remove next to nothing here)
						if (SharesDetector(hits[i], hits[j])){
							sameVetoed++;
							continue;
						}
						double pT2  = momenta[j].Perp();
						double rap2 = 0.5*log((energy[j] + momenta[j].Z())/(energy[j] - momenta[j].Z()));

						// Pair four-vector in lab frame (before boosting)
						TLorentzVector p1(momenta[i], energy[i]);
						TLorentzVector p2(momenta[j], energy[j]);
						TLorentzVector pairLV = p1 + p2;

						double yPair = pairLV.Rapidity();
						double kT    = 0.5 * pairLV.Perp();

						// Boost into pair rest frame
						TVector3 pairSum = (momenta[i] + momenta[j]);
						TVector3 boostVec = pairSum*(1.0/(energy[i] + energy[j]));

						p1.Boost(-boostVec);
						p2.Boost(-boostVec);

						TVector3 p1Rel = p1.Vect();
						TVector3 p2Rel = p2.Vect();

						TVector3 poyntingRel 	= 0.5*(p1Rel - p2Rel);
						double qRel 		= poyntingRel.Mag();
						double qLong 		= poyntingRel.Z();
						double qPerp		= poyntingRel.Perp();

						// out = along the pair's transverse momentum
						// side = perpendicular to it and to the beam
						TVector3 outAxis(pairSum.X(), pairSum.Y(), 0.0);
						outAxis = outAxis.Unit(); 
						TVector3 sideAxis = TVector3(0, 0, 1).Cross(outAxis);
						double qOut  = poyntingRel.Dot(outAxis);
						double qSide = poyntingRel.Dot(sideAxis);

						corBuf.set(
								NZ[i], NZ[j], 
								qRel, qLong, 
								qPerp, pairSum.Mag(), 
								qOut, qSide,
								kT, yPair, Microball.fmulti,
								rap1, rap2, pT1, pT2
						);

						corTree.Fill();
					}
				}
			}


			// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Event mixing %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			EventBuffer current;
			current.fill(momenta, energy, NZ, hits, Microball.fmulti);

			std::deque<EventBuffer>& window = windows[Microball.fmulti / MIX_MULTI_WIDTH];
			if (window.size() >= WINDOWSIZE){
				mixWithWindow(current, window, mixBuf, mixTree, mixedVetoed);
				window.pop_front();
			}
			window.push_back(current);
		}
	}


	std::cout << "\nDetector veto: " << sameVetoed << " same-event and " << mixedVetoed
		  << " mixed-event pairs removed (shared front strip, back strip or CsI)\n";

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
