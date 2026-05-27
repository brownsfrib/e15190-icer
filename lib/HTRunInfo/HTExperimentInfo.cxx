#include <HTExperimentInfo.h>

//________________________________________________
HTExperimentInfo::HTExperimentInfo() :
fDataMerged(false),
fFirstRun(0),
fLastRun(0)
{}

//________________________________________________
HTExperimentInfo::~HTExperimentInfo()
{
  if(fDAQConfigurationFileName) delete [] fDAQConfigurationFileName;
  if(fPedestalFileName) delete [] fPedestalFileName;
  if(fMappingFileName) delete [] fMappingFileName;
  if(fRunEvtFilePath) delete [] fRunEvtFilePath;
  if(fRunTitle) delete [] fRunTitle;

  //E15190-Analysis-Framework data
  if(fBeam) delete [] fBeam;
  if(fBeamEnergy) delete [] fBeamEnergy;
  if(fTarget) delete [] fTarget;
  if(fNWAPositionCalibrationFileName) delete [] fNWAPositionCalibrationFileName;
  if(fNWBPositionCalibrationFileName) delete [] fNWBPositionCalibrationFileName;
  if(fNWATimeOffsetCalibrationFileName) delete [] fNWATimeOffsetCalibrationFileName;
  if(fNWBTimeOffsetCalibrationFileName) delete [] fNWBTimeOffsetCalibrationFileName;
  if(fNWAGainMatchingCalibrationFileName) delete [] fNWAGainMatchingCalibrationFileName;
  if(fNWBGainMatchingCalibrationFileName) delete [] fNWBGainMatchingCalibrationFileName;
  if(fNWAPulseHeightCalibrationFileName) delete [] fNWAPulseHeightCalibrationFileName;
  if(fNWBPulseHeightCalibrationFileName) delete [] fNWBPulseHeightCalibrationFileName;
  if(fNWAGeometryFileName) delete [] fNWAGeometryFileName;
  if(fNWBGeometryFileName) delete [] fNWBGeometryFileName;
  if(fNWAPSDFlatteningFileName) delete [] fNWAPSDFlatteningFileName;
  if(fNWBPSDFlatteningFileName) delete [] fNWBPSDFlatteningFileName;
  if(fNWAPSDResidualFileName) delete [] fNWAPSDResidualFileName;
  if(fNWBPSDResidualFileName) delete [] fNWBPSDResidualFileName;
  if(fNWAPSDCutsFileName) delete [] fNWAPSDCutsFileName;
  if(fNWBPSDCutsFileName) delete [] fNWBPSDCutsFileName;
  if(fNWASaturationCorrectionFileName) delete [] fNWASaturationCorrectionFileName;
  if(fNWBSaturationCorrectionFileName) delete [] fNWBSaturationCorrectionFileName;
  if(fFATimeCalibrationFileName) delete [] fFATimeCalibrationFileName;
  if(fFAPulseHeightCorrectionFileName) delete [] fFAPulseHeightCorrectionFileName;
  if(fVWGainMatchingCalibrationFileName) delete [] fVWGainMatchingCalibrationFileName;
  if(fVWDETOFPIDCalibrationFileName) delete [] fVWDETOFPIDCalibrationFileName;
  if(fVWGeometryFileName) delete [] fVWGeometryFileName;
  if(fVWPositionCalibrationFileName) delete [] fVWPositionCalibrationFileName;
  if(fMBDetectorStatusFileName) delete [] fMBDetectorStatusFileName;
  if(fMBGeometryFileName) delete [] fMBGeometryFileName;
  if(fMBImpactParameterFileName) delete [] fMBImpactParameterFileName;
  if(fMBHitConditionFileName) delete [] fMBHitConditionFileName;
  if(fHiRASiEnergyCalibrationFileName) delete [] fHiRASiEnergyCalibrationFileName;
  if(fHiRACsIEnergyCalibrationFileName) delete [] fHiRACsIEnergyCalibrationFileName;
  if(fHiRACsIPulserFileName) delete [] fHiRACsIPulserFileName;
  if(fHiRADetectorStatusFileName) delete [] fHiRADetectorStatusFileName;
  if(fHiRASiHiLowMatchingFileName) delete [] fHiRASiHiLowMatchingFileName;
  if(fHiRAGeometryFileName) delete [] fHiRAGeometryFileName;
  if(fHiRAPIDFileName) delete [] fHiRAPIDFileName;
  if(fIsJunk) delete [] fIsJunk;
}

//________________________________________________
void HTExperimentInfo::Clear()
{
  fExperimentName.clear();
  fExperimentTitle.clear();
  fEvtFilePath.clear();
  fUnpackedRootFilePath.clear();
  fMappedRootFilePath.clear();
}

//________________________________________________
int HTExperimentInfo::InitClass(const char *file_name)
{
  Clear();
  int NLinesRead=0;

  //Set general setup configuration
  NLinesRead += LoadSetupConfiguration(file_name);

  fDAQConfigurationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fPedestalFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fMappingFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fRunEvtFilePath=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fRunTitle=(std::string*)new std::string[fLastRun-fFirstRun+1];

  //E15190-Analysis-Framework data
  fBeam=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fBeamEnergy=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fTarget=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWAPositionCalibrationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWBPositionCalibrationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWATimeOffsetCalibrationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWBTimeOffsetCalibrationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWAGainMatchingCalibrationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWBGainMatchingCalibrationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWAPulseHeightCalibrationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWBPulseHeightCalibrationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWAGeometryFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWBGeometryFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWAPSDFlatteningFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWBPSDFlatteningFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWAPSDResidualFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWBPSDResidualFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWAPSDCutsFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWBPSDCutsFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWASaturationCorrectionFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fNWBSaturationCorrectionFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fFATimeCalibrationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fFAPulseHeightCorrectionFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fVWGainMatchingCalibrationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fVWDETOFPIDCalibrationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fVWGeometryFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fVWPositionCalibrationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fMBDetectorStatusFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fMBGeometryFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fMBImpactParameterFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fMBHitConditionFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fHiRASiEnergyCalibrationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fHiRACsIEnergyCalibrationFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fHiRACsIPulserFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fHiRADetectorStatusFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fHiRASiHiLowMatchingFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fHiRAGeometryFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fHiRAPIDFileName=(std::string*)new std::string[fLastRun-fFirstRun+1];
  fIsJunk=(bool*)new bool[fLastRun-fFirstRun+1];

  // Retrieving all previously stored run titles from database file
  RetrieveRunTitlesFromDatabaseFile();

  //Set run-by-run setup configuration
  for(int run_num=fFirstRun; run_num<=fLastRun; run_num++)
  {
    //NOTE: I initialize here fIsJunk to false for the current run. Later, this value can be changed only to true.
    fIsJunk[run_num-fFirstRun]=false;
    fRunEvtFilePath[run_num-fFirstRun].assign(fEvtFilePath); //In case the evt file path is unique and the option --run is never specified
    NLinesRead += LoadRunConfiguration(file_name,run_num);
    // Only if the run title was not previously found retrieve it from evt file and store into the database file
    if(fRunTitle[run_num-fFirstRun].empty()) {
      const char * TempRunTitle=RetrieveRunTitleFromEvtFile(Form("%srun%d/run-%04d-00.evt",fRunEvtFilePath[run_num-fFirstRun].c_str(),run_num,run_num));
      if(TempRunTitle!=0) {
        fRunTitle[run_num-fFirstRun].assign(TempRunTitle);
        StoreRunTitleInDatabaseFile(TempRunTitle, run_num);
      }
    }
  }

  return NLinesRead;
}

//________________________________________________
int HTExperimentInfo::LoadSetupConfiguration(const char *file_name)
{
  std::ifstream FileIn(file_name);
  if(!FileIn.is_open()) {
    return -1;
  }

  int NRead=0;

  while (!FileIn.eof())
  {
    std::string LineRead;
    std::getline(FileIn, LineRead);
    std::string LineReadCommentLess(LineRead.substr(0,LineRead.find("*")));

    if(LineReadCommentLess.empty()) continue;

    if(LineReadCommentLess.find_first_not_of(' ') == std::string::npos) continue;

    if(LineReadCommentLess.find("set ")!=std::string::npos) {
      ParseSetConfigLine(LineReadCommentLess.c_str());
    }

    if(LineReadCommentLess.find("FIRST_RUN")!=std::string::npos) {
      fFirstRun=atoi(LineReadCommentLess.substr(LineReadCommentLess.find("=")+1).c_str());
    }
    if(LineReadCommentLess.find("LAST_RUN")!=std::string::npos) {
      fLastRun=atoi(LineReadCommentLess.substr(LineReadCommentLess.find("=")+1).c_str());
    }

    NRead++;
  }
  FileIn.close();

  return NRead;
}

//________________________________________________
int HTExperimentInfo::LoadRunConfiguration(const char *file_name, int run_num)
{
  std::ifstream FileIn(file_name);
  if(!FileIn.is_open()) {
    return -1;
  }

  int NRead=0;

  while (!FileIn.eof())
  {
    std::string LineRead;
    std::getline(FileIn, LineRead);
    std::string LineReadCommentLess(LineRead.substr(0,LineRead.find("*")));

    if(LineReadCommentLess.empty()) continue;

    if(LineReadCommentLess.find_first_not_of(' ') == std::string::npos) continue;

    if(LineReadCommentLess.find("set ")!=std::string::npos) {
      ParseSetConfigLineRunInfo(LineReadCommentLess.c_str(), run_num);
    }

    NRead++;
  }
  FileIn.close();

  return NRead;
}

//________________________________________________
HTRunInfo * HTExperimentInfo::GetRunInfo(int run_num) const
{
  HTRunInfo * newRunInfo = new HTRunInfo(run_num,fRunTitle[run_num-fFirstRun].c_str());

  if(newRunInfo->LoadDAQSettings(fDAQConfigurationFileName[run_num-fFirstRun].c_str())<=0) {
    delete newRunInfo;
    return 0;
  }
  if(newRunInfo->SetPedestalsFile(fPedestalFileName[run_num-fFirstRun].c_str())!=0) {
    delete newRunInfo;
    return 0;
  }
  if(newRunInfo->SetMappingFile(fMappingFileName[run_num-fFirstRun].c_str())!=0) {
    delete newRunInfo;
    return 0;
  }
  if(!fRunEvtFilePath[run_num-fFirstRun].empty()) {
    newRunInfo->SetEvtFilePath(fRunEvtFilePath[run_num-fFirstRun].c_str());
  } else {
    newRunInfo->SetEvtFilePath(fEvtFilePath.c_str());
  }

  //E15190-Analysis-Framework paths
  newRunInfo->SetBeam(     fBeam[run_num-fFirstRun].c_str());
  newRunInfo->SetBeamEnergy(     fBeamEnergy[run_num-fFirstRun].c_str());
  newRunInfo->SetTarget(     fTarget[run_num-fFirstRun].c_str());
  newRunInfo->SetNWAPositionCalibrationFile(     fNWAPositionCalibrationFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWBPositionCalibrationFile(	 fNWBPositionCalibrationFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWATimeOffsetCalibrationFile(	 fNWATimeOffsetCalibrationFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWBTimeOffsetCalibrationFile(	 fNWBTimeOffsetCalibrationFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWAGainMatchingCalibrationFile( fNWAGainMatchingCalibrationFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWBGainMatchingCalibrationFile( fNWBGainMatchingCalibrationFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWAPulseHeightCalibrationFile( fNWAPulseHeightCalibrationFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWBPulseHeightCalibrationFile( fNWBPulseHeightCalibrationFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWAGeometryCalibrationFile(	 fNWAGeometryFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWBGeometryCalibrationFile(	 fNWBGeometryFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWAPSDFlatteningFile(	 fNWAPSDFlatteningFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWBPSDFlatteningFile(	 fNWBPSDFlatteningFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWAPSDResidualFile(	 fNWAPSDResidualFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWBPSDResidualFile(	 fNWBPSDResidualFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWAPSDCutsFile(	 fNWAPSDCutsFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWBPSDCutsFile(	 fNWBPSDCutsFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWASaturationCorrectionFile(	 fNWASaturationCorrectionFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetNWBSaturationCorrectionFile(	 fNWBSaturationCorrectionFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetFATimeCalibrationFile( 	 fFATimeCalibrationFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetFAPulseHeightCorrectionFile(	 fFAPulseHeightCorrectionFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetVWGainMatchingCalibrationFile(	 fVWGainMatchingCalibrationFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetVWDETOFPIDCalibrationFile(	 fVWDETOFPIDCalibrationFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetVWGeometryFile(	 fVWGeometryFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetVWPositionCalibrationFile(	 fVWPositionCalibrationFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetMBDetectorStatusFile(		 fMBDetectorStatusFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetMBGeometryFile(		 fMBGeometryFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetMBImpactParameterFile( 	 fMBImpactParameterFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetMBHitConditionFile(		 fMBHitConditionFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetHiRASiEnergyCalibrationFile(	 fHiRASiEnergyCalibrationFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetHiRACsIEnergyCalibrationFile(	 fHiRACsIEnergyCalibrationFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetHiRACsIPulserFile(		 fHiRACsIPulserFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetHiRADetectorStatusFile(	 fHiRADetectorStatusFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetHiRASiHiLowMatchingFile(	 fHiRASiHiLowMatchingFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetHiRAGeometryFile(		 fHiRAGeometryFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetHiRAPIDFile(		 fHiRAPIDFileName[run_num-fFirstRun].c_str());
  newRunInfo->SetJunk(		 fIsJunk[run_num-fFirstRun]);

  return newRunInfo;
}

//________________________________________________
void HTExperimentInfo::ParseSetConfigLine(const char *line_to_parse)
{
  std::string LineToParse(line_to_parse);

  LineToParse.assign(LineToParse.substr(LineToParse.find("set ")+4)); //remove set command

  std::istringstream LineStream(LineToParse);

  std::string ValueToSet;
  LineStream>>ValueToSet;
  std::string NewValue(LineToParse.substr(LineToParse.find("\"")+1,LineToParse.find_last_of("\"")-(LineToParse.find("\"")+1)));

  if(ValueToSet.compare("EXPERIMENT_NAME")==0) {
    fExperimentName.assign(NewValue);
  } else if (ValueToSet.compare("EXPERIMENT_TITLE")==0) {
    fExperimentTitle.assign(NewValue);
  } else if (ValueToSet.compare("MERGED_DATA")==0) {
    NewValue.compare("true")==0 ? fDataMerged=true : fDataMerged=false;
  } else if (ValueToSet.compare("EVENT_FILE_PATH")==0) {
    if(LineToParse.find("--run")==std::string::npos) { //EvtFileName might be different run-by-run, if --run is specified don't set it now but in ParseSetConfigLineRunInfo
      fEvtFilePath.assign(NewValue);
    }
  } else if (ValueToSet.compare("HiRAEVTUNPACKER_ROOT_FILE_PATH")==0) {
    fUnpackedRootFilePath.assign(NewValue);
  } else if (ValueToSet.compare("HiRAEVTMAPPER_ROOT_FILE_PATH")==0) {
    fMappedRootFilePath.assign(NewValue);
  } else if (ValueToSet.compare("ANALYSISFRAMEWORK_ROOT_FILE_PATH")==0) {
    fAnalyzedRootFilePath.assign(NewValue);
  } else if (ValueToSet.compare("ANALYSISFRAMEWORK_HISTOGRAM_FILE_PATH")==0) {
    fAnalyzedHistogramFilePath.assign(NewValue);
  } else if (ValueToSet.compare("DETECTOR_TO_ANALYZE")==0) {
    fDetectorToAnalyze.assign(NewValue);
  }

  return;
}

//________________________________________________
void HTExperimentInfo::ParseSetConfigLineRunInfo(const char *line_to_parse, int run_num)
{
  std::string LineToParse(line_to_parse);

  LineToParse.assign(LineToParse.substr(LineToParse.find("set ")+4)); //remove set command

  std::istringstream LineStream(LineToParse);

  std::string ValueToSet;
  LineStream>>ValueToSet;
  std::string NewValue;
  std::string Option;
  bool RunFound=false;

  //NOTE: for a future improvement take into account more possible options not only --run and --exclude
  //A --run or --exclude option can contain or "," or "-" as a separator, not combination of both
  // Loop on the option strings, every time one finds -- this is an option string, e.g. --run, --exclude
  // 2018/08/20 Included the possibility of specifying only 1 run with --run or --exclude options
  while(LineStream>>Option && Option.find("--")!=std::string::npos) {
    if(Option.find("--run=")!=std::string::npos) {
      Option.assign(Option.substr(Option.find("--run=")+6));
      std::istringstream LineRunStream(Option);
      if(Option.find(",")!=std::string::npos) {
        std::string RunToInclude;
        while(std::getline(LineRunStream, RunToInclude, ',')) {
          if(run_num==std::stoi(RunToInclude)) RunFound=true;
        }
      }
      if(Option.find("-")!=std::string::npos) {
        std::string StartRun;
        std::string StopRun;
        std::getline(LineRunStream, StartRun, '-');
        std::getline(LineRunStream, StopRun, '-');
        int StartRunNum=std::stoi(StartRun);
        int StopRunNum=std::stoi(StopRun);
        if(run_num>=StartRunNum && run_num<=StopRunNum) RunFound=true;
      }
      if(Option.find(",")==std::string::npos && Option.find("-")==std::string::npos && !Option.empty()) {
        if(run_num==std::stoi(Option)) RunFound=true;
      }
    } else if (Option.find("--exclude=")!=std::string::npos) {
        Option.assign(Option.substr(Option.find("--exclude=")+10));
        std::istringstream LineExcludeStream(Option);
        if(Option.find(",")!=std::string::npos) {
          std::string RunToExclude;
          while(std::getline(LineExcludeStream, RunToExclude, ',')) {
            if(run_num==std::stoi(RunToExclude)) return; //this run is excluded
          }
        }
        if(Option.find("-")!=std::string::npos) {
          std::string StartRun;
          std::string StopRun;
          std::getline(LineExcludeStream, StartRun, '-');
          std::getline(LineExcludeStream, StopRun, '-');
          int StartRunNum=std::stoi(StartRun);
          int StopRunNum=std::stoi(StopRun);
          if(run_num>=StartRunNum && run_num<=StopRunNum) return; //this run belong to a bunch of runs to be excluded
        }
        if(Option.find(",")==std::string::npos && Option.find("-")==std::string::npos && !Option.empty()) {
          if(run_num==std::stoi(Option)) return;
        }
      }
    }

  if(!RunFound) return;

  NewValue.assign(LineToParse.substr(LineToParse.find("\"")+1,LineToParse.find_last_of("\"")-(LineToParse.find("\"")+1)));

  // if I'm here so run_num has been found in the configuration line
  if(ValueToSet.compare("DAQ_CONFIG")==0) {
    fDAQConfigurationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("PEDESTAL_VALUES")==0) {
    fPedestalFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("CHANNEL_MAPPING")==0) {
    fMappingFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("EVENT_FILE_PATH")==0) {
    fRunEvtFilePath[run_num-fFirstRun].assign(NewValue);
  }

  // E15190-Analysis-Framework data
  else if (ValueToSet.compare("EXPERIMENT_INFO")==0) {
    std::string NewInfo;
    std::string WhatToSet;
    std::istringstream LineStreamExpInfo(LineToParse);
    while(LineStreamExpInfo>>NewInfo) {
      if(NewInfo.compare("EXPERIMENT_INFO")==0 || NewInfo.find("--run")!=std::string::npos) continue;
      if(NewInfo.find("-")!=std::string::npos) {
        WhatToSet.assign(NewInfo.substr(NewInfo.find("-")+1,NewInfo.find_last_of("=")-(NewInfo.find("-")+1)));
        NewValue.assign(NewInfo.substr(NewInfo.find("\"")+1,NewInfo.find_last_of("\"")-(NewInfo.find("\"")+1)));
        if(WhatToSet.compare("beam")==0) {
          fBeam[run_num-fFirstRun].assign(NewValue);
        } else if(WhatToSet.compare("energy")==0) {
          fBeamEnergy[run_num-fFirstRun].assign(NewValue);
        } else if(WhatToSet.compare("target")==0) {
          fTarget[run_num-fFirstRun].assign(NewValue);
        }
      }
    }
  } else if (ValueToSet.compare("NWA_POSITION_CALIBRATION")==0) {
    fNWAPositionCalibrationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWB_POSITION_CALIBRATION")==0) {
    fNWBPositionCalibrationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWA_TIMEOFFSET_CALIBRATION")==0) {
    fNWATimeOffsetCalibrationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWB_TIMEOFFSET_CALIBRATION")==0) {
    fNWBTimeOffsetCalibrationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWA_GAIN_MATCHING_CALIBRATION")==0) {
    fNWAGainMatchingCalibrationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWB_GAIN_MATCHING_CALIBRATION")==0) {
    fNWBGainMatchingCalibrationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWA_PULSEHEIGHT_CALIBRATION")==0) {
    fNWAPulseHeightCalibrationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWB_PULSEHEIGHT_CALIBRATION")==0) {
    fNWBPulseHeightCalibrationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWA_GEOMETRY")==0) {
    fNWAGeometryFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWB_GEOMETRY")==0) {
    fNWBGeometryFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWA_PSD_FLATTENING")==0) {
    fNWAPSDFlatteningFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWB_PSD_FLATTENING")==0) {
    fNWBPSDFlatteningFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWA_PSD_RESIDUAL")==0) {
    fNWAPSDResidualFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWB_PSD_RESIDUAL")==0) {
    fNWBPSDResidualFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWA_ISGAMMA_CUTS")==0) {
    fNWAPSDCutsFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWB_ISGAMMA_CUTS")==0) {
    fNWBPSDCutsFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWA_SATURATION_CORRECTION")==0) {
    fNWASaturationCorrectionFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("NWB_SATURATION_CORRECTION")==0) {
    fNWBSaturationCorrectionFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("FA_TIME_CALIBRATION")==0) {
    fFATimeCalibrationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("FA_PULSE_HEIGHT_CORRECTION")==0) {
    fFAPulseHeightCorrectionFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("VW_GAIN_MATCHING_CALIBRATION")==0) {
    fVWGainMatchingCalibrationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("VW_DETOF_PID_CALIBRATION")==0) {
    fVWDETOFPIDCalibrationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("VW_GEOMETRY")==0) {
    fVWGeometryFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("VW_POSITION_CALIBRATION")==0) {
    fVWPositionCalibrationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("MB_DETECTOR_STATUS")==0) {
    fMBDetectorStatusFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("MB_GEOMETRY")==0) {
    fMBGeometryFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("MB_IMPACT_PARAMETER")==0) {
    fMBImpactParameterFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("MB_HIT_CONDITION")==0) {
    fMBHitConditionFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("HIRA_SI_ENERGY_CALIBRATION")==0) {
    fHiRASiEnergyCalibrationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("HIRA_CSI_ENERGY_CALIBRATION")==0) {
    fHiRACsIEnergyCalibrationFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("HIRA_CSI_PULSER_CALIBRATION")==0) {
    fHiRACsIPulserFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("HIRA_DETECTOR_STATUS")==0) {
    fHiRADetectorStatusFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("HIRA_SI_HILOWGAIN")==0) {
    fHiRASiHiLowMatchingFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("HIRA_GEOMETRY")==0) {
    fHiRAGeometryFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("HIRA_PID")==0) {
    fHiRAPIDFileName[run_num-fFirstRun].assign(NewValue);
  } else if (ValueToSet.compare("JUNK_RUN")==0) {
    fIsJunk[run_num-fFirstRun]=true;
  }

  return;

}

//________________________________________________
bool HTExperimentInfo::IsDataMerged() const
{
  return fDataMerged;
}

//________________________________________________
const char * HTExperimentInfo::GetName() const
{
  return fExperimentName.c_str();
}

//________________________________________________
const char * HTExperimentInfo::GetTitle() const
{
  return fExperimentTitle.c_str();
}

//________________________________________________
const char * HTExperimentInfo::GetEvtFilePath() const
{
  return fEvtFilePath.c_str();
}

//________________________________________________
const char * HTExperimentInfo::GetRootFilePath() const
{
  return fUnpackedRootFilePath.c_str();
}

//________________________________________________
const char * HTExperimentInfo::GetMappedRootFilePath() const
{
  return fMappedRootFilePath.c_str();
}

//________________________________________________
const char * HTExperimentInfo::GetAnalyzedRootFilePath() const
{
  return fAnalyzedRootFilePath.c_str();
}

//________________________________________________
const char * HTExperimentInfo::GetAnalyzedHistogramFilePath() const
{
  return fAnalyzedHistogramFilePath.c_str();
}

//________________________________________________
const char * HTExperimentInfo::GetDetectorToAnalyze() const
{
  return fDetectorToAnalyze.c_str();
}

//________________________________________________
int HTExperimentInfo::RetrieveRunTitlesFromDatabaseFile()
{
  std::ifstream FileIn(Form("config/%s.run",fExperimentName.c_str()));
  if(!FileIn.is_open()) {
    return -1;
  }
  int NRead=0;

  while (!FileIn.eof())
  {
    std::string LineRead;
    std::getline(FileIn, LineRead);

    if(LineRead.empty()) continue;
    if(LineRead.find('*')==0) continue;
    if(LineRead.find_first_not_of(' ') == std::string::npos) continue;

    std::istringstream LineStream(LineRead);

    int run_num;

    LineStream>>run_num;
    if(run_num>=fFirstRun && run_num<=fLastRun) {
      NRead++;
      fRunTitle[run_num-fFirstRun].assign(LineRead.substr(LineRead.find_first_not_of(' ',LineRead.find_first_of(' '))));
    }
  }

  FileIn.close();
  return NRead;
}

//________________________________________________
void HTExperimentInfo::StoreRunTitleInDatabaseFile(const char * run_title, int run_num)
{
  std::ofstream FileOut(Form("config/%s.run",fExperimentName.c_str()),std::ios::app);

  FileOut << run_num << " " << run_title << std::endl;

  FileOut.close();
}

//________________________________________________
const char * HTExperimentInfo::RetrieveRunTitleFromEvtFile(const char * evtfile_name) const
{
  std::ifstream evtfile;
  evtfile.open(evtfile_name,std::ios::binary);

  if(!evtfile.is_open()) {
    return 0;
  }

  unsigned short *point;
  std::string RunTitle;

  for(int j=0; !evtfile.eof(); j++)  // loop over items in a evtfile
  {
    // Reading Header ================================
    int const hBufferWords = 4;
    int const hBufferBytes = hBufferWords*2;
    unsigned short hBuffer[hBufferWords];
    evtfile.read((char*)hBuffer,hBufferBytes);

    point = hBuffer;
    int nbytes = *point++;
    point++;
    int type = *point++;
    // ===============================================

    // Reading the rest of the buffer until I find start of run ================
    const int dBufferBytes = nbytes - 8; //Skipping the inclusive size and data type
    const int dBufferWords = dBufferBytes/2; //Calculating 16 bit words from bytes

    unsigned short dBuffer[dBufferWords];
    evtfile.read((char*)dBuffer,dBufferBytes);
    point = dBuffer;
    // =====================================================================

    // Checking if the body header exists ============
    //This is to read the Body Header
    // Body header has
    int BHsize = *point++;
    point++;

    if(BHsize==20) {
      point +=4; //Skipping the timestamp for now;
      point++;
      point++;
      point++;
      point++;
    }
    else {
      //Buffers with no body header
    }
    // ===============================================

    if (type == 1) { // start of run
      RunTitle.assign((char*)(point+8));
      break;
    }
  }

  evtfile.close();

  return RunTitle.c_str();
}
