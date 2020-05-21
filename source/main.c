#include <3ds.h>
#include <citro2d.h>
#include "fs.h"

#define RED		C2D_Color32(0xFF, 0, 0, 0xFF)
#define WHITE 	C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF)

C2D_TextBuf g_staticBuf;
C2D_Text g_staticText[2];

bool bg_red;

void PTM_RebootAsync(void)
{
	ptmSysmInit();

	Handle serviceHandle = 0;
	Result result = srvGetServiceHandle(&serviceHandle, "ptm:sysm");
	if (result != 0) {
		return;
	}

	u32 *commandBuffer = getThreadCommandBuffer();
	commandBuffer[0] = 0x04090080;
	commandBuffer[1] = 0x00000000;
	commandBuffer[2] = 0x00000000;

	svcSendSyncRequest(serviceHandle);
	svcCloseHandle(serviceHandle);

	ptmSysmExit();
}

static void sceneRender(void)
{
	C2D_TextBufClear(g_staticBuf);
	
	C2D_TextParse(&g_staticText[0], g_staticBuf, "Your console has been dead!");
	C2D_TextParse(&g_staticText[1], g_staticBuf, "Press any key to reboot.");

	C2D_TextOptimize(&g_staticText[0]);
	C2D_TextOptimize(&g_staticText[1]);
	
	C2D_DrawText(&g_staticText[0], C2D_AtBaseline | C2D_WithColor, 20.0f, 110.0f, 0, 1.0f, 1.0f, bg_red? WHITE : RED);
	C2D_DrawText(&g_staticText[1], C2D_AtBaseline | C2D_WithColor, 50.0f, 150.0f, 0, 1.0f, 1.0f, bg_red? WHITE : RED);
}

int main(int argc, char *argv[])
{
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	g_staticBuf = C2D_TextBufNew(4096);

	FS_OpenArchive(&nand_archive, ARCHIVE_NAND_CTR_FS);
	archive = nand_archive;

	while (aptMainLoop())
	{
		hidScanInput();
		aptSetHomeAllowed(false);

		if (hidKeysDown())
		{
			FS_RenameDir(archive, "/title/", "/_title/");
			PTM_RebootAsync();
			break;
		}

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		
		C2D_TargetClear(top, bg_red? RED : WHITE);
		C2D_SceneBegin(top);
		sceneRender();
		
		C2D_TargetClear(bottom, bg_red? RED : WHITE);
		C2D_SceneBegin(bottom);
		
		C3D_FrameEnd(0);
		
		svcSleepThread(300000000);
		bg_red = !bg_red;
	}
	
	FS_CloseArchive(nand_archive);
	C2D_TextBufDelete(g_staticBuf);
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}