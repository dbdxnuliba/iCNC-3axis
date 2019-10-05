#pragma once
#include <StdAfx.h>
#include <stdio.h>
#include "..\..\UTIL\INC\util.h"
#include "..\INC\link.h"

int linkInit(LINK_DATA  *linkDataPtr)
{
//	delByteSpace((char *)linkDataPtr, sizeof(LINK_DATA));//checking char from char; DEC_DATA's size //ALREADY CLEARED EVERYTHING IN LINKDATA
	linkDataPtr->M30ProgramEnd = false;

	linkDataPtr->systemReset = false;
	linkDataPtr->decResetValid = false;
	linkDataPtr->intpResetValid = false;
	linkDataPtr->mmiResetValid = false;


	linkDataPtr->mmiToSysBlockValid = false;
	linkDataPtr->sysToDecBlockValid = false;
	linkDataPtr->mmiToSysBlockValid = false;
	linkDataPtr->intpToDecBlockValid = false;
	linkDataPtr->intpToSysBlockValid = false;

	delByteSpace((char *)&linkDataPtr->errorData, sizeof(ERROR_DATA));
//	linkDataPtr->mmiToSysBlockPtr = &linkDataPtr->mmiToSysBlock;
//	linkDataPtr->decToIntpBlockPtr = &linkDataPtr->decToIntpBlock;
//	linkDataPtr->intpToFormBlockPtr = &linkDataPtr->intpToFormBlock;
//	linkDataPtr->intpToDecBlockPtr = &linkDataPtr->intpToDecBlock;
//	linkDataPtr->formToIntpBlockPtr = &linkDataPtr->formToIntpBlock;

   return 1;
} //linkInit()