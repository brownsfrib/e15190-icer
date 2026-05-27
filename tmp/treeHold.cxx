// C/C++ includes
#include <iostream>
#include <iomanip>
#include <sstream>
#include <time.h>
#include <tuple>
#include <map>
#include <set>
#include <vector>
#include <functional>
#include <algorithm>
#include <utility>

// ROOT includes
#include <TROOT.h>
#include <TApplication.h>
#include <TSystem.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TChain.h>
#include <TH1.h>
#include <TF1.h>
#include <TLorentzVector.h>
#include <TVector3.h>
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


// =====================================================================
// Per-event buffer used for sliding-window event mixing
// =====================================================================

struct Queue {
    static const size_t maxHiraMult = 12;
    int eventIdx;
    int hiraMult;
    double bhat;
    int A[maxHiraMult];
    int Z[maxHiraMult];
    double energy[maxHiraMult];
    TVector3 momenta[maxHiraMult];

    void Initialize() {
        hiraMult = -100;
        bhat     = -100;
        for (int i = 0; i < (int)maxHiraMult; i++) {
            A[i]       = -100;
            Z[i]       = -100;
            energy[i]  = -100;
            momenta[i].SetXYZ(-100, -100, -100);
        }
    }

    void Dump() {
        std::cout << "\033[1;33mEvent " << eventIdx << " Data: \033[0m\n";
        std::cout << "Reduced impact: " << bhat << '\t';
        std::cout << "Hira mult: "      << hiraMult << '\n';
        for (int i = 0; i < hiraMult; i++) {
            std::cout << "Index: "    << i << '\t';
            std::cout << "A: "        << A[i] << '\t';
            std::cout << "Z: "        << Z[i] << '\t';
            std::cout << "Energy: "   << energy[i] << '\t';
            std::cout << "Momentum: " << momenta[i].Mag() << '\n';
        }
        std::cout << '\n';
    }
};

struct Mixed {
    static const size_t maxMixed = 12 * WINDOWSIZE;
    double bhat[maxMixed];
    int A[maxMixed];
    int Z[maxMixed];
    double energy[maxMixed];
    TVector3 momenta[maxMixed];

    void Initialize() {
        for (int i = 0; i < (int)maxMixed; i++) {
            bhat[i]   = -100;
            A[i]      = -100;
            Z[i]      = -100;
            energy[i] = -100;
            momenta[i].SetXYZ(-100, -100, -100);
        }
    }
};


// =====================================================================
// Particle-pair key + canonical ordering
// =====================================================================
// A pair is stored canonically: lower (A,Z) first. This lets downstream
// queries on the TTree use a single ordering and not have to handle
// (p,d) vs (d,p) separately.

struct Particles {
    std::pair<int, int> A;  // (N, Z) for the first particle
    std::pair<int, int> B;  // (N, Z) for the second particle

    void Dump() {
        std::cout << "Particle A: " << A.first << '\t' << A.second << '\n';
        std::cout << "Particle B: " << B.first << '\t' << B.second << '\n';
    }
};

static inline bool lessAZ(const std::pair<int, int>& x,
                          const std::pair<int, int>& y) {
    if (x.first != y.first) return x.first < y.first;
    return x.second < y.second;
}

struct ParticlesLess {
    bool operator()(const Particles& p1, const Particles& p2) const {
        if (p1.A.first  != p2.A.first ) return p1.A.first  < p2.A.first;
        if (p1.A.second != p2.A.second) return p1.A.second < p2.A.second;
        if (p1.B.first  != p2.B.first ) return p1.B.first  < p2.B.first;
        return p1.B.second < p2.B.second;
    }
};

static inline Particles makeKey(const std::pair<int, int>& a,
                                const std::pair<int, int>& b) {
    Particles k;
    if (lessAZ(a, b) || (a == b)) {
        k.A = a; k.B = b;
    } else {
        k.A = b; k.B = a;
    }
    return k;
}


// =====================================================================
// PairTreeBuffer — connects to a TTree and packs one pair per Fill()
// =====================================================================
//
// Storage choices:
//   - float for kinematics (16-digit precision is wasted here; halves file size)
//   - int for A/Z (could be Char_t to save more bytes but readability matters)
//
// Canonical ordering is enforced in set():
//   lower A first; ties broken by lower Z.

struct PairTreeBuffer {
    float qRel, qLong, qPerp, pairSum, kT, yPair;
    float centrality;
    float bhat;
    int A1, Z1, A2, Z2;

    void connect(TTree* t) {
        t->Branch("qRel",       &qRel,       "qRel/F");
        t->Branch("qLong",      &qLong,      "qLong/F");
        t->Branch("qPerp",      &qPerp,      "qPerp/F");
        t->Branch("pairSum",    &pairSum,    "pairSum/F");
        t->Branch("kT",         &kT,         "kT/F");
        t->Branch("yPair",      &yPair,      "yPair/F");
        t->Branch("centrality", &centrality, "centrality/F");
        t->Branch("bhat",       &bhat,       "bhat/F");
        t->Branch("A1",         &A1,         "A1/I");
        t->Branch("Z1",         &Z1,         "Z1/I");
        t->Branch("A2",         &A2,         "A2/I");
        t->Branch("Z2",         &Z2,         "Z2/I");
    }

    // nz1, nz2 are (N, Z) — same convention as `particleID` and `NZ`
    void set(const std::pair<int, int>& nz1,
             const std::pair<int, int>& nz2,
             double qR, double qL, double qP, double pSum,
             double kt, double yp, double cent, double bh) {
        int a1 = nz1.first + nz1.second, z1 = nz1.second;
        int a2 = nz2.first + nz2.second, z2 = nz2.second;
        if (a2 < a1 || (a2 == a1 && z2 < z1)) {
            std::swap(a1, a2);
            std::swap(z1, z2);
        }
        A1 = a1; Z1 = z1; A2 = a2; Z2 = z2;
        qRel       = (float)qR;
        qLong      = (float)qL;
        qPerp      = (float)qP;
        pairSum    = (float)pSum;
        kT         = (float)kt;
        yPair      = (float)yp;
        centrality = (float)cent;
        bhat       = (float)bh;
    }
};


// =====================================================================
// Small utilities
// =====================================================================

static std::vector<double> parseEdges(const std::string& s) {
    std::vector<double> v;
    std::stringstream ss(s);
    double d;
    while (ss >> d) v.push_back(d);
    return v;
}

static int findBin(double val, const std::vector<double>& edges) {
    for (int i = 0; i < (int)edges.size() - 1; i++) {
        bool inBin0 = (val >= edges[i]);
        bool inBin1 = (i == (int)edges.size() - 2
                       ? val <= edges[i + 1]
                       : val <  edges[i + 1]);
        if (inBin0 && inBin1) return i;
    }
    return -1;
}


// =====================================================================
// main
// =====================================================================

int main(int argc, char* argv[]) {

    int runNumber = std::stoi(argv[1]);
    std::string configPath = (argc > 2) ? argv[2] : "input.env";
    TEnv env(configPath.c_str());

    // Centrality edges are still used to bin the IMPACT-PARAMETER histogram
    // (which is needed to convert bhat -> centrality fraction). They no
    // longer drive output file splitting — centrality is just a column in
    // the tree.
    std::vector<double> centEdges = parseEdges(env.GetValue("Correlation.CentEdges", ""));
    const int nCent = (int)centEdges.size() - 1;

    // ----- Build the experiment / run framework as before -----
    HTExperimentInfo* ExpInfo = new HTExperimentInfo();
    ExpInfo->InitClass("config/HiRAEVT.conf");

    std::string data_path(ExpInfo->GetAnalyzedRootFilePath());
    TChain* dataChain = new TChain(Form("E%s", ExpInfo->GetName()));

    clock_t fStartTime = clock();

    // ----- Mass table -----
    std::string tablePath = PROJECT_ROOT "/data/mass_1.mas20";
    MassEval2020 isotopeTable(tablePath);

    // ----- Species of interest (canonical, N then Z) -----
    const size_t uniquePairs = 5;
    std::vector<std::pair<int, int>> particleID;
    particleID.push_back(std::make_pair(0, 1));  // proton
    particleID.push_back(std::make_pair(1, 1));  // deuteron
    particleID.push_back(std::make_pair(2, 1));  // triton
    particleID.push_back(std::make_pair(1, 2));  // He3
    particleID.push_back(std::make_pair(2, 2));  // alpha

    // ----- Build the set of valid pair keys (used as a filter on Fill) -----
    std::set<Particles, ParticlesLess> validPairs;
    for (int i = 0; i < (int)uniquePairs; i++) {
        for (int j = i; j < (int)uniquePairs; j++) {
            validPairs.insert(makeKey(particleID[i], particleID[j]));
        }
    }

    ParseDatabase dataBase;
    std::string runNumber_name = std::to_string(runNumber);

    // ----- Resolve and check the input file -----
    const char* inputFileName = Form("%sCalibratedData_%04d.root",
                                     data_path.c_str(), runNumber);
    if (gSystem->AccessPathName(inputFileName)) {
        std::string message = "\033[1;31mError in finding input file for run number : "
                              + runNumber_name + ".\033[0m";
        throw std::runtime_error(message);
    }

    // ----- Run info -----
    HTRunInfo* CurrRunInfo = ExpInfo->GetRunInfo(runNumber);
    std::string beamProfile(CurrRunInfo->GetBeam());
    double beamEnergy = std::stod(CurrRunInfo->GetBeamEnergy());
    std::string targetProfile(CurrRunInfo->GetTarget());

    if (CurrRunInfo->IsJunk()) {
        std::cout << "\033[1;31mRun " << runNumber
                  << " is junk -> the run will be skipped.\033[0m\n";
        throw std::runtime_error("");
    }

    std::cout << "Run Title: " << CurrRunInfo->GetTitle() << '\n';
    std::cout << "Beam: "      << beamProfile << " @ " << beamEnergy << " MeV/u\n";
    std::cout << "Target: "    << targetProfile << '\n';

    int n_files = dataChain->Add(inputFileName);
    if (n_files <= 0) {
        std::string message = "\033[1;31m" + std::to_string(n_files)
                            + " Root files added to chain for run " + runNumber_name
                            + ". Exiting.\033[1;31m";
        throw std::runtime_error(message);
    } else {
        std::cout << "\033[1;31m" << n_files
                  << " Root files added to chain for run " << runNumber
                  << ".\033[0m\n";
    }

    // ----- Build the analyzer -----
    E15190Reader E15190Analyzer(dataChain, CurrRunInfo,
                                ExpInfo->GetDetectorToAnalyze(), 1);

    int mbGeoLines = E15190Analyzer.LoadMBGeometry(CurrRunInfo->GetMBGeometryFileName());
    if (mbGeoLines <= 0) {
        std::string message = "\033[1;31mError in loading microball geometry. "
                              "Check the E15190Reader class first, then the MBGeometry class.\033[0m";
        throw std::runtime_error(message);
    }

    // ----- Beam relativistic parameters -----
    BeamRelativisticParams beamParams(CurrRunInfo);
    double beamLorentz          = beamParams.GetSystemLorentz();
    double beamBeta             = beamParams.GetSystemBeta();
    double centerOfMassRapidity = beamParams.GetFrameRapidity();
    double projectileRapidity   = beamParams.GetProjectileRapidity();
    double rapidityScaler       = 1.0;
    (void)beamLorentz; (void)beamBeta;
    (void)centerOfMassRapidity; (void)projectileRapidity; (void)rapidityScaler;

    int nentries = E15190Analyzer.fE15190Reader->GetEntries(true);
    int jentry = 0;

    TDCSpareChannels* Triggers = (TDCSpareChannels*)E15190Analyzer.GetTDCObject("TDC");

    // ----- First pass: build the impact-parameter distribution -----
    std::string centName = "cent_" + std::to_string(runNumber);
    TH1D impactHist(centName.c_str(), "", 100, 0.0, 1.0);
    while (E15190Analyzer.fE15190Reader->Next()) {
        double percentage = 100.0 * (double)E15190Analyzer.fE15190Reader->GetCurrentEntry();
        percentage /= (double)nentries;
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << percentage;
        std::cout << "Calculating centrality ... " << ss.str() << "%\r" << std::flush;

        MicroballCalibratedData& Microball = **E15190Analyzer.fMicroballCal;
        HiRACalibratedData& HiRA           = **E15190Analyzer.fHiRACal;

        Triggers->FillOutputBranches();
        double MicroHiRATrigger = Triggers->GetBranchValue("TDCTriggers.uBallHiRA_TRG");
        double eventImpact      = Microball.fbhat;

        if ((MicroHiRATrigger >= 0)
            && (Microball.fmulti > 0)
            && (Microball.fb     > 0)
            && (HiRA.fmulti      > 0)) {
            impactHist.Fill(eventImpact);
        }
    }
    impactHist.Scale(1.0 / impactHist.Integral());

    // bhat -> centrality-fraction mapping via cumulative bin distribution
    auto centMap = [](double x, const TH1D& h) {
        const TAxis* ax = h.GetXaxis();
        const int nb    = ax->GetNbins();
        const double xmin = ax->GetXmin();
        const double xmax = ax->GetXmax();
        if (x <= xmin) return 0.0;
        if (x >= xmax) return 1.0;

        double total = 0.0;
        for (int b = 1; b <= nb; b++) total += h.GetBinContent(b);
        if (total <= 0.0) return 0.0;

        int bin = ax->FindBin(x);
        if (bin < 1)  bin = 1;
        if (bin > nb) bin = nb;

        double cum = 0.0;
        for (int b = 1; b < bin; b++) cum += h.GetBinContent(b);

        const double low  = ax->GetBinLowEdge(bin);
        const double high = ax->GetBinUpEdge(bin);
        const double frac = (high > low) ? (x - low) / (high - low) : 0.0;

        const double c = (cum + frac * h.GetBinContent(bin)) / total;
        return std::max(0.0, std::min(1.0, c));
    };

    E15190Analyzer.fE15190Reader->Restart();
    std::cout << '\n' << std::flush;


    // ----- Open output file and create the trees -----
    std::string outputFileName = dataBase.GetOutputPath(runNumber)
                               + "correlations_" + std::to_string(runNumber) + ".root";
    TFile* outputFile = TFile::Open(outputFileName.c_str(), "RECREATE", "", 5);
    if (!outputFile || outputFile->IsZombie()) {
        throw std::runtime_error("Failed to open output file: " + outputFileName);
    }

    TTree* corTree = new TTree("cor", "Correlated pairs");
    TTree* mixTree = new TTree("mix", "Mixed-event pairs");
    corTree->SetAutoFlush(-30000000);  // flush every ~30 MB of basket data
    mixTree->SetAutoFlush(-30000000);

    PairTreeBuffer corBuf, mixBuf;
    corBuf.connect(corTree);
    mixBuf.connect(mixTree);

    // ----- Per-centrality event buffers (sliding-window mixing) -----
    std::vector<std::vector<Queue>> eventBuffers(nCent);

    // ----- Main event loop -----
    while (E15190Analyzer.fE15190Reader->Next()) {
        PrintStatus(jentry, nentries, fStartTime);

        MicroballCalibratedData& Microball = **E15190Analyzer.fMicroballCal;
        HiRACalibratedData& HiRA           = **E15190Analyzer.fHiRACal;

        Triggers->FillOutputBranches();
        double MicroHiRATrigger = Triggers->GetBranchValue("TDCTriggers.uBallHiRA_TRG");
        double eventImpact      = Microball.fbhat;
        double centrality       = centMap(eventImpact, impactHist);
        int    HiraMult         = HiRA.fmulti;
        (void)MicroHiRATrigger; (void)HiraMult;

        int centBin = findBin(centrality, centEdges);

        bool processEvent =
               (Microball.fmulti > 0)
            && (Microball.fb     > 0)
            && (HiRA.fmulti      > 0)
            && (centBin >= 0);

        if (!processEvent) {
            jentry++;
            continue;
        }

        // ----- Build per-event particle list -----
        std::vector<TVector3> momenta;
        std::vector<std::pair<int, int>> NZ;  // (N, Z)
        std::vector<double> energy;

        for (int i = 0; i < HiRA.fmulti; i++) {
            int A = HiRA.fAId[i];
            int Z = HiRA.fZId[i];
            double particleEnergy = HiRA.fKinEnergy[i]
                                  + isotopeTable.Mass((A - Z), Z, true);
            double momentum = HiRA.fMomentum[i];
            double theta    = HiRA.fTheta[i];
            double phi      = HiRA.fPhi[i];
            double pXi = momentum * std::cos(phi) * std::sin(theta);
            double pYi = momentum * std::sin(phi) * std::sin(theta);
            double pZi = momentum * std::cos(theta);
            TVector3 vec(pXi, pYi, pZi);

            if ((momentum >= 0) && (A > 0) && (Z > 0)) {
                momenta.push_back(vec);
                energy.push_back(particleEnergy);
                NZ.push_back(std::make_pair(A - Z, Z));
            }
        }

        // ===== Correlated pairs =====
        if (momenta.size() >= 2) {
            for (int i = 0; i < (int)momenta.size(); i++) {
                for (int j = i + 1; j < (int)momenta.size(); j++) {

                    TLorentzVector p1(momenta[i], energy[i]);
                    TLorentzVector p2(momenta[j], energy[j]);
                    TLorentzVector pairLV = p1 + p2;
                    double yPair = pairLV.Rapidity();
                    double kT    = 0.5 * pairLV.Perp();

                    TVector3 pairSum  = (momenta[i] + momenta[j]);
                    TVector3 boostVec = pairSum * (1.0 / (energy[i] + energy[j]));

                    p1.Boost(-boostVec);
                    p2.Boost(-boostVec);

                    TVector3 p1Rel = p1.Vect();
                    TVector3 p2Rel = p2.Vect();

                    TVector3 poyntingRel = 0.5 * (p1Rel - p2Rel);
                    double qRel  = poyntingRel.Mag();
                    double qLong = poyntingRel.Z();
                    double qPerp = poyntingRel.Perp();

                    Particles key = makeKey(NZ[i], NZ[j]);
                    if (validPairs.find(key) == validPairs.end()) continue;

                    corBuf.set(NZ[i], NZ[j],
                               qRel, qLong, qPerp, pairSum.Mag(), kT, yPair,
                               centrality, eventImpact);
                    corTree->Fill();
                }
            }
        }

        // ===== Event mixing =====
        Queue eventData;
        eventData.Initialize();
        eventData.eventIdx = E15190Analyzer.fE15190Reader->GetCurrentEntry();
        eventData.hiraMult = std::min((int)momenta.size(), (int)Queue::maxHiraMult);
        eventData.bhat     = eventImpact;
        for (int i = 0; i < eventData.hiraMult; i++) {
            eventData.A[i]       = NZ[i].first + NZ[i].second;
            eventData.Z[i]       = NZ[i].second;
            eventData.energy[i]  = energy[i];
            eventData.momenta[i] = momenta[i];
        }

        std::vector<Queue>& buf = eventBuffers[centBin];

        if ((int)buf.size() >= WINDOWSIZE) {
            Mixed mixedData;
            mixedData.Initialize();
            int iter = 0;
            for (int i = 0; i < WINDOWSIZE; i++) {
                for (int j = 0; j < buf[i].hiraMult; j++) {
                    mixedData.bhat[j + iter]    = buf[i].bhat;
                    mixedData.A[j + iter]       = buf[i].A[j];
                    mixedData.Z[j + iter]       = buf[i].Z[j];
                    mixedData.energy[j + iter]  = buf[i].energy[j];
                    TVector3 mom = buf[i].momenta[j];
                    mixedData.momenta[j + iter].SetXYZ(mom.X(), mom.Y(), mom.Z());
                }
                iter += buf[i].hiraMult;
            }

            for (int i = 0; i < iter; i++) {
                int Ai = mixedData.A[i];
                int Zi = mixedData.Z[i];
                for (int j = i + 1; j < iter; j++) {
                    int Aj = mixedData.A[j];
                    int Zj = mixedData.Z[j];
                    if (!((mixedData.momenta[i].Mag() >= 0)
                          && (mixedData.momenta[j].Mag() >= 0)
                          && (Ai > 0) && (Aj > 0)
                          && (Zi > 0) && (Zj > 0))) continue;

                    TLorentzVector p1(mixedData.momenta[i], mixedData.energy[i]);
                    TLorentzVector p2(mixedData.momenta[j], mixedData.energy[j]);
                    TLorentzVector pairLV = p1 + p2;
                    double yPair = pairLV.Rapidity();
                    double kT    = 0.5 * pairLV.Perp();

                    TVector3 pairSum  = (mixedData.momenta[i] + mixedData.momenta[j]);
                    TVector3 pairBeta = pairSum * (1.0 / (mixedData.energy[i]
                                                          + mixedData.energy[j]));

                    p1.Boost(-pairBeta);
                    p2.Boost(-pairBeta);

                    TVector3 p1Rel = p1.Vect();
                    TVector3 p2Rel = p2.Vect();

                    TVector3 poyntingRel = 0.5 * (p1Rel - p2Rel);
                    double qRel  = poyntingRel.Mag();
                    double qLong = poyntingRel.Z();
                    double qPerp = poyntingRel.Perp();

                    auto nz_i = std::make_pair(Ai - Zi, Zi);
                    auto nz_j = std::make_pair(Aj - Zj, Zj);
                    Particles key = makeKey(nz_i, nz_j);
                    if (validPairs.find(key) == validPairs.end()) continue;

                    // Use the *current event's* centrality and bhat as the
                    // tag for mixed pairs. This guarantees cor/mix are
                    // matched at the centrality level for downstream
                    // ratio histograms filtered by centrality bin.
                    mixBuf.set(nz_i, nz_j,
                               qRel, qLong, qPerp, pairSum.Mag(), kT, yPair,
                               centrality, eventImpact);
                    mixTree->Fill();
                }
            }

            buf.erase(buf.begin());
        }

        buf.push_back(eventData);
        jentry++;
    }

    // ----- Write out -----
    outputFile->cd();
    corTree->Write();
    mixTree->Write();
    outputFile->Close();
    delete outputFile;

    delete dataChain;
    return 0;
}
