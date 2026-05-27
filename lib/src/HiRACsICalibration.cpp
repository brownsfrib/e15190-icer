#include <HiRACsICalibration.h>

EnergyLossModule gLISEModule;

//______________________________________________
HiRACsICalibrationManager::HiRACsICalibrationManager() :
fPulserLoaded(false)
{
  for(int i=0; i<CSICALIB_NUM_TEL; i++) {
    for(int j=0; j<CSICALIB_NUM_CSI_TEL; j++) {
      fCsIChToVInterpolated [i*CSICALIB_NUM_CSI_TEL+j]=0;
      fCsIChToVExtrapolated [i*CSICALIB_NUM_CSI_TEL+j]=0;
    }
  }
  for(int i=0; i<Z_MAX; i++) {
    for(int j=0; j<A_MAX; j++) {
      for(int k=0; k<CSICALIB_NUM_TEL*CSICALIB_NUM_CSI_TEL; k++) {
        fCalib[i][j][k]=0;
      }
    }
  }
}

//______________________________________________
HiRACsICalibrationManager::~HiRACsICalibrationManager()
{
  for(int i=0; i<CSICALIB_NUM_TEL; i++) {
    for(int j=0; j<CSICALIB_NUM_CSI_TEL; j++) {
      if(fCsIChToVInterpolated [i*CSICALIB_NUM_CSI_TEL+j]) {
        delete fCsIChToVInterpolated [i*CSICALIB_NUM_CSI_TEL+j];
        delete fCsIChToVExtrapolated [i*CSICALIB_NUM_CSI_TEL+j];
      }
    }
  }
  for(int i=0; i<Z_MAX; i++) {
    for(int j=0; j<A_MAX; j++) {
      for(int k=0; k<CSICALIB_NUM_TEL*CSICALIB_NUM_CSI_TEL; k++) {
        if (fCalib[i][j][k]) delete fCalib[i][j][k];
      }
    }
  }
}

//______________________________________________
void HiRACsICalibrationManager::Clear()
{
  for(int i=0; i<CSICALIB_NUM_TEL; i++) {
    for(int j=0; j<CSICALIB_NUM_CSI_TEL; j++) {
      if(fChValues[i*CSICALIB_NUM_CSI_TEL+j].size()) {
        fChValues [i*CSICALIB_NUM_CSI_TEL+j].clear();
        fVoltageValues [i*CSICALIB_NUM_CSI_TEL+j].clear();
      }
      if(fCsIChToVInterpolated [i*CSICALIB_NUM_CSI_TEL+j]) {
        delete fCsIChToVInterpolated [i*CSICALIB_NUM_CSI_TEL+j];
        delete fCsIChToVExtrapolated [i*CSICALIB_NUM_CSI_TEL+j];
      }
    }
  }
  fPulserLoaded=false;
}

//______________________________________________
int HiRACsICalibrationManager::LoadPulserInfo(const char * file_name)
{
  Clear();

  std::ifstream FileIn(file_name);
  if(!FileIn.is_open()) {
    printf("Error: error while reading pulser file\n");
    return -1;
  }
  int NRead=0;

  while (!FileIn.eof())
  {
    std::string LineRead;
    std::getline(FileIn, LineRead);

    if(LineRead.empty()) continue;
    if(LineRead.find('*')==0) continue;

    std::istringstream LineStream(LineRead);

    int numtel;
    int numcsi;
    int peaknum;
    double voltage;
    double channel;

    LineStream >> numtel >> numcsi >> peaknum >> voltage >> channel;

    if(channel==0) continue;

    fChValues [numtel*CSICALIB_NUM_CSI_TEL+numcsi].push_back(channel);
    fVoltageValues [numtel*CSICALIB_NUM_CSI_TEL+numcsi].push_back(voltage);

    NRead++;
  }


  for(int i=0; i<CSICALIB_NUM_TEL; i++) {
    for(int j=0; j<CSICALIB_NUM_CSI_TEL; j++) {
      fCsIChToVExtrapolated [i*CSICALIB_NUM_CSI_TEL+j] = new TGraph(fChValues[i*CSICALIB_NUM_CSI_TEL+j].size(), fChValues[i*CSICALIB_NUM_CSI_TEL+j].data(), fVoltageValues[i*CSICALIB_NUM_CSI_TEL+j].data());
      fCsIChToVInterpolated [i*CSICALIB_NUM_CSI_TEL+j] = new TSpline3(Form("CsIChToVInterpolated%02d_%02d", i, j), fCsIChToVExtrapolated [i*CSICALIB_NUM_CSI_TEL+j]);
      //fCsIChToVInterpolated [i*CSICALIB_NUM_CSI_TEL+j]->SetBit(TGraph::kIsSortedX);
    }
  }

  fPulserLoaded=true;
  return NRead;
}

//______________________________________________
double HiRACsICalibrationManager::GetVoltageValue(double ch, int numtel, int numcsi) const
{
  if(fCsIChToVInterpolated[numtel*CSICALIB_NUM_CSI_TEL+numcsi]==0) return -1;
  if(ch>=fCsIChToVInterpolated[numtel*CSICALIB_NUM_CSI_TEL+numcsi]->GetXmin() && ch<=fCsIChToVInterpolated[numtel*CSICALIB_NUM_CSI_TEL+numcsi]->GetXmax()) {
    return fCsIChToVInterpolated[numtel*CSICALIB_NUM_CSI_TEL+numcsi]->Eval(ch);
  } else {
    return fCsIChToVExtrapolated[numtel*CSICALIB_NUM_CSI_TEL+numcsi]->Eval(ch,0,"");
  }
}

// CsI Calibrations are obtained with a different procedure depending on the isotope
// according to the following cases:
// *  Z=1 : A=1, A=2 and A=3 are calibrated by using a light corrected formula. Because of the time consuming calculation, this formula is computed
//    only once at the beginning of the analysis process to create fiducial points that are then interpolated. This calibration starts from the CsI values in Volt.
// *  Z=2 : A=3 and A=4 as a first approximation, these calibrations are here considered linear V -> MeV
// *  Z=3 : A=6 and A=7 are for now linear
// *  Z=4 : A=7 are for now linear
//
//______________________________________________
double HiRACsICalibrationManager::GetEnergyValue(double ch, int numtel, int numcsi, int Z, int A) const
{
  if(fCalib[Z][A][numtel*CSICALIB_NUM_CSI_TEL+numcsi]==0 || !fPulserLoaded) return -1;
  if(Z==1 || Z==2) { //Get Energy from Z=1 and Z=2 calibrations
    return fCalib[Z][A][numtel*CSICALIB_NUM_CSI_TEL+numcsi]->GetEnergy(GetVoltageValue(ch,numtel,numcsi));
  } else if (Z==3 || Z==4) { //Get Energy from Z=3 and Z=4 calibrations
    return fCalib[1][1][numtel*CSICALIB_NUM_CSI_TEL+numcsi] ?
    fCalib[Z][A][numtel*CSICALIB_NUM_CSI_TEL+numcsi]->GetEnergy(fCalib[1][1][numtel*CSICALIB_NUM_CSI_TEL+numcsi]->GetEnergy(GetVoltageValue(ch,numtel,numcsi))) : -1;
  }
}

//______________________________________________
void HiRACsICalibrationManager::DrawChVoltage(int numtel, int numcsi) const
{
  if(!fPulserLoaded) {
    printf("No pulser loaded\n");
    return;
  }

  const int NPoints = fChValues [numtel*CSICALIB_NUM_CSI_TEL+numcsi].size();
  if(NPoints==0) {
    printf("No pulser available for this crystal\n");
    return;
  }
  const double *CsChPoints = fChValues [numtel*CSICALIB_NUM_CSI_TEL+numcsi].data();
  const double *CsVoltagePoints = fVoltageValues [numtel*CSICALIB_NUM_CSI_TEL+numcsi].data();

  const int NPointsInterpolation = NPoints*1000;
  double CsChPointsInterpolation[NPointsInterpolation];
  double CsVoltagePointsInterpolation[NPointsInterpolation];

  for(int i=0; i<NPointsInterpolation; i++) {
    CsChPointsInterpolation[i]=double(i*4096)/NPointsInterpolation;
    CsVoltagePointsInterpolation[i]=fCsIChToVExtrapolated[numtel*CSICALIB_NUM_CSI_TEL+numcsi]->Eval(CsChPointsInterpolation[i],0,"");
  }

  TGraph *PulserGraph = new TGraph(NPoints,CsChPoints,CsVoltagePoints);
  TGraph *PulserGraphInterpolation = new TGraph(NPointsInterpolation,CsChPointsInterpolation,CsVoltagePointsInterpolation);

  //TCanvas c1 ("c1", "", 800, 600);
  PulserGraphInterpolation->Draw("AL");
  PulserGraph->Draw("same *");
  PulserGraphInterpolation->SetLineColor(kRed);
  PulserGraph->SetMarkerColor(kBlue);
  PulserGraphInterpolation->SetTitle(Form("HiRA_CsI_Pulser_VvsCh_%02d_%02d",numtel,numcsi));
  //c1.Print(Form("pictures/HiRA_CsI_Pulser_VvsCh_%02d_%02d.png",numtel,numcsi));

  return;
}

//______________________________________________
int HiRACsICalibrationManager::LoadEnergyCalibration(const char * file_name)
{
  std::ifstream FileIn(file_name);
  if(!FileIn.is_open()) {
    printf("Error: error while opening CsI calibration file\n");
    return -1;
  }
  int NRead=0;

  while (!FileIn.eof())
  {
    std::string LineRead;
    std::getline(FileIn, LineRead);

    if(LineRead.empty()) continue;
    if(LineRead.find('*')==0) continue;

    std::istringstream LineStream(LineRead);

    int Z;
    int A;
    int numtel;
    int numcsi;
    double normalization;
    double gradient;

    LineStream >> Z >> A >> numtel >> numcsi >> normalization >> gradient;

    if(Z>Z_MAX && A>A_MAX) continue;

    fCalib[Z][A][numtel*CSICALIB_NUM_CSI_TEL+numcsi]= new HiRACsICalibration(Z, A);

    fCalib[Z][A][numtel*CSICALIB_NUM_CSI_TEL+numcsi]->SetNumParameters(2);
    fCalib[Z][A][numtel*CSICALIB_NUM_CSI_TEL+numcsi]->SetParameter(0,normalization);
    fCalib[Z][A][numtel*CSICALIB_NUM_CSI_TEL+numcsi]->SetParameter(1,gradient);
    fCalib[Z][A][numtel*CSICALIB_NUM_CSI_TEL+numcsi]->InitCalibration();

    NRead++;
  }

  return NRead;
}

//______________________________________________
HiRACsICalibration * HiRACsICalibrationManager::GetCalibration(int numtel, int numcsi, int Z, int A) const
{
  return fCalib[Z][A][numtel*CSICALIB_NUM_CSI_TEL+numcsi];
}

//______________________________________________
HiRACsICalibration::HiRACsICalibration(int Z, int A) :
fSimulationModule(0),
fNumParameters(0),
fParameters(0),
fCalibrationFunc(0),
fCalibrationInitialized(false),
fZ(Z),
fA(A),
fVtoEInterpolated(0),
fVtoEExtrapolated(0)
{
  fSimulationModule= new HiRACsISimulation();
}

//______________________________________________
HiRACsICalibration::~HiRACsICalibration()
{
  delete fSimulationModule;
}

//______________________________________________
void HiRACsICalibration::SetNumParameters(int num_parameters)
{
  fNumParameters=num_parameters;
  fParameters = new double[fNumParameters];
  return;
}

//______________________________________________
void HiRACsICalibration::SetParameter(int par_to_set, double value)
{
  fParameters[par_to_set]=value;
}

//______________________________________________
void HiRACsICalibration::InitCalibration()
{
  //protons, deuterons or tritons
  if((fZ==1 && fA==1) || (fZ==1 && fA==2) || (fZ==1 && fA==3)) {  //Calibration for Z=1
    int NPoints=100;
    for(int i=1; i<NPoints; i++)
    {
      double range=(i)*CsILength/NPoints;
      double energy=gLISEModule.GetEnergyFromRange(fZ,fA,range,"CsI");
      fLISEEnergyMeV.push_back(energy);
      TF1 * light_response = new TF1("light_response","1-[0]*x", 0, 200000);
      light_response->SetParameter(0, fParameters[1]);
      double Light=fParameters[0]*fSimulationModule->GetSimulatedLightFromEnergy(fZ,fA,energy,light_response,CsILength);
      fCsIRawV.push_back(Light);
      delete light_response;
    }
    fCalibrationInitialized=true;
    fVtoEExtrapolated= new TGraph(fCsIRawV.size(), fCsIRawV.data(), fLISEEnergyMeV.data());
    fVtoEInterpolated = new TSpline3("VtoEInterpolated", fVtoEExtrapolated);
  } else if(fZ==2) {  //Calibration for Z=2
    fCalibrationInitialized=true;
    fCalibrationFunc = new TF1 ("fCalibrationFunc", "[0]+[1]*x", 0, 10);
    fCalibrationFunc->SetParameters(fParameters);
  } else if (fZ==3 || fZ==4) {
    fCalibrationInitialized=true;
    fCalibrationFunc = new TF1 ("fCalibrationFunc", "[0]+[1]*x", 0, 10);
    fCalibrationFunc->SetParameters(fParameters);
  } else {
    fCalibrationInitialized=false;
  }
}

//______________________________________________
double HiRACsICalibration::GetEnergy(double V) const
{
  if(!fCalibrationInitialized) {
    return -1;
  }
  if(fZ==1) { //Get Energy for Z=1
    if(V>=fVtoEInterpolated->GetXmin() && V<=fVtoEInterpolated->GetXmax()) {
      return fVtoEInterpolated->Eval(V);
    } else {
      return fVtoEExtrapolated->Eval(V,0,"");
    }
  } else if(fZ==2) { //Get Energy for Z=2
    return fCalibrationFunc->Eval(V);
  } else if(fZ==3 || fZ==4) { //Get Energy for Z=3 and Z=4
    return fCalibrationFunc->Eval(V);
  } else return -1;
}

//______________________________________________
void HiRACsICalibration::CheckCalibrationValidity(const char * file_name, int tel, int csi)
{
  //TCanvas * c1 = new TCanvas("check","Check on Calibrations", 800, 600);
  std::ifstream FileIn(file_name);

  if(!FileIn.is_open()) {
    printf("Error: file %s not found\n", file_name);
    return;
  }

  int npoints=0;
  double csivoltage[500];
  double err_csivoltage[500];
  double energy[500];
  double err_energy[500];

  while (!FileIn.eof()) {
    std::string LineRead;
    std::getline(FileIn, LineRead);

    if(LineRead.empty()) continue;
    if(LineRead.find('*')==0) continue;

    std::istringstream LineStream(LineRead);

    int numtel;
    int numcsi;

    LineStream >> numtel>> numcsi;
    if(numtel!=tel || numcsi!=csi) continue;
    LineStream >> csivoltage[npoints] >> err_csivoltage[npoints] >> energy[npoints] >> err_energy[npoints] ;

    if(err_energy[npoints]==0) continue;

    npoints++;
  }

  const int NPointsInterpolation = npoints*1000;
  double CsIVoltagePointsInterpolation[NPointsInterpolation];
  double CsIEnergyPointsInterpolation[NPointsInterpolation];

  for(int i=0; i<NPointsInterpolation; i++) {
    CsIVoltagePointsInterpolation[i]=double(i*2)/NPointsInterpolation;
    CsIEnergyPointsInterpolation[i]=fVtoEExtrapolated->Eval(CsIVoltagePointsInterpolation[i],0,"");
  }

  TGraph * GraphCalibration = new TGraph(NPointsInterpolation,CsIVoltagePointsInterpolation,CsIEnergyPointsInterpolation);
  TGraphErrors * GraphPoints = new TGraphErrors(npoints, csivoltage, energy, err_csivoltage, err_energy);

  GraphCalibration->Draw("AL");
  GraphPoints->Draw("same *");
  GraphCalibration->SetLineColor(kRed);
  GraphPoints->SetMarkerColor(kBlue);
  GraphCalibration->SetTitle(Form("HiRA_CsI_EvsV_%02d_%02d",tel,csi));
}

//______________________________________________
int HiRACsISimulation::LoadEnergyLossFile(const char * file_name)
{
  ClearEnergyLossInfo();

  std::ifstream FileIn(file_name);
  if(!FileIn.is_open()) {
    printf("Error: error while reading energy loss file\n");
    return -1;
  }
  int NRead=0;

  while (!FileIn.eof())
  {
    std::string LineRead;
    std::getline(FileIn, LineRead);

    if(LineRead.empty()) continue;
    if(LineRead.find('*')==0) continue;

    std::istringstream LineStream(LineRead);

    double energy;
    double eloss;

    for(int i=0; i<NUM_MODELS_ELOSS; i++) {
      LineStream >> energy >> eloss;
      LiseELoss[i].push_back(eloss);
    }

    ParticleEnergy.push_back(energy);

    NRead++;
  }

  Emin=ParticleEnergy[0];
  Emax=ParticleEnergy[ParticleEnergy.size()-1];

  for(int i=0; i<NUM_MODELS_ELOSS; i++) {
    SplineInterpolator[i].SetData(ParticleEnergy,LiseELoss[i]);
  }

  return NRead;
}

//______________________________________________
void HiRACsISimulation::ClearEnergyLossInfo()
{
  ParticleEnergy.clear();
  for(int i=0; i<NUM_MODELS_ELOSS; i++) {
    if(LiseELoss[i].size()) {
      LiseELoss[i].clear();
    }
  }
}

//______________________________________________
HiRACsISimulation::HiRACsISimulation() :
NucData(0)
{
  NucData=new nuclear_masses(PROJECT_ROOT "/data/masses.conf");
}

//______________________________________________
HiRACsISimulation::~HiRACsISimulation()
{
  delete NucData;
}


//______________________________________________
double HiRACsISimulation::GetSimulatedLightFromEnergy(int Z, int A, double Einc, TF1 * LightResponse, double CsIthickness_um, int model)
{
  double Precision=0.0001;
  double dThicknessMin=CsIthickness_um*1E-4;
  double IntegrateThickness=0;
  double dThickness=dThicknessMin;
  double Eresidual=Einc;
  double ELoss=0;
  double SimulatedLight=0;

  double mass_uma=NucData->get_mass_Z_A_uma(Z,A);

  char material[]="CsI";
  if(LoadEnergyLossFile(Form("input/LISE_ELoss_Z%02d_A%02d_%s.dat", Z, A, material))<=0) {
    printf("Error: information not present for Z=%d A=%d material=%s\n", Z, A, material);
    return -100;
  }

  for(;IntegrateThickness<CsIthickness_um; IntegrateThickness+=dThickness)
  {
    if(Eresidual<=Emin*mass_uma) { //the particle stopped in the material
      ELoss=Einc;
      return SimulatedLight+Eresidual*( LightResponse ? LightResponse->Eval(IntegrateThickness) : 1 );
    }

    if(SplineInterpolator[model].Deriv(Eresidual/mass_uma)!=0) {
      dThickness=fmin(dThicknessMin,std::abs(Precision/(SplineInterpolator[model].Eval(Eresidual/mass_uma)*SplineInterpolator[model].Deriv(Eresidual/mass_uma)))); //variable integration step with fixed precision
    }

    double ELossStep=dThickness*SplineInterpolator[model].Eval(Eresidual/mass_uma);

    SimulatedLight+=( LightResponse ? LightResponse->Eval(IntegrateThickness) : 1 )*ELossStep;

    ELoss+=ELossStep;
    Eresidual-=ELossStep;
  }

  return SimulatedLight;
}
