#ifndef HTEXPERIMENTINFO_H
#define HTEXPERIMENTINFO_H

/*
* HTExperimentInfo class
* Implemented by Daniele Dell'Aquila (dellaqui@nscl.msu.edu)
* v1.0 04/13/2018
*
* The class contains information for each
* configuration file to read run-by-run.
* It can return a HTRunInfo object with the
* information of a specific run.
*
*/


#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <TROOT.h>

#include <HTRunInfo.h>

class HTExperimentInfo
{
public:
  HTExperimentInfo();                             //! Constructor
  ~HTExperimentInfo();                            //! Destructor

  int InitClass(const char *);                    //! Class Initializer

  int LoadSetupConfiguration(const char *);       //! Read general setup lines from configuration file of the experiment
  int LoadRunConfiguration(const char *, int);    //! Read run configuration for a specific run

  HTRunInfo * GetRunInfo(int) const;              //! Get HTRunInfo class for a specific run number
  bool IsDataMerged() const;                      //!
  const char * GetName() const;                   //!
  const char * GetTitle() const;                  //!
  const char * GetEvtFilePath() const;            //!
  const char * GetRootFilePath() const;           //!
  const char * GetMappedRootFilePath() const;     //!
  const char * GetAnalyzedRootFilePath() const;   //!
  const char * GetAnalyzedHistogramFilePath() const;   //!
  const char * GetDetectorToAnalyze() const;      //!

  void Clear();                                   //!

private:
  std::string * fDAQConfigurationFileName;
  std::string * fPedestalFileName;
  std::string * fMappingFileName;
  std::string * fRunEvtFilePath;
  std::string * fRunTitle;

  //E15190-Analysis-Framework data members
  std::string * fBeam;
  std::string * fBeamEnergy;
  std::string * fTarget;
  std::string * fNWAPositionCalibrationFileName;
  std::string * fNWBPositionCalibrationFileName;
  std::string * fNWATimeOffsetCalibrationFileName;
  std::string * fNWBTimeOffsetCalibrationFileName;
  std::string * fNWAGainMatchingCalibrationFileName;
  std::string * fNWBGainMatchingCalibrationFileName;
  std::string * fNWAPulseHeightCalibrationFileName;
  std::string * fNWBPulseHeightCalibrationFileName;
  std::string * fNWAGeometryFileName;
  std::string * fNWBGeometryFileName;
  std::string * fNWAPSDFlatteningFileName;
  std::string * fNWBPSDFlatteningFileName;
  std::string * fNWAPSDResidualFileName;
  std::string * fNWBPSDResidualFileName;
  std::string * fNWAPSDCutsFileName;
  std::string * fNWBPSDCutsFileName;
  std::string * fNWASaturationCorrectionFileName;
  std::string * fNWBSaturationCorrectionFileName;
  std::string * fFATimeCalibrationFileName;
  std::string * fFAPulseHeightCorrectionFileName;
  std::string * fVWGainMatchingCalibrationFileName;
  std::string * fVWDETOFPIDCalibrationFileName;
  std::string * fVWGeometryFileName;
  std::string * fVWPositionCalibrationFileName;
  std::string * fMBDetectorStatusFileName;
  std::string * fMBGeometryFileName;
  std::string * fMBImpactParameterFileName;
  std::string * fMBHitConditionFileName;
  std::string * fHiRASiEnergyCalibrationFileName;
  std::string * fHiRACsIEnergyCalibrationFileName;
  std::string * fHiRACsIPulserFileName;
  std::string * fHiRADetectorStatusFileName;
  std::string * fHiRASiHiLowMatchingFileName;
  std::string * fHiRAGeometryFileName;
  std::string * fHiRAPIDFileName;
  bool * fIsJunk;

  bool fDataMerged;
  std::string fExperimentName;
  std::string fExperimentTitle;
  std::string fEvtFilePath;
  std::string fUnpackedRootFilePath;
  std::string fMappedRootFilePath;
  std::string fAnalyzedRootFilePath;
  std::string fAnalyzedHistogramFilePath;
  std::string fDetectorToAnalyze;

  int fFirstRun;
  int fLastRun;

  void ParseSetConfigLine(const char *);              //! Parse a configuration line containing the command "set" and extract Experiment info
  void ParseSetConfigLineRunInfo(const char *, int);  //! Parse a configuration line containing the command "set" and extract Run info
  const char * RetrieveRunTitleFromEvtFile(const char *) const;     //! Returns run title taken from the evt file given as input
  void StoreRunTitleInDatabaseFile(const char *, int);  //! Save run title into database file. The file takes the name of the experiment.
  int RetrieveRunTitlesFromDatabaseFile();  //! Retrieves all the run title from the database file storing into fRunTitle. The missing run titles are then retrieved from evt files.
};

#endif
