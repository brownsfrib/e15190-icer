#include "include/MBHitCondition.h"

//____________________________________________________
MBHitCondition::MBHitCondition() :
fFiducialCuts(new TCutG **[N_MICROBALL_RINGS]),
fNoiseFiducialCuts(new TCutG **[N_MICROBALL_RINGS]),
fFileCuts(0),
fCutsLoaded(false)
{
  for(int i=0; i<N_MICROBALL_RINGS; i++) {
    fFiducialCuts[i]= new TCutG*[N_MICROBALL_DETS_PER_RING[i]];
    fNoiseFiducialCuts[i]= new TCutG*[N_MICROBALL_DETS_PER_RING[i]];
    for(int j=0; j< N_MICROBALL_DETS_PER_RING[i]; j++) {
      fFiducialCuts[i][j]=0;
      fNoiseFiducialCuts[i][j]=0;
    }
  }
}

//____________________________________________________
MBHitCondition::~MBHitCondition()
{
  for(int i=0; i<N_MICROBALL_RINGS; i++) {
    delete [] fFiducialCuts[i];
    delete [] fNoiseFiducialCuts[i];
  }
  delete [] fFiducialCuts;
  delete [] fNoiseFiducialCuts;
  if(fFileCuts) {
    fFileCuts->Close();
    delete fFileCuts;
  }
}

//____________________________________________________
int MBHitCondition::LoadFastSlowCuts(const char * file_name)
{
  int NCuts=0;

  fFileCuts = new TFile(file_name);
  if(fFileCuts->IsZombie()) {
    return -1;
  }

  for(int i=0; i<N_MICROBALL_RINGS; i++) {
    for(int j=0; j< N_MICROBALL_DETS_PER_RING[i]; j++) {
      fFiducialCuts[i][j]=(TCutG*)fFileCuts->Get(Form("FSCut_R%02d_%02d", i+1, j+1));
      fNoiseFiducialCuts[i][j]=(TCutG*)fFileCuts->Get(Form("NoiseCut_R%02d_%02d", i+1, j+1));
      if(fFiducialCuts[i][j]) NCuts++;
    }
  }

  if(NCuts>0) fCutsLoaded=true;
  return NCuts;
}

//____________________________________________________
bool MBHitCondition::IsHit(int num_ring, int num_det, double fast, double slow, double time) const
{
  if(!fCutsLoaded) return false;
  if(time>0 && fFiducialCuts[num_ring-1][num_det-1]!=0 && fFiducialCuts[num_ring-1][num_det-1]->IsInside(slow,fast) &&
    (fNoiseFiducialCuts[num_ring-1][num_det-1]!=0 ? !fNoiseFiducialCuts[num_ring-1][num_det-1]->IsInside(slow,fast) : true)) return true;
  else return false;
}
