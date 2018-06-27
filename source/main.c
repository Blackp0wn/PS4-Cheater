#include "ps4.h"
#include "include/kernel.h"
#include "include/proc.h"

int(*sceSysUtilSendSystemNotificationWithText)(int messageType, char* message);

void sysNotify(char* msg) {
	sceSysUtilSendSystemNotificationWithText(222, msg);
}

int _main(void) {
	initKernel();
	initLibc();

	kexec(kernelPayload, NULL);

	int sysUtil = sceKernelLoadStartModule("/system/common/lib/libSceSysUtil.sprx", 0, NULL, 0, 0, 0);
	RESOLVE(sysUtil, sceSysUtilSendSystemNotificationWithText);
    int gamePID = findProcess("eboot.bin");
	procAttach(gamePID);
	u64 gameCheck;
	procReadBytes(gamePID, 0x00000, (void*)&gameCheck, sizeof(gameCheck));

	if (gameCheck == 0x00000) {
	        u8 bytes[] = { 0x00};
	        procWriteBytes(gamePID, (void*)0x000000, bytes, sizeof(bytes));
	        sysNotify("Uncharted4 v1.00 Detected");
	        procDetach(gamePID);
	}
	else if (gameCheck == 0x000000) {
	        u8 bytes[] = { 0x00};
	        procWriteBytes(gamePID, (void*)0x000000, bytes, sizeof(bytes));
	        sysNotify("Uncharted4 v1.32 Detected");
	        procDetach(gamePID);
	}
	else {
		sysNotify("Failed to Find Game Version");
		procDetach(gamePID);
		return FALSE;
	}
    

	return 0;
}
