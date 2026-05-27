#include <E15190Reader.h>

//____________________________________________________
void E15190Reader::BuildCalibratedEvent(){
	if(fIsHiRA){
		//Analysis of the HiRA detector
		fHiRACalibratedData.fmulti=0;
		
		//Creation of a buffer of calibrated data for the event
		for(int i=0; i<NUM_TEL; i++){
			fHiRACalibratedDataBuffer.fmultistripf=0;
			fHiRACalibratedDataBuffer.fmultistripb=0;
			fHiRACalibratedDataBuffer.fmulticsi=0;
			HTHiRAData * HiRA = fHiRA[i]->Get();
			for(int j=0; j<HiRA->fEF.fmulti; j++){
				if(IsStripfBad(i,HiRA->fEF.fnumstrip[j])){
					continue;
				}
				fHiRACalibratedDataBuffer.fnumstripf[fHiRACalibratedDataBuffer.fmultistripf]=HiRA->fEF.fnumstrip[j];
				fHiRACalibratedDataBuffer.fEnergySifHi[fHiRACalibratedDataBuffer.fmultistripf]=HiRA->fEF.fEnergyHi[j];
				fHiRACalibratedDataBuffer.fEnergySifLo[fHiRACalibratedDataBuffer.fmultistripf]=HiRA->fEF.fEnergyLo[j];
				fHiRACalibratedDataBuffer.fTimeSif[fHiRACalibratedDataBuffer.fmultistripf]=HiRA->fEF.fTime[j];
				fHiRACalibratedDataBuffer.fEnergySifMatched[fHiRACalibratedDataBuffer.fmultistripf]=GetSifHiLowMatched(HiRA->fEF.fEnergyHi[j], HiRA->fEF.fEnergyLo[j], i, HiRA->fEF.fnumstrip[j]);
				fHiRACalibratedDataBuffer.fEnergySifCal[fHiRACalibratedDataBuffer.fmultistripf]=GetSifHiLowMatchedEMeV(HiRA->fEF.fEnergyHi[j], HiRA->fEF.fEnergyLo[j], i, HiRA->fEF.fnumstrip[j]);
				fHiRACalibratedDataBuffer.fmultistripf++;
			}
			for(int j=0; j<HiRA->fEB.fmulti; j++){
				if(IsStripbBad(i,HiRA->fEB.fnumstrip[j])){
					continue;
				}
				fHiRACalibratedDataBuffer.fnumstripb[fHiRACalibratedDataBuffer.fmultistripb]=HiRA->fEB.fnumstrip[j];
				fHiRACalibratedDataBuffer.fEnergySibHi[fHiRACalibratedDataBuffer.fmultistripb]=HiRA->fEB.fEnergyHi[j];
				fHiRACalibratedDataBuffer.fEnergySibLo[fHiRACalibratedDataBuffer.fmultistripb]=HiRA->fEB.fEnergyLo[j];
				fHiRACalibratedDataBuffer.fTimeSib[fHiRACalibratedDataBuffer.fmultistripb]=HiRA->fEB.fTime[j];
				fHiRACalibratedDataBuffer.fEnergySibMatched[fHiRACalibratedDataBuffer.fmultistripb]=GetSibHiLowMatched(HiRA->fEB.fEnergyHi[j], HiRA->fEB.fEnergyLo[j], i, HiRA->fEB.fnumstrip[j]);
				fHiRACalibratedDataBuffer.fEnergySibCal[fHiRACalibratedDataBuffer.fmultistripb]=GetSibHiLowMatchedEMeV(HiRA->fEB.fEnergyHi[j], HiRA->fEB.fEnergyLo[j], i, HiRA->fEB.fnumstrip[j]);
				fHiRACalibratedDataBuffer.fmultistripb++;
			}
			for(int j=0; j<HiRA->fCsI.fmulti; j++){
				fHiRACalibratedDataBuffer.fnumcsi[fHiRACalibratedDataBuffer.fmulticsi]=HiRA->fCsI.fnumcsi[j];
				fHiRACalibratedDataBuffer.fEnergycsi[fHiRACalibratedDataBuffer.fmulticsi]=HiRA->fCsI.fEnergy[j];
				fHiRACalibratedDataBuffer.fTimecsi[fHiRACalibratedDataBuffer.fmulticsi]=HiRA->fCsI.fTime[j];
				fHiRACalibratedDataBuffer.fEnergycsiCal[fHiRACalibratedDataBuffer.fmulticsi]=GetCsIEMeV(HiRA->fCsI.fEnergy[j], i, HiRA->fCsI.fnumcsi[j]);
				fHiRACalibratedDataBuffer.fmulticsi++;
			}
			
			//Pixelization for the current telescope
			fHiRAPixelizationModule->Pixelization(i,&fHiRACalibratedDataBuffer,&fHiRACalibratedData);
		}
		
		//Particle Identification
		if(fHiRAIdentificationLoaded){
			fHiRAIdentifiationModule->PerformIdentification(&fHiRACalibratedData);
		}
		
		//Geometry and Energy
		for(int i=0; i<fHiRACalibratedData.fmulti; i++){
			fHiRACalibratedData.fTheta[i]=GetThetaPixel(fHiRACalibratedData.fnumtel[i], fHiRACalibratedData.fnumstripf[i], fHiRACalibratedData.fnumstripb[i]);
			fHiRACalibratedData.fPhi[i]=GetPhiPixel(fHiRACalibratedData.fnumtel[i], fHiRACalibratedData.fnumstripf[i], fHiRACalibratedData.fnumstripb[i]);
			fHiRACalibratedData.fEnergycsiCal[i]=GetCsIEMeV(fHiRACalibratedData.fEnergycsi[i],fHiRACalibratedData.fnumtel[i],fHiRACalibratedData.fnumcsi[i],fHiRACalibratedData.fZ[i],fHiRACalibratedData.fA[i]);
			fHiRACalibratedData.fEnergycsiCalProtons[i]=GetCsIEMeV(fHiRACalibratedData.fEnergycsi[i],fHiRACalibratedData.fnumtel[i],fHiRACalibratedData.fnumcsi[i], 1, 1);
			fHiRACalibratedData.fKinEnergy[i]=fHiRACalibratedDataBuffer.fEnergySifMatched[i]+fHiRACalibratedData.fEnergycsiCal[i];
			fHiRACalibratedData.fMomentum[i]=-9999;
		}
	}

	if(fIsMB){
		HTMicroballData * Microball = fMicroball->Get();
		//Filling calibrated data structures
		fMicroballCalibratedData.fmulti=0;
		for(int i=0; i<Microball->fmulti; i++){
			if(!IsMBDetectorBad(Microball->fnumring[i], Microball->fnumdet[i]) && IsMBHit(Microball->fnumring[i], Microball->fnumdet[i],Microball->fFast[i],Microball->fTail[i],Microball->fTime[i])) {
				fMicroballCalibratedData.fnumring[fMicroballCalibratedData.fmulti] =Microball->fnumring[i];
				fMicroballCalibratedData.fnumdet[fMicroballCalibratedData.fmulti]  =Microball->fnumdet[i];
				fMicroballCalibratedData.fTail[fMicroballCalibratedData.fmulti]    =Microball->fTail[i];
				fMicroballCalibratedData.fFast[fMicroballCalibratedData.fmulti]    =Microball->fFast[i];
				fMicroballCalibratedData.fTime[fMicroballCalibratedData.fmulti]    =Microball->fTime[i];
				fMicroballCalibratedData.fmulti++;
			}
		}
	}

	if(fIsTDC) {
		fTDCAdditionalChannels->FillOutputBranches();
	}
}

//____________________________________________________
void E15190Reader::BuildCalibratedTree(const char * file_name, Long64_t evt_amount){
	// this method constructs a calibrated data tree
	
	TFile * FileOut = new TFile(file_name, "RECREATE");
	if(FileOut->IsZombie()){
		printf("Error: Cannot create file %s\n", file_name);
		return;
	}
	// Initializing output TTree
	TTree * TreeOut = new TTree ("E15190", "Calibrated Data");
	if(fIsTDC){
		fTDCAdditionalChannels->SetOutputTree(TreeOut);
	}
	if(fIsMB){
		TreeOut->Branch("uBall.","MicroballCalibratedData",&fMicroballCalibratedData,32000,2);
	}
	if(fIsHiRA){
		TreeOut->Branch("HiRA.","HiRACalibratedData",&fHiRACalibratedData,32000,2);
	}

	TreeOut->GetUserInfo()->Add(fBeam);
	TreeOut->GetUserInfo()->Add(fBeamEnergy);
	TreeOut->GetUserInfo()->Add(fTarget);
	
	TreeOut->SetAutoSave(5000000);
	
	Long64_t nentries=fChain->GetEntries();
	if(evt_amount!=0){
		nentries=evt_amount;
	}
	Long64_t jentry=0;
	std::cout << "found " << nentries << " entries\n";
	fStartTime = clock(); // initializing start time
	for(;fE15190Reader->Next() && jentry<nentries; jentry++){
		if((jentry % 100000) == 0){
			PrintPercentage(jentry,nentries);
		}
		
		BuildCalibratedEvent();
		
		TreeOut->Fill();
	}
	
	//Save Output Tree to file
	TreeOut->AutoSave();
	
	// closing output file
	FileOut->Close();
}


// 7/25/24 - Custom methods, forked by Sam from Som
//____________________________________________________
int E15190Reader::Next(){
	int value = fE15190Reader->Next();
	std::cout << "Index: " << value << '\n';
	if(!fIsDataCalibrated && value) {
		BuildCalibratedEvent();
	}
	return value;
}


// Initialize TDC trigger channels for readout
//____________________________________________________
void E15190Reader::SetTDC(){
	fTDCAdditionalChannels->FillOutputBranches();
	return;
}

void* E15190Reader::GetTDCObject(const char* passName){
	if (strcmp(passName, "TDC") == 0 && fIsTDC){
		return (void *)fTDCAdditionalChannels;
	}
}

// Method to fetch TDC trigger leaves
//____________________________________________________
double E15190Reader::GetTDCValue(std::string triggerName){
	double triggerData = fTDCAdditionalChannels->GetBranchValue(triggerName.c_str());
	return triggerData;
}

//____________________________________________________

// 12/9
// All of these methods are depricated
// Now we access the TTreeReader stored in this class directly
// I hate this framework. I need to clean up this class but honestly
// at this point I don't care anymore
MicroballCalibratedData* E15190Reader::GetMicroball(){
	MicroballCalibratedData* object;
	if (fMicroballCal->Get() == NULL){
		std::cout << "\033[1;31mMicroballCal pointer is null \033[0m\n";
	}
	object = fMicroballCal->Get();
	std::cout << "\033[1;31mE15190: Microball mult - " << object->fmulti << "\033[0m\n";
	return object;
}

HiRACalibratedData* E15190Reader::GetHiRA(){
	HiRACalibratedData* object;
	if (fHiRACal->Get() == NULL){
		std::cout << "\033[1;31mHiRACal pointer is null \033[0m\n";
	}
	object = fHiRACal->Get();
	std::cout << "\033[1;31mE15190: HiRA Mult - " << object->fmulti << "\033[0m\n";
	return object;
}
