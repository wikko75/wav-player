#include "utils.h"
#include <stdio.h>

void wav_print_pa_host_info()
{
    const PaHostApiIndex host_index = Pa_GetDefaultHostApi();

    if (host_index < 0)
    {
        puts("Seems like default host API can't be retrived : (");
        return;
    }

    const PaHostApiInfo* info = Pa_GetHostApiInfo(host_index);

    if (!info)
    {
        puts("Error getting host API info!");
        return;
    }

    printf(
        "\n======= HOST API INFO =======\n"
        "%-25s %s\n"
        "%-25s %d\n"
        "%-25s %d\n"
        "%-25s %d\n"
        "=============================\n\n",
        "Name:",                  info->name,
        "Device count:",          info->deviceCount,
        "Default input device:",  info->defaultInputDevice,
        "Default output device:", info->defaultOutputDevice
    );
}

void wav_print_pa_device_info(const PaDeviceIndex index)
{
    if (index < 0)
    {
        puts("Seems like device index can't be retrived : (");
        return;
    }
    const PaDeviceInfo* info = Pa_GetDeviceInfo(index);

    if (!info)
    {
        puts("Error getting device info!");
        return;
    }

    printf(
        "\n======= DEVICE INFO =======\n"
        "%-30s %s\n"
        "%-30s %d\n"
        "%-30s %d\n"
        "%-30s %f\n"
        "%-30s %f\n"
        "%-30s %f\n"
        "%-30s %f\n"
        "%-30s %f\n"
        "=============================\n\n",
        "Name:",                        info->name,
        "Max input channels:",          info->maxInputChannels,
        "Max output channels:",         info->maxOutputChannels,
        "Default sample rate:",         info->defaultSampleRate,
        "Default high input latency:",  info->defaultHighInputLatency,
        "Default high output latency:", info->defaultHighOutputLatency,
        "Default low input latency:",   info->defaultLowInputLatency,
        "Defualt low output latency:",  info->defaultLowOutputLatency
    );
}

int wav_print_fmt_info(const wav_header *header)
{
    if (!header)
    {
        return WAV_HEADER_NOT_PROVIDED;
    }

   printf(
    "=========== WAV HEADER ===========\n"
    "%-20s %.4s\n"
    "%-20s %u\n"
    "%-20s %.4s\n"
    "%-20s %u\n"
    "%-20s %u\n"
    "%-20s %u\n"
    "%-20s %u Hz\n"
    "%-20s %u\n"
    "%-20s %u\n"
    "%-20s %u\n"
    "%-20s %u\n"
    "==================================\n",
    "Tag:",              header->tag,
    "Chunk size:",       header->chunk_size,
    "File type:",        header->file_type,
    "Format chunk size:",header->fmt_size,
    "Format:",           header->format,
    "No. of channels:",  header->channels,
    "Sample rate:",      header->sample_rate,
    "Byte rate:",        header->byte_rate,
    "Block align:",      header->block_align,
    "Bits per sample:",  header->bits_per_sample,
    "Data size:",        header->data_size
    );

    return WAV_PRINT_SUCCESS;
}

const char* wav_get_errno_str(const int error_code)
{
    switch (error_code)
    {
    case WAV_HEADER_NOT_PROVIDED:
        return "wav header was not provided";
    case WAV_HEADER_WRONG_TAG:
        return "wrong wav header TAG field value - should be RIFF";
    case WAV_HEADER_WRONG_TYPE:
        return "wrong wav header FORMAT field value - should be WAVE";
    case WAV_HEADER_WRONG_FORMAT:
        return "wrong wav header FORMAT field value - should be 1 for PCM";
    default:
        return "standard library or wrong error_code provided! no description!";
    }
}
