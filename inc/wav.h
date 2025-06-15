
#ifndef _WAV_H_
#define _WAV_H_ 

#include <inttypes.h>

#define WAV_HEADER_PARSE_SUCCESS  0
#define WAV_FILE_READ_SUCCESS     0
#define WAV_FILE_PLAY_SUCCESS     0
#define WAV_PRINT_SUCCESS         0
#define WAV_HEADER_NOT_PROVIDED  -1
#define WAV_HEADER_WRONG_TAG     -2
#define WAV_HEADER_WRONG_TYPE    -3
#define WAV_HEADER_WRONG_FORMAT  -4


#pragma pack(push, 1)
typedef struct
{
    // RIFF header starts
    char        tag[4];         // RIFF
    uint32_t    chunk_size;     // whole struct
    char        file_type[4];   // WAV
    // fmt subchunk starts
    char        id_fmt[4];
    uint32_t    fmt_size;
    uint16_t    format;         // prob. PCM = 1 
    uint16_t    channels;
    uint32_t    sample_rate;
    uint32_t    byte_rate;      // (channels * sample_rate * bits per sample) / 8
    uint16_t    block_align;    // (bits_per_sample * channels) / 8 
    uint16_t    bits_per_sample;
    // data subchunk starts
    char        id_data[4];     // "data"
    uint32_t    data_size;
} wav_header;
#pragma pack(pop)

typedef struct 
{
    void* buffer;
} wav_data;

int wav_play(const char* path);

int wav_parse_header(wav_header* header);

int wav_read(const char *path, wav_header* header, wav_data* raw_data);

int wav_play_audio(wav_header* header, wav_data* raw_data);

#endif
