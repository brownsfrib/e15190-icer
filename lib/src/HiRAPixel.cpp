#include <HiRAPixel.h>

HiRAPixel::HiRAPixel(int id0) :
fmultipixel(0),
fnumtel(new int[NUM_MAX_PARTICLES_PER_TEL]),
fnumstripf(new int[NUM_MAX_PARTICLES_PER_TEL]),
fnumstripb(new int[NUM_MAX_PARTICLES_PER_TEL]),
fnumcsi(new int[NUM_MAX_PARTICLES_PER_TEL]),
fEnergySif(new double[NUM_MAX_PARTICLES_PER_TEL]),
fEnergySib(new double[NUM_MAX_PARTICLES_PER_TEL]),
fEnergyCsI(new double[NUM_MAX_PARTICLES_PER_TEL]),
fEnergy(new double[NUM_MAX_PARTICLES_PER_TEL]),
fstripfindex(new int[NUM_MAX_PARTICLES_PER_TEL]),
fstripbindex(new int[NUM_MAX_PARTICLES_PER_TEL]),
fcsiindex(new int[NUM_MAX_PARTICLES_PER_TEL]),
fipid(new int[NUM_MAX_PARTICLES_PER_TEL]),
fidZ(new int[NUM_MAX_PARTICLES_PER_TEL]),
fidA(new int[NUM_MAX_PARTICLES_PER_TEL]),
id(id0),
Front(new elist()),
Back(new elist()),
Csi(new elist())
{}

HiRAPixel::~HiRAPixel()
{
  delete [] fnumtel;
  delete [] fnumstripf;
  delete [] fnumstripb;
  delete [] fnumcsi;
  delete [] fEnergySif;
  delete [] fEnergySib;
  delete [] fEnergyCsI;
  delete [] fEnergy;
  delete [] fstripfindex;
  delete [] fstripbindex;
  delete [] fcsiindex;
  delete [] fipid;
  delete [] fidZ;
  delete [] fidA;

  delete Front;
  delete Back;
  delete Csi;
}

void HiRAPixel::Reset()
{
  fmultipixel=0;
  Front->reset();
  Back->reset();
  Csi->reset();
}

//----------------------------------------------
//This is the multihit routine where we will match the front/back pairs to the correct CsI
int HiRAPixel::multiHitCsi()
{
  // find number of soultions ,i.e. back and front pairs of strips
  // with the same energy
  int isol = multiHit();
  if (isol <=0) return 0;

  int NWellDefinedPixels=0;

  //now assign each of these solutions a Csi detector location
  int mult[4]={0};  //array for multipility of Si solution for each Csi
  int sil[4][NUM_MAX_PARTICLES_PER_TEL]; //contains a lits of solutions for each Csi
  for (int i=0;i<fmultipixel;i++) {
     fcsiindex[i]=-1; //reset csi index to -1 for all the obtained pixels
     int ifront = fnumstripf[i];
     int iback = fnumstripb[i];
     for (int icsi=0;icsi<4;icsi++) {
         if (ifront >= FrontLow[icsi] &&
             ifront <= FrontHigh[icsi] &&
             iback  >= BackLow[icsi]  &&
             iback  <= BackHigh[icsi]) {
               sil[icsi][mult[icsi]] = i; //pixels corresponding to the CsI icsi
               mult[icsi]++; //multiplicity of pixels correspondent to the CsI icsi
               break;
	        }
      }
  }

  //make array of detect csi energies
  float energy[4]={-1.};
  float energyR[4]={-1};
  int csiindex[4]={-1};
  for (int i=0;i<Csi->Nstore;i++) {
     energy[Csi->Order[i].strip] = Csi->Order[i].energy;
     energyR[Csi->Order[i].strip] = Csi->Order[i].energyR;
     csiindex[Csi->Order[i].strip] = Csi->Order[i].detectorindex;
  }

 //loop over csi location
 for (int icsi = 0;icsi<4;icsi++) {
   if (mult[icsi] == 0) {
     continue; // no solution for this location, ignore csi
   }
   else if (mult[icsi] > 1) { // more than one si solution for a single Csi location
     for (int j=0;j<mult[icsi];j++) {
       fipid[sil[icsi][j]] = 0;
     }
     //For now we just ignore this csi
     #ifdef PIXELIZATION_DEBUG
     DEBUG_PIXELIZATION_NUM_MULTIPLEPIXEL_CSI++;
     #endif
     continue;
   }
   // only one si solution for this csi location
   else {
     int ii = sil[icsi][0];
     //now see if this csi fired
     if (energy[icsi] <= 0.) {
       //no csi recorded for this event
       //stopped in silicon
       fEnergy[ii] = 0.;
       #ifdef PIXELIZATION_DEBUG
       DEBUG_PIXELIZATION_NUM_PIXEL_NOCSI++;
       #endif
       continue;
     }
     // If I'm here this csi fired and I record it
     NWellDefinedPixels++;
     CsIhit = icsi + id*4;
     fcsiindex[ii]=csiindex[icsi];
     fnumcsi[ii] = icsi;
     #ifdef PIXELIZATION_DEBUG
     DEBUG_PIXELIZATION_NUM_VALID_PIXELS++;
     #endif
    }
  }
  return NWellDefinedPixels;
}

//-------------------------------------------------------
//This is the multihit routine to match the front with the back
int HiRAPixel::multiHit()
{
  int Ntries = min(Front->Nstore,Back->Nstore); //At most you can try to match the fewest number of strips
  if (Ntries > 4) Ntries =4; //You can only match at most the number of hits equal to the number of crystals
  fmultipixel = 0;
  if (Ntries <= 0) return 0; //This is a safety value that shouldn't be needed

  for (NestDim = Ntries;NestDim>0;NestDim--) {
      dstripMin = 1000;
      deMin = 10000.;

      //look for best solution
      loop(0);

      //check to see if best possible solution is reasonable
      bool leave = 0;
      for (int i=0;i<NestDim;i++) {
	      benergy = Back->Order[arrayB[i]].energy;
	      fenergy = Front->Order[i].energy;
	      float accept = 3;
	      if(fenergy < 10.) accept =1.;
	      if (fabs(benergy-fenergy) >accept) {
	        leave = 1;
	        break;
	      }
    	}

      if (leave) continue;
      // now load solution
      for (int i=0;i<NestDim;i++) {
	      fenergy = Front->Order[i].energy;

	      fEnergySif[i] = fenergy;
	      fnumstripf[i] = Front->Order[i].strip;
	      fnumstripb[i] = Back->Order[arrayB[i]].strip;
        fstripfindex[i] = Front->Order[i].detectorindex;
	      fstripbindex[i] = Back->Order[arrayB[i]].detectorindex;
        fnumtel[i] = id;
      }
      fmultipixel = NestDim;
      break;
    }

  return fmultipixel;
}

//****************************************************
//recursive subroutine  used for multihit subroutine
void HiRAPixel::loop(int depth)
{
  if (depth == NestDim ) {
      // do stuff here
      int dstrip = 0;
      float de = 0.;
      for (int i=0;i<NestDim;i++) {
	  //We will assume at this point that the high and low gain have been matched
	  benergy = Back->Order[NestArray[i]].energy;
	  fenergy = Front->Order[i].energy;
	  de += abs(benergy-fenergy);
  	}

  if (dstrip < dstripMin) {
          dstripMin = dstrip;
          for (int i=0;i<NestDim;i++)
	  arrayD[i] = NestArray[i];
	 }


  if (de < deMin) {
          deMin = de;
          for (int i=0;i<NestDim;i++)
	  arrayB[i] = NestArray[i];
	}
      return;

  }

  for (int i=0;i<NestDim;i++){
      NestArray[depth] = i;
      int leave = 0;
      for (int j=0;j<depth;j++){
	      if (NestArray[j] == i) {
	       leave =1;
         break;
	      }
    	}
      if (leave) continue;
      loop(depth+1);
    }
}

void HiRAPixel::LoadStrips(int F0low, int F1low,int F2low, int F3low,
		 int F0hi,  int F1hi, int F2hi,  int F3hi,
		 int B0low, int B1low,int B2low, int B3low,
		 int B0hi,  int B1hi, int B2hi,  int B3hi)
{
  FrontLow[0] = F0low;
  FrontLow[1] = F1low;
  FrontLow[2] = F2low;
  FrontLow[3] = F3low;

  FrontHigh[0] = F0hi;
  FrontHigh[1] = F1hi;
  FrontHigh[2] = F2hi;
  FrontHigh[3] = F3hi;

  BackLow[0] = B0low;
  BackLow[1] = B1low;
  BackLow[2] = B2low;
  BackLow[3] = B3low;

  BackHigh[0] = B0hi;
  BackHigh[1] = B1hi;
  BackHigh[2] = B2hi;
  BackHigh[3] = B3hi;

}
