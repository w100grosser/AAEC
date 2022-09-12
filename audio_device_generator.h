#pragma once
#include "udp_sender.h"
#include <mmdeviceapi.h>
#include <stdio.h>
#include <audioclient.h>
#include <windows.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <thread>

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000
#define MAX_THREADS 3
#define BUF_SIZE 255

typedef struct audio_devices
{
	IMMDeviceCollection* pCollectionInput = NULL;
	IMMDeviceCollection* pCollectionOutput = NULL;
	IMMDevice* pEndpointInputDefault = NULL;
	IPropertyStore* pInputDefaultProps = NULL;
	IMMDevice* pEndpointOutputDefault = NULL;
	IPropertyStore* pOutputDefaultProps = NULL;
	IMMDevice* pEndpointCable = NULL;
	IMMDevice* pInputEndpoint[20];
	IPropertyStore* pInputProps[20];
	LPWSTR pInputwszID[20];
	IMMDevice* pOutputEndpoint[20];
	IPropertyStore* pOutputProps[20];
	LPWSTR pOutputwszID[20];
	UINT CountInput = 0;
	UINT CountOutput = 0;
	PROPVARIANT InputvarName[20];
	PROPVARIANT OutputvarName[20];
	IMMDeviceEnumerator* pEnumerator = NULL;
	IAudioClient* pAudioClientInput = NULL;
	IAudioClient* pAudioClientOutput = NULL;
	IAudioClient* pAudioClientCable = NULL;
	IAudioCaptureClient* pCaptureClient = NULL;
	IAudioRenderClient* pRenderClient = NULL;
	IAudioRenderClient* pCableClient = NULL;


} AudioDevices, * pAudioDevices;

class audio_device_generator
{

public:

	//void get_default_capture_audio();
	//void get_default_render_audio();
	//void get_cable_audio();
	void init(pAudioDevices pall_audio_devices);

private:

	HRESULT hr = S_OK;
	AudioDevices all_audio_devices;
	pAudioDevices pall_audio_devices = &all_audio_devices;

};

