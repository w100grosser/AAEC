#include "audio_device_generator.h"

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

//void audio_device_generator::get_default_render_audio()
//{
//
//}
//
//void audio_device_generator::get_default_capture_audio()
//{
//
//}
//
//void audio_device_generator::get_cable_audio()
//{
//
//}

void audio_device_generator::init(pAudioDevices pall_audio_devices)
{
	CoInitialize(0);
	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&(pall_audio_devices->pEnumerator));


	hr = pall_audio_devices->pEnumerator->EnumAudioEndpoints(
		eRender, DEVICE_STATE_ACTIVE,
		&(pall_audio_devices->pCollectionOutput));

	hr = pall_audio_devices->pEnumerator->EnumAudioEndpoints(
		eCapture, DEVICE_STATE_ACTIVE,
		&(pall_audio_devices->pCollectionInput));


	hr = pall_audio_devices->pEnumerator->GetDefaultAudioEndpoint(
		eRender, eConsole, &(pall_audio_devices->pEndpointOutputDefault));


	hr = pall_audio_devices->pEnumerator->GetDefaultAudioEndpoint(
		eCapture, eConsole, &(pall_audio_devices->pEndpointInputDefault));

	hr = pall_audio_devices->pCollectionInput->GetCount(&(pall_audio_devices->CountInput));

	hr = pall_audio_devices->pCollectionOutput->GetCount(&(pall_audio_devices->CountOutput));


	if (pall_audio_devices->CountInput == 0 || pall_audio_devices->CountOutput == 0)
	{
		printf("No endpoints found.\n");
	}

	// Each loop prints the name of an endpoint device.
	for (ULONG i = 0; i < pall_audio_devices->CountInput; i++)
	{
		// Get pointer to endpoint number i.
		hr = pall_audio_devices->pCollectionInput->Item(i, & (pall_audio_devices->pInputEndpoint[i]));

			// Get the endpoint ID string.
			hr = pall_audio_devices->pInputEndpoint[i]->GetId(&(pall_audio_devices->pInputwszID[i]));

			hr = pall_audio_devices->pInputEndpoint[i]->OpenPropertyStore(
				STGM_READ, &(pall_audio_devices->pInputProps[i]));

			
		// Initialize container for property value.
			PropVariantInit(&(pall_audio_devices->InputvarName[i]));

		// Get the endpoint's friendly-name property.
		hr = pall_audio_devices->pInputProps[i]->GetValue(
			PKEY_Device_FriendlyName, &(pall_audio_devices->InputvarName[i]));

		// Print endpoint friendly name and endpoint ID.
		printf("Input Endpoint %d: \"%S\" (%S)\n",
			i, (pall_audio_devices->InputvarName[i]), (char*)(pall_audio_devices->InputvarName[i].pwszVal));
	}

	// Each loop prints the name of an endpoint device.
	for (ULONG i = 0; i < pall_audio_devices->CountOutput; i++)
	{
		// Get pointer to endpoint number i.
		hr = pall_audio_devices->pCollectionOutput->Item(i, &(pall_audio_devices->pOutputEndpoint[i]));

		// Get the endpoint ID string.
		hr = pall_audio_devices->pOutputEndpoint[i]->GetId(&(pall_audio_devices->pOutputwszID[i]));

		hr = pall_audio_devices->pOutputEndpoint[i]->OpenPropertyStore(
			STGM_READ, &(pall_audio_devices->pOutputProps[i]));


		// Initialize container for property value.
		PropVariantInit(&(pall_audio_devices->OutputvarName[i]));

		// Get the endpoint's friendly-name property.
		hr = pall_audio_devices->pOutputProps[i]->GetValue(
			PKEY_Device_FriendlyName, &(pall_audio_devices->OutputvarName[i]));

		// Print endpoint friendly name and endpoint ID.
		printf("Output Endpoint %d: \"%S\" (%S)\n",
			i, (pall_audio_devices->OutputvarName[i]), (char*)(pall_audio_devices->OutputvarName[i].pwszVal));

		int b = 0;

		while (b < 6)
		{
			
			if (pall_audio_devices->OutputvarName[i].pwszVal[b] != "CABLE Inp"[b])
			{
				//printf("\n%d\t%d", b, i);
				break;
			}
			b++;
		};

		if (b == 6) {
			pall_audio_devices->pEndpointCable = pall_audio_devices->pOutputEndpoint[i];
			printf("\nFound the device --->>> %S\n\n", pall_audio_devices->OutputvarName[i].pwszVal);
		}
	}
}