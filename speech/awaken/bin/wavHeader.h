/*
 * @Description:  
 * @version:  V1.0
 * @Company: 
 * @Author: Lipingping
 * @Date: 2019-09-19 11:29:07
 * @LastEditors: Lipingping
 * @LastEditTime: 2019-09-19 11:35:24
 */
#ifndef _WAV_H
#define _WAV_H
 
#include <stdio.h>
#include <stdlib.h>
 
#define SAMPLE_RATE 16000
#define QUANTIZATION 16
#define BYTES_EACH_SAMPLE 2
#define CHANNEL_NUN 1
#define FORMAT_TAG 1
 
struct RIFF_CHUNK
{
    char riffid[4];
    unsigned int dwSize;
    char riffType[4];
};
typedef struct RIFF_CHUNK WAVE_HEADER;
 
struct FORMAT_CHUNK
{
    char fccid[4];
    unsigned int dwSize;
    short wFormatTag;
    short wChannels;
    unsigned int dwSamplesPerSec;
    unsigned int dwAvgBytePerSec;
    short wBlockAlign;
    short uiBitsPerSample;
};
typedef struct FORMAT_CHUNK FORMAT;
 
struct DATA_CHUNK
{
    char fccid[4];
    unsigned int dwSize;
};
 
typedef struct DATA_CHUNK DATA;
 
#endif
