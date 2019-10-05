#include <StdAfx.h>
#include <stdio.h>
#include "..\INC\sys.h"




void sysInit(SYS_DATA *sysDataPtr)
{
//	delByteSpace((char *)sysDataPtr, sizeof(SYS_DATA));
	sysDataPtr->sysState = Sys_Run;
	sysDataPtr->sysResetState = Reset_Null;

	sysDataPtr->OpMode = MEM_Mode;
	sysDataPtr->newOpMode = MEM_Mode;
	for(int i = 0; i < 200; i++)
	{
		sysDataPtr->ncFileName[i] = NULL;
	}
	sysDataPtr->ncFileValid = false;
	sysDataPtr->mmiToSysBlockValid = false;
	sysDataPtr->sysToDecBlockValid = false;
	sysDataPtr->sysToIntpBlockValid = false;
	sysDataPtr->sysToMmiBlockValid = false;

	initFifo(&sysDataPtr->linkDataPtr->sysToIntpFifo.fifoHead, SYS_TO_INTP);

}// sysInit()

void sysCtl(SYS_DATA *sysDataPtr)
{
	//check mode change
	bool modeChange;
	if(sysDataPtr->linkDataPtr->systemOpMode != sysDataPtr->mmiToSysBlock.opMode && sysDataPtr->mmiToSysBlockValid == true)
	{
		sysDataPtr->newOpMode = sysDataPtr->mmiToSysBlock.opMode;
		modeChange = true;

		sysDataPtr->linkDataPtr->systemOpMode = sysDataPtr->newOpMode;
		sysDataPtr->OpMode = sysDataPtr->newOpMode;
	}
	//check all reset events
	bool resetJudgment = (sysDataPtr->mmiToSysBlock.reset && sysDataPtr->mmiToSysBlock.resetValid)
						|| modeChange || sysDataPtr->mmiToSysBlock.mouseUp  || sysDataPtr->linkDataPtr->systemReset ;
	//check reset
	if (resetJudgment == true)
	{
		sysDataPtr->linkDataPtr->systemReset = true;

		sysDataPtr->sysState = Sys_Reset;
		resetJudgment = false;
		modeChange = false;
	}

	switch(sysDataPtr->sysState)
	{
	case Sys_Run:
		sysMain(sysDataPtr);
		break;
	case Sys_Reset:
		sysReset(sysDataPtr);
		break;
	}
	return;
}// sysCtl()

void sysReset(SYS_DATA *sysDataPtr)
{

	bool ResetFinish =  sysDataPtr->linkDataPtr->decResetValid && sysDataPtr->linkDataPtr->intpResetValid 
					  && sysDataPtr->linkDataPtr->mmiResetValid;
	if (ResetFinish == true) //all FBs reset finished
	{
		sysDataPtr->linkDataPtr->systemReset = false;
		linkInit(sysDataPtr->linkDataPtr);
		sysDataPtr->linkDataPtr->systemOpMode = sysDataPtr->newOpMode;

		sysDataPtr->mmiToSysBlock.reset = false;
		sysDataPtr->mmiToSysBlock.resetValid = false;
		delByteSpace((char *)&sysDataPtr->mmiToSysBlock, sizeof(MMI_TO_SYS_BLOCK));
		delByteSpace((char *)&sysDataPtr->sysToDecBlock, sizeof(SYS_TO_DEC_BLOCK));
		delByteSpace((char *)&sysDataPtr->sysToIntpBlock, sizeof(SYS_TO_INTP_DATA));

		sysDataPtr->mmiToSysBlock.opMode = sysDataPtr->newOpMode;
		sysDataPtr->ncFileValid = false;
		sysDataPtr->mmiToSysBlockValid = false;
		sysDataPtr->sysToDecBlockValid = false;
		sysDataPtr->sysToIntpBlockValid = false;
		sysDataPtr->sysToMmiBlockValid = false;

		sysDataPtr->sysState = Sys_Run;
	}
	return;
}//sysReset
