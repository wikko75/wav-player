#ifndef _UTILS_H_
#define _UTILS_H_

#include "portaudio.h"
#include "wav.h"

void wav_print_pa_host_info();

void wav_print_pa_device_info(const PaDeviceIndex index);

int wav_print_fmt_info(const wav_header* header);

const char* wav_get_errno_str(const int error_code);

#endif
