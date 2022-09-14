#include "audio_device_generator.h"


using namespace std;
#pragma once
class audio_processor
{
	public:
		void init(pAudioDevices pall_audio_devices);
		void start(BOOL loopback, BOOL speakers);
		INT read_audio(BYTE* input_data, UINT32 * num_frames_availabe, UINT32 * ppointer);
		INT write_audio(BYTE* output_data, UINT32 * num_frames_availabe, UINT32 * ppointer);
		INT SetFormat(WAVEFORMATEX* pwfx);
		void InputThreadFunction();
		void OutputThreadFunction();

	private:
		WAVEFORMATEXTENSIBLE  format;
		float transfer_buffer_left[44100];
		float transfer_buffer_right[44100];
		float transfer_buffer[88200];
		UINT32 data_blocks_num = 44100;
		UINT32* pdata_blocks_num = &data_blocks_num;
		BYTE read = true;
		BYTE write = false;
		BYTE * pread = &read;
		BYTE * pwrite = &write;
		UINT32 read_frames_num = 0;
		UINT32 written_frames_num = 0;
		UINT32 pointer = 0;
		UINT32 * ppointer = &pointer;
		UINT32* pread_frames_num = &read_frames_num;
		UINT32* pwritten_frames_num = &written_frames_num;
		pAudioDevices pall_audio_devices;
		thread * pInputThread;
		thread * pOutputThread;
		thread * pCableThread;
		WAVEFORMATEX* pwfx;
		void CableThreadFunction();
		packet_sender sender;
};

