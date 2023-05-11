/* 

Get real-time contact quality readings

  */

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include "emokit/emokit.h"

int quit;
void cleanup(int i){
	fprintf(stdout,"Shutting down\n");
	quit=1; 
}

int main(int argc, char **argv)
{
	struct emokit_device* d;
	signal(SIGINT, cleanup); //trap cntrl c
	

	quit=0;

	d = emokit_create();
	int count=emokit_get_count(d, EMOKIT_VID, EMOKIT_PID);
	printf("Current epoc devices connected: %d\n", count );
	int r = emokit_open(d, EMOKIT_VID, EMOKIT_PID, 1);
	if(r != 0)
	{
		emokit_close(d);
		emokit_delete(d);
		d = emokit_create();
		r = emokit_open(d, EMOKIT_VID, EMOKIT_PID, 0);
		if (r!=0) {
			printf("CANNOT CONNECT: %d\n", r);
			return 1;
		}
	}
	printf("Connected to headset.\n");
	
	r = emokit_read_data_timeout(d,1000);
	if (r<=0) {
		if(r<0)
			fprintf(stderr, "Error reading from headset\n");
		else
			fprintf(stderr, "Headset Timeout...\n");
		emokit_close(d);
		emokit_delete(d);
		return 1;
	}
	char timestamp[14 + 1];
	char milliseconds[6 + 1];
	// print CSV header
	fprintf(stdout, "timestamp,F3_V,F3_Q,FC6_V,FC6_Q,P7_V,P7_Q,T8_V,T8_Q,F7_V,F7_Q,F8_V,F8_Q,T7_V,T7_Q,P8_V,P8_Q,AF4_V,AF4_Q,F4_V,F4_Q,AF3_V,AF3_Q,O2_V,O2_Q,O1_V,O1_Q,FC5_V,FC5_Q\n");
	struct emokit_frame c;
	while (!quit) {
		int err = emokit_read_data_timeout(d, 1000);
		if(err > 0) {
			c = emokit_get_next_frame(d);
			struct timeval tv;
			gettimeofday(&tv, NULL);
			time_t t = tv.tv_sec;
			struct tm *tm = localtime(&t);
			strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", tm);
			sprintf(milliseconds, "%06d", (int) tv.tv_usec);
			fprintf(stdout,"%s%s,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d\n",
			timestamp,milliseconds,c.cq.F3, c.F3, c.cq.FC6, c.FC6, c.cq.P7, c.P7, c.cq.T8, c.T8, c.cq.F7, c.F7, c.cq.F8, c.F8, c.cq.T7, c.T7, c.cq.P8, c.P8, c.cq.AF4, c.AF4, c.cq.F4, c.F4, c.cq.AF3, c.AF3, c.cq.O2, c.O2, c.cq.O1, c.O1, c.cq.FC5, c.FC5);
			fflush(stdout);
		} else if(err == 0) {
			fprintf(stderr, "Headset Timeout...\n");
		}
	}
	emokit_close(d);
	emokit_delete(d);
	return 0;
}

