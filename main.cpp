#include "includes.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int kbhit()
{
	int n;
	ioctl(0, FIONREAD, &n);
	return n;
}

int main()
{
	DBG_PRT("welcome use videocms_emb system ^_^\n");
	av_startup();
	DBG_PRT("please input charactor exit system\n");
	do 
	{
		usleep(1000 * 1000);
		DBG_PRT("welcome use videocms_emb system ^_^\n");
	} while (!kbhit());
	return AV_OK;
}