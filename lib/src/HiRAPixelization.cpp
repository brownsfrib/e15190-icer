#include <HiRAPixelization.h>

//____________________________________________________
HiRAPixelization::HiRAPixelization(int num_telescopes) :
fNumTel(num_telescopes),
fpixel(new HiRAPixel*[fNumTel])
{
  for(int i=0;i<fNumTel;i++) {
    fpixel[i] = new HiRAPixel(i);
    //Sets high and low Si strips for the CsI detectors
    fpixel[i]->LoadStrips(0, 0, 16,16, //Front low   0,1,2,3
       15,15,31,31, //Front high  0,1,2,3
       16, 0,0,16, //back low     0,1,2,3
       31,15,15,31); //back high  0,1,2,3
  }
}

//____________________________________________________
HiRAPixelization::~HiRAPixelization()
{
  for(int i=0;i<fNumTel;i++) {
    delete fpixel[i];
  }
  delete [] fpixel;
}

//____________________________________________________
int HiRAPixelization::Pixelization(int ntel, HiRACalibratedBuffer * pEvent, HiRACalibratedData * pCalibratedEvent)
{
   // Clear data structures and set to 0 the multiplicity of pixels identified
   fpixel[ntel]->Reset();

   #ifdef PIXELIZATION_DEBUG
   if(pEvent->fmultistripf>0 || pEvent->fmulticsi>0 || pEvent->fmulticsi>0) DEBUG_PIXELIZATION_NUM_HIRA_EVENTS++;
   #endif

  // Loop on strips front
   for(int i=0; i<pEvent->fmultistripf; i++)
   {
     if(pEvent->fEnergySifCal[i] > 0.4) {
       fpixel[ntel]->Front->Add(i, pEvent->fnumstripf[i],pEvent->fEnergySifCal[i],pEvent->fEnergySifLo[i],pEvent->fEnergySifHi[i],pEvent->fTimeSif[i]);
     }
   }

   // Loop on strips back
   for(int i=0; i<pEvent->fmultistripb; i++)
   {
     if(pEvent->fEnergySibCal[i] > 0.4) {
       fpixel[ntel]->Back->Add(i, pEvent->fnumstripb[i],pEvent->fEnergySibCal[i],pEvent->fEnergySibLo[i],pEvent->fEnergySibHi[i],pEvent->fTimeSib[i]);
     }
   }

   // Loop on CsI
   for(int i=0; i<pEvent->fmulticsi; i++)
   {
     if(pEvent->fEnergycsi[i] == 4096) continue;
     if(pEvent->fEnergycsi[i] > 0) {
       fpixel[ntel]->Csi->Add(i, pEvent->fnumcsi[i],pEvent->fEnergycsiCal[i],0.,pEvent->fEnergycsi[i],pEvent->fTimecsi[i]);
     }
   }

   #ifdef PIXELIZATION_DEBUG
   if(fpixel[ntel]->Front->Nstore>0 &&  fpixel[ntel]->Back->Nstore==0 && fpixel[ntel]->Csi->Nstore==0) DEBUG_PIXELIZATION_NUM_FRONT++;
   if(fpixel[ntel]->Front->Nstore==0 &&  fpixel[ntel]->Back->Nstore>0 && fpixel[ntel]->Csi->Nstore==0) DEBUG_PIXELIZATION_NUM_BACK++;
   if(fpixel[ntel]->Front->Nstore==0 &&  fpixel[ntel]->Back->Nstore==0 && fpixel[ntel]->Csi->Nstore>0) DEBUG_PIXELIZATION_NUM_CSI++;
   if(fpixel[ntel]->Front->Nstore>0 &&  fpixel[ntel]->Back->Nstore>0 && fpixel[ntel]->Csi->Nstore==0) DEBUG_PIXELIZATION_NUM_FRONT_BACK++;
   if(fpixel[ntel]->Front->Nstore>0 &&  fpixel[ntel]->Back->Nstore==0 && fpixel[ntel]->Csi->Nstore>0) DEBUG_PIXELIZATION_NUM_FRONT_CSI++;
   if(fpixel[ntel]->Front->Nstore==0 &&  fpixel[ntel]->Back->Nstore>0 && fpixel[ntel]->Csi->Nstore>0) DEBUG_PIXELIZATION_NUM_BACK_CSI++;
   if(fpixel[ntel]->Front->Nstore>0 &&  fpixel[ntel]->Back->Nstore>0 && fpixel[ntel]->Csi->Nstore>0) DEBUG_PIXELIZATION_NUM_FRONT_BACK_CSI++;
   #endif

   if(fpixel[ntel]->Front->Nstore==0 || fpixel[ntel]->Back->Nstore==0 || fpixel[ntel]->Csi->Nstore==0) return 0; //Not possible to define at least one pxiel in the telescope

   // Taking care of charge sharing in the front and back!
   fpixel[ntel]->Front->Neighbours("Front",0.,0.);
   fpixel[ntel]->Back->Neighbours("Back",0.,0.);

   // Pixel recongition
   if(fpixel[ntel]->Csi->Nstore>=1)
   {
     fpixel[ntel]->multiHitCsi();
   }

   //Number of particles already in the HiRA event
   int NParticlesStored=pCalibratedEvent->fmulti;

   // Building Calibrated Event
   for(int i=0; i<fpixel[ntel]->fmultipixel; i++) {
     if(fpixel[ntel]->fstripfindex[i]<0 || fpixel[ntel]->fstripbindex[i]<0 || fpixel[ntel]->fcsiindex[i]<0) continue; //I record the particle only if it has a valid stripf, stripb and csi

     pCalibratedEvent->fnumtel[pCalibratedEvent->fmulti]=ntel;
     pCalibratedEvent->fnumstripf[pCalibratedEvent->fmulti]=pEvent->fnumstripf[fpixel[ntel]->fstripfindex[i]];
     pCalibratedEvent->fEnergySifHi[pCalibratedEvent->fmulti]=pEvent->fEnergySifHi[fpixel[ntel]->fstripfindex[i]];
     pCalibratedEvent->fEnergySifLo[pCalibratedEvent->fmulti]=pEvent->fEnergySifLo[fpixel[ntel]->fstripfindex[i]];
     pCalibratedEvent->fTimeSif[pCalibratedEvent->fmulti]=pEvent->fTimeSif[fpixel[ntel]->fstripfindex[i]];
     pCalibratedEvent->fEnergySifMatched[pCalibratedEvent->fmulti]=pEvent->fEnergySifMatched[fpixel[ntel]->fstripfindex[i]];
     pCalibratedEvent->fEnergySifCal[pCalibratedEvent->fmulti]=pEvent->fEnergySifCal[fpixel[ntel]->fstripfindex[i]];
     pCalibratedEvent->fnumstripb[pCalibratedEvent->fmulti]=pEvent->fnumstripb[fpixel[ntel]->fstripbindex[i]];
     pCalibratedEvent->fEnergySibHi[pCalibratedEvent->fmulti]=pEvent->fEnergySibHi[fpixel[ntel]->fstripbindex[i]];
     pCalibratedEvent->fEnergySibLo[pCalibratedEvent->fmulti]=pEvent->fEnergySibLo[fpixel[ntel]->fstripbindex[i]];
     pCalibratedEvent->fTimeSib[pCalibratedEvent->fmulti]=pEvent->fTimeSib[fpixel[ntel]->fstripbindex[i]];
     pCalibratedEvent->fEnergySibMatched[pCalibratedEvent->fmulti]=pEvent->fEnergySibMatched[fpixel[ntel]->fstripbindex[i]];
     pCalibratedEvent->fEnergySibCal[pCalibratedEvent->fmulti]=pEvent->fEnergySibCal[fpixel[ntel]->fstripbindex[i]];
     pCalibratedEvent->fnumcsi[pCalibratedEvent->fmulti]=pEvent->fnumcsi[fpixel[ntel]->fcsiindex[i]];
     pCalibratedEvent->fEnergycsi[pCalibratedEvent->fmulti]=pEvent->fEnergycsi[fpixel[ntel]->fcsiindex[i]];
     pCalibratedEvent->fEnergycsiCal[pCalibratedEvent->fmulti]=pEvent->fEnergycsiCal[fpixel[ntel]->fcsiindex[i]]; //CsI energy is here still the one for protons, that will be used for PID
     pCalibratedEvent->fTimecsi[pCalibratedEvent->fmulti]=pEvent->fTimecsi[fpixel[ntel]->fcsiindex[i]];
     pCalibratedEvent->fmulti++;
   }

   return pCalibratedEvent->fmulti-NParticlesStored;
}
