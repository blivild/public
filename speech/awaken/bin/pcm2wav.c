/*
 * @Description:  
 * @version:  V1.0
 * @Company: 
 * @Author: Lipingping
 * @Date: 2019-09-19 11:30:20
 * @LastEditors: Lipingping
 * @LastEditTime: 2019-09-19 11:35:36
 */
 
#include "wavHeader.h"
 
long getFileSize(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return -1;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}
 
void WriteWaveHeader(FILE *fptr, unsigned int num)
{
    WAVE_HEADER WaveHeader;
    FORMAT WaveFormat;
    DATA WaveData;
    /**************RIFF chunk 参数定义*********************/
    WaveHeader.riffid[0] = 'R'; //ChunkID
    WaveHeader.riffid[1] = 'I';
    WaveHeader.riffid[2] = 'F';
    WaveHeader.riffid[3] = 'F';
    WaveHeader.dwSize = num * BYTES_EACH_SAMPLE + 36; //ChunkSize
 
    WaveHeader.riffType[0] = 'W'; //Format
    WaveHeader.riffType[1] = 'A';
    WaveHeader.riffType[2] = 'V';
    WaveHeader.riffType[3] = 'E';
 
    /**************fmt chunk 参数定义**********************/
    WaveFormat.fccid[0] = 'f'; //SubChunkID
    WaveFormat.fccid[1] = 'm';
    WaveFormat.fccid[2] = 't';
    WaveFormat.fccid[3] = ' ';
 
    WaveFormat.dwSize = 16;                                                     //SubChunkSize
    WaveFormat.wFormatTag = 1;                                                  //AudioFormat
    WaveFormat.wChannels = CHANNEL_NUN;                                         //NumChannels
    WaveFormat.dwSamplesPerSec = SAMPLE_RATE;                                   //SampleRate
    WaveFormat.dwAvgBytePerSec = BYTES_EACH_SAMPLE * SAMPLE_RATE * CHANNEL_NUN; //ByteRate
    WaveFormat.wBlockAlign = BYTES_EACH_SAMPLE;                                 //BlockAlign
    WaveFormat.uiBitsPerSample = QUANTIZATION;                                  //BitsPerSample
 
    /**************Data chunk 参数定义**********************/
    WaveData.fccid[0] = 'd'; //BlockID
    WaveData.fccid[1] = 'a';
    WaveData.fccid[2] = 't';
    WaveData.fccid[3] = 'a';
    WaveData.dwSize = num * BYTES_EACH_SAMPLE; //BlockSize
 
    fwrite(&WaveHeader, sizeof(WAVE_HEADER), 1, fptr);
    fwrite(&WaveFormat, sizeof(FORMAT), 1, fptr);
    fwrite(&WaveData, sizeof(DATA), 1, fptr);
}
 
void convertPcm2Wav(const char *inputFile, const char *wavFile)
{
    char buffer[4 * 1024 * 1024] = {};
 
    FILE *frpcm, *fwwav;
    frpcm = fopen(inputFile, "rb");
    fwwav = fopen(wavFile, "wb");
    if (!frpcm || !fwwav)
    {
        printf("ERROR: failed to fopen.\n");
        exit(1);
    }
 
    int file_sizes = getFileSize(inputFile);
    int numOfword = fread(buffer, 1, file_sizes, frpcm);
 
    WriteWaveHeader(fwwav, numOfword);               //向fptr指向的wav文件写入wav标准头文件
    fwrite(buffer, sizeof(short), numOfword, fwwav); //向fptr指向的wav文件写入buffer指向的fcm源文件
 
    if (frpcm)
    {
        fclose(frpcm);
        frpcm = NULL;
    }
 
    if (fwwav)
    {
        fclose(fwwav);
        fwwav = NULL;
    }
}
 
int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("ERROR: the number of parameters is short.\n  Usage: %s input.pcm output.wav\n", argv[0]);
        exit(1);
    }
 
    convertPcm2Wav(argv[1], argv[2]);
    return 0;
}
