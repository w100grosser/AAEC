#include "audio_processor.h"

audio_device_generator my_audio_device_generator;

int main()
{
	printf("yes");
	BOOL loopback = false;
	BOOL speakers = false;
	AudioDevices all_audio_devices;
	pAudioDevices pall_audio_devices = &all_audio_devices;
	my_audio_device_generator.init(pall_audio_devices);
	printf("%c", all_audio_devices.InputvarName[0].pwszVal[4]);
	audio_processor our_audio_processor;
	our_audio_processor.init(pall_audio_devices);
	our_audio_processor.start(loopback, speakers);


}