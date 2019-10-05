#pragma once
#include <stdio.h>
#include <string.h>
#include "..\..\LINK\INC\link.h"
#include "..\..\UTIL\INC\util.h"

enum SYS_STATE {Sys_Null=0, Sys_Run, Sys_Reset};
enum SYS_RESET {Reset_Null=0, Reset_1, Reset_2};

struct SYS_DATA
{
	LINK_DATA *linkDataPtr;
	SYS_STATE sysState;
	SYS_RESET sysResetState;

	OP_MODE OpMode;
	OP_MODE newOpMode;
	char	ncFileName[200];
	bool	ncFileValid;

	//input
	MMI_TO_SYS_BLOCK mmiToSysBlock;
	bool			 mmiToSysBlockValid;

	INTP_TO_SYS_BLOCK intpToSysBlock;
	bool			  intpToSysBlockValid;

	//output
	SYS_TO_MMI_BLOCK sysToMmiBlock;
	bool			 sysToMmiBlockValid;

	SYS_TO_DEC_BLOCK sysToDecBlock;
	bool			 sysToDecBlockValid;

	SYS_TO_INTP_DATA sysToIntpBlock;
	bool			  sysToIntpBlockValid;
};// struct SYS_DATA

//functions in sysCtl
void sysInit(SYS_DATA *sysDataPtr);
void sysCtl(SYS_DATA *sysDataPtr);
void sysReset(SYS_DATA *sysDataPtr);
void sysClose(SYS_DATA *sysDataPtr);

//functions in sysMain
void sysMain(SYS_DATA *sysDataPtr);
void sysInput(SYS_DATA *sysDataPtr);
void sysOutput(SYS_DATA *sysDataPtr);

//functions in sysOutput
void sysOpMEM(SYS_DATA *sysDataPtr);