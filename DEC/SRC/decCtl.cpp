#pragma once
#include <StdAfx.h>
#include <stdio.h>
#include "..\INC\dec.h"


void decInit(DEC_DATA *decDataPtr)
{
//	delByteSpace((char *)decDataPtr, sizeof(DEC_DATA));//checking char from char; DEC_DATA's size
	decDataPtr->decState = Dec_Idle;
	for (int i = 0; i < 500; i++)
	{
		decDataPtr->line[i] = NULL;
	}
	decDataPtr->filePtr = NULL;
	decDataPtr->asciiPtr = decDataPtr->line;
	decDataPtr->markN = false;
	decDataPtr->markF = false;
	decDataPtr->markX = false;
	decDataPtr->markY = false;
	decDataPtr->markZ = false;
	decDataPtr->markU = false;
	decDataPtr->markV = false;
	decDataPtr->markW = false;
	decDataPtr->markM = false;
	decDataPtr->markG90G91 = false;

	//NC_BLOCK
	decDataPtr->decToIntpBlockValid = false;

	decDataPtr->lineNumber = 0;
	decDataPtr->ncCmd = Null_Cmd;

	decDataPtr->feedRate = 0;
	decDataPtr->feedRateValid = false;
	decDataPtr->relAbs = false; //G91rel:true G90abs:false 
	decDataPtr->M00 = false;
	decDataPtr->M01 = false;
	for(int j = 0; j < 3; j++)
	{
		decDataPtr->xyzCoordinate[j] = 0;//0:X 1:Y 2:Z
		decDataPtr->xyzValid[j] = false;//
		decDataPtr->uvwCoordinate[j] = 0;//0:U 1:V 2:W
		decDataPtr->uvwValid[j] = false;
		decDataPtr->ijkCenter[j] = 0;//circle center to x[0]i y[1]j z[2]k
		decDataPtr->ijkValid[j] = false;
	}
	decDataPtr->plane = XY; //XY, YZ, XZ
	decDataPtr->direction = CW; //CW, CCW

	//RT_FUNCTION
	decDataPtr->dwellTime = 0; //dwell time in sec
	for (int i = 0; i < 80; i++)
	{
		decDataPtr->programName[80] = NULL;//program end
	}

	decDataPtr->programNumber = 0;//program end

	initFifo(&decDataPtr->linkDataPtr->decToIntpFifo.fifoHead, DEC_TO_INTP);
	delByteSpace((char *)&decDataPtr->decToIntpBlock, sizeof(NC_BLOCK));

	return;
} //decInit




void decCtl(DEC_DATA *decDataPtr)
{
	if (decDataPtr->linkDataPtr->systemReset == true)
		decReset(decDataPtr);
	else
		decMain(decDataPtr);
	return;
} //decCtl()

void decReset(DEC_DATA *decDataPtr)
{
	if (decDataPtr->linkDataPtr->decResetValid == false)
	{
		if (decDataPtr->filePtr != NULL)
			fclose(decDataPtr->filePtr);

		decInit(decDataPtr);
		decDataPtr->linkDataPtr->decResetValid = true;
	}
	return;
} //decReset()

void decClose(DEC_DATA *decDataPtr)
{

} //decClose
