#include "StdAfx.h"
#include <stdio.h>
#include "..\INC\cnc.h"


// cncInit //
int cncInit(CNC_DATA *cncDataPtr)
{
	cncDataPtr->mmiDataPtr = &cncDataPtr->mmiData;
	cncDataPtr->decDataPtr = &cncDataPtr->decData;
	cncDataPtr->intpDataPtr = &cncDataPtr->intpData;
	cncDataPtr->sysDataPtr = &cncDataPtr->sysData;
	cncDataPtr->linkDataPtr = &cncDataPtr->linkData;

	cncDataPtr->mmiDataPtr->linkDataPtr = &cncDataPtr->linkData;
	cncDataPtr->sysDataPtr->linkDataPtr = &cncDataPtr->linkData;
	cncDataPtr->decDataPtr->linkDataPtr = &cncDataPtr->linkData;
	cncDataPtr->intpDataPtr->linkDataPtr = &cncDataPtr->linkData;

	mmiInit(cncDataPtr->mmiDataPtr);
	decInit(cncDataPtr->decDataPtr);
	intpInit(cncDataPtr->intpDataPtr);
	sysInit(cncDataPtr->sysDataPtr);
	linkInit(cncDataPtr->linkDataPtr);
	
	return 0;
}//cncInit

int cncNrtCtl(CNC_DATA *cncDataPtr)
{	
	mmiCtl(cncDataPtr->mmiDataPtr);
	sysCtl(cncDataPtr->sysDataPtr);
	decCtl(cncDataPtr->decDataPtr);
	return 0;
} // cncNrtCtl()

int cncRtCtl(CNC_DATA *cncDataPtr)
{

	intpCtl(cncDataPtr->intpDataPtr);
//	pclCtl(cncDataPtr->pclDataPtr);
	return 0;
} // cncRtCtl()

