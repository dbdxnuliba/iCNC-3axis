#pragma once
#include <StdAfx.h>
#include <stdio.h>
#include "..\INC\mmi.h"

// mmiInit //
void mmiInit(MMI_DATA *mmiDataPtr)
{
//	delByteSpace((char *)mmiDataPtr, sizeof(MMI_DATA)); //checking char from char; size of MMI_DATA
	mmiDataPtr->timerCounter = 0;
	mmiDataPtr->formInput = false;
	mmiDataPtr->opMode = MEM_Mode;

	for (int i = 0; i < 200; i++)
	{
		mmiDataPtr->ncFileName[i] = NULL;
	}
	mmiDataPtr->ncFileValid = false;
	mmiDataPtr->feedOverride = 100;
	mmiDataPtr->M01 = false;
	mmiDataPtr->feedHold = false;
	mmiDataPtr->singleBlock = false;
	mmiDataPtr->wheelRatio = 0;

	mmiDataPtr->start = false;
	mmiDataPtr->startValid = false;
	mmiDataPtr->reset = false;
	mmiDataPtr->resetValid = false;
	mmiDataPtr->teachStart = false;
	mmiDataPtr->teachStartValid = false;
	mmiDataPtr->record = false;
	mmiDataPtr->recordValid = false;
	mmiDataPtr->teachEnd = false;
	mmiDataPtr->teachEndValid = false;
	mmiDataPtr->emergencyStop = false;
	mmiDataPtr->emergencyStopValid = false;

	mmiDataPtr->uvwButton[0] = 0;
	mmiDataPtr->uvwButton[1] = 0;
	mmiDataPtr->uvwButton[2] = 0;
	mmiDataPtr->mouseUp = false;


	mmiDataPtr->axisButton = 0; //1=X+ 2=X- 3=Y+ 4=Y- 5=Z+ 6=Z-
	mmiDataPtr->axisButtonValid = false;


	return;
}//mmiInit()

// mmiCtl //
void mmiCtl(MMI_DATA *mmiDataPtr)
{//mmiCtl();
	
	if (mmiDataPtr->linkDataPtr->systemReset == true)
	{
		mmiReset(mmiDataPtr);
	}
	else
		mmiMain(mmiDataPtr);
	
	return;
} //mmiCtl()

// mmiClose //
void mmiClose(MMI_DATA *mmiDataPtr)
{

	
	return;
}//mmiClose

// mmiReset //
void mmiReset(MMI_DATA *mmiDataPtr)
{
	if (mmiDataPtr->linkDataPtr->mmiResetValid == false)
	{
		int timerTemp = mmiDataPtr->timerCounter;
//		OP_MODE opModeTemp = mmiDataPtr->opMode;
		mmiInit(mmiDataPtr);
		mmiDataPtr->opMode = mmiDataPtr->linkDataPtr->systemOpMode;
		mmiDataPtr->mmiToSysBlock.opMode = mmiDataPtr->linkDataPtr->systemOpMode;
		mmiDataPtr->timerCounter = timerTemp;
		mmiDataPtr->linkDataPtr->mmiResetValid = true;
		
	}
	return;
}//mmiReset