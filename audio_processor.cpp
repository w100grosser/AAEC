#include "audio_processor.h"

int audio_processor::read_audio(BYTE* pinput_data, UINT32* pnum_frames_availabe)
{
	if (pinput_data != NULL && *(pwrite) && !*(pread))
	{
		*pread = false;
		float* bData = (float*)pinput_data;
		*pdata_blocks_num = *pnum_frames_availabe *format.Format.nChannels;

		for (int i = 0; i < *pdata_blocks_num; i ++)
		{
			transfer_buffer[i] = bData[i] / 4;
			*pread_frames_num = *pread_frames_num + 1;
		}
		sender.send_packet((char*)bData, *pnum_frames_availabe * format.Format.nChannels);
		*pread = true;
		*pwrite = false;
	}
	else {
		*pnum_frames_availabe = 0;
	}

	return 0;
}

int audio_processor::write_audio(BYTE* output_data, UINT32* pnum_frames_availabe)
{
	if (output_data != NULL && *(pread) && !*(pwrite))
	{
		*pwrite = false;
		float* bData = (float*)output_data;
		if (*pnum_frames_availabe < *pdata_blocks_num / 2) {
		}
		else {
			*pnum_frames_availabe = *pdata_blocks_num / 2;
		}

		for (int i = 0; i < *pnum_frames_availabe * format.Format.nChannels; i++)
		{
			bData[i] = transfer_buffer[i];
			*pwritten_frames_num = *pwritten_frames_num + 1;
		}
		//sender.send_packet((char*)bData, *pnum_frames_availabe * format.Format.nChannels);
		*pread = false;
		*pwrite = true;
	}
	else {
		*pnum_frames_availabe = 0;
	}

	return 0;
}

int audio_processor::SetFormat(WAVEFORMATEX* pwfx)
{
	printf("wFormatTag is %x\n", pwfx->wFormatTag);
	format.Format.wFormatTag = pwfx->wFormatTag;
	printf("nChannels is %x\n", pwfx->nChannels);
	format.Format.nChannels = pwfx->nChannels;
	printf("nSamplesPerSec is %d\n", pwfx->nSamplesPerSec);
	format.Format.nSamplesPerSec = pwfx->nSamplesPerSec;
	printf("nAvgBytesPerSec is %d\n", pwfx->nAvgBytesPerSec);
	format.Format.nAvgBytesPerSec = pwfx->nAvgBytesPerSec;
	printf("wBitsPerSample is %d\n", pwfx->wBitsPerSample);
	format.Format.wBitsPerSample = pwfx->wBitsPerSample;
	printf("nBlockAlign is %d\n", pwfx->nBlockAlign);
	format.Format.nBlockAlign = pwfx->nBlockAlign;

	return 0;
}

void audio_processor::init(pAudioDevices pall_audio_devices)
{
	audio_processor::pall_audio_devices = pall_audio_devices;
	sender.init("27015");
}

void audio_processor::start()
{
	HRESULT hr;
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;
	UINT32 bufferFrameCount;
	UINT32 packetLength = 0;

	//numFramesAvailable = 100000;

	hr = pall_audio_devices->pEndpointInputDefault->Activate(
		IID_IAudioClient, CLSCTX_ALL,
		NULL, (void**)& (pall_audio_devices->pAudioClientInput));

	hr = pall_audio_devices->pAudioClientInput->GetMixFormat(&pwfx);

	hr = pall_audio_devices->pAudioClientInput->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		0,
		hnsRequestedDuration,
		0,
		pwfx,
		NULL);

	// Get the size of the allocated buffer.
	hr = pall_audio_devices->pAudioClientInput->GetBufferSize(&bufferFrameCount);

	hr = pall_audio_devices->pAudioClientInput->GetService(
		IID_IAudioCaptureClient,
		(void**)&(pall_audio_devices->pCaptureClient));

	// Notify the audio sink which format to use.
	hr = SetFormat(pwfx);

	// Calculate the actual duration of the allocated buffer.
	hnsActualDuration = (double)REFTIMES_PER_SEC *
		bufferFrameCount / pwfx->nSamplesPerSec;

	//id = 0;
	//if (id != 0)
	//{
	//	hr = pEnumerator->GetDevice(
	//		*id,
	//		&pDeviceOutput
	//	);
	//}
	//else {
	//	hr = pEnumerator->GetDefaultAudioEndpoint(
	//		eRender, eConsole, &pDeviceOutput);
	//}


	hr = pall_audio_devices->pEndpointOutputDefault->Activate(
		IID_IAudioClient, CLSCTX_ALL,
		NULL, (void**)&(pall_audio_devices->pAudioClientOutput));

	hr = pall_audio_devices->pAudioClientOutput->GetMixFormat(&pwfx);

	hr = pall_audio_devices->pAudioClientOutput->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		0,
		hnsRequestedDuration,
		0,
		pwfx,
		NULL);

	// Get the actual size of the allocated buffer.
	hr = pall_audio_devices->pAudioClientOutput->GetBufferSize(&bufferFrameCount);

	hr = pall_audio_devices->pAudioClientOutput->GetService(
		IID_IAudioRenderClient,
		(void**)&(pall_audio_devices->pRenderClient));


	hr = pall_audio_devices->pEndpointOutputDefault->OpenPropertyStore(
		STGM_READ, &(pall_audio_devices->pOutputDefaultProps));
	PROPVARIANT varNameOutput;
	hr = pall_audio_devices->pOutputDefaultProps->GetValue(
		PKEY_Device_FriendlyName, &varNameOutput);


	printf("\nTHIS IS OUTPUT DEVICE %S", varNameOutput.pwszVal);

	// Grab the entire buffer for the initial fill operation.
	//hr = pall_audio_devices->pRenderClient->GetBuffer(bufferFrameCount, &pData);

	//hr = pall_audio_devices->pRenderClient->ReleaseBuffer(bufferFrameCount, flags);

	// Calculate the actual duration of the allocated buffer.
	hnsActualDuration = (double)REFTIMES_PER_SEC *
		bufferFrameCount / pwfx->nSamplesPerSec;

	hr = pall_audio_devices->pAudioClientOutput->Start();  // Start playing.


	hr = pall_audio_devices->pAudioClientInput->Start();  // Start recording.


	thread InputThread(&audio_processor::InputThreadFunction, this);
	thread OutputThread(&audio_processor::OutputThreadFunction, this);

	// Wait for thread t1 to finish
	InputThread.join();

	// Wait for thread t2 to finish
	OutputThread.join();
}

void audio_processor::InputThreadFunction()
{
	REFERENCE_TIME hnsActualDuration;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	UINT32 numFramesPadding;
	UINT32 packetLength = 0;
	DWORD flags;
	BYTE* pData4;
	float* pfData;
	HRESULT hr;


	// Get the actual size of the allocated buffer.
	hr = pall_audio_devices->pAudioClientInput->GetBufferSize(&bufferFrameCount);

	hnsActualDuration = (double)REFTIMES_PER_SEC *
		bufferFrameCount / pwfx->nSamplesPerSec;

	while (1)
	{
		while (true)
		{

			// Sleep for half the buffer duration.
			//Sleep(hnsActualDuration / REFTIMES_PER_SEC / 2);
			hr = pall_audio_devices->pCaptureClient->GetNextPacketSize(&packetLength);

			//printf("\nRecording, current index %d\n", *pData1_pointer1);
			//printf("\nTHIS IS FIRST %d", 22);
			while (packetLength != 0)
			{
				// Get the available data in the shared buffer.
				hr = pall_audio_devices->pCaptureClient->GetBuffer(
					&pData4,
					&numFramesAvailable,
					&flags, NULL, NULL);
				//printf("frames avail %d\n", numFramesAvailable);
				if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
				{
					printf("AUDCLNT_BUFFERFLAGS_SILENT");
					pData4 = NULL;  // Tell endpoint2local to write silence.
				}

				read_audio(pData4, &numFramesAvailable);

				//printf("%d, %d", numFramesAvailable, pData4[0]);
				hr = pall_audio_devices->pCaptureClient->ReleaseBuffer(numFramesAvailable);

				hr = pall_audio_devices->pCaptureClient->GetNextPacketSize(&packetLength);
			}
		}
	}

}

void audio_processor::OutputThreadFunction()
{
	REFERENCE_TIME hnsActualDuration;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	UINT32 numFramesPadding;
	UINT32 packetLength = 0;
	BYTE* pData;
	HRESULT hr;
	int flags = 0;

	// Get the actual size of the allocated buffer.
	hr = pall_audio_devices->pAudioClientOutput->GetBufferSize(&bufferFrameCount);

	hnsActualDuration = (double)REFTIMES_PER_SEC / pwfx->nSamplesPerSec;


	// Each loop fills about half of the shared buffer.
	while (true)
	{

		// See how much buffer space is available.
		hr = pall_audio_devices->pAudioClientOutput->GetCurrentPadding(&numFramesPadding);

		numFramesAvailable = bufferFrameCount - numFramesPadding;

		// Grab all the available space in the shared buffer.
		hr = pall_audio_devices->pRenderClient->GetBuffer(numFramesAvailable, &pData);

		//    // Get next 1/2-second of data from the audio source.
		write_audio(pData, &numFramesAvailable);

		// Sleep for half the buffer duration.
		if (numFramesAvailable > 0) {
			//Sleep((DWORD)(hnsActualDuration / REFTIMES_PER_MILLISEC / numFramesAvailable));
		}

		hr = pall_audio_devices->pRenderClient->ReleaseBuffer(numFramesAvailable, flags);

		//// Sleep for half the buffer duration.
		//Sleep((DWORD)(hnsActualDuration / REFTIMES_PER_MILLISEC / 2));
	}

}

void audio_processor::CableThreadFunction()
{

}