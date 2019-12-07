#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <whb/proc.h>
#include <whb/log.h>
#include <whb/log_console.h>
#include <ramfs.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char line[128];
	
	// initialize procui/console
	WHBProcInit();
	WHBLogConsoleInit();
	
	// initialize ramfs library
	int res = ramfsInit();
	if (res) {
		WHBLogPrintf(">> Failed to init ramfs: %d", res);
		goto end;
	}
	
	// open helloworld.txt
	FILE *fp = fopen("resin:/helloworld.txt", "r");
	if (fp == NULL) {
		WHBLogPrint(">> Failed to open file");
		goto end;
	}
	
	WHBLogPrint(">> Content of resin:/helloworld.txt:");
	
	// output content to console
	while (fgets(line, sizeof(line), fp) != NULL)
		WHBLogPrint(line);
	
	// cleanup
	fclose(fp);

end:
	WHBLogPrint(">> Done. Press Home to exit");

	// draw the contents of console to screen
	WHBLogConsoleDraw();
	
	// wait for the user to exit
	while(WHBProcIsRunning())
		OSSleepTicks(OSMillisecondsToTicks(100));

	// deinitialize ramfs library
	ramfsExit();

	// deinitialize procui/console
	WHBLogConsoleFree();
	WHBProcShutdown();

	return 0;
} 
