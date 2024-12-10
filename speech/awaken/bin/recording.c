
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <alsa/asoundlib.h>  

#define IVW_AUDIO_FILE_NAME "./awake.pcm"

int size;  
snd_pcm_t *handle;  
unsigned int val;  
snd_pcm_uframes_t frames;  
char *buffer;  

void init_sound_capture()
{
	int rc;  
	int dir;  
	snd_pcm_hw_params_t *params;  

	rc = snd_pcm_open(&handle, "hw:0,0", SND_PCM_STREAM_CAPTURE, 0);  
	if (rc < 0) {  
		fprintf(stderr,"unable to open pcm device: %s\n",snd_strerror(rc));  
		exit(1);  
	}  

	/* Allocate a hardware parameters object. */  
	snd_pcm_hw_params_alloca(&params);  

	/* Fill it in with default values. */  
	snd_pcm_hw_params_any(handle, params);  

	/* Set the desired hardware parameters. */  

	/* Interleaved mode */  
	snd_pcm_hw_params_set_access(handle, params,  
			SND_PCM_ACCESS_RW_INTERLEAVED);  

	/* Signed 16-bit little-endian format */  
	snd_pcm_hw_params_set_format(handle, params,  
			SND_PCM_FORMAT_S16_LE);  

	/* One channels (stereo) */  
	snd_pcm_hw_params_set_channels(handle, params, 1);  

	/* 16000 bits/second sampling rate (CD quality) */  
	val = 16000;  
	snd_pcm_hw_params_set_rate_near(handle, params,  
			&val, &dir);  

	/* Set period size to 32 frames. */  
	frames = 32;  
	snd_pcm_hw_params_set_period_size_near(handle,  
			params, &frames, &dir);  
	/* Write the parameters to the driver */  
	rc = snd_pcm_hw_params(handle, params);  
	if (rc < 0) {  
		fprintf(stderr,  
				"unable to set hw parameters: %s\n",  
				snd_strerror(rc));  exit(1);  
	}  


	/* Use a buffer large enough to hold one period */  
	snd_pcm_hw_params_get_period_size(params,  
			&frames, &dir);  
	size = frames * 2; /* 2 bytes/sample, 2 channels */  
	buffer = (char *) malloc(size);  

	/* We want to loop for 2 seconds */  
	snd_pcm_hw_params_get_period_time(params,  
			&val, &dir);  

}

void sound_capture_save_file()
{
	FILE *fp;
	long loops;  
	int rc;  

	fp = fopen(IVW_AUDIO_FILE_NAME, "w+");
	if(fp == NULL){
		printf("open file error\n");
		exit(1);
	}

	loops = 2000000 / val;  

	while (loops > 0) {  
		loops--;  
		rc = snd_pcm_readi(handle, buffer, frames);  
		if (rc == -EPIPE) {  
			/* EPIPE means overrun */  
			fprintf(stderr, "overrun occurred\n");  
			snd_pcm_prepare(handle);  
		} else if (rc < 0) {  
			fprintf(stderr,  
					"error from read: %s\n",  
					snd_strerror(rc));  
		} else if (rc != (int)frames) {  
			fprintf(stderr, "short read, read %d frames\n", rc);  
		}  
		rc = fwrite(buffer, 1, size, fp);  
		if (rc != size)  
			fprintf(stderr,  
					"short write: wrote %d bytes\n", rc);  
	}  
	
	fclose(fp);
	
}


void exit_sound_capture(void)
{
	snd_pcm_drop(handle);
	snd_pcm_unlink(handle);
	snd_pcm_hw_free(handle);
	snd_pcm_close(handle);
	free(buffer);

}

int main(int argc, char *argv[]) 
{

	init_sound_capture();
	sound_capture_save_file();
	exit_sound_capture();


	return 0;
}
