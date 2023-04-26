/* 

Get real-time contact quality readings

  */

#include <stdio.h>
#include <string.h>
#include <signal.h>
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

	struct emokit_frame c;
	while (!quit) {
		int err = emokit_read_data_timeout(d, 1000);
		if(err > 0) {
			c = emokit_get_next_frame(d);			
			fprintf(stdout,"F3 Reading: %4d, Quality: %4d, FC6 Reading: %4d, Quality: %4d, P7 Reading: %4d, Quality: %4d, T8 Reading: %4d, Quality: %4d, F7  Reading: %4d, Quality: %4d, F8  Reading: %4d, Quality: %4d, T7  Reading: %4d, Quality: %4d, P8  Reading: %4d, Quality: %4d, AF4 Reading: %4d, Quality: %4d, F4  Reading: %4d, Quality: %4d, AF3 Reading: %4d, Quality: %4d, O2  Reading: %4d, Quality: %4d, O1  Reading: %4d, Quality: %4d, FC5 Reading: %4d, Quality: %4d\n",
			c.cq.F3, c.F3, c.cq.FC6, c.FC6, c.cq.P7, c.P7, c.cq.T8, c.T8, c.cq.F7, c.F7, c.cq.F8, c.F8, c.cq.T7, c.T7, c.cq.P8, c.P8, c.cq.AF4, c.AF4, c.cq.F4, c.F4, c.cq.AF3, c.AF3, c.cq.O2, c.O2, c.cq.O1, c.O1, c.cq.FC5, c.FC5);
			fflush(stdout);
		} else if(err == 0) {
			fprintf(stderr, "Headset Timeout...\n");
		}
	}
	emokit_close(d);
	emokit_delete(d);
	return 0;
}

