#include "include/E15190Reader.h"

//____________________________________________________
E15190Reader::E15190Reader(TChain * Chain, HTRunInfo * CurrRunInfo, const char * DataType, bool IsDataCalibrated) :
	fCurrRunInfo(CurrRunInfo),
	fHiRA(new TTreeReaderValue<HTHiRAData> *[NUM_TEL]),
	fChain(Chain),
	fStartTime(clock()),
	fDegToRad(TMath::Pi()/180.),
	fRadToDeg(180./TMath::Pi()),
	fSpeedOfLight(29.9792),
	fIsTDC(false),
	fIsMB(false),
	fIsHiRA(false),
	fIsDataCalibrated(IsDataCalibrated),
	fMBGeometryLoaded(false),
	fMBStatusLoaded(false),
	fMBHitConditionLoaded(false),
	fMBCentralityLoaded(false),
	fHiRACsICalibrated(false),
	fHiRACsIPulserCalibrated(false),
	fHiRASiCalibrated(false),
	fHiRAGeometryCalibrated(false),
	fHiRAStripBadLoaded(false),
	fHiRASiHiLowMatched(false),
	fMicroballStatus(new MBDetectorStatus()),
	fMicroballGeometry(new MBGeometry()),
	fMicroballHitCondition(new MBHitCondition()),
	fMicroballCentrality(new MBImpactParameter()),
	fHiRAGeometryTab(new HiRAGeometry(NUM_TEL,NUM_STRIP_F,NUM_STRIP_B)),
	fSiCalibrationTools(new HiRASiCalibration(NUM_TEL,NUM_STRIP_F,NUM_STRIP_B)),
	fCsICalibrationModule(new HiRACsICalibrationManager()),
	fHiRAStatus(new HiRADetectorStatus(NUM_TEL,NUM_STRIP_F,NUM_STRIP_B)),
	fHiRAIdentifiationModule(new HiRAIdentification()),
	fHiRAPixelizationModule(new HiRAPixelization(NUM_TEL))
{
	//Parsing DataType string to allocate specific detectors
	std::string DetectorsIncluded(DataType);
	std::istringstream StreamDetectorsIncluded(DetectorsIncluded);
	std::string DetectorToAdd;
	while(std::getline(StreamDetectorsIncluded, DetectorToAdd, '-')){
		if(DetectorToAdd.compare("uBall")==0){
			fIsMB=true;
		}
		if(DetectorToAdd.compare("HiRA")==0){
			fIsHiRA=true;
		}
		if(DetectorToAdd.compare("TDC")==0){
			fIsTDC=true;
		}
	}
	
	if(fChain!=0){
		fE15190Reader = new TTreeReader(fChain);
		if(!fIsDataCalibrated){
			if(fIsMB){
				fMicroball = new TTreeReaderValue<HTMicroballData>(*fE15190Reader, "uBall.");
			}
			if(fIsHiRA){
				for(int i=0; i<NUM_TEL; i++){
					fHiRA[i] = new TTreeReaderValue<HTHiRAData>(*fE15190Reader, Form("HiRA.tele%d.",i));
				}
			}
		}
		else{
			if(fIsMB){ 
				fMicroballCal = new TTreeReaderValue<MicroballCalibratedData>(*fE15190Reader, "uBall.");
			}
			if(fIsHiRA){
				fHiRACal = new TTreeReaderValue<HiRACalibratedData>(*fE15190Reader, "HiRA.");
			}
		}
		
		if(fIsTDC){
			fTDCAdditionalChannels = new TDCSpareChannels();
			fTDCAdditionalChannels->InitFromMapping(fE15190Reader, fCurrRunInfo->GetMappingFile());
		}
	}
}

//____________________________________________________
E15190Reader::~E15190Reader(){
	if(fChain!=0){
		delete fE15190Reader;
		if(!fIsDataCalibrated){
			if (fIsMB){
				delete fMicroball;
			}
			if (fIsHiRA){
				for(int i=0; i<NUM_TEL; i++){
					delete fHiRA[i];
				}
			}
		}
		else{
			if (fIsMB){
				delete fMicroballCal;	      
			}
			if (fIsHiRA){
				delete fHiRACal;
			}
		}
	}
	
	if(fHiRA){
		delete [] fHiRA;
	}
}

//____________________________________________________
void E15190Reader::InitAllCalibrations(){
	LoadMBGeometry(fCurrRunInfo->GetMBGeometryFileName());
	LoadMBDetectorStatus(fCurrRunInfo->GerMBDetectorStatusFileName());
	LoadMBFastSlowHitCondition(fCurrRunInfo->GetMBHitConditionFileName());
	LoadMBCentrality(fCurrRunInfo->GetMBImpactParameterFileName());
	LoadHiRAGeometry(fCurrRunInfo->GetHiRAGeometryFileName());
	LoadHiRASiCalibration(fCurrRunInfo->GetHiRASiEnergyCalibrationFileName());
	LoadHiRAStripBad(fCurrRunInfo->GetHiRADetectorStatusFileName());
	LoadHiRACsIPulserInfo(fCurrRunInfo->GetHiRACsIPulserFileName());
	LoadHiRASiHiLowMatching(fCurrRunInfo->GetHiRASiHiLowMatchingFileName());
	LoadHiRACsICalibration(fCurrRunInfo->GetHiRACsIEnergyCalibrationFileName());
	LoadHiRAIdentification(fCurrRunInfo->GetHiRAPIDFileName());
	
	fBeam = new TNamed("Beam", fCurrRunInfo->GetBeam());
	fBeamEnergy = new TNamed("Beam Energy (AMeV)", fCurrRunInfo->GetBeamEnergy());
	fTarget = new TNamed("Target", fCurrRunInfo->GetTarget());
}


//____________________________________________________
int E15190Reader::LoadMBGeometry(const char * file_name){
	if(!fIsMB){
		return 0;
	}
	int NLines=fMicroballGeometry->LoadGeometry(file_name);
	if(NLines>0){
		fMBGeometryLoaded=true;
		printf("Loaded Microball geometry from file %s\n", file_name);
		return NLines;
	}
	else{
		fMBGeometryLoaded=false;
		printf("Error: Error while loading Microball geometry %s\n", file_name);
		return -1;
	}
}

//____________________________________________________
int E15190Reader::LoadMBDetectorStatus(const char * file_name){
	if(!fIsMB){
		return 0;
	}
	int NLines=fMicroballStatus->LoadBadDetectors(file_name);
	if(NLines>0){
		fMBStatusLoaded=true;
		printf("Loaded Microball status from file %s\n", file_name);
		return NLines;
	}
	else{
		fMBStatusLoaded=false;
		printf("Error: Error while loading Microball status %s\n", file_name);
		return -1;
	}
}

//____________________________________________________
int E15190Reader::LoadMBFastSlowHitCondition(const char * file_name){
	if(!fIsMB){
		return 0;
	}
	int NLines=fMicroballHitCondition->LoadFastSlowCuts(file_name);
	if(NLines>0){
		fMBHitConditionLoaded=true;
		printf("Loaded Microball Fast Slow Hit conditions from file %s\n", file_name);
		return NLines;
	}
	else{
		fMBHitConditionLoaded=false;
		printf("Error: Error while loading Microball Fast Slow Hit conditions %s\n", file_name);
		return -1;
	}
}

//____________________________________________________
int E15190Reader::LoadMBCentrality(const char * file_name){
	if(!fIsMB){
		return 0;
	}
	int NLines=fMicroballCentrality->LoadImpactParameter(file_name);
	if(NLines>0){
		fMBCentralityLoaded=true;
		printf("Loaded Microball centrality conditions from file %s\n", file_name);
		return NLines;
	}
	else{
		fMBCentralityLoaded=false;
		printf("Error: Error while loading Microball centrality conditions %s\n", file_name);
		return -1;
	}
}

// 7/25/24 - Updated to fetch the crystal distances from the Microball calibration file :)
// Distances are in cm !!! don't forget it !!!
//____________________________________________________
double E15190Reader::GetMBDistance(int num_ring, int num_det) const {
	return fMBGeometryLoaded ? fMicroballGeometry->GetDistance(num_ring, num_det) : -9999;
}

//____________________________________________________
double E15190Reader::GetMBTheta(int num_ring, int num_det) const {
	return fMBGeometryLoaded ? fMicroballGeometry->GetTheta(num_ring, num_det) : -9999;
}

//____________________________________________________
double E15190Reader::GetMBPhi(int num_ring, int num_det) const {
	return fMBGeometryLoaded ? fMicroballGeometry->GetPhi(num_ring, num_det) : -9999;
}

//____________________________________________________
double E15190Reader::GetMBThetaRandom(int num_ring, int num_det) const {
	return fMBGeometryLoaded ? fMicroballGeometry->GetThetaRandom(num_ring, num_det) : -9999;
}

//____________________________________________________
double E15190Reader::GetMBPhiRandom(int num_ring, int num_det) const {
	return fMBGeometryLoaded ? fMicroballGeometry->GetPhiRandom(num_ring, num_det) : -9999;
}

//____________________________________________________
double E15190Reader::GetMBImpactParameter(int multiplicity) const {
	return fMBCentralityLoaded ? fMicroballCentrality->GetImpactParameter(multiplicity) : -9999;
}

//____________________________________________________
double E15190Reader::GetMBbhat(int multiplicity) const {
	return fMBCentralityLoaded ? fMicroballCentrality->Getbhat(multiplicity) : -9999;
}

//____________________________________________________
bool E15190Reader::IsMBDetectorBad (int num_ring, int num_det) const {
	return fMBStatusLoaded ? fMicroballStatus->IsBad(num_ring,num_det) : false;
}

//____________________________________________________
bool E15190Reader::IsMBHit (int num_ring, int num_det, double fast, double tail, double time) const {
	return fMBHitConditionLoaded ? fMicroballHitCondition->IsHit(num_ring,num_det,fast,tail,time) : false;
}

//____________________________________________________
int E15190Reader::LoadHiRAGeometry(const char * file_name){
	if(!fIsHiRA){
		return 0;
	}
	int NLines=fHiRAGeometryTab->LoadPixels(file_name);
	if(NLines>0){
		fHiRAGeometryCalibrated=true;
		printf("Loaded HiRA Geometry from file %s\n", file_name);
		return NLines;
	}
	else{
		fHiRAGeometryCalibrated=false;
		printf("Error: Error while loading HiRA Geometry %s\n", file_name);
		return -1;
	}
}

//____________________________________________________
int E15190Reader::LoadHiRAStripBad(const char * file_name){
	if(!fIsHiRA){
		 return 0;
	}
	int NLines=fHiRAStatus->LoadStipBad(file_name);
	if(NLines>0){
		fHiRAStripBadLoaded=true;
		printf("Loaded HiRA Strip bad from file %s\n", file_name);
		return NLines;
	}
	else{
		fHiRAStripBadLoaded=false;
		printf("Error: Error while loading HiRA Strip bad %s\n", file_name);
		return -1;
	}
}

//____________________________________________________
int E15190Reader::LoadHiRASiCalibration(const char * file_name){
	if(!fIsHiRA){
		return 0;
	}
	int NLines=fSiCalibrationTools->LoadCalibration(file_name);
	if(NLines>0){
		fHiRASiCalibrated=true;
		printf("Loaded HiRA Silicon calibration from file %s\n", file_name);
		return NLines;
	}
	else{
		fHiRASiCalibrated=false;
	printf("Error: Error while loading HiRA Silicon calibration %s\n", file_name);
	return -1;
	}
}

//____________________________________________________
int E15190Reader::LoadHiRACsICalibration(const char * file_name){
	if(!fIsHiRA){
		return 0;
	}
	int NLines=fCsICalibrationModule->LoadEnergyCalibration(file_name);
	if(NLines>0){
		fHiRACsICalibrated=true;
		printf("Loaded HiRA CsI calibrations from file %s\n", file_name);
		return NLines;
	}
	else{
		fHiRACsICalibrated=false;
		printf("Error: Error while loading HiRA CsI calibrations from file %s\n", file_name);
		return -1;
	}
}

//____________________________________________________
int E15190Reader::LoadHiRASiHiLowMatching(const char * file_name){
	if(!fIsHiRA){
		return 0;
	}
	int NLines=fSiCalibrationTools->LoadMatching(file_name);
	if(NLines>0){
		fHiRASiHiLowMatched=true;
		printf("Loaded HiRA Silicon Hi Gain Low Gain from file %s\n", file_name);
		return NLines;
	}
	else{
		fHiRASiHiLowMatched=false;
		printf("Error: Error while loading HiRA Silicon Hi Gain Low Gain %s\n", file_name);
		return -1;
	}
}

//____________________________________________________
int E15190Reader::LoadHiRACsIPulserInfo(const char * file_name){
	if(!fIsHiRA){
		return 0;
	}
	int NLines=fCsICalibrationModule->LoadPulserInfo(file_name);
	if(NLines>0){
		fHiRACsIPulserCalibrated=true;
		printf("Loaded HiRA CsI pulser settings from file %s\n", file_name);
		return NLines;
	}
	else{
		fHiRACsIPulserCalibrated=false;
		printf("Error: Error while loading HiRA CsI pulser settings %s\n", file_name);
		return -1;
	}
}

//____________________________________________________
int E15190Reader::LoadHiRAIdentification(const char * file_name){
	if(!fIsHiRA){
		return 0;
	}
	int NLines=fHiRAIdentifiationModule->LoadIdentification(file_name);
	if(NLines>0){
		fHiRAIdentificationLoaded=true;
		printf("Loaded HiRA PID from file %s\n", file_name);
		return NLines;
	}
	else{
		fHiRAIdentificationLoaded=false;
		printf("Error: Error while loading HiRA PID %s\n", file_name);
		return -1;
	}
}

//____________________________________________________
bool E15190Reader::IsStripfBad(int telescope, int strip_front) const {
	return fHiRAStripBadLoaded ? fHiRAStatus->IsStripfBad(telescope, strip_front) : false;
}

//____________________________________________________
bool E15190Reader::IsStripbBad(int telescope, int strip_back) const {
	return fHiRAStripBadLoaded ? fHiRAStatus->IsStripbBad(telescope, strip_back) : false;
}

//____________________________________________________
double E15190Reader::GetThetaPixel(int telescope, int strip_front, int strip_back) const {
	return fHiRAGeometryCalibrated ? fHiRAGeometryTab->GetThetaPixel(telescope,strip_front,strip_back) : -9999;
}

//____________________________________________________
double E15190Reader::GetPhiPixel(int telescope, int strip_front, int strip_back) const {
	return fHiRAGeometryCalibrated ? fHiRAGeometryTab->GetPhiPixel(telescope,strip_front,strip_back) : -9999;
}

//____________________________________________________
double E15190Reader::GetThetaPixelDeg(int telescope, int strip_front, int strip_back) const {
	return fHiRAGeometryCalibrated ? fHiRAGeometryTab->GetThetaPixelDeg(telescope,strip_front,strip_back) : -9999;
}

//____________________________________________________
double E15190Reader::GetPhiPixelDeg(int telescope, int strip_front, int strip_back) const {
	return fHiRAGeometryCalibrated ? fHiRAGeometryTab->GetPhiPixelDeg(telescope,strip_front,strip_back) : -9999;
}

//____________________________________________________
double E15190Reader::GetSifIntercept(int telescope, int numstrip) const {
	return fHiRASiCalibrated ? fSiCalibrationTools->GetIntercept(telescope,numstrip,0) : -9999;
}

//____________________________________________________
double E15190Reader::GetSibIntercept(int telescope, int numstrip) const {
	return fHiRASiCalibrated ? fSiCalibrationTools->GetIntercept(telescope,numstrip,1) : -9999;
}

//____________________________________________________
double E15190Reader::GetSifSlope(int telescope, int numstrip) const {
	return fHiRASiCalibrated ? fSiCalibrationTools->GetSlope(telescope,numstrip,0) : -9999;
}

//____________________________________________________
double E15190Reader::GetSibSlope(int telescope, int numstrip) const {
	return fHiRASiCalibrated ? fSiCalibrationTools->GetSlope(telescope,numstrip,1) : -9999;
}

//____________________________________________________
double E15190Reader::GetSifHiLowMatched(int chHi, int chLow, int telescope, int numstrip) const {
	return fHiRASiHiLowMatched ? fSiCalibrationTools->GetHiLowMatched(gRandom->Uniform(chHi-0.5,chHi+0.5),gRandom->Uniform(chLow-0.5,chLow+0.5),telescope,numstrip,0) : -9999;
}

//____________________________________________________
double E15190Reader::GetSibHiLowMatched(int chHi, int chLow, int telescope, int numstrip) const {
	return fHiRASiHiLowMatched ? fSiCalibrationTools->GetHiLowMatched(gRandom->Uniform(chHi-0.5,chHi+0.5),gRandom->Uniform(chLow-0.5,chLow+0.5),telescope,numstrip,1): -9999;
}

//____________________________________________________
double E15190Reader::GetSifHiLowMatchedEMeV(int chHi, int chLow, int telescope, int numstrip) const {
	return fHiRASiHiLowMatched ? fSiCalibrationTools->GetEnergyHiLowMatched(gRandom->Uniform(chHi-0.5,chHi+0.5),gRandom->Uniform(chLow-0.5,chLow+0.5),telescope,numstrip,0): -9999;
}

//____________________________________________________
double E15190Reader::GetSibHiLowMatchedEMeV(int chHi, int chLow, int telescope, int numstrip) const {
	return fHiRASiHiLowMatched ? fSiCalibrationTools->GetEnergyHiLowMatched(gRandom->Uniform(chHi-0.5,chHi+0.5),gRandom->Uniform(chLow-0.5,chLow+0.5),telescope,numstrip,1): -9999;
}

//____________________________________________________
double E15190Reader::GetCsIEMeV(int ch, int telescope, int numcsi, int Z, int A) const {
	return Z>0&&A>0&&fHiRACsICalibrated&&fHiRACsIPulserCalibrated ? fCsICalibrationModule->GetEnergyValue(gRandom->Uniform(ch-0.5,ch+0.5), telescope, numcsi, Z, A) : -9999;
}

//____________________________________________________
double E15190Reader::GetSifEMeV(int ch, int telescope, int numstripf) const {
	return fHiRASiCalibrated ? fSiCalibrationTools->GetEnergy(gRandom->Uniform(ch-0.5,ch+0.5),telescope,numstripf,0) : -9999;
}

//____________________________________________________
double E15190Reader::GetSibEMeV(int ch, int telescope, int numstripb) const {
	return fHiRASiCalibrated ? fSiCalibrationTools->GetEnergy(gRandom->Uniform(ch-0.5,ch+0.5),telescope,numstripb,1) : -9999;
}

//____________________________________________________
void E15190Reader::PrintPercentage(Long64_t jentry, Long64_t nentries) const {
	double time_elapsed = (double)(clock() - fStartTime)/CLOCKS_PER_SEC;
	std::cout << "  Percentage = " << std::fixed << std::setprecision(1) << std::setw(5) << (100*double(jentry)/nentries) << " %";
	std::cout << "   [";
	int printindex=0;
	for(; printindex<int(100*double(jentry)/nentries); printindex+=5){
		std::cout << "=";
	}
	for(; printindex<100; printindex+=5){
		std::cout << " ";
	}
	std::cout << "]   " << "elapsed time " << std::setprecision(1) <<
	(time_elapsed<60 ? time_elapsed : (time_elapsed<3600 ? time_elapsed/60 : time_elapsed/3600)) <<
	(time_elapsed<60 ? " s; " : (time_elapsed<3600 ? " m; " : " h; "));
	if(jentry>0){
		double time_remaining = (time_elapsed/jentry)*(nentries-jentry);
		std::cout << " estimated remaining time " << std::setprecision(1) <<
		(time_remaining<60 ? time_remaining : (time_remaining<3600 ? time_remaining/60 : time_remaining/3600)) <<
		(time_remaining<60 ? " s      " : (time_remaining<3600 ? " m      " : " h      "));
	}
	std::cout << "\r";
	std::cout.flush();
}

//____________________________________________________
void E15190Reader::PrintPercentageSimple(Long64_t jentry, Long64_t nentries) const {
	std::cout << "  Percentage = " << std::fixed << std::setprecision(1) << std::setw(5) << (100*double(jentry)/nentries) << " %";
	std::cout << "   [";
	int printindex=0;
	for(; printindex<int(100*double(jentry)/nentries); printindex+=5){
		std::cout << "=";
	}
	for(; printindex<100; printindex+=5){
		std::cout << " ";
	}
	std::cout << "]   \r"; std::cout.flush();
}

//____________________________________________________
void E15190Reader::Loop(const char * file_name, Long64_t evt_amount){
	// this method loops on the first "evt_amount" data entries
	// and creates a ROOT otput file named "file_name"
	// where the user can write some ROOT output
	TFile * FileOut = new TFile(file_name, "RECREATE");
	
	Long64_t nentries=fChain->GetEntries();
	if(evt_amount!=0) {
		nentries=evt_amount;
	}
	Long64_t jentry=0;
	std::cout << "found " << nentries << " entries\n";
	fStartTime = clock(); // initializing start time
	for(;fE15190Reader->Next() && jentry<nentries; jentry++){
		if(jentry%100000==0){
			PrintPercentage(jentry,nentries);
		}
		
		if (fIsMB){
			HTMicroballData * Microball = fMicroball->Get();
			//Insert Microball code here
		}
		if (fIsHiRA){
			HTHiRAData * HiRATel[NUM_TEL];
			for(int i=0; i<NUM_TEL; i++){
				HiRATel[i] = fHiRA[i]->Get();
			}
			//Insert HiRA code here
		}
		if (fIsTDC){
			;
			//fTDCAdditionalChannels->FillOutputBranches();
			//Insert TDC spare channels code here. EX: To retrieve TDCTriggers.MASTER_TRG do fTDCAdditionalChannels->GetBranchValue("TDCTriggers.MASTER_TRG");
		}
		
		jentry++;
	}
	
	// closing output file
	FileOut->Close();
}

//____________________________________________________
void E15190Reader::LoopOnCalibratedData(const char * file_name, Long64_t evt_amount){
	// this method loops on the first "evt_amount" data entries
	// and creates a ROOT otput file named "file_name"
	// where the user can write some ROOT output
	TFile * FileOut = new TFile(file_name, "RECREATE");
	
	Long64_t nentries=fChain->GetEntries();
	if(evt_amount!=0) {
		nentries=evt_amount;
	}

	Long64_t jentry=0;
	std::cout << "found " << nentries << " entries\n";
	fStartTime = clock(); // initializing start time

	TTree TestTree("testTree", "Testing some Microball + HiRA coincidence stuff");

	int nMultMax = 1000;
	Int_t HiRAMult;
	TestTree.Branch("HiRAMult", &HiRAMult);
	
	Double_t impact;
	TestTree.Branch("impact", &impact);

	Double_t momentum[nMultMax];
	TestTree.Branch("momentum", momentum);

	Double_t beta[nMultMax];
	TestTree.Branch("beta", beta);

	Int_t A[nMultMax];
	TestTree.Branch("A", A);

	Int_t Z[nMultMax];
	TestTree.Branch("Z", Z);

	for(;fE15190Reader->Next() && jentry<nentries; jentry++){

		if(jentry%100000==0){
			PrintPercentage(jentry,nentries);
		}

		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		// So apparently when the HiRA flag is above the Microball flag 
		// this analysis framework works, but as SOON as you move it 
		// below the Microball tag it breaks. Great framework guys.
		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		
		/*
		if (fIsHiRA) {
			HiRACalibratedData * HiRA = fHiRACal->Get();
			//std::cout << "\033[1;33mHiRA\033[0m" << '\n';
			//Insert HiRA code here
		}
		*/

		if (fIsMB) {
			double amupernucleon = 0.931494028;
			HiRACalibratedData * HiRA = fHiRACal->Get();
			MicroballCalibratedData * Microball = fMicroballCal->Get();

			if ((HiRA->fmulti != 0) && (Microball->fmulti != 0)){
				HiRAMult = HiRA->fmulti;
				impact = Microball->fb;
				for (size_t i=0; i < HiRAMult; i++){
					int particleA 		= HiRA->fAId[i];
					int particleZ 		= HiRA->fZId[i];
					double kineticEnergy 	= HiRA->fKinEnergy[i]/1000.0;
					double moment 		= HiRA->fMomentum[i]/1000.0;

					if ((particleA > 0) && (particleZ > 0) && (kineticEnergy > 0) && (moment > 0)){
						A[i] 		= particleA;
						Z[i] 		= particleZ;
						momentum[i] 	= moment;

						double mass 	= particleA*amupernucleon;
						double lorentz 	= (kineticEnergy/mass) + 1.0;
						beta[i] 	= sqrt(1.0 - 1.0/std::pow(lorentz, 2.0));
					}
				}
				TestTree.Fill();
			}

		}

		// Do NOT uncomment this. Read the comment above the HiRA flag above.
		/*
		if (fIsHiRA) {
			HiRACalibratedData * HiRA = fHiRACal->Get();
			std::cout << "\033[1;33mHiRA\033[0m" << '\n';
			//Insert HiRA code here
		}
		*/

		if (fIsTDC) {
			fTDCAdditionalChannels->FillOutputBranches();
			//Insert TDC spare channels code here. EX: To retrieve TDCTriggers.MASTER_TRG do fTDCAdditionalChannels->GetBranchValue("TDCTriggers.MASTER_TRG");
		}

		jentry++;
	}

	TestTree.Write();
	FileOut->Close();
	
	return;
}






