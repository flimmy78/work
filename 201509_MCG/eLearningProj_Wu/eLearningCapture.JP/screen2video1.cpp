// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "screen2video1.h"

// Dispatch interfaces referenced by this interface
#include "videocompressors1.h"
#include "AudioCompressors1.h"
#include "WMVProfiles1.h"
#include "AudioInputPins1.h"
#include "audiodevices1.h"

/////////////////////////////////////////////////////////////////////////////
// CScreen2Video

IMPLEMENT_DYNCREATE(CScreen2Video, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CScreen2Video properties

short CScreen2Video::GetVideoCompressor()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void CScreen2Video::SetVideoCompressor(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

CVideoCompressors CScreen2Video::GetVideoCompressors()
{
	LPDISPATCH pDispatch;
	GetProperty(0x2, VT_DISPATCH, (void*)&pDispatch);
	return CVideoCompressors(pDispatch);
}

void CScreen2Video::SetVideoCompressors(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

CString CScreen2Video::GetFileName()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void CScreen2Video::SetFileName(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

BOOL CScreen2Video::GetUseVideoCompressor()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void CScreen2Video::SetUseVideoCompressor(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

BOOL CScreen2Video::GetCaptureAudio()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void CScreen2Video::SetCaptureAudio(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

short CScreen2Video::GetAudioCompressor()
{
	short result;
	GetProperty(0x6, VT_I2, (void*)&result);
	return result;
}

void CScreen2Video::SetAudioCompressor(short propVal)
{
	SetProperty(0x6, VT_I2, propVal);
}

BOOL CScreen2Video::GetUseAudioCompressor()
{
	BOOL result;
	GetProperty(0x7, VT_BOOL, (void*)&result);
	return result;
}

void CScreen2Video::SetUseAudioCompressor(BOOL propVal)
{
	SetProperty(0x7, VT_BOOL, propVal);
}

CAudioCompressors CScreen2Video::GetAudioCompressors()
{
	LPDISPATCH pDispatch;
	GetProperty(0x8, VT_DISPATCH, (void*)&pDispatch);
	return CAudioCompressors(pDispatch);
}

void CScreen2Video::SetAudioCompressors(LPDISPATCH propVal)
{
	SetProperty(0x8, VT_DISPATCH, propVal);
}

short CScreen2Video::GetWMVProfile()
{
	short result;
	GetProperty(0x9, VT_I2, (void*)&result);
	return result;
}

void CScreen2Video::SetWMVProfile(short propVal)
{
	SetProperty(0x9, VT_I2, propVal);
}

CWMVProfiles CScreen2Video::GetWMVProfiles()
{
	LPDISPATCH pDispatch;
	GetProperty(0xa, VT_DISPATCH, (void*)&pDispatch);
	return CWMVProfiles(pDispatch);
}

void CScreen2Video::SetWMVProfiles(LPDISPATCH propVal)
{
	SetProperty(0xa, VT_DISPATCH, propVal);
}

CString CScreen2Video::GetWMVCustomProfile()
{
	CString result;
	GetProperty(0xb, VT_BSTR, (void*)&result);
	return result;
}

void CScreen2Video::SetWMVCustomProfile(LPCTSTR propVal)
{
	SetProperty(0xb, VT_BSTR, propVal);
}

short CScreen2Video::GetCaptureLeft()
{
	short result;
	GetProperty(0xc, VT_I2, (void*)&result);
	return result;
}

void CScreen2Video::SetCaptureLeft(short propVal)
{
	SetProperty(0xc, VT_I2, propVal);
}

short CScreen2Video::GetCaptureTop()
{
	short result;
	GetProperty(0xd, VT_I2, (void*)&result);
	return result;
}

void CScreen2Video::SetCaptureTop(short propVal)
{
	SetProperty(0xd, VT_I2, propVal);
}

short CScreen2Video::GetCaptureWidth()
{
	short result;
	GetProperty(0xe, VT_I2, (void*)&result);
	return result;
}

void CScreen2Video::SetCaptureWidth(short propVal)
{
	SetProperty(0xe, VT_I2, propVal);
}

short CScreen2Video::GetCaptureHeight()
{
	short result;
	GetProperty(0xf, VT_I2, (void*)&result);
	return result;
}

void CScreen2Video::SetCaptureHeight(short propVal)
{
	SetProperty(0xf, VT_I2, propVal);
}

CString CScreen2Video::GetLicenseKey()
{
	CString result;
	GetProperty(0x10, VT_BSTR, (void*)&result);
	return result;
}

void CScreen2Video::SetLicenseKey(LPCTSTR propVal)
{
	SetProperty(0x10, VT_BSTR, propVal);
}

BOOL CScreen2Video::GetCaptureCursor()
{
	BOOL result;
	GetProperty(0x11, VT_BOOL, (void*)&result);
	return result;
}

void CScreen2Video::SetCaptureCursor(BOOL propVal)
{
	SetProperty(0x11, VT_BOOL, propVal);
}

short CScreen2Video::GetFps()
{
	short result;
	GetProperty(0x12, VT_I2, (void*)&result);
	return result;
}

void CScreen2Video::SetFps(short propVal)
{
	SetProperty(0x12, VT_I2, propVal);
}

CAudioInputPins CScreen2Video::GetAudioInputPins()
{
	LPDISPATCH pDispatch;
	GetProperty(0x13, VT_DISPATCH, (void*)&pDispatch);
	return CAudioInputPins(pDispatch);
}

void CScreen2Video::SetAudioInputPins(LPDISPATCH propVal)
{
	SetProperty(0x13, VT_DISPATCH, propVal);
}

short CScreen2Video::GetAudioInputPin()
{
	short result;
	GetProperty(0x14, VT_I2, (void*)&result);
	return result;
}

void CScreen2Video::SetAudioInputPin(short propVal)
{
	SetProperty(0x14, VT_I2, propVal);
}

short CScreen2Video::GetAudioDevice()
{
	short result;
	GetProperty(0x15, VT_I2, (void*)&result);
	return result;
}

void CScreen2Video::SetAudioDevice(short propVal)
{
	SetProperty(0x15, VT_I2, propVal);
}

CAudioDevices CScreen2Video::GetAudioDevices()
{
	LPDISPATCH pDispatch;
	GetProperty(0x16, VT_DISPATCH, (void*)&pDispatch);
	return CAudioDevices(pDispatch);
}

void CScreen2Video::SetAudioDevices(LPDISPATCH propVal)
{
	SetProperty(0x16, VT_DISPATCH, propVal);
}

long CScreen2Video::GetCaptureHWND()
{
	long result;
	GetProperty(0x17, VT_I4, (void*)&result);
	return result;
}

void CScreen2Video::SetCaptureHWND(long propVal)
{
	SetProperty(0x17, VT_I4, propVal);
}

long CScreen2Video::GetSWFAudioBitrate()
{
	long result;
	GetProperty(0x18, VT_I4, (void*)&result);
	return result;
}

void CScreen2Video::SetSWFAudioBitrate(long propVal)
{
	SetProperty(0x18, VT_I4, propVal);
}

long CScreen2Video::GetSWFAudioChannels()
{
	long result;
	GetProperty(0x19, VT_I4, (void*)&result);
	return result;
}

void CScreen2Video::SetSWFAudioChannels(long propVal)
{
	SetProperty(0x19, VT_I4, propVal);
}

long CScreen2Video::GetSWFAudioSampleRate()
{
	long result;
	GetProperty(0x1a, VT_I4, (void*)&result);
	return result;
}

void CScreen2Video::SetSWFAudioSampleRate(long propVal)
{
	SetProperty(0x1a, VT_I4, propVal);
}

short CScreen2Video::GetSWFOutputWidth()
{
	short result;
	GetProperty(0x1b, VT_I2, (void*)&result);
	return result;
}

void CScreen2Video::SetSWFOutputWidth(short propVal)
{
	SetProperty(0x1b, VT_I2, propVal);
}

short CScreen2Video::GetSWFOutputHeight()
{
	short result;
	GetProperty(0x1c, VT_I2, (void*)&result);
	return result;
}

void CScreen2Video::SetSWFOutputHeight(short propVal)
{
	SetProperty(0x1c, VT_I2, propVal);
}

long CScreen2Video::GetSWFVideoBitrate()
{
	long result;
	GetProperty(0x1d, VT_I4, (void*)&result);
	return result;
}

void CScreen2Video::SetSWFVideoBitrate(long propVal)
{
	SetProperty(0x1d, VT_I4, propVal);
}

long CScreen2Video::GetOutputType()
{
	long result;
	GetProperty(0x1e, VT_I4, (void*)&result);
	return result;
}

void CScreen2Video::SetOutputType(long propVal)
{
	SetProperty(0x1e, VT_I4, propVal);
}

long CScreen2Video::GetFLVAudioBitrate()
{
	long result;
	GetProperty(0x1f, VT_I4, (void*)&result);
	return result;
}

void CScreen2Video::SetFLVAudioBitrate(long propVal)
{
	SetProperty(0x1f, VT_I4, propVal);
}

long CScreen2Video::GetFLVAudioChannels()
{
	long result;
	GetProperty(0x20, VT_I4, (void*)&result);
	return result;
}

void CScreen2Video::SetFLVAudioChannels(long propVal)
{
	SetProperty(0x20, VT_I4, propVal);
}

long CScreen2Video::GetFLVAudioSampleRate()
{
	long result;
	GetProperty(0x21, VT_I4, (void*)&result);
	return result;
}

void CScreen2Video::SetFLVAudioSampleRate(long propVal)
{
	SetProperty(0x21, VT_I4, propVal);
}

long CScreen2Video::GetFLVVideoBitrate()
{
	long result;
	GetProperty(0x22, VT_I4, (void*)&result);
	return result;
}

void CScreen2Video::SetFLVVideoBitrate(long propVal)
{
	SetProperty(0x22, VT_I4, propVal);
}

short CScreen2Video::GetFLVOutputHeight()
{
	short result;
	GetProperty(0x23, VT_I2, (void*)&result);
	return result;
}

void CScreen2Video::SetFLVOutputHeight(short propVal)
{
	SetProperty(0x23, VT_I2, propVal);
}

short CScreen2Video::GetFLVOutputWidth()
{
	short result;
	GetProperty(0x24, VT_I2, (void*)&result);
	return result;
}

void CScreen2Video::SetFLVOutputWidth(short propVal)
{
	SetProperty(0x24, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CScreen2Video operations

BOOL CScreen2Video::Start()
{
	BOOL result;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CScreen2Video::Stop()
{
	InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

short CScreen2Video::ScreenWidth()
{
	short result;
	InvokeHelper(0x27, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CScreen2Video::ScreenHeight()
{
	short result;
	InvokeHelper(0x28, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL CScreen2Video::RefreshAudioInputPin(short iAudioDeviceIndex)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x29, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		iAudioDeviceIndex);
	return result;
}

void CScreen2Video::Pause()
{
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CScreen2Video::Resume()
{
	InvokeHelper(0x2b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CScreen2Video::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
