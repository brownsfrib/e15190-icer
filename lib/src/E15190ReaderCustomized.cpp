#include "include/E15190Reader.h"



void degree2rad(double &angle)
{
  angle = angle/180.*3.1415926;
}

void rad2degree(double &angle)
{
  angle = angle/3.1415926*180;
}



void sphere2xyz(double r, double theta, double phi, double &x, double &y, double &z)
{
  degree2rad(theta);
  degree2rad(phi);
  x = r*sin(theta)*cos(phi);
  y = r*sin(theta)*sin(phi);
  z = r*cos(theta);
}



void get_r_angle(double x1, double y1, double z1, double x2, double y2, double z2, double &r12, double &angle)
{
  x2 = x2-x1;
  y2 = y2-y1;
  z2 = z2-z1;

  r12 = sqrt(x2*x2 + y2*y2 + z2*z2);

  double ab = x1*x2 + y1*y2 + z1*z2;
  double r1 = sqrt(x1*x1 + y1*y1 + z1*z1);
  angle  = acos(ab/(r12*r1));

  rad2degree(angle);
}

double getEk(double tof, double d)
{
  double v =  (d/100) / (tof*pow(10,-9));
  double beta = v / (3*pow(10,8));
  double m0 = 939.57;
  double m = m0 / sqrt(1-beta*beta);
  double Ek = m - m0;

  return Ek;
}

// //____________________________________________________
// void E15190Reader::CreateUsefulVetoWallHistograms(const char * file_name, Long64_t evt_amount)
// {
//   // this method constructs some useful VetoWall histograms

//   if(!fIsNWB || !fIsFA || !fIsVW) return;

//   TFile * FileOut = new TFile(file_name, "RECREATE");
//   if(FileOut->IsZombie()) {
//     printf("Error: Cannot create file %s\n", file_name);
//     return;
//   }

//   // Initializing output histograms
//   TH2D * VWDETOF[NUM_BARS_VW];
//   TH2D * VWDETOFMatched[NUM_BARS_VW];
//   for(int i=0; i<NUM_BARS_VW; i++) {
//     VWDETOF[i] = new TH2D (Form("VWDETOF%02d",i), Form("VWDETOF%02d",i), 600,0,300,1024,0,4096);
//     VWDETOFMatched[i] = new TH2D (Form("VWDETOF%02dMatched",i), Form("VWDETOF%02dMatched",i), 600,0,300,1024,0,4096);
//   }

//   Long64_t nentries=fChain->GetEntries();
//   if(evt_amount!=0) {
//     nentries=evt_amount;
//   }
//   Long64_t jentry=0;
//   std::cout << "found " << nentries << " entries\n";
//   for(;fE15190Reader->Next() && jentry<nentries; jentry++)
//   {
//     if(jentry%100000==0) {
//       PrintPercentage(jentry,nentries);
//     }

//     NeutronWallCalibratedData * NWB = fNWBCal->Get();
//     ForwardArrayCalibratedData * ForwardArray = fForwardArrayCal->Get();
//     VetoWallCalibratedData * VetoWall = fVetoWallCal->Get();

//     for(int i=0; i<NWB->fmulti; i++) {
//       double TOF = NWB->fTimeMean[i]-ForwardArray->fTimeMin;
//       for(int j=0; j<VetoWall->fmulti; j++) {
//         VWDETOF[VetoWall->fnumbar[j]]->Fill(TOF,VetoWall->fGeoMean[j]);
//         VWDETOFMatched[VetoWall->fnumbar[j]]->Fill(TOF,VetoWall->fGeoMeanMatched[j]);
//       }
//     }
//   }

//   //Writing histograms to file
//   for(int i=0; i<NUM_BARS_VW; i++) {
//     FileOut->WriteTObject(VWDETOF[i], VWDETOF[i]->GetName());
//     FileOut->WriteTObject(VWDETOFMatched[i], VWDETOFMatched[i]->GetName());
//   }

//   // closing output file
//   FileOut->Close();
// }

// //____________________________________________________
// void E15190Reader::CreateUsefulForwardArrayHistograms(const char * file_name, Long64_t evt_amount)
// {
//   // this method constructs some useful VetoWall histograms

//   if(!fIsNWB || !fIsFA) return;

//   TFile * FileOut = new TFile(file_name, "RECREATE");
//   if(FileOut->IsZombie()) {
//     printf("Error: Cannot create file %s\n", file_name);
//     return;
//   }

//   // Initializing output histograms
//   TH1D * FANWBToF[NUM_DETECTORS_FA];
//   for(int i=0; i<NUM_DETECTORS_FA; i++) {
//     FANWBToF[i] = new TH1D(Form("FA%02dNWBToF", i+1),Form("FA%02dNWBToF", i+1),600,-150,150);
//   }

//   Long64_t nentries=fChain->GetEntries();
//   if(evt_amount!=0) {
//     nentries=evt_amount;
//   }
//   Long64_t jentry=0;
//   std::cout << "found " << nentries << " entries\n";
//   for(;fE15190Reader->Next() && jentry<nentries; jentry++)
//   {
//     if(jentry%100000==0) {
//       PrintPercentage(jentry,nentries);
//     }

//     NeutronWallCalibratedData * NWB = fNWBCal->Get();
//     ForwardArrayCalibratedData * ForwardArray = fForwardArrayCal->Get();

//     for(int FAhit=0; FAhit<ForwardArray->fmulti; FAhit++)
//     {
//       for(int NWhit=0; NWhit<NWB->fmulti; NWhit++) {
//         // I record the ToF spectrum only if the FA element has a significant energy (more than 300)
//         if(ForwardArray->fE[FAhit]>300) FANWBToF[ForwardArray->fnumdet[FAhit]-1]->Fill(NWB->fTimeMean[NWhit]-ForwardArray->fTime[FAhit]);
//       }
//     }

//   }

//   //Writing histograms to file
//   for(int i=0; i<NUM_DETECTORS_FA; i++) {
//     FileOut->WriteTObject(FANWBToF[i], FANWBToF[i]->GetName());
//   }

//   // closing output file
//   FileOut->Close();
// }


// //____________________________________________________
// void E15190Reader::CreateUsefulHiRAHistograms(const char * file_name, Long64_t evt_amount)
// {
//   // this method constructs some useful HiRA histograms

//   if(!fIsHiRA) return;

//   TFile * FileOut = new TFile(file_name, "RECREATE");
//   if(FileOut->IsZombie()) {
//     printf("Error: Cannot create file %s\n", file_name);
//     return;
//   }

//   // Initializing output histograms
//   //TH2D * HiRADERAWCSIRAW_SINGLESTRIPCSI=new TH2D("DE_Raw_ERaw_00_05_00", "DE_Raw_ERaw_00_05_00", 3000,0,3000,4000,0,8000);
//   TH2D * HiRADECALCSIRAW[NUM_CSI_TEL*NUM_TEL];
//   TH2D * HiRADECALCSICALPROTONS[NUM_TEL];
//   TH2D * HiRADECALCSICALALLPROTONS = new TH2D(Form("DECal_ECal_Protons"),Form("DECal_ECal_Protons"),2000,0,900,1000,0,200);
//   TH2D * HiRADECALCSICALALL = new TH2D(Form("DECal_ECal"),Form("DECal_ECal"),2000,0,500,900,0,200);
//   for(int i=0; i<NUM_TEL; i++) {
//     HiRADECALCSICALPROTONS[i] = new TH2D(Form("DECal_ECal_Protons_%02d", i),Form("DECal_ECal_Protons_%02d", i),2000,0,500,1000,0,200);
//     for(int j=0; j<NUM_CSI_TEL; j++) {
//       HiRADECALCSIRAW[NUM_CSI_TEL*i+j] = new TH2D(Form("DECal_ERaw_%02d_%02d", i, j ),Form("DECal_ERaw_%02d_%02d", i, j ),3000,0,3000,3000,0,200);
//     }
//   }

//   Long64_t nentries=fChain->GetEntries();
//   if(evt_amount!=0) {
//     nentries=evt_amount;
//   }
//   Long64_t jentry=0;
//   std::cout << "found " << nentries << " entries\n";
//   for(;fE15190Reader->Next() && jentry<nentries; jentry++)
//   {
//     if(jentry%100000==0) {
//       PrintPercentage(jentry,nentries);
//     }

//     HiRACalibratedData * HiRA = fHiRACal->Get();

//     for(int i=0; i< HiRA->fmulti; i++) {

//       //if(HiRA->fnumtel[i]==0 && HiRA->fnumcsi[i]==0 && HiRA->fnumstripf[i]==5) HiRADERAWCSIRAW_SINGLESTRIPCSI->Fill(HiRA->fEnergycsi[i], HiRA->fEnergySifHi[i]);

//       HiRADECALCSICALALLPROTONS->Fill(HiRA->fEnergycsiCalProtons[i], HiRA->fEnergySifCal[i]);
//       HiRADECALCSICALPROTONS[HiRA->fnumtel[i]]->Fill(HiRA->fEnergycsiCalProtons[i], HiRA->fEnergySifCal[i]);
//       HiRADECALCSICALALL->Fill(HiRA->fEnergycsiCal[i], HiRA->fEnergySifCal[i]);
//       HiRADECALCSIRAW[NUM_CSI_TEL*HiRA->fnumtel[i]+HiRA->fnumcsi[i]]->Fill(HiRA->fEnergycsi[i], HiRA->fEnergySifCal[i]);
//     }

//   }

//   //Writing histograms to file
//   //FileOut->WriteTObject(HiRADERAWCSIRAW_SINGLESTRIPCSI, HiRADERAWCSIRAW_SINGLESTRIPCSI->GetName());
//   FileOut->WriteTObject(HiRADECALCSICALALLPROTONS, HiRADECALCSICALALLPROTONS->GetName());
//   FileOut->WriteTObject(HiRADECALCSICALALL, HiRADECALCSICALALL->GetName());
//   for(int i=0; i<NUM_TEL; i++) {
//     FileOut->WriteTObject(HiRADECALCSICALPROTONS[i], HiRADECALCSICALPROTONS[i]->GetName());
//     for(int j=0; j<NUM_CSI_TEL; j++) {
//       FileOut->WriteTObject(HiRADECALCSIRAW[NUM_CSI_TEL*i+j], HiRADECALCSIRAW[NUM_CSI_TEL*i+j]->GetName());
//     }
//   }

//   // closing output file
//   FileOut->Close();
// }


// void E15190Reader::testniu()
// {
//   std::cout << "testniu" << std::endl;
// }


/*
//____________________________________________________
void E15190Reader::BuildCorrelationTree(const char * file_name, Long64_t evt_amount)
{
  // this method builds a tree useful for correlation studies
  TFile * FileOut = new TFile(file_name, "RECREATE");

  TTree * CorrTree = new TTree("tree","A tree with data structures convenient for correlation");
  Int_t NWA_fmulti_n;
  Int_t NWB_fmulti_n;
  Int_t NWA_fnumbar[25];
  Int_t NWB_fnumbar[25];
  Int_t VetoWall_fmulti;
  Double_t NWA_fCalGeoMean[25];
  Double_t NWA_fGeoMeanSaturationCorrected[25];
  Double_t NWA_fGM[25];
  Double_t NWB_fCalGeoMean[25];
  Double_t NWB_fGeoMeanSaturationCorrected[25];
  Double_t NWB_fGM[25];
  Double_t NWA_fKEnergy[25];
  Double_t NWB_fKEnergy[25];
  Double_t NWA_fTOF[25];
  Double_t NWB_fTOF[25];
  Double_t NWA_fDistRancm[25];
  Double_t NWB_fDistRancm[25];
  Double_t NWA_fXcm[25];
  Double_t NWB_fXcm[25];
  Double_t NWA_fTheta[25];
  Double_t NWA_fPhi[25];
  Double_t NWB_fTheta[25];
  Double_t NWB_fPhi[25];
  Double_t NWA_fThetaRan[25];
  Double_t NWA_fPhiRan[25];
  Double_t NWB_fThetaRan[25];
  Double_t NWB_fPhiRan[25];
  double NWA_fX[25];
  double NWA_fY[25];
  double NWA_fZ[25];
  double NWB_fX[25];
  double NWB_fY[25];
  double NWB_fZ[25];
  
  Double_t VetoWall_fnumbar[25];
  Double_t VetoWall_fYcm[25];
  Double_t VetoWall_fGeoMeanMatched[25];
  Double_t VetoWall_fTheta[25];
  Double_t VetoWall_fPhi[25];
  Double_t VetoWall_fThetaRan[25];
  Double_t VetoWall_fPhiRan[25];
  Double_t VetoWall_fDistRancm[25];

  double AB_fTheta[25];
  double AB_fTOF[25];
  double AB_fR[25]; //distance
  double AB_fEnergy[25];
  
  CorrTree->Branch("NWA.fmulti_n",&NWA_fmulti_n,"NWA.fmulti_n/I");
  CorrTree->Branch("NWA.fnumbar",NWA_fnumbar,"NWA.fnumbar[NWA.fmulti_n]/I");
  // CorrTree->Branch("NWA.fCalGeoMean",NWA_fCalGeoMean,"NWA.fCalGeoMean[NWA.fmulti_n]/D");
  // CorrTree->Branch("NWA.fGeoMeanSaturationCorrected",NWA_fGeoMeanSaturationCorrected,"NWA.fGeoMeanSaturationCorrected[NWA.fmulti_n]/D");
  CorrTree->Branch("NWA.fGM",NWA_fGeoMeanSaturationCorrected,"NWA.fGeoMeanSaturationCorrected[NWA.fmulti_n]/D");

  CorrTree->Branch("NWA.fEnergy",NWA_fKEnergy,"NWA.fKEnergy[NWA.fmulti_n]/D");
  // CorrTree->Branch("NWA.fKEnergy",NWA_fKEnergy,"NWA.fKEnergy[NWA.fmulti_n]/D");
  CorrTree->Branch("NWA.fTOF",NWA_fTOF,"NWA.fTOF[NWA.fmulti_n]/D");
  CorrTree->Branch("NWA.fDistRancm",NWA_fDistRancm,"NWA.fDistRancm[NWA.fmulti_n]/D");
  CorrTree->Branch("NWA.fR",NWA_fDistRancm,"NWA.fDistRancm[NWA.fmulti_n]/D");
  CorrTree->Branch("NWA.fXcm",NWA_fXcm,"NWA.fXcm[NWA.fmulti_n]/D");
  CorrTree->Branch("NWA.fTheta",NWA_fTheta,"NWA.fTheta[NWA.fmulti_n]/D");
  CorrTree->Branch("NWA.fPhi",NWA_fPhi,"NWA.fPhi[NWA.fmulti_n]/D");
  CorrTree->Branch("NWA.fThetaRan",NWA_fThetaRan,"NWA.fThetaRan[NWA.fmulti_n]/D");
  CorrTree->Branch("NWA.fPhiRan",NWA_fPhiRan,"NWA.fPhiRan[NWA.fmulti_n]/D");
  CorrTree->Branch("NWB.fmulti_n",&NWB_fmulti_n,"NWB.fmulti_n/I");
  CorrTree->Branch("NWB.fnumbar",NWB_fnumbar,"NWB.fnumbar[NWB.fmulti_n]/I");
  // CorrTree->Branch("NWB.fCalGeoMean",NWB_fCalGeoMean,"NWB.fCalGeoMean[NWB.fmulti_n]/D");
  // CorrTree->Branch("NWB.fGeoMeanSaturationCorrected",NWB_fGeoMeanSaturationCorrected,"NWB.fGeoMeanSaturationCorrected[NWB.fmulti_n]/D");
  CorrTree->Branch("NWB.fGM",NWB_fGeoMeanSaturationCorrected,"NWB.fGeoMeanSaturationCorrected[NWB.fmulti_n]/D");
  // CorrTree->Branch("NWB.fKEnergy",NWB_fKEnergy,"NWB.fKEnergy[NWB.fmulti_n]/D");
  CorrTree->Branch("NWB.fEnergy",NWB_fKEnergy,"NWB.fKEnergy[NWB.fmulti_n]/D");
  CorrTree->Branch("NWB.fTOF",NWB_fTOF,"NWB.fTOF[NWB.fmulti_n]/D");
  CorrTree->Branch("NWB.fDistRancm",NWB_fDistRancm,"NWB.fDistRancm[NWB.fmulti_n]/D");
  CorrTree->Branch("NWB.fR",NWB_fDistRancm,"NWB.fDistRancm[NWB.fmulti_n]/D");
  CorrTree->Branch("NWB.fXcm",NWB_fXcm,"NWB.fXcm[NWB.fmulti_n]/D");
  CorrTree->Branch("NWB.fTheta",NWB_fTheta,"NWB.fTheta[NWB.fmulti_n]/D");
  CorrTree->Branch("NWB.fPhi",NWB_fPhi,"NWB.fPhi[NWB.fmulti_n]/D");
  CorrTree->Branch("NWB.fThetaRan",NWB_fThetaRan,"NWB.fThetaRan[NWB.fmulti_n]/D");
  CorrTree->Branch("NWB.fPhiRan",NWB_fPhiRan,"NWB.fPhiRan[NWB.fmulti_n]/D");
  CorrTree->Branch("VetoWall.fmulti",&VetoWall_fmulti,"VetoWall.fmulti/I");

  CorrTree->Branch("NWA.fX",NWA_fX,"NWA.fX[NWA.fmulti_n]/D");
  CorrTree->Branch("NWA.fY",NWA_fY,"NWA.fY[NWA.fmulti_n]/D");
  CorrTree->Branch("NWA.fZ",NWA_fZ,"NWA.fZ[NWA.fmulti_n]/D");
  CorrTree->Branch("NWB.fX",NWB_fX,"NWB.fX[NWB.fmulti_n]/D");
  CorrTree->Branch("NWB.fY",NWB_fY,"NWB.fY[NWB.fmulti_n]/D");
  CorrTree->Branch("NWB.fZ",NWB_fZ,"NWB.fZ[NWB.fmulti_n]/D");

  
  // CorrTree->Branch("VetoWall.fnumbar",VetoWall_fnumbar,"VetoWall.fnumbar[VetoWall.fmulti]/D");
  // CorrTree->Branch("VetoWall.fYcm",VetoWall_fYcm,"VetoWall.fYcm[VetoWall.fmulti]/D");
  // CorrTree->Branch("VetoWall.fGeoMeanMatched",VetoWall_fGeoMeanMatched,"VetoWall.fGeoMeanMatched[VetoWall.fmulti]/D");
  // CorrTree->Branch("VetoWall.fTheta",VetoWall_fTheta,"VetoWall.fTheta[VetoWall.fmulti]/D");
  // CorrTree->Branch("VetoWall.fPhi",VetoWall_fPhi,"VetoWall.fPhi[VetoWall.fmulti]/D");
  // CorrTree->Branch("VetoWall.fThetaRan",VetoWall_fThetaRan,"VetoWall.fThetaRan[VetoWall.fmulti]/D");
  // CorrTree->Branch("VetoWall.fPhiRan",VetoWall_fPhiRan,"VetoWall.fPhiRan[VetoWall.fmulti]/D");
  // CorrTree->Branch("VetoWall.fDistRancm",VetoWall_fDistRancm,"VetoWall.fDistRancm[VetoWall.fmulti]/D");

  CorrTree->Branch("AB.fTOF",&AB_fTOF,"AB.fTOF/D");
  CorrTree->Branch("AB.fR",&AB_fR,"AB.fR/D");
  CorrTree->Branch("AB.fTheta",&AB_fTheta,"AB.fTheta/D");
  CorrTree->Branch("AB.fEnergy",&AB_fEnergy,"AB.fEnergy/D");



    
  CorrTree->SetAutoSave(5000000);

  Long64_t nentries=fChain->GetEntries();
  if(evt_amount!=0) {
    nentries=evt_amount;
  }
  Long64_t jentry=0;
  std::cout << "found " << nentries << " entries\n";
  for(;fE15190Reader->Next() && jentry<nentries; jentry++)
    {
      if(jentry%100000==0) {
	PrintPercentage(jentry,nentries);
      }



      ForwardArrayCalibratedData * ForwardArray;
      VetoWallCalibratedData * VetoWall;
    

      //initialize multiplicity of wall A and B
      NWA_fmulti_n=0;
      NWB_fmulti_n=0;

      //********************************************************************
      if (fIsFA) {
	ForwardArray = fForwardArrayCal->Get();
	//Insert ForwardArray code here
      }
      if (fIsVW) {
	VetoWall = fVetoWallCal->Get();
	//Insert VetoWall code here
	VetoWall_fmulti=VetoWall->fmulti;
	// for (int i=0; i<VetoWall_fmulti; i++) {
	//   VetoWall_fnumbar[i]=VetoWall->fnumbar[i];
        // VetoWall_fYcm[i]=VetoWall->fYcm[i];
        // VetoWall_fGeoMeanMatched[i]=VetoWall->fGeoMeanMatched[i];
        // VetoWall_fTheta[i]=VetoWall->fTheta[i];
        // VetoWall_fPhi[i]=VetoWall->fPhi[i];
        // VetoWall_fThetaRan[i]=VetoWall->fThetaRan[i];
        // VetoWall_fPhiRan[i]=VetoWall->fPhiRan[i];
        // VetoWall_fDistRancm[i]=VetoWall->fDistRancm[i];
	// }

      }


    //********************************************************************
      if (fIsNWB) {
	NeutronWallCalibratedData * NWB = fNWBCal->Get();
	//loop on NWB event
	for(int i=0; i<NWB->fmulti; i++) {
	  if(NWB->fPSDRight[i]>0.5) {
	    NWB_fnumbar[NWB_fmulti_n]=NWB->fnumbar[i];
	    NWB_fCalGeoMean[NWB_fmulti_n]=NWB->fCalGeoMean[i];
	    NWB_fGeoMeanSaturationCorrected[NWB_fmulti_n]=NWB->fGeoMeanSaturationCorrected[i];
	    NWB_fKEnergy[NWB_fmulti_n]=-9999; //calculate it with the numbers of the hit "i"
	    NWB_fTOF[NWB_fmulti_n]=ForwardArray->fTimeMin!=-9999 ? NWB->fTimeMean[i]-ForwardArray->fTimeMin : -9999;
	    NWB_fDistRancm[NWB_fmulti_n]=NWB->fDistRancm[i];
	  
	    NWB_fKEnergy[NWB_fmulti_n] = getEk(NWB_fTOF[NWB_fmulti_n], NWB_fDistRancm[NWB_fmulti_n]);

	  
	    NWB_fXcm[NWB_fmulti_n]=NWB->fXcm[i];
	    NWB_fTheta[NWB_fmulti_n]=NWB->fTheta[i];
	    NWB_fPhi[NWB_fmulti_n]=NWB->fPhi[i];
	    NWB_fThetaRan[NWB_fmulti_n]=NWB->fThetaRan[i];
	    NWB_fPhiRan[NWB_fmulti_n]=NWB->fPhiRan[i];


	    sphere2xyz( NWB_fDistRancm[NWB_fmulti_n],
			NWB_fTheta[NWB_fmulti_n],
			NWB_fPhi[NWB_fmulti_n],
			NWB_fX[NWB_fmulti_n],
			NWB_fY[NWB_fmulti_n],
			NWB_fZ[NWB_fmulti_n] );

	   
	    // std::cout << NWB_fDistRancm[NWB_fmulti_n] << "\t"
	    // 	    << NWB_fTheta[NWB_fmulti_n] << "\t"
	    // 	    << NWB_fPhi[NWB_fmulti_n] << "\t"
	    // 	    << NWB_fX[NWB_fmulti_n] << "\t"
	    // 	    << NWB_fY[NWB_fmulti_n] << "\t"
	    // 	    << NWB_fZ[NWB_fmulti_n] << "\t"
	    // 	    << std::endl;
 
	  
	    NWB_fmulti_n++;
	  }
	}
      }

      
      //for eff cal
      if(VetoWall_fmulti!=0 || NWB_fmulti_n!=1)
      	continue;



      //********************************************************************
      if (fIsNWA) {
	NeutronWallCalibratedData * NWA = fNWACal->Get();
	//loop on NWA event
	for(int i=0; i<NWA->fmulti; i++) {
	  if(NWA->fPSDRight[i]>0.5) {
	    NWA_fnumbar[NWA_fmulti_n]=NWA->fnumbar[i];
	    NWA_fCalGeoMean[NWA_fmulti_n]=NWA->fCalGeoMean[i];
	    NWA_fGeoMeanSaturationCorrected[NWA_fmulti_n]=NWA->fGeoMeanSaturationCorrected[i];
	    // NWA_fKEnergy[NWA_fmulti_n]=-9999; //calculate it with the numbers of the hit "i"

    
	  
	    NWA_fTOF[NWA_fmulti_n]=ForwardArray->fTimeMin!=-9999 ? NWA->fTimeMean[i]-ForwardArray->fTimeMin : -9999;
	    NWA_fDistRancm[NWA_fmulti_n]=NWA->fDistRancm[i];


	    NWA_fKEnergy[NWA_fmulti_n] = getEk(NWA_fTOF[NWA_fmulti_n], NWA_fDistRancm[NWA_fmulti_n]);
	  
	 
	    NWA_fXcm[NWA_fmulti_n]=NWA->fXcm[i];
	    NWA_fTheta[NWA_fmulti_n]=NWA->fTheta[i];
	    NWA_fPhi[NWA_fmulti_n]=NWA->fPhi[i];
	    NWA_fThetaRan[NWA_fmulti_n]=NWA->fThetaRan[i];
	    NWA_fPhiRan[NWA_fmulti_n]=NWA->fPhiRan[i];

	    sphere2xyz( NWA_fDistRancm[NWA_fmulti_n],
			NWA_fTheta[NWA_fmulti_n],
			NWA_fPhi[NWA_fmulti_n],
			NWA_fX[NWA_fmulti_n],
			NWA_fY[NWA_fmulti_n],
			NWA_fZ[NWA_fmulti_n] );


	    AB_fTOF[NWA_fmulti_n] = NWA_fTOF[NWA_fmulti_n] - NWB_fTOF[0];
	    get_r_angle(
			NWB_fX[0], NWB_fY[0], NWB_fZ[0],
			NWA_fX[NWA_fmulti_n], NWA_fY[NWA_fmulti_n], NWA_fZ[NWA_fmulti_n],
			AB_fR[NWA_fmulti_n], AB_fTheta[NWA_fmulti_n] );
	    AB_fEnergy[NWA_fmulti_n] = getEk(AB_fTOF[NWA_fmulti_n], AB_fR[NWA_fmulti_n]);

      

	  
	    NWA_fmulti_n++;

	    
	  }
	}
      }


      // if (fIsMB) {
      //   MicroballCalibratedData * Microball = fMicroballCal->Get();
      //   //Insert Microball code here
      // }
      // if (fIsHiRA) {
      //   HiRACalibratedData * HiRA = fHiRACal->Get();
      //   //Insert HiRA code here
      // }
      // if (fIsTDC) {
      //   fTDCAdditionalChannels->FillOutputBranches();
      //   //Insert TDC spare channels code here. EX: To retrieve TDCTriggers.MASTER_TRG do fTDCAdditionalChannels->GetBranchValue("TDCTriggers.MASTER_TRG");
      // }


      // // all data
      // if(VetoWall_fmulti!=0 || NWA_fmulti_n<1 || NWB_fmulti_n<1)
      //   continue;

    
      // // for clean check
      // if(VetoWall_fmulti!=0 || NWA_fmulti_n!=1 || NWB_fmulti_n!=1)
      //   continue;
      // if(NWA_fTOF[0]<0 || NWB_fTOF[0]<0 || NWA_fTOF[0]<NWB_fTOF[0])
      //   continue;
      // if(NWA_fDistRancm[0]>545 || NWB_fDistRancm[0]>475)
      //   continue;


    


      // // std::cout << VetoWall_fmulti << "\t"
      // // 	      << NWA_fmulti_n << "\t"
      // // 	      << NWB_fmulti_n << "\t"
      // // 	      << NWB_fTOF[0] << "\t"
      // // 	      << NWA_fTOF[0] << "\t"
      // // 	      << std::endl;


    



      // if(!(AB_fEnergy < NWB_fKEnergy[0]) || NWB_fKEnergy[0]>200 || NWA_fKEnergy[0]>200)
      // 	continue;


      // // std::cout << AB_fTOF << "\t"
      // // 	      << AB_fR << "\t"
      // //   	      << AB_fTheta << "\t"
      // // 	      << AB_fEnergy << "\t"
      // // 	      << NWB_fKEnergy[0] - AB_fEnergy
      // // 	      << std::endl;





    

      //Fill the tree
      CorrTree->Fill();

      jentry++;
    }

  // writing tree to output file
  CorrTree->AutoSave();

  // closing output file
  FileOut->Close();
}
*/
