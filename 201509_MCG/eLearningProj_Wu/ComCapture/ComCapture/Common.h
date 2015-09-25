#ifndef _COMMON_H_
#define _COMMON_H_

#define WM_FGNOTIFY			WM_APP+1		// Notify Capture Status for Parent Window

typedef enum {
	ENUMCAPTUREDEVICE_CAM_AUDIO=0,
	ENUMCAPTUREDEVICE_CAM_VIDEO=1
	// ENUMCAPTUREDEVICE_DV=2,
	// ENUMCAPTUREDEVICE_ANALOGVIDEO=3,
} ENUMCAPTUREDEVICE;

typedef enum {
	// ENUMSESSION_CAM=0,
	// ENUMSESSION_DV=1,
	// ENUMSESSION_ANALOG=2,
	// ENUMSESSION_AUDIO=3,
	// ENUMSESSION_SCREEN=4,
	// ENUMSESSION_PHOTONSCREEN=5,
	ENUMSESSION_CAM=0,
	ENUMSESSION_AUDIO=1,
	ENUMSESSION_SCREEN=2,
	ESESSION_IPCAM=3
} ENUMSESSION;

#endif