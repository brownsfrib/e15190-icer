#ifndef E15190_SHARED_H
#define E15190_SHARED_H

//#define PIXELIZATION_DEBUG

//+++++++++++++++++++++++++++++++
//Physical quantities
extern const double gSpeedOfLight;  //m/s
//+++++++++++++++++++++++++++++++

#ifdef PIXELIZATION_DEBUG
//HiRA Pixelization Debug
extern long unsigned int DEBUG_PIXELIZATION_NUM_HIRA_EVENTS;
extern long unsigned int DEBUG_PIXELIZATION_NUM_FRONT;
extern long unsigned int DEBUG_PIXELIZATION_NUM_BACK;
extern long unsigned int DEBUG_PIXELIZATION_NUM_CSI;
extern long unsigned int DEBUG_PIXELIZATION_NUM_FRONT_BACK;
extern long unsigned int DEBUG_PIXELIZATION_NUM_FRONT_BACK_CSI;
extern long unsigned int DEBUG_PIXELIZATION_NUM_FRONT_CSI;
extern long unsigned int DEBUG_PIXELIZATION_NUM_BACK_CSI;
extern long unsigned int DEBUG_PIXELIZATION_NUM_PIXEL_NOCSI;
extern long unsigned int DEBUG_PIXELIZATION_NUM_MULTIPLEPIXEL_CSI;
extern long unsigned int DEBUG_PIXELIZATION_NUM_VALID_PIXELS;
#endif

//===============================
extern const int NUM_BARS_NWA;
extern const int NUM_BARS_NWB;
extern const int NUM_BARS_VW;
extern const int NUM_DETECTORS_FA;
//===============================

//===============================
extern const int NUM_DETS_MICROBALL;
extern const int N_MICROBALL_RINGS;
extern const int N_MICROBALL_DETS_PER_RING[];
//===============================

//===============================
extern const int NUM_TEL;
extern const int NUM_STRIP_F;
extern const int NUM_STRIP_B;
extern const int NUM_CSI_TEL;
extern const int NUM_CSI_STRIP;
extern const int NUM_TOWERS;
extern const int NUM_MAX_PARTICLES_PER_TEL;
//===============================

#endif
