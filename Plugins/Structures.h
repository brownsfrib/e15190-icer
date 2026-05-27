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


#define WINDOWSIZE 20

struct PairTreeBuffer{
	float qRel, qLong, qPerp, pairSum, kT, yPair;
	int A1, Z1, A2, Z2;
	
	void connect(TTree& tree){
		tree.Branch("qRel",       &qRel,       "qRel/F");
		tree.Branch("qLong",      &qLong,      "qLong/F");
		tree.Branch("qPerp",      &qPerp,      "qPerp/F");
		tree.Branch("pairSum",    &pairSum,    "pairSum/F");
		tree.Branch("kT",         &kT,         "kT/F");
		tree.Branch("yPair",      &yPair,      "yPair/F");
		tree.Branch("centrality", &centrality, "centrality/F");
		tree.Branch("bhat",       &bhat,       "bhat/F");
		tree.Branch("A1",         &A1,         "A1/I");
		tree.Branch("Z1",         &Z1,         "Z1/I");
		tree.Branch("A2",         &A2,         "A2/I");
		tree.Branch("Z2",         &Z2,         "Z2/I");
	}
	
	void set(
			const std::pair<int,int>& nz1, const std::pair<int,int>& nz2,
			double qR, double qL, double qP, double pSum,
			double kt, double yp, double cent, double bh)
	{
		int a1 = nz1.first + nz1.second, z1 = nz1.second;
		int a2 = nz2.first + nz2.second, z2 = nz2.second;
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

		A1 		= a1; 
		Z1 		= z1; 
		A2 		= a2; 
		Z2 		= z2;
		qRel       	= static_cast<float>qR;
		qLong      	= static_cast<float>qL;
		qPerp      	= static_cast<float>qP;
		pairSum    	= static_cast<float>pSum;
		kT         	= static_cast<float>kt;
		yPair      	= static_cast<float>yp;
	}
};

