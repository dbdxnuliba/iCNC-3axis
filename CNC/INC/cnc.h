#pragma once
#include <string.h>
#include <stdio.h>
#include "..\..\MMI\INC\mmi.h"
#include "..\..\DEC\INC\dec.h"
#include "..\..\INTP\INC\intp.h"

#include "..\..\SYS\INC\sys.h"
#include "..\..\LINK\INC\link.h"
#include "..\..\UTIL\INC\util.h"

struct CNC_DATA
{
	int cncState;

	MMI_DATA	mmiData;
	MMI_DATA	*mmiDataPtr;
	
	DEC_DATA	decData;		
	DEC_DATA	*decDataPtr;	

	LINK_DATA	linkData;
	LINK_DATA	*linkDataPtr;	

	SYS_DATA	sysData;
	SYS_DATA	*sysDataPtr;

	INTP_DATA	intpData;
	INTP_DATA	*intpDataPtr;

/*
	PCL_DATA	pclData;
	PCL_DATA	*pclDataPtr;
*/


};//struct CNC_DATA

int cncInit(CNC_DATA *cncDataPtr);
int cncNrtCtl(CNC_DATA *cncDataPtr);
int cncRtCtl(CNC_DATA *cncDataPtr);
int cncClose(CNC_DATA *cncDataPtr);