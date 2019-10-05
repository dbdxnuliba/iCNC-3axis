#pragma once
#include <StdAfx.h>
#include <stdio.h>
#include <string.h>
#include "..\INC\mmi.h"

// mmiMain //
void mmiMain(MMI_DATA *mmiDataPtr)
{
	formToMmiUpdate(mmiDataPtr);
	mmiToFormUpdate(mmiDataPtr);

}//mmiMain

// formToMmiUpdate (MMI Output to SYS) //
void formToMmiUpdate(MMI_DATA *mmiDataPtr)
{
	//**********************
	//long-term change type
	mmiDataPtr->mmiToSysBlock.opMode = mmiDataPtr->opMode;
	mmiDataPtr->mmiToSysBlock.feedOverride = mmiDataPtr->feedOverride;
	mmiDataPtr->mmiToSysBlock.M01 = mmiDataPtr->M01;
	mmiDataPtr->mmiToSysBlock.feedHold = mmiDataPtr->feedHold;
	mmiDataPtr->mmiToSysBlock.singleBlock = mmiDataPtr->singleBlock;
	strcpy_s(mmiDataPtr->mmiToSysBlock.ncFileName, mmiDataPtr->ncFileName);
	mmiDataPtr->mmiToSysBlock.ncFileValid = mmiDataPtr->ncFileValid;

	//**********************
	//short-term change
	//start
	if(mmiDataPtr->startValid == true)
	{
		mmiDataPtr->mmiToSysBlock.start = mmiDataPtr->start;
		mmiDataPtr->mmiToSysBlock.startValid = true;
		mmiDataPtr->startValid = false;
	}
	//continuee
	if(mmiDataPtr->continueValid == true)
	{
		mmiDataPtr->mmiToSysBlock.continuee = mmiDataPtr->continuee;
		mmiDataPtr->mmiToSysBlock.continueValid = true;
		mmiDataPtr->continueValid = false;
	}
	//reset
	if(mmiDataPtr->resetValid == true)
	{
		mmiDataPtr->mmiToSysBlock.reset = mmiDataPtr->reset;
		mmiDataPtr->mmiToSysBlock.resetValid = true;
		mmiDataPtr->resetValid = false;
	}
	//emergencyStop
	if(mmiDataPtr->emergencyStopValid == true)
	{
		mmiDataPtr->mmiToSysBlock.emergencyStop = mmiDataPtr->emergencyStop;
		mmiDataPtr->mmiToSysBlock.emergencyStopValid = true;
		mmiDataPtr->emergencyStopValid = false;
	}
	//JOG/WHEEL mode button

	if(mmiDataPtr->axisButtonValid == true)
	{
		mmiDataPtr->mmiToSysBlock.axisButton = mmiDataPtr->axisButton;
		mmiDataPtr->mmiToSysBlock.axisButtonValid = true;
		mmiDataPtr->axisButtonValid = false;
	}
	if(mmiDataPtr->mouseUpValid == true)
	{
		mmiDataPtr->mmiToSysBlock.mouseUp = mmiDataPtr->mouseUp;
		mmiDataPtr->mmiToSysBlock.mouseUpValid = true;
		mmiDataPtr->mouseUpValid = false;
	}

	//*****************************
	//check formInput so as to transport Block to link
	//*****************************
	if(mmiDataPtr->formInput == true)
	{	//check mmiToSysBlockValid
		if(mmiDataPtr->linkDataPtr->mmiToSysBlockValid == false)//no data in block of link
		{	//transport block from mmi to link
			mmiDataPtr->linkDataPtr->mmiToSysBlock = mmiDataPtr->mmiToSysBlock;
			mmiDataPtr->linkDataPtr->mmiToSysBlockValid = true;

			delByteSpace((char *)&mmiDataPtr->mmiToSysBlock, sizeof(MMI_TO_SYS_BLOCK));//clear block in MMI
			mmiDataPtr->mmiToSysBlock.opMode = mmiDataPtr->linkDataPtr->systemOpMode;
			mmiDataPtr->formInput = false; //resume waiting
		}
	}
}//formToMmiUpdate

// mmiToFormUpdate (mmiInput) //
void mmiToFormUpdate(MMI_DATA *mmiDataPtr)
{
	// intp to sys, valid change here
	/*
	if (mmiDataPtr->linkDataPtr->intpToSysBlockValid == true)
	{
		mmiDataPtr->momtVb = mmiDataPtr->linkDataPtr->intpToSysBlock.momtVb;
		for (int i = 0; i < 3; i++)
		{
			mmiDataPtr->xyzSetValue[i] = mmiDataPtr->linkDataPtr->intpToSysBlock.xyzNew[i];
			mmiDataPtr->uvwSetValue[i] = mmiDataPtr->linkDataPtr->intpToSysBlock.uvwNew[i];
		}
		mmiDataPtr->linkDataPtr->intpToSysBlockValid = false;
	}
	*/

}//mmiToFormUpdate

