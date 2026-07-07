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
#include <TROOT.h>
#include <TApplication.h>
#include <TSystem.h> 
#include <TGraphErrors.h>
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
	//TTreeReaderValue<MicroballCalibratedData> microballData(reader, "uBall.");
	TTreeReaderValue<double> MBHiraTrig(reader, "TDCTriggers.uBallHiRA_TRG");
	TTreeReaderValue<MicroballCalibratedData> microballData(reader, "uBall.");
        TTreeReaderValue<HiRACalibratedData> hiraData(reader, "HiRA.");



	//const float y_beam = 0.222337430585781;
	const float y_cm = 0.348685850975481;
	TH1D diff_flow("diff_rap", "Differential flow?", 
		10, -0.2, 0.2);
	diff_flow.GetXaxis()->SetTitle("y_{lab}/y_{beam}");
	diff_flow.GetXaxis()->CenterTitle(true);
	diff_flow.GetYaxis()->SetTitle("v_{1}");
	diff_flow.GetYaxis()->CenterTitle(true);


	// Error stuff
	std::map<int, float> sumWp2;
	std::map<int, float> sumWpD2;
	std::map<int, float> sumWWp;
	std::map<int, float> sumWWpCD;
	float sumW2 = 0.0;
	float sumWC2 = 0.0;

	std::map<int, float> m_p;
	std::map<int, float> m_q;
	std::map<int, float> weight_p;
	std::map<int, float> sumRe_p;	
	std::map<int, float> sumIm_p;	
	std::map<int, float> sumM_p;	
	float sumRe_q = 0.0;
	float sumIm_q = 0.0;
	float sumM_q  = 0.0;



	const float harmonic = 1.0;
	const int multLo = 10;
	const int multHi = 15;
	const float refRapLo = -0.00; // don't go above 0.15 to start
	const float refRapHi = 0.10;
	const float refpTLo = 200;
	const float refpTHi = 400;

	float run_cumulant = 0.0; // weighted average of event cumulant
	float run_weights  = 0.0; // denominator
	while (reader.Next()){
		//bool trigger = *MBHiraTrig >= 0;
		MicroballCalibratedData& microball = *microballData;
		HiRACalibratedData& hira	   = *hiraData;
		if (
			(microball.fmulti > multLo)
			&&(microball.fmulti <= multHi)
			&&(microball.fmulti > 1)
			&&(hira.fmulti > 1)
		){

			// Calculate Q vector for all particles
			std::complex<float> Qn(0.0, 0.0);	
			std::map<int, std::complex<float>> Pn;

			float refMult = 0.0;
			for (int i=0; i < microball.fmulti; i++){
				Qn += std::complex<float>(
                                                std::cos(harmonic*microball.fPhi[i]),
                                                std::sin(harmonic*microball.fPhi[i])
                                                );
				refMult++;
			}
			for (int i=0; i < hira.fmulti; i++){
				const int A 		= hira.fAId[i];
				const int Z 		= hira.fZId[i];
				const float phi 	= hira.fPhi[i];
				const float theta 	= hira.fTheta[i];
				const float momenta 	= hira.fMomentum[i];
				TVector3 track(
						std::cos(phi)*std::sin(theta), 
						std::sin(phi)*std::sin(theta), 
						std::cos(theta)
					      );
				track *= momenta;
				const float energy 	= hira.fKinEnergy[i] + \
							  isotopeTable.Mass((A-Z), Z, true);
				float rap		= 0.5*log((energy + track.Z())/(energy - track.Z()));
				rap -= y_cm;

				/*
				if (
					(rap > refRapLo)
					&&(rap <= refRapHi)
				){

					Qn += std::complex<float>(
                                	                std::cos(harmonic*phi),
                                	                std::sin(harmonic*phi)
                                	                );
					refMult++;
				}
				*/

				if (
					(A == 1)
					&&(Z == 1)
				){
					// Differential flow stuff
					// As we iterate through our particles of interest,
					// we need to check and build a scaler
					// for POI's that also fall into the RFP bounds
					int bin = diff_flow.GetXaxis()->FindBin(rap);
					m_p[bin] += 1;
					Pn[bin] += std::complex<float>(
							std::cos(harmonic*phi), 
							std::sin(harmonic*phi)
							);
				}
			}

			if (refMult <= 1){
				continue;
			}
			// Detector acceptance calculations
                        sumRe_q += std::real(Qn);
                        sumIm_q += std::imag(Qn);
                        sumM_q += refMult;

                        const float weight = refMult*(refMult - 1.0);
                        const float event_cumulant = (std::norm(Qn) - refMult)/(weight);
                        run_cumulant += event_cumulant*weight;
                        run_weights += weight;

			// Error stuff
			sumW2 += weight*weight;
			sumWC2 += event_cumulant*event_cumulant*weight;

			std::map<int, std::complex<float>>::iterator it;
			for (it = Pn.begin(); it != Pn.end(); it++){
				//const float cum = std::real(it->second*std::conj(Qn) - m_q[it->first]);
				//weight_p[it->first] += m_p[it->first]*microball.fmulti - m_q[it->first];  

				//const float cum = std::real(it->second*std::conj(Qn));
				//weight_p[it->first] += m_p[it->first]*microball.fmulti;

				const float cum = std::real(it->second*std::conj(Qn)) - m_q[it->first];
				const float wPrime = m_p[it->first]*refMult - m_q[it->first];
				const float d2ev = cum/wPrime;
				weight_p[it->first] += wPrime;

				diff_flow.AddBinContent(it->first, cum);

				// Detector acceptance calculations
				sumRe_p[it->first] += std::real(it->second);
				sumIm_p[it->first] += std::imag(it->second);
				sumM_p[it->first] += m_p[it->first];

				// Error stuff
				sumWp2[it->first] += wPrime*wPrime;
				sumWpD2[it->first] += wPrime*d2ev*d2ev;
				sumWWp[it->first] += weight*wPrime;
				sumWWpCD[it->first] += weight*wPrime*event_cumulant*d2ev;
			}
			m_p.clear();
			m_q.clear();
		}
	}

	/* TODO update avg_cumulant to refelect detector acceptances
	//				  \
	// This is your integrated flow :D--<
	//			    	  /
	// (to first order at least) */
	const float cosPhiRef 	= sumRe_q/sumM_q;
	const float sinPhiRef 	= sumIm_q/sumM_q;
	const float avg2	= run_cumulant/run_weights;
	float c2 		= run_cumulant/run_weights - (cosPhiRef*cosPhiRef + sinPhiRef*sinPhiRef);
	float s2c		= (sumW2/(run_weights*run_weights - sumW2))*(sumWC2/run_weights - avg2*avg2);

	std::ostringstream ss;
	ss << std::fixed << std::setprecision(0) << "v_{" << harmonic << "}";
	std::string graphTitle = "^{40}Ca (140 MeV/u) + ^{58}Ni " + ss.str() + " (10-40\% Centrality)"; 
	TGraphErrors flowGraph;
	flowGraph.SetTitle(graphTitle.c_str());
	flowGraph.GetXaxis()->SetTitle("y_{lab} - y_{cm}");
	flowGraph.GetXaxis()->CenterTitle(true);
	flowGraph.GetYaxis()->SetTitle(ss.str().c_str());
	flowGraph.GetYaxis()->CenterTitle(true);
	flowGraph.GetXaxis()->SetRangeUser(-0.4, 0.4);
	flowGraph.SetMarkerStyle(23);
	flowGraph.SetMarkerColor(kRed);

	for (int i=1; i <= diff_flow.GetNbinsX(); i++){
		float rap 			= diff_flow.GetBinCenter(i);
		const float avg2p 		= diff_flow.GetBinContent(i)/weight_p[i];
		const float s2d		= (sumWp2[i]/(weight_p[i]*weight_p[i] - sumWp2[i])) * \
						  (sumWpD2[i]/weight_p[i] - avg2p*avg2p);
		const float cov			= (sumWWpCD[i]/sumWWp[i] - avg2*avg2p) * \
						  (sumWWp[i]/(run_weights*weight_p[i]));
		const float cosPsi 		= sumRe_p[i]/sumM_p[i];
		const float sinPsi 		= sumIm_p[i]/sumM_p[i];
		float d2 			= avg2p - (cosPhiRef*cosPsi + sinPhiRef*sinPsi);

		std::cout << "<<2>> = " << run_cumulant/run_weights
			<< "   accept = " << (cosPhiRef*cosPhiRef + sinPhiRef*sinPhiRef) << '\n';
		if (c2 < 0){
			std::cout << "c2: " << c2 << '\n';
			c2 = 1.0;
		}
		const float flow_harmonic 	= d2;std::sqrt(fabs(c2));
		double s2v 			= s2d/c2 + flow_harmonic*flow_harmonic*s2c/(4*c2*c2*c2) - \
						  flow_harmonic*cov/(c2*c2);
		const float sv			= std::sqrt(std::max(0.0, s2v));

		flowGraph.SetPoint(i-1, rap, flow_harmonic);
		flowGraph.SetPointError(i-1, diff_flow.GetBinWidth(i)/2.0, sv);
	}



	TApplication app("app", &argc, argv);
	TCanvas can("can", "", 800, 600);
	can.SetGrid();
	flowGraph.Draw("ALP");
	app.Run();

	return 0;
}
