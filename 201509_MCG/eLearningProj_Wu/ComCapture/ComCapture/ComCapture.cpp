// ComCapture.cpp : DLL �G�N�X�|�[�g�̎����ł��B

//
// ����: COM+ 1.0 ���:
//      �R���|�[�l���g���C���X�g�[������ɂ́AMicrosoft Transaction Explorer ���C���X�g�[�����Ă��������B
//      �o�^�͊���ł͍s���܂���B 

#include "stdafx.h"
#include "resource.h"
#include "ComCapture_i.h"
#include "dllmain.h"
#include "xdlldata.h"


// DLL �� OLE �ɂ���ăA�����[�h�ł���悤�ɂ��邩�ǂ������w�肵�܂��B
STDAPI DllCanUnloadNow(void)
{
	#ifdef _MERGE_PROXYSTUB
	HRESULT hr = PrxDllCanUnloadNow();
	if (hr != S_OK)
		return hr;
#endif
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (AfxDllCanUnloadNow()==S_OK && _AtlModule.GetLockCount()==0) ? S_OK : S_FALSE;
	}

// �v�����ꂽ�^�̃I�u�W�F�N�g���쐬����N���X �t�@�N�g����Ԃ��܂��B
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	#ifdef _MERGE_PROXYSTUB
	if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
		return S_OK;
#endif
		return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

// DllRegisterServer - �G���g�����V�X�e�� ���W�X�g���ɒǉ����܂��B
STDAPI DllRegisterServer(void)
{
	// �I�u�W�F�N�g�A�^�C�v ���C�u��������у^�C�v ���C�u�������̂��ׂẴC���^�[�t�F�C�X��o�^���܂�
	HRESULT hr = _AtlModule.DllRegisterServer();
	#ifdef _MERGE_PROXYSTUB
	if (FAILED(hr))
		return hr;
	hr = PrxDllRegisterServer();
#endif
		return hr;
}

// DllUnregisterServer - �G���g�������W�X�g������폜���܂��B
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	#ifdef _MERGE_PROXYSTUB
	if (FAILED(hr))
		return hr;
	hr = PrxDllRegisterServer();
	if (FAILED(hr))
		return hr;
	hr = PrxDllUnregisterServer();
#endif
		return hr;
}

// DllInstall - ���[�U�[����уR���s���[�^�[���Ƃ̃V�X�e�� ���W�X�g�� �G���g����ǉ��܂��͍폜���܂��B
STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine)
{
	HRESULT hr = E_FAIL;
	static const wchar_t szUserSwitch[] = L"user";

	if (pszCmdLine != NULL)
	{
		if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
		{
			ATL::AtlSetPerUserRegistration(true);
		}
	}

	if (bInstall)
	{	
		hr = DllRegisterServer();
		if (FAILED(hr))
		{
			DllUnregisterServer();
		}
	}
	else
	{
		hr = DllUnregisterServer();
	}

	return hr;
}

