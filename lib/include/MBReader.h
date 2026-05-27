#ifndef MBREADER_H
#define MBREADER_H

#include <string.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <TChain.h>
#include <TMath.h>
#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphErrors.h>
#include <TCutG.h>
#include <TMultiGraph.h>
#include <TLatex.h>
#include <TCanvas.h>
#include "HTMicroballRootEvent.h"
#include "MBCalibratedRootEvent.h"
#include "MBDetectorStatus.h"
#include "MBGeometry.h"
#include "MBHitCondition.h"
#include "MBImpactParameter.h"

#include "shared.h"


class MBReader
{
public :
  MBReader(TChain * Chain=0, bool IsDataCalibrated=0, const char * DataType="MB");
  ~MBReader();

  int LoadGeometry(const char * file_name);
  int LoadDetectorStatus(const char * file_name);
  int LoadMBFastSlowHitCondition(const char * file_name);
  int LoadMBCentrality(const char * file_name);

  double GetTheta(int num_ring, int num_det) const;
  double GetPhi(int num_ring, int num_det) const;
  double GetThetaRandom(int num_ring, int num_det) const;
  double GetPhiRandom(int num_ring, int num_det) const;
  double GetImpactParameter(int multiplicity) const;
  double Getbhat(int multiplicity) const;
  bool IsBad(int num_ring, int num_det) const;
  bool IsHit (int num_ring, int num_det, double fast, double tail, double time) const;

  // Examples
  void   Loop(const char *, Long64_t evt_amount=0);
  void   LoopOnCalibratedData(const char *, Long64_t evt_amount=0);

  // Output data methods
  void   BuildCalibratedTree(const char *, Long64_t evt_amount=0);
  void   BuildCalibratedEvent();

  // Customized methods
  void   CreateFastSlowPlots(const char *, Long64_t evt_amount=0);

private :
  TTreeReader * fMBReader;
  TTreeReaderValue<HTMicroballData> *fMicroball;
  TTreeReaderValue<MicroballCalibratedData> *fMicroballCal;

  MicroballCalibratedData fMBCalibratedData;

  TChain      * fChain;

  double fDegToRad;
  double fRadToDeg;
  double fSpeedOfLight;    //cm/ns

  bool fIsMB;
  bool fIsDataCalibrated;
  bool fGeometryLoaded;
  bool fStatusLoaded;
  bool fHitConditionLoaded;
  bool fCentralityLoaded;

  MBDetectorStatus * fMicroballStatus;
  MBGeometry * fMicroballGeometry;
  MBHitCondition * fMicroballHitCondition;
  MBImpactParameter * fMicroballCentrality;
};

#endif
