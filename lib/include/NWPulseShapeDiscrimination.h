#ifndef NWPULSESHAPEDISCRIMINATION_H
#define NWPULSESHAPEDISCRIMINATION_H

#include <TCutG.h>
#include <TFile.h>
#include <TF1.h>
#include <TGraph.h>
#include <string>
#include <fstream>
#include <sstream>
#include <TRandom3.h>

class NWPulseShapeDiscrimination
{
public :
	NWPulseShapeDiscrimination(int TotNumBars);
	~NWPulseShapeDiscrimination();

	int LoadPSDFlattening(const char *);
	int LoadPSDResidual(const char *);
	int LoadIsGammaCuts(const char *);

	double GetPSDFlattened(double ch, double ch_fast, int numbar) const;
	double GetLeftPSDResidual(double left_ch, double left_ch_fast, double right_ch, double right_ch_fast, double Xcm, int numbar) const;
	double GetRightPSDResidual(double left_ch, double left_ch_fast, double right_ch, double right_ch_fast, double Xcm, int numbar) const;
	bool IsGamma(double ch, double ch_fast, int numbar , double ch_left , double ch_right) const;

private :
	int fNumBars;
	double * fPSDFlatteningFactor;
	TFile * fFileCuts;
	TCutG ** fFiducialCuts;
	double fPSDResidualFactor[25][2][2][5];
	TF1 *f1_gn[25][2][2];
	TGraph *gr[25][5];
	//vector<vector<vector<vector<double>>>> fPSDResidualFactor;
	//vector<vector<vector<TF1 *>>> f1_gn;
	//vector<vector<TGraph *>> gr;

	bool fPSDFlatteningLoaded;
	bool fPSDResidualCalculated;
	bool fCutsLoaded;
};

#endif
