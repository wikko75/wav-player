#include <stdio.h>
#include "portaudio.h"
#include "wav.h"
#include "utils.h"

int main() 
{
    PaError result = Pa_Initialize();

    if (result != paNoError)
    {
        printf("Error initializing PortAudio!\nCause: %s\n", Pa_GetErrorText(result));
        return -1;
    }

    // Query info about default Host API, input / output devices
    wav_print_pa_host_info();
    wav_print_pa_device_info(Pa_GetDefaultInputDevice());
    wav_print_pa_device_info(Pa_GetDefaultOutputDevice());
    
    int status = wav_play("signals/test_sine.wav");

    if (status != WAV_FILE_PLAY_SUCCESS)
    {
        puts("Error playing .wav file!");
    }
    
    result = Pa_Terminate();

    if(result != paNoError)
    {
        printf("Error terminating PortAudio!\nCause: %s\n", Pa_GetErrorText(result));
        return -1;
    }

    return 0;
}
