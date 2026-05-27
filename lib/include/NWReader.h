#ifndef NWREADER_H
#define NWREADER_H

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
#include "HTNeutronWallRootEvent.h"
#include "HTForwardArrayRootEvent.h"
#include "HTVetoWallRootEvent.h"
#include "NWPositionCalibration.h"
#include "NWCosmicRayManager.h"
#include "NWCalibratedRootEvent.h"
#include "NWTimeCalibration.h"
#include "NWGeometry.h"
#include "FATimeCalibration.h"

#include "shared.h"


class NWReader
{
public :
  NWReader(TChain * Chain=0, const char * DataType="NWA NWB FA VW", bool IsDataCalibrated=0);
  ~NWReader();

  int LoadNWPositionCalibration(const char * file_name, const char * WallToCalibrate);
  int LoadNWTimeCalibration(const char * file_name, const char * WallToCalibrate);
  int LoadNWPulseHeightCalibration(const char * file_name, const char * WallToCalibrate);
  int LoadNWCosmicRayPosition(const char *, const char * WallToCalibrate);
  int LoadNWGeometryFiducialPoints(const char *, const char * WallToCalibrate);
  int LoadFATimeCalibration(const char * file_name);
  int LoadTimePulseHeightCorrection(const char * file_name);

  double GetNWAXcm(int num_bar, double tleft, double tright) const;
  double GetNWBXcm(int num_bar, double tleft, double tright) const;
  double GetNWACosmicCenter(int num_bar) const;
  double GetNWACosmicStartingPoint(int num_bar) const;
  double GetNWBCosmicCenter(int num_bar) const;
  double GetNWBCosmicStartingPoint(int num_bar) const;
  double GetNWATimeMean(int num_bar, double tleft, double tright) const;
  double GetNWBTimeMean(int num_bar, double tleft, double tright) const;
  double GetNWATheta(int num_bar, double Xcm) const;
  double GetNWAPhi(int num_bar, double Xcm) const;
  double GetNWAThetaRan(int num_bar, double Xcm) const;
  double GetNWAPhiRan(int num_bar, double Xcm) const;
  double GetNWADistance(int num_bar, double Xcm) const;
  double GetNWADistanceRan(int num_bar, double Xcm) const;
  double GetNWBTheta(int num_bar, double Xcm) const;
  double GetNWBPhi(int num_bar, double Xcm) const;
  double GetNWBThetaRan(int num_bar, double Xcm) const;
  double GetNWBPhiRan(int num_bar, double Xcm) const;
  double GetNWBDistance(int num_bar, double Xcm) const;
  double GetNWBDistanceRan(int num_bar, double Xcm) const;
  double GetFATimeOffset(int num_det) const;
  double GetFATimePulseHeightCorrection(int num_det, double pulse_height) const;

  // Examples
  void   Loop(const char *, Long64_t evt_amount=0);
  void   LoopOnCalibratedData(const char *, Long64_t evt_amount=0);

  // Output data methods
  void   BuildCosmicRayData(const char *, Long64_t evt_amount=0);
  void   BuildCalibratedTree(const char *, Long64_t evt_amount=0);
  void   BuildCalibratedEvent();

  // Customized methods
  void   CreateNWCosmicsUsefulPlots(const char *, Long64_t evt_amount=0);
  void   CreateNWToFPlots(const char *, Long64_t evt_amount=0);

private :
  //The TTreeReader
  TTreeReader * fNWReader;
  //TTreeReaderValue for non-calibrated classes
  TTreeReaderValue<HTNeutronWallData> *fNWA;
  TTreeReaderValue<HTNeutronWallData> *fNWB;
  TTreeReaderValue<HTVetoWallData> *fVetoWall;
  TTreeReaderValue<HTForwardArrayData> *fForwardArray;
  //TTreeReaderValue for calibrated classes
  TTreeReaderValue<NeutronWallCalibratedData> *fNWACal;
  TTreeReaderValue<NeutronWallCalibratedData> *fNWBCal;
  TTreeReaderValue<VetoWallCalibratedData> *fVetoWallCal;
  TTreeReaderValue<ForwardArrayCalibratedData> *fForwardArrayCal;

  //Calibrated objects for event building
  NeutronWallCalibratedData fNWACalibratedData;
  NeutronWallCalibratedData fNWBCalibratedData;
  VetoWallCalibratedData fVetoWallCalibratedData;
  ForwardArrayCalibratedData fForwardArrayCalibratedData;

  TChain      * fChain;

  double fDegToRad;
  double fRadToDeg;
  double fSpeedOfLight;    //cm/ns
  double fNWBarLength;     //cm
  double fNWBarHigh;       //cm
  double fNWBarThickness;  //cm

  bool fIsNWA;
  bool fIsNWB;
  bool fIsFA;
  bool fIsVW;
  bool fIsDataCalibrated;
  bool fNWAPositionCalibrated;
  bool fNWBPositionCalibrated;
  bool fNWTimeCalibrated;
  bool fNWPulseHeightCalibrated;
  bool fNWACosmicRayPositionLoaded;
  bool fNWBCosmicRayPositionLoaded;
  bool fNWATimeCalibrated;
  bool fNWBTimeCalibrated;
  bool fNWAGeometryCalibrated;
  bool fNWBGeometryCalibrated;
  bool fFATimeCalibrated;

  NWPositionCalibration * fNWBPositionCalibration;
  NWPositionCalibration * fNWAPositionCalibration;
  NWCosmicRayManager    * fNWACosmicRayInfo;
  NWCosmicRayManager    * fNWBCosmicRayInfo;
  NWTimeCalibration     * fNWATimeCalibration;
  NWTimeCalibration     * fNWBTimeCalibration;
  NWGeometry            * fNWAGeometry;
  NWGeometry            * fNWBGeometry;
  FATimeCalibration     * fFATimeCalibration;
};

#endif
