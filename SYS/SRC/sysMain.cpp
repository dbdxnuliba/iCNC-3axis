#pragma once
#include <StdAfx.h>
#include <stdio.h>
#include <string.h>
#include "..\INC\sys.h"

void sysMain(SYS_DATA *sysDataPtr)
{
	sysInput(sysDataPtr);
	sysOutput(sysDataPtr);
	
	return;
}

void sysInput(SYS_DATA *sysDataPtr)
{
	//check block Valid (mmi to sys)
	if(sysDataPtr->linkDataPtr->mmiToSysBlockValid == true)
	{
		//transport the block from link to sys
		sysDataPtr->mmiToSysBlock = sysDataPtr->linkDataPtr->mmiToSysBlock;
		sysDataPtr->mmiToSysBlockValid = true;

		if (sysDataPtr->linkDataPtr->mmiToSysBlock.continuee && sysDataPtr->linkDataPtr->mmiToSysBlock.continueValid 
			&& sysDataPtr->linkDataPtr->mmiToSysBlockValid)
		{
			sysDataPtr->sysToIntpBlock.continuee = sysDataPtr->mmiToSysBlock.continuee;
			sysDataPtr->sysToIntpBlock.continueValid = true;
		}
		else
			sysDataPtr->sysToIntpBlock.continueValid = false;

		sysDataPtr->linkDataPtr->mmiToSysBlockValid = false; // resume waiting
	}

	//check block Valid (intp to sys)
//	if (sysDataPtr->linkDataPtr->intpToSysBlockValid == true)
//	{	//transport the block from intp to sys
		sysDataPtr->intpToSysBlock = sysDataPtr->linkDataPtr->intpToSysBlock;
//		sysDataPtr->linkDataPtr->intpToSysBlockValid = false;
//	}

	switch(sysDataPtr->linkDataPtr->systemOpMode)
	{
	case MEM_Mode:
	case MDI_Mode:
	case HOME_Mode:
	case JOG_Mode:
	case WHEEL_Mode:
		//check ncFileValid
		if(sysDataPtr->linkDataPtr->mmiToSysBlock.ncFileValid == true)
		{	//copy file name from the block to sys
			strcpy_s( sysDataPtr->ncFileName, sysDataPtr->mmiToSysBlock.ncFileName);
			sysDataPtr->ncFileValid = true;
			sysDataPtr->linkDataPtr->mmiToSysBlock.ncFileValid = false; //resume waiting
		}// 
		
		sysDataPtr->sysToIntpBlock.feedHold = sysDataPtr->mmiToSysBlock.feedHold; //?!
		sysDataPtr->sysToIntpBlock.feedOverride = sysDataPtr->mmiToSysBlock.feedOverride;
		sysDataPtr->sysToIntpBlock.singleBlock = sysDataPtr->mmiToSysBlock.singleBlock;
		sysDataPtr->sysToIntpBlock.M01 = sysDataPtr->mmiToSysBlock.M01;
		sysDataPtr->sysToIntpBlock.wheelRatio = sysDataPtr->mmiToSysBlock.wheelRatio;

		break;
	default:
		break;
	}
}// sysInput()

void sysOutput(SYS_DATA *sysDataPtr)
{
	//output intp content
	
	switch(sysDataPtr->linkDataPtr->systemOpMode)
	{
	case MEM_Mode:
	case MDI_Mode:
	case HOME_Mode:
		if(sysDataPtr->mmiToSysBlock.start == true && sysDataPtr->mmiToSysBlock.startValid== true && sysDataPtr->ncFileValid == true)
		{	//copy file name from SYS_DATA to sysToDecBlock of SYS_DATA
			strcpy_s(sysDataPtr->sysToDecBlock.ncFileName, sysDataPtr->ncFileName);
			sysDataPtr->sysToDecBlock.ncFileValid = true;
			sysDataPtr->ncFileValid = false;
			//check if sysToDecBlock
			if(sysDataPtr->linkDataPtr->sysToDecBlockValid == false)
			{	//transport the block to link
				sysDataPtr->linkDataPtr->sysToDecBlock = sysDataPtr->sysToDecBlock;
				sysDataPtr->linkDataPtr->sysToDecBlockValid = true;
			}
		}
		writeFifo(&sysDataPtr->linkDataPtr->sysToIntpFifo.fifoHead, &sysDataPtr->sysToIntpBlock);
		sysDataPtr->sysToIntpBlock.continueValid = false;
/*		if (sysDataPtr->linkDataPtr->sysToIntpBlockValid == false )
		{
			sysDataPtr->linkDataPtr->sysToIntpBlock = sysDataPtr->sysToIntpBlock;
			sysDataPtr->linkDataPtr->sysToIntpBlockValid = true;
		}*/
		break;
	case JOG_Mode:
	case WHEEL_Mode:
		if(sysDataPtr->mmiToSysBlock.axisButtonValid == true && sysDataPtr->mmiToSysBlock.axisButton != 0 
			&& sysDataPtr->ncFileValid == true)
		{
			strcpy_s(sysDataPtr->sysToDecBlock.ncFileName, sysDataPtr->ncFileName);
			sysDataPtr->sysToDecBlock.ncFileValid = true;
			sysDataPtr->ncFileValid = false;
			//check if sysToDecBlock
			if(sysDataPtr->linkDataPtr->sysToDecBlockValid == false)
			{	//transport the block to link
				sysDataPtr->linkDataPtr->sysToDecBlock = sysDataPtr->sysToDecBlock;
				sysDataPtr->linkDataPtr->sysToDecBlockValid = true;
			}
		}
		writeFifo(&sysDataPtr->linkDataPtr->sysToIntpFifo.fifoHead, &sysDataPtr->sysToIntpBlock);
/*		if (sysDataPtr->linkDataPtr->sysToIntpBlockValid == false )
		{
			sysDataPtr->linkDataPtr->sysToIntpBlock = sysDataPtr->sysToIntpBlock;
			sysDataPtr->linkDataPtr->sysToIntpBlockValid = true;
		}*/
	default:
		break;
	}
}// sysOutput()


void sysOpMEM(SYS_DATA *sysDataPtr)
{

}// sysOpMEM()