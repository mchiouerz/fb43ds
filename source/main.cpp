#include <stdlib.h>
#include <stdio.h>
#include <3ds.h>
#include "webrequest.h"
#include "fb.h"


extern u32* gxCmdBuf;
u32* gpuOut = (u32*)0x1F119400;
u32* gpuDOut = (u32*)0x1F370800;

extern LPDEFFUNC pfn_State;

extern int widgets_draws();
extern int widgets_touch_events(touchPosition *p);

int main(int argc, char** argv)
{
	touchPosition lastTouch;
		
	srvInit();	
	aptInit();	
	CWebRequest::InitializeClient();	
	gfxInit();
	hidInit(NULL);	
	GPU_Init(NULL);
	gfxSet3D(false);
	fsInit();
	pfn_State = fb_init;
	srand(svcGetSystemTick());
	while(aptMainLoop()){
		hidScanInput();		
		u32 press = hidKeysDown();
		u32 held = hidKeysHeld();
		u32 release = hidKeysUp();
		if (held & KEY_TOUCH){			
			hidTouchRead(&lastTouch);
			widgets_touch_events(&lastTouch);
			held &= ~KEY_TOUCH;
		}
		if(pfn_State)
			pfn_State(0);
		widgets_draws();
		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForEvent(GSPEVENT_VBlank0, false);
	}
	fb_destroy(0);
	CWebRequest::DestroyClient();
	fsExit();
	hidExit();
	gfxExit();
	aptExit();
	srvExit();
	return 0;
}