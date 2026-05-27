#ifndef E15190READER_H
#define E15190READER_H

#include <string.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <iostream>
#include <iomanip>
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
#include <time.h>

#include <TDCSpareChannels.h>

#include <HTMicroballRootEvent.h>
#include <HTHiRARootEvent.h>
#include <HiRACalibratedRootEvent.h>
#include <HTRunInfo.h>

#include <MBCalibratedRootEvent.h>
#include <MBDetectorStatus.h>
#include <MBGeometry.h>
#include <MBHitCondition.h>
#include <MBImpactParameter.h>

#include <HiRAGeometry.h>
#include <HiRASiCalibration.h>
#include <HiRACsICalibration.h>
#include <HiRADetectorStatus.h>
#include <HiRAIdentification.h>
#include <HiRAPixelization.h>

#include <shared.h>

/**************************************
E15190Reader class
created by Daniele Dell'Aquila - v1.0 06/22/2018
(dellaqui@nscl.msu.edu)
This class is the core of a unified framework to analyze data of E15190 NSCL Experiment
This framework provides basic tools to loop on raw HiRAEVT data and Calibrated data
by selectively enabling the detector(s) of interest
This class provides methods to get calibrated quantities from raw data structures
A general method to calibrate the whole experiment producing a new tree is provided within the framework (BuildCalibratedTree)
To get calibrated data structure for the current event use the method BuildCalibratedEvent()

*/

class E15190Reader
{
public :
  E15190Reader(TChain *, HTRunInfo *, const char * opt="HiRA-NWA-NWB-VW-FA-uBall-TDC", bool IsDataCalibrated=0);
  ~E15190Reader();

  void InitAllCalibrations();

  int LoadMBGeometry(const char * file_name);
  int LoadMBDetectorStatus(const char * file_name);
  int LoadMBFastSlowHitCondition(const char * file_name);
  int LoadMBCentrality(const char * file_name);
  int LoadHiRAGeometry(const char *);
  int LoadHiRACsICalibration(const char *);
  int LoadHiRASiCalibration(const char *);
  int LoadHiRAStripBad(const char *);
  int LoadHiRASiHiLowMatching(const char *);
  int LoadHiRACsIPulserInfo(const char *);
  int LoadHiRAIdentification(const char *);

  // Microball methods
  double GetMBDistance(int num_ring, int num_det) const;
  double GetMBTheta(int num_ring, int num_det) const;
  double GetMBPhi(int num_ring, int num_det) const;
  double GetMBThetaRandom(int num_ring, int num_det) const;
  double GetMBPhiRandom(int num_ring, int num_det) const;
  double GetMBImpactParameter(int multiplicity) const;
  double GetMBbhat(int multiplicity) const;
  bool IsMBDetectorBad(int num_ring, int num_det) const;
  bool IsMBHit (int num_ring, int num_det, double fast, double tail, double time) const;
  //HiRA methods
  bool IsStripfBad(int telescope, int strip_front) const;
  bool IsStripbBad(int telescope, int strip_back) const;
  double GetThetaPixel(int telescope, int strip_front, int strip_back) const;
  double GetPhiPixel(int telescope, int strip_front, int strip_back) const;
  double GetThetaPixelDeg(int telescope, int strip_front, int strip_back) const;
  double GetPhiPixelDeg(int telescope, int strip_front, int strip_back) const;
  double GetSifIntercept(int telescope, int numstrip) const;
  double GetSibIntercept(int telescope, int numstrip) const;
  double GetSifSlope(int telescope, int numstrip) const;
  double GetSibSlope(int telescope, int numstrip) const;
  double GetCsIIntercept(int telescope, int numcsi) const;
  double GetCsISlope(int telescope, int numcsi) const;
  double GetSifHiLowMatched(int chHi, int chLow, int telescope, int numstrip) const;
  double GetSibHiLowMatched(int chHi, int chLow, int telescope, int numstrip) const;
  double GetCsIEMeV(int ch, int telescope, int numcsi, int Z=1, int A=1) const;
  double GetSifEMeV(int ch, int telescope, int numstripf) const;
  double GetSibEMeV(int ch, int telescope, int numstripb) const;
  double GetSifHiLowMatchedEMeV(int chHi, int chLow, int telescope, int numstrip) const;
  double GetSibHiLowMatchedEMeV(int chHi, int chLow, int telescope, int numstrip) const;

  // Examples
  void   Loop(const char *, Long64_t evt_amount=0);
  void   LoopOnCalibratedData(const char *, Long64_t evt_amount=0);

  // Output data methods
  void   BuildCalibratedTree(const char *, Long64_t evt_amount=0);
  void   BuildCalibratedEvent();

  //Customized methods
  // input here customized methods
  void   CreateUsefulVetoWallHistograms(const char *, Long64_t evt_amount=0);
  void   CreateUsefulForwardArrayHistograms(const char *, Long64_t evt_amount=0);
  void   CreateUsefulHiRAHistograms(const char *, Long64_t evt_amount=0);
  void   BuildCorrelationTree(const char *, Long64_t evt_amount=0);


  // 7/25/24 - Stealing some of Som's methods
  // Hopefully we can write code in a cleaner way? Avoid some seg violations?
  //Standalone methods
  //These methods are used to retrieve event-by-event information in a standalone code
  int Next();  //Read the next event. Returns 0 if end-of-file.
  //void * GetData(const char * det_name); //returns a pointer with data of a particular detector identified by det_name.
  MicroballCalibratedData* GetMicroball();
  HiRACalibratedData* GetHiRA();

  void SetTDC();
  double GetTDCValue(std::string triggerName);
  void * GetTDCObject(const char* passName);


  // void   testniu();

//private :
public :
  //The Run Info
  HTRunInfo * fCurrRunInfo;
  //The TTreeReader
  TTreeReader * fE15190Reader;
  //TDC Spare channels
  TDCSpareChannels * fTDCAdditionalChannels;
  //TTreeReaderValue for non-calibrated classes
  TTreeReaderValue<HTMicroballData> *fMicroball;
  TTreeReaderValue<HTHiRAData> **fHiRA;
  //TTreeReaderValue for calibrated classes
  TTreeReaderValue<MicroballCalibratedData> *fMicroballCal;
  TTreeReaderValue<HiRACalibratedData> *fHiRACal;

  //Calibrated objects for event building
  MicroballCalibratedData fMicroballCalibratedData;
  HiRACalibratedData fHiRACalibratedData;
  HiRACalibratedBuffer fHiRACalibratedDataBuffer;

  TChain      * fChain;

  TNamed * fBeam;
  TNamed * fBeamEnergy;
  TNamed * fTarget;

  clock_t fStartTime;      // number of clocks since the start of the program initialized if required as the start for time measurement

  double fDegToRad;
  double fRadToDeg;
  double fSpeedOfLight;    //cm/ns

  bool fIsTDC;
  bool fIsMB;
  bool fIsHiRA;
  bool fIsDataCalibrated;
  bool fMBGeometryLoaded;
  bool fMBStatusLoaded;
  bool fMBHitConditionLoaded;
  bool fMBCentralityLoaded;
  bool fHiRACsICalibrated;
  bool fHiRACsIPulserCalibrated;
  bool fHiRASiCalibrated;
  bool fHiRAGeometryCalibrated;
  bool fHiRAStripBadLoaded;
  bool fHiRASiHiLowMatched;
  bool fHiRAIdentificationLoaded;

  MBDetectorStatus      * fMicroballStatus;
  MBGeometry            * fMicroballGeometry;
  MBHitCondition        * fMicroballHitCondition;
  MBImpactParameter     * fMicroballCentrality;

  HiRAGeometry *fHiRAGeometryTab;
  HiRASiCalibration *fSiCalibrationTools;
  HiRACsICalibrationManager *fCsICalibrationModule;
  HiRADetectorStatus *fHiRAStatus;
  HiRAIdentification *fHiRAIdentifiationModule;
  HiRAPixelization * fHiRAPixelizationModule;

  void PrintPercentage(Long64_t, Long64_t) const;
  void PrintPercentageSimple(Long64_t, Long64_t) const;

};

#endif
