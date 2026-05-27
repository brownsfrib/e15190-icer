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
#include <TChain.h>
#include <TH2.h>
#include <THnSparse.h>
#include <TH3.h>
#include <TF1.h>
#include <TLorentzVector.h>
#include <TRandom.h>
#include <TEnv.h>

// Framework includes 
#include <E15190Reader.h>
#include <HTExperimentInfo.h>
#include <HTRunInfo.h>

// Sam's includes (primarily for flow analysis)
#include "ame20parser.h"
#include "./Plugins/StatusMethod.hh"
#include "./Plugins/BeamRelativisticParams.hh"
#include "./Plugins/ParseDatabase.hh"


#define WINDOWSIZE 20


// Structure to hold event data for event mixing
struct Queue{
	static const size_t maxHiraMult = 12;
	int eventIdx;
	int hiraMult;
	double bhat;
	int A[maxHiraMult];
	int Z[maxHiraMult];
	double energy[maxHiraMult];
	TVector3 momenta[maxHiraMult];

	void Initialize(){
		hiraMult 	= -100;
		bhat 		= -100;
		for (int i=0; i < (int)maxHiraMult; i++){
			A[i] 		= -100;
			Z[i] 		= -100;
			energy[i] 	= -100;
			momenta[i].SetXYZ(-100, -100, -100);
		}
	}

	void Dump(){
		std::cout << "\033[1;33mEvent " << eventIdx << " Data: \033[0m\n";
		std::cout << "Reduced impact: " << bhat << '\t';
		std::cout << "Hira mult: " << hiraMult << '\n';
		for (int i=0; i < hiraMult; i++){
			std::cout << "Index: " << i << '\t';
			std::cout << "A: " << A[i] << '\t';
			std::cout << "Z: " << Z[i] << '\t';
			std::cout << "Energy: " << energy[i] << '\t';
			std::cout << "Momentum: " << momenta[i].Mag() << '\n';
		}
		std::cout << '\n';
	}
};

struct Mixed{
	static const size_t maxMixed = 12*WINDOWSIZE;
	double bhat[maxMixed];
	int A[maxMixed];
	int Z[maxMixed];
	double energy[maxMixed]; 
	TVector3 momenta[maxMixed];

	void Initialize(){
		for (int i=0; i < (int)maxMixed; i++){
			bhat[i] 	= -100;
			A[i] 		= -100;
			Z[i] 		= -100;
			energy[i] 	= -100;
			momenta[i].SetXYZ(-100, -100, -100);
		}
	}
};


struct Particles{
	std::pair<int, int> A;
	std::pair<int, int> B;

	void Dump(){
		std::cout << "Particle A: " << A.first << '\t' << A.second << '\n';
		std::cout << "Particle B: " << B.first << '\t' << B.second << '\n';
	}
};

static inline bool lessAZ(const std::pair<int,int>& x, const std::pair<int,int>& y){
	if (x.first != y.first){
		return x.first < y.first;
	}
	return x.second < y.second;
}

struct ParticlesLess {
    bool operator()(const Particles& p1, const Particles& p2) const {
        if (p1.A.first != p2.A.first){
		return p1.A.first < p2.A.first;
	}
        if (p1.A.second!= p2.A.second){
		return p1.A.second< p2.A.second;
	}
        if (p1.B.first != p2.B.first){
		return p1.B.first < p2.B.first;
	}
        return p1.B.second < p2.B.second;
    }
};

static inline Particles makeKey(const std::pair<int,int>& a, const std::pair<int,int>& b){
	Particles k;
	if (lessAZ(a,b) || (a==b)){ 
		k.A = a; k.B = b; 
	}
	else{ 
		k.A = b; k.B = a;
	}
	return k;
}

struct Hists{
	Particles particles;
	THnSparseD* cor_rel_total;
	THnSparseD* mix_rel_total;
};


static std::vector<double> parseEdges(const std::string& s){
	std::vector<double> v;
	std::stringstream ss(s);
	double d;
	while (ss >> d) v.push_back(d);
	return v;
}

static int findBin(double val, const std::vector<double>& edges){
	for (int i=0; i < (int)edges.size()-1; i++){
		bool inBin0 = (val >= edges[i]);
		bool inBin1 = (i == (int)edges.size()-2 ? val <= edges[i+1] : val < edges[i+1]);
		if ((inBin0) && (inBin1)){
			return i;
		}
	}
	return -1;
}

static std::string fmt(double v){
	std::ostringstream s;
	s << std::fixed << std::setprecision(2) << v;
	return s.str();
}


int main (int argc, char* argv[]){

	int runNumber = std::stoi(argv[1]);
	std::string configPath = (argc > 2) ? argv[2] : "input.env";
	TEnv env(configPath.c_str());
	std::vector<double> centEdges = parseEdges(env.GetValue("Correlation.CentEdges", ""));
	const int nCent = (int)centEdges.size() - 1;

	// Building Experiment Info class
	HTExperimentInfo * ExpInfo = new HTExperimentInfo();
	ExpInfo->InitClass("config/HiRAEVT.conf");
	
	// Initializing input file
	std::string data_path(ExpInfo->GetAnalyzedRootFilePath());
	TChain* dataChain = new TChain(Form("E%s",ExpInfo->GetName()));

	clock_t fStartTime = clock();

	// 6D THnSparseD: q_rel, q_long, q_perp, pair_sum, kT, y_pair
	int qRelBins 	= 2000;
	int qLongBins 	= 2000;
	int qPerpBins 	= 2000;
	int qSumBins 	= 2000;
	int kTBins	= 2000;
	int yPairBins	= 2000;

	const int coordSize		= 6;
	int coordBins[coordSize] 	= {qRelBins, qLongBins, qPerpBins, qSumBins, kTBins, yPairBins};
	double coordMin[coordSize] 	= {0.0, -1000.0, 0.0, 0.0, 0.0, 0.0};
	double coordMax[coordSize] 	= {2000.0, 1000.0, 2000.0, 6000.0, 4000.0, 1.0};

	// Creation of mass table
	std::string tablePath = PROJECT_ROOT "/data/mass_1.mas20";
	MassEval2020 isotopeTable(tablePath);

	// N then Z (not A!)
	const size_t uniquePairs = 5;
	std::vector<std::pair<int, int>> particleID;
	particleID.push_back(std::make_pair(0, 1)); // proton
	particleID.push_back(std::make_pair(1, 1)); // deuteron
	particleID.push_back(std::make_pair(2, 1)); // triton
	particleID.push_back(std::make_pair(1, 2)); // He3
	particleID.push_back(std::make_pair(2, 2)); // alpha

	// One full set of particle-pair histograms per centrality bin
	std::vector<std::map<Particles, Hists, ParticlesLess>> histSets(nCent);
	int counter = 0;
	for (int c = 0; c < nCent; c++){
		for (int i=0; i < (int)uniquePairs; i++){
			Hists hist;
			for (int j=i; j < (int)uniquePairs; j++){
				Particles particles = makeKey(particleID[i], particleID[j]);
				hist.particles = particles;
				std::string tag = std::to_string(c) + "_" + std::to_string(counter);

				std::string cor_rel_total = "cor_rel_total_" + tag;
				hist.cor_rel_total = new THnSparseD(cor_rel_total.c_str(), "Correlated events", 
						coordSize, coordBins, coordMin, coordMax);
				hist.cor_rel_total->GetAxis(0)->SetTitle("Q (MeV/c)");
				hist.cor_rel_total->GetAxis(1)->SetTitle("Q_{Z} (MeV/c)");
				hist.cor_rel_total->GetAxis(2)->SetTitle("Q_{Perp} (MeV/c)");
				hist.cor_rel_total->GetAxis(3)->SetTitle("Pair_{Sum} (MeV/c)");
				hist.cor_rel_total->GetAxis(4)->SetTitle("k_{T} (MeV/c)");
				hist.cor_rel_total->GetAxis(5)->SetTitle("y_{pair}");

				std::string mix_rel_total = "mix_rel_total_" + tag;
				hist.mix_rel_total = new THnSparseD(mix_rel_total.c_str(), "Mixed events", 
						coordSize, coordBins, coordMin, coordMax);
				hist.mix_rel_total->GetAxis(0)->SetTitle("Q (MeV/c)");
				hist.mix_rel_total->GetAxis(1)->SetTitle("Q_{Z} (MeV/c)");
				hist.mix_rel_total->GetAxis(2)->SetTitle("Q_{Perp} (MeV/c)");
				hist.mix_rel_total->GetAxis(3)->SetTitle("Pair_{Sum} (MeV/c)");
				hist.mix_rel_total->GetAxis(4)->SetTitle("k_{T} (MeV/c)");
				hist.mix_rel_total->GetAxis(5)->SetTitle("y_{pair}");

				histSets[c][particles] = hist;
				counter++;
			}
		}
	}

	ParseDatabase dataBase;
	std::string runNumber_name = std::to_string(runNumber);

	// Setting filename and testing for existence 
	const char* inputFileName = Form("%sCalibratedData_%04d.root", data_path.c_str(), runNumber);
	if (gSystem->AccessPathName(inputFileName)){
		std::string message = "\033[1;31mError in finding input file for run number : " + runNumber_name + ".\033[0m";
		throw std::runtime_error(message);
	}
	
	//Building HTRunInfo class ///////////
	HTRunInfo * CurrRunInfo = ExpInfo->GetRunInfo(runNumber);
	std::string beamProfile(CurrRunInfo->GetBeam());
	double beamEnergy = std::stod(CurrRunInfo->GetBeamEnergy());
	std::string targetProfile(CurrRunInfo->GetTarget());

	if(CurrRunInfo->IsJunk()){
		std::cout << "\033[1;31mRun " << runNumber;
		std::cout << " is junk -> the run will be skipped.\033[0m" << '\n';
		throw std::runtime_error("");
	}

	std::cout << "Run Title: " << CurrRunInfo->GetTitle() << '\n';
	std::cout << "Beam: " << beamProfile << " @ ";
	std::cout << beamEnergy << " MeV/u\n";
	std::cout << "Target: " << targetProfile << '\n';

	int n_files = dataChain->Add(inputFileName);
	if (n_files <= 0){
		std::string message = "\033[1;31m" + std::to_string(n_files);
		message += " Root files added to chain for run " + runNumber_name + ". Exiting.\033[1;31m";
		throw std::runtime_error(message);
	}
	else{
		std::cout << "\033[1;31m";
		std::cout << n_files << " Root files added to chain for run " << runNumber << ".\033[0m" << '\n';
	}

	//Building framework /////////////////
	E15190Reader E15190Analyzer(dataChain, CurrRunInfo, ExpInfo->GetDetectorToAnalyze(), 1);

	int mbGeoLines = E15190Analyzer.LoadMBGeometry(CurrRunInfo->GetMBGeometryFileName());
	if (mbGeoLines <= 0){
		std::string message = "\033[1;31mError in loading microball geometry.";
		message += "Check the E15190Reader class first, then the MBGeometry class.\033[0m";	
		throw std::runtime_error(message);
	}

	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%% RELATIVISTIC BEAM PARAMETERS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	BeamRelativisticParams beamParams(CurrRunInfo);
	double beamLorentz 		= beamParams.GetSystemLorentz();
	double beamBeta 		= beamParams.GetSystemBeta();
	double centerOfMassRapidity 	= beamParams.GetFrameRapidity();
	double projectileRapidity	= beamParams.GetProjectileRapidity();
	double rapidityScaler 		= 1.0;

	int nentries = E15190Analyzer.fE15190Reader->GetEntries(true);
	int jentry = 0;

	TDCSpareChannels* Triggers = (TDCSpareChannels*)E15190Analyzer.GetTDCObject("TDC");

	// Creating centrality map for each run
	std::string centName = "cent_" + std::to_string(runNumber);
	TH1D impactHist(centName.c_str(), "", 100, 0.0, 1.0);
	while (E15190Analyzer.fE15190Reader->Next()){
		double percentage = 100.0*(double)E15190Analyzer.fE15190Reader->GetCurrentEntry(); 
		percentage /= (double)nentries;
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << percentage;
		std::cout << "Calcualting centrality ... " << ss.str() << "\%\r" << std::flush;

                MicroballCalibratedData &Microball      = **E15190Analyzer.fMicroballCal;
                HiRACalibratedData &HiRA                = **E15190Analyzer.fHiRACal;

		Triggers->FillOutputBranches();
                double MicroHiRATrigger         = Triggers->GetBranchValue("TDCTriggers.uBallHiRA_TRG");
                double MicroNWTrigger           = Triggers->GetBranchValue("TDCTriggers.uBallNW_TRG");
                double eventImpact              = Microball.fbhat;
                int HiraMult                    = HiRA.fmulti;

                if (
			(MicroHiRATrigger >= 0)
                        &&(Microball.fmulti > 0)
                        &&(Microball.fb > 0)
                        &&(HiRA.fmulti > 0)
                ){
			impactHist.Fill(eventImpact);		
		}
	}
	impactHist.Scale(1.0/impactHist.Integral());
	auto centMap = [](double x, const TH1D& h) {
		const TAxis* ax = h.GetXaxis();
		const int nb = ax->GetNbins();
		const double xmin = ax->GetXmin();
		const double xmax = ax->GetXmax();
		if (x <= xmin){
			return 0.0;
		}
		if (x >= xmax){
			return 1.0;
		}
		
		double total = 0.0;
		for (int b=1; b<=nb; b++){
			total += h.GetBinContent(b);
		}
		if (total <= 0.0){
			return 0.0;
		}
		
		int bin = ax->FindBin(x);
		if (bin < 1){
			bin = 1;
		}
		if (bin > nb){
			bin = nb;
		}
		
		double cum = 0.0; // hehe
		for (int b=1; b<bin; b++){
			cum += h.GetBinContent(b);
		}
		
		const double low  = ax->GetBinLowEdge(bin);
		const double high = ax->GetBinUpEdge(bin);
		const double frac = (high > low) ? (x - low)/(high - low) : 0.0;
		
		const double c = (cum + frac*h.GetBinContent(bin)) / total;
		return std::max(0.0, std::min(1.0, c));
	};

	E15190Analyzer.fE15190Reader->Restart();
	std::cout << '\n' << std::flush;

	// Per-centrality event buffers for mixing
	std::vector<std::vector<Queue>> eventBuffers(nCent);


	while(E15190Analyzer.fE15190Reader->Next()){
		PrintStatus(jentry, nentries, fStartTime);

		MicroballCalibratedData &Microball 	= **E15190Analyzer.fMicroballCal;
		HiRACalibratedData &HiRA 		= **E15190Analyzer.fHiRACal;

		Triggers->FillOutputBranches();
		bool processEvent 		= false;
		double MicroHiRATrigger 	= Triggers->GetBranchValue("TDCTriggers.uBallHiRA_TRG");
		double MicroNWTrigger		= Triggers->GetBranchValue("TDCTriggers.uBallNW_TRG");
		double eventImpact		= Microball.fbhat;
		double centrality		= centMap(eventImpact, impactHist);
		int HiraMult			= HiRA.fmulti;

		int centBin = findBin(centrality, centEdges);

		if (
			(Microball.fmulti > 0)
			&&(Microball.fb > 0)
			&&(HiRA.fmulti > 0)
			&&(centBin >= 0)
		){
			processEvent = true;
		}

		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Correlation analysis %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		if (processEvent){

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

						Particles key = makeKey(NZ[i], NZ[j]);
						std::map<Particles, Hists, ParticlesLess>::iterator it = histSets[centBin].find(key);
						if (it == histSets[centBin].end()){
							continue;
						}
						Hists& hist = it->second;
						double vals[coordSize] = {qRel, qLong, qPerp, pairSum.Mag(), kT, yPair};
						hist.cor_rel_total->Fill(vals);
					}
				}
			}

			// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Event mixing %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			Queue eventData;
			eventData.Initialize();
			eventData.eventIdx	= E15190Analyzer.fE15190Reader->GetCurrentEntry();
			eventData.hiraMult	= std::min((int)momenta.size(), (int)Queue::maxHiraMult);
			eventData.bhat		= eventImpact;
			for (int i=0; i < eventData.hiraMult; i++){
				eventData.A[i]		= NZ[i].first + NZ[i].second;
				eventData.Z[i]		= NZ[i].second;
				eventData.energy[i]	= energy[i];
				eventData.momenta[i]	= momenta[i];
			}

			std::vector<Queue>& buf = eventBuffers[centBin];

			if ((int)buf.size() >= WINDOWSIZE){
				Mixed mixedData;
				mixedData.Initialize();
				int iter = 0;
				for (int i=0; i < WINDOWSIZE; i++){
					for (int j=0; j < buf[i].hiraMult; j++){
						mixedData.bhat[j + iter]	= buf[i].bhat;
						mixedData.A[j + iter]		= buf[i].A[j];
						mixedData.Z[j + iter]		= buf[i].Z[j];
						mixedData.energy[j + iter]	= buf[i].energy[j];

						TVector3 mom = buf[i].momenta[j];
						mixedData.momenta[j + iter].SetXYZ(mom.X(), mom.Y(), mom.Z());
					}
					iter += buf[i].hiraMult;
				}

				for (int i=0; i < iter; i++){
					int Ai = mixedData.A[i];
					int Zi = mixedData.Z[i];
					for (int j=i+1; j < iter; j++){
						int Aj = mixedData.A[j];
						int Zj = mixedData.Z[j];
						if (
							(mixedData.momenta[i].Mag() >= 0)
							&&(mixedData.momenta[j].Mag() >= 0)
							&&(Ai > 0)
							&&(Aj > 0)
							&&(Zi > 0)
							&&(Zj > 0)
						){
							// Pair four-vector in lab frame (before boosting)
							TLorentzVector p1(mixedData.momenta[i], mixedData.energy[i]);
							TLorentzVector p2(mixedData.momenta[j], mixedData.energy[j]);
							TLorentzVector pairLV = p1 + p2;
							double yPair = pairLV.Rapidity();
							double kT    = 0.5 * pairLV.Perp();

							// Boost into pair rest frame
							TVector3 pairSum = (mixedData.momenta[i] + mixedData.momenta[j]);
							TVector3 pairBeta = pairSum*(1.0/(mixedData.energy[i] + mixedData.energy[j]));

							p1.Boost(-pairBeta);
							p2.Boost(-pairBeta);

							TVector3 p1Rel = p1.Vect();
							TVector3 p2Rel = p2.Vect();

							TVector3 poyntingRel 	= 0.5*(p1Rel - p2Rel);
							double qRel		= poyntingRel.Mag();
							double qLong		= poyntingRel.Z();
							double qPerp		= poyntingRel.Perp();

							Particles key = makeKey(std::make_pair(Ai-Zi, Zi), std::make_pair(Aj-Zj, Zj));
							std::map<Particles, Hists, ParticlesLess>::iterator it = histSets[centBin].find(key);
							if (it == histSets[centBin].end()){
								continue;
							}
							Hists& hist = it->second;

							double vals[coordSize] = {qRel, qLong, qPerp, pairSum.Mag(), kT, yPair};
							hist.mix_rel_total->Fill(vals);
						}
					}
				}

				buf.erase(buf.begin());
			}

			buf.push_back(eventData);
		}
	}

	// Write one output file per centrality bin
	for (int c=0; c < nCent; c++){
		std::map<Particles, Hists, ParticlesLess>& hmap = histSets[c];

		bool hasEntries = false;
		std::map<Particles, Hists, ParticlesLess>::iterator check;
		for (check = hmap.begin(); check != hmap.end(); check++){
			if (check->second.cor_rel_total->GetEntries() > 0){
				hasEntries = true;
				break;
			}
		}
		if (!hasEntries){
			continue;
		}

		std::string outputFileName = dataBase.GetOutputPath(runNumber);
		outputFileName += "correlations_" +
			std::to_string(runNumber) +
			"_b_" + fmt(centEdges[c]) + "_" + fmt(centEdges[c+1]) +
			".root";
		TFile* outputFile = TFile::Open(outputFileName.c_str(), "RECREATE");

		std::map<Particles, Hists, ParticlesLess>::iterator it;
		for (it = hmap.begin(); it != hmap.end(); it++){
			Hists& hist = it->second;
			if (hist.cor_rel_total->GetEntries() != 0){
				std::string nameA = isotopeTable.GetIsotopeName(it->first.A);
				std::string nameB = isotopeTable.GetIsotopeName(it->first.B);
				std::string corName = "cor_" + nameA + "_" + nameB;
				std::string mixName = "mix_" + nameA + "_" + nameB;

				hist.cor_rel_total->SetName(corName.c_str());
				hist.cor_rel_total->Write();

				hist.mix_rel_total->SetName(mixName.c_str());
				hist.mix_rel_total->Write();
			}
		}

		outputFile->Write("", TObject::kOverwrite);
		outputFile->Close();
		delete outputFile;
	}

	// Cleanup
	for (int c = 0; c < nCent; c++){
		std::map<Particles, Hists, ParticlesLess>::iterator it;
		for (it = histSets[c].begin(); it != histSets[c].end(); it++){
			delete it->second.cor_rel_total;
			delete it->second.mix_rel_total;
		}
	}

	delete dataChain;

  return 0;

}
