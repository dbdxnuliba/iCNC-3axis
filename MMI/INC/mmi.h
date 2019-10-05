#pragma once

#include <string.h>
#include <stdio.h>
#include "..\..\LINK\INC\link.h"
#include "..\..\UTIL\INC\util.h"


enum MMI_STATE {};


struct MMI_DATA
{
	LINK_DATA *linkDataPtr;
	int timerCounter;		//timer counter
	bool formInput;
	// following sync with MMI to SYS block
	OP_MODE opMode;			//operation mode

	char ncFileName[200];	//nc file
	bool ncFileValid;
	double feedOverride;		//0-200 (%)		
	bool M01;
	bool feedHold;
	bool singleBlock;
	double wheelRatio;			//0,10,100

	bool start;
	bool startValid;
	bool continuee;
	bool continueValid;
	bool reset;
	bool resetValid;
	bool teachStart;	//if necessary
	bool teachStartValid;
	bool record;
	bool recordValid;
	bool teachEnd;
	bool teachEndValid;	//if necessary
	bool emergencyStop;
	bool emergencyStopValid;

	int	axisButton;		//1X+ 2X- 3Y+ 4Y- 5Z+ 6Z-
	bool axisButtonValid;
	int uvwButton[3];
	bool mouseUp;
	bool mouseUpValid;
	// above have sync with MMI to SYS block
	
	// above should sync with MMI to SYS block but not yet

	MMI_TO_SYS_BLOCK mmiToSysBlock;
	MMI_TO_SYS_BLOCK *mmiToSysBlockPtr;

//	SYS_TO_MMI_BLOCK mmiToSysBlock;

	double xyzSetValue[3];
	double uvwSetValue[3];
	double momtVb;

};//struct MMI_DATA


//functions in mmiCtl
void mmiInit(MMI_DATA *mmiDataPtr);
void mmiCtl(MMI_DATA *mmiDataPtr);
void mmiClose(MMI_DATA *mmiDataPtr);
void mmiReset(MMI_DATA *mmiDataPtr);


//functions in mmiMain
void mmiMain(MMI_DATA *mmiDataPtr);
void formToMmiUpdate(MMI_DATA *mmiDataPtr);
void mmiToFormUpdate(MMI_DATA *mmiDataPtr);

