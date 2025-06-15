#include "wav.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "portaudio.h"
#include "utils.h"


int wav_play(const char *path)
{
    wav_header  header;
    wav_data    raw_data;
    
    const int status = wav_read(path, &header, &raw_data);
    
    if (status != WAV_FILE_READ_SUCCESS)
    {
        puts("Error reading wav data!");
        return 1;
    }
    
    return wav_play_audio(&header, &raw_data);
}

int wav_read(const char *path, wav_header* header, wav_data* raw_data)
{
    FILE* wav_file = fopen(path, "rb");
    
    if (!wav_file)
    {
        printf("Error opening a file! Code: %d\nError desc. %s\n", errno, strerror(errno));
        fclose(wav_file);
        return 1;
    }
    
    fread(header, sizeof(wav_header), 1, wav_file);
    
    int status = wav_parse_header(header);
    
    if (status != WAV_HEADER_PARSE_SUCCESS)
    {
        printf("Error parsing wav header! Code: %d\nError desc. %s\n", status, wav_get_errno_str(status));
        fclose(wav_file);
        return 1;
    }
    
    status = wav_print_fmt_info(header);
    
    if (status != WAV_PRINT_SUCCESS)
    {
        return 1;
    }
    
    raw_data->buffer = malloc(header->data_size * sizeof(char));
    
    if (!raw_data->buffer)
    {
        printf("Error allocating buffer memory! Code: %d\nError desc. %s\n", errno, strerror(errno));
        fclose(wav_file);
        return 1;
    }
    
    const size_t samples_count  = header->data_size / sizeof(int16_t);
    const size_t frames_count   = header->data_size / (header->channels * sizeof(int16_t));
    size_t samples_read         = fread(raw_data->buffer, sizeof(int16_t), samples_count, wav_file);
    
    printf("Frames count: %zu\n", frames_count);
    printf("Data bytes read: %zu\n\n", samples_read * sizeof(int16_t));
    
    fclose(wav_file);
    
    return WAV_FILE_READ_SUCCESS;
}

int wav_parse_header(wav_header *header)
{
    if (!header)
    {
        return WAV_HEADER_NOT_PROVIDED;
    }
    
    if (memcmp(header->tag, "RIFF", 4) != 0)
    {
        return WAV_HEADER_WRONG_TAG;
    }
    
    if (memcmp(header->file_type, "WAVE", 4) != 0)
    {
        return WAV_HEADER_WRONG_TYPE;
    }

    if (header->format != 1)
    {
        return WAV_HEADER_WRONG_FORMAT;
    }

    return WAV_HEADER_PARSE_SUCCESS;
}

int wav_play_audio(wav_header* header, wav_data* raw_data)
{
    if (!header || !raw_data)
    {
        puts("No header or raw data provided!");
        return -1;
    }

    PaStream* output_stream = NULL;
    PaError   result        = paNotInitialized;


    const long   frames_per_buffer = 1024;
    const size_t frames_count      = header->data_size / (header->channels * sizeof(int16_t));
 

    result = Pa_OpenDefaultStream(
        &output_stream,
        0,
        header->channels,
        paInt16,
        (double)header->sample_rate,
        frames_per_buffer,
        NULL,
        NULL
    );

    if (result != paNoError)
    {
        printf("Error opening PaStream!\nError desc. %s\n", Pa_GetErrorText(result));
        return -1;
    }
    
    result = Pa_StartStream(output_stream);

    if (result != paNoError)
    {
        printf("Error starting PaStream!\nError desc. %s\n", Pa_GetErrorText(result));
        return -1;
    }
    
    const size_t buffer_count    = frames_count / frames_per_buffer;
    size_t       frames_left     = frames_count;
    size_t       frames_to_write = frames_per_buffer;

    for (size_t i = 0; i < buffer_count; ++i)
    {
        if (frames_left < frames_per_buffer)
        {
            frames_to_write = frames_left;
        }
        
        result = Pa_WriteStream(output_stream, raw_data->buffer + ( sizeof(int16_t) * frames_per_buffer * i ), frames_to_write);
    
        if (result != paNoError)
        {
            printf("Error writing to PaStream!\nError desc. %s\n", Pa_GetErrorText(result));
            return -1;
        }

        frames_left -= frames_per_buffer * (i + 1);
    }

    result = Pa_StopStream(output_stream);
    
    if (result != paNoError)
    {
        printf("Error stopping PaStream!\nError desc. %s\n", Pa_GetErrorText(result));
        return -1;
    }

    result = Pa_CloseStream(output_stream);

    if (result != paNoError)
    {
        printf("Error closing PaStream!\nError desc. %s\n", Pa_GetErrorText(result));
        return -1;
    }

    return WAV_FILE_PLAY_SUCCESS;
}
