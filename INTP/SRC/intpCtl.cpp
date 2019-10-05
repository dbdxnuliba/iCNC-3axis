#pragma once
#include <StdAfx.h>
#include <stdio.h>
#include "..\INC\intp.h"

void intpInit(INTP_DATA *intpDataPtr)
{
	/*
#if HD_TIMER
	intpDataPtr->cyclingTime = REAL_SAMPLING_TIME*0.001; //in s
	intpDataPtr->display = 5;
	intpDataPtr->displayCounter = 5;
#else
	intpDataPtr->cyclingTime = FORM_SAMPLING_TIME*0.001; //in s
	intpDataPtr->display = 1;
	intpDataPtr->displatCounter = 1;
#endif
	*/
	intpDataPtr->intpState =  Intp_Idle;

	intpDataPtr->l1 = 75.0;
	intpDataPtr->l2 = 75.0;

	intpDataPtr->xyzStart[0] = 75.0;
	intpDataPtr->xyzStart[1] = 75.0;
	intpDataPtr->xyzStart[2] = INIT_Z_POSITION;

	intpDataPtr->uvwStart[0] = 0.0;
	intpDataPtr->uvwStart[1] = 90.0;
	intpDataPtr->uvwStart[2] = INIT_Z_POSITION; //w = z
	
	for(int i = 0; i < 3; i++)
	{
 		intpDataPtr->xyzNew[i] = intpDataPtr->xyzStart[i];
		intpDataPtr->xyzOld[i] = intpDataPtr->xyzStart[i];
		intpDataPtr->xyzEnd[i] = intpDataPtr->xyzStart[i];

		intpDataPtr->uvwNew[i] = intpDataPtr->uvwStart[i];
		intpDataPtr->uvwOld[i] = intpDataPtr->uvwStart[i];
		intpDataPtr->uvwEnd[i] = intpDataPtr->uvwStart[i];

		intpDataPtr->uvwIncrement[i] = 0;
	}
	
	intpDataPtr->uvwResolution[0] = 0.0018;
	intpDataPtr->uvwResolution[1] = 0.0018;
	intpDataPtr->uvwResolution[2] = 0.0025;

	intpDataPtr->g00Speed = DEFAULT_SPEED; //mm/s
	intpDataPtr->cyclingTime = timeInterval;


	intpDataPtr->slopeDataPtr = &intpDataPtr->slopeData;
	slopeInit(intpDataPtr->slopeDataPtr);

	//init NC_BLOCK
	delByteSpace((char *)&intpDataPtr->ncBlock, sizeof(NC_BLOCK));

	intpDataPtr->ncBlock.ncCmd = G00_Motion;
	intpDataPtr->ncBlock.motionFunction.motion.feedRate = DEFAULT_SPEED;
	intpDataPtr->ncBlock.motionFunction.motion.relAbs = true;
	
	return;
} //intpInit()

void intpCtl(INTP_DATA *intpDataPtr)
{
	if (intpDataPtr->linkDataPtr->systemReset == true)
		intpReset(intpDataPtr);
	else
		intpMain(intpDataPtr);
	return;
} //intpCtl()

void intpReset(INTP_DATA *intpDataPtr)
{
	if (intpDataPtr->linkDataPtr->intpResetValid == false)
	{
		switch (intpDataPtr->intpResetState)
		{
		case Intp_Reset_Pre:
			intpResetPre(intpDataPtr);
			break;
		case Intp_Reset_Exe:
			intpResetExe(intpDataPtr);
			break;
		case Intp_Reset_Close:
			intpResetClose(intpDataPtr);
			//change intpResetValid = false;
			break;
	
		}
	}
	//reset process

	return;
}// intpReset()

void intpClose(INTP_DATA *intpDataPtr)
{
	//close process

	return;
}