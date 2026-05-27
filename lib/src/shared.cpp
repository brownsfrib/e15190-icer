#include <shared.h>

const double gSpeedOfLight=2.99792458E+8;

#ifdef PIXELIZATION_DEBUG
long unsigned int DEBUG_PIXELIZATION_NUM_HIRA_EVENTS=0;
long unsigned int DEBUG_PIXELIZATION_NUM_FRONT=0;
long unsigned int DEBUG_PIXELIZATION_NUM_BACK=0;
long unsigned int DEBUG_PIXELIZATION_NUM_CSI=0;
long unsigned int DEBUG_PIXELIZATION_NUM_FRONT_BACK=0;
long unsigned int DEBUG_PIXELIZATION_NUM_FRONT_BACK_CSI=0;
long unsigned int DEBUG_PIXELIZATION_NUM_FRONT_CSI=0;
long unsigned int DEBUG_PIXELIZATION_NUM_BACK_CSI=0;
long unsigned int DEBUG_PIXELIZATION_NUM_PIXEL_NOCSI=0;
long unsigned int DEBUG_PIXELIZATION_NUM_MULTIPLEPIXEL_CSI=0;
long unsigned int DEBUG_PIXELIZATION_NUM_VALID_PIXELS=0;
#endif

const int NUM_BARS_NWA = 25;
const int NUM_BARS_NWB =25;
const int NUM_BARS_VW =25;
const int NUM_DETECTORS_FA =18;

const int NUM_DETS_MICROBALL = 96;
const int N_MICROBALL_RINGS = 9;
const int N_MICROBALL_DETS_PER_RING[] = {6, 10, 12, 12, 14, 14, 12, 10, 6};

const int NUM_TEL = 12;
const int NUM_STRIP_F =32;
const int NUM_STRIP_B =32;
const int NUM_CSI_TEL =4;
const int NUM_CSI_STRIP=2;
const int NUM_TOWERS = 3;
const int NUM_MAX_PARTICLES_PER_TEL = 4;
