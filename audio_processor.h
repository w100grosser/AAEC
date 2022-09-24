#include "audio_device_generator.h"


using namespace std;
#pragma once
class audio_processor
{
	public:
		void init(pAudioDevices pall_audio_devices);
		void start(BOOL loopback, BOOL speakers);
		INT read_audio(BYTE* output_data, UINT32* pnum_frames_availabe, UINT32* ppointer, float* pTransferBuffer_left, float* pTransferBuffer_right, int index);
		INT write_audio(BYTE* output_data, UINT32* pnum_frames_availabe, UINT32* ppointer, float** pTransferBuffer_left, float** pTransferBuffer_right);
		INT process_audio(float* mic_data, float* speakers_data, float* output_data, INT32 pmic_data_pointer, INT32 pspeakers_data_pointer);
		INT SetFormat(WAVEFORMATEX* pwfx);
		void InputThreadFunction();
		void OutputThreadFunction();
		void MicThreadFunction();

	private:
		WAVEFORMATEXTENSIBLE  format;
		float transfer_buffer_left[44100];
		float transfer_buffer_right[44100];
		float transfer_buffer_mic[88200];
		float transfer_buffer_output[88200];
		float * ptransfer_buffer_left[2];
		float* ptransfer_buffer_right[2];
		UINT32 data_blocks_num = 0;
		UINT32 local_data_blocks_num[2] = {0, 0};
		UINT32* pdata_blocks_num = &data_blocks_num;
		BYTE read[2] = { true, true };
		BYTE write = true;
		BYTE * pread = read;
		BYTE * pwrite = &write;
		UINT32 read_frames_num = 0;
		UINT32 written_frames_num = 0;
		UINT32 PointerMic = 0;
		UINT32 * pPointerMic = &PointerMic;
		UINT32 pPointer[2] = {0, 0};
		UINT32* pread_frames_num = &read_frames_num;
		UINT32* pwritten_frames_num = &written_frames_num;
		pAudioDevices pall_audio_devices;
		thread * pInputThread;
		thread * pOutputThread;
		thread * pCableThread;
		WAVEFORMATEX* pwfx;
		packet_sender sender;


		DOUBLE * pfft_input_mic_left;
		DOUBLE * pfft_input_mic_right;
		DOUBLE * pfft_input_speakers_left;
		DOUBLE * pfft_input_speakers_right;
		DOUBLE * pfft_output_mic_left;
		DOUBLE * pfft_output_mic_right;
		DOUBLE * pfft_output_speakers_left;
		DOUBLE * pfft_output_speakers_right;

		fftw_plan* pfft_dct_mic_left;
		fftw_plan* pfft_dct_mic_right;
		fftw_plan* pfft_idct_mic_left;
		fftw_plan* pfft_idct_mic_right;
		fftw_plan* pfft_dct_speakers_left;
		fftw_plan* pfft_dct_speakers_right;
		fftw_plan* pfft_idct_speakers_left;
		fftw_plan* pfft_idct_speakers_right;
};

