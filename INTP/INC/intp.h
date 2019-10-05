#pragma once

#include <stdio.h>
#include <math.h>
#include "..\..\LINK\INC\link.h"
#include "..\..\UTIL\INC\util.h"
//PI, acc, and timeInterval are included in util.h

enum INTP_STATE 
{
	Intp_Null = 0, Intp_Idle, Intp_Pre, Intp_Exe, Intp_Close, Intp_RT_Function,
	Intp_G04_Wait, Intp_Hold, Intp_Reset, Intp_M30_Program_End, Intp_Error
};
enum INTP_RESET_STATE {Intp_Reset_Pre, Intp_Reset_Exe, Intp_Reset_Close};

enum SLOPE_STATE
{
	Slope_Null=0, Slope_Idle, Slope_Acce, Slope_Const, Slope_Peak, Slope_Dece, Slope_End, Slope_Hold
};

enum SLOPE_TYPE { Type_Null=0, Type_One_Step, Type_Const_Feed, Type_Triangle, Type_Trapezoid, Type_Reset };

struct SLOPE_DATA
{
	SLOPE_STATE slopeState;
	SLOPE_TYPE	slopeType;
	double      acceleration;

	double      totalLength; //sum+residual
	double      sumLength; 
	double      residualLength;
	double      extraLength;
	double		feedLength;
	double      peakVb;
	double		extraVb;

	double      feed; // F
	double      momtVb; // velocity

	double		vbStep; // acceleration * cyclingTime 
	double		stepLength; // vbStep* cyclingTime int	acceCounter;
	int         acceCounter; 
	int         deceCounter;
	int			feedCounter;
	int         intpCounter; 
	
	int			acceStep;
	int			deceStep;
	int			feedStep;

	bool		extraValid;
	bool        resetHold;
	bool        intpEnd;
};//struct SLOPE_DATA

struct INTP_DATA
{
	LINK_DATA	*linkDataPtr;

	INTP_STATE	intpState;
	INTP_RESET_STATE intpResetState;

	//data for temporary store
	double	xyzStart[3];
	double	xyzEnd[3];
	double	xyzNew[3];  //每一段算出來要走的點
	double	xyzOld[3];
	double	xyzRelative[3];
	double	linearFactor[3];
	double	deltaLength;
	double	ijkCenter[3];
	double	uvwStart[3];
	double	uvwEnd[3];
	double	uvwNew[3];
	double	uvwOld[3];
	double	uvwRelative[3];

	int		uvwIncrement[3];
	double	uvwResolution[3];
	
	int		g04Counter;
	double	dwellTime;
	double	cyclingTime;
	double	g00Speed;
	bool	m30ProgramEnd;

	double	l1,	l2; //桿長

	NC_BLOCK	ncBlock;

	SYS_TO_INTP_DATA sysToIntpBlock;


	SLOPE_DATA	slopeData;
	SLOPE_DATA	*slopeDataPtr;

};//struct INTP_DATA

//functions in intpCtl
void intpInit(INTP_DATA *intpDataPtr);
void intpCtl(INTP_DATA *intpDataPtr);
void intpReset(INTP_DATA *intpDataPtr);
void intpClose(INTP_DATA *intpDataPtr);

//functions in intpMain
void intpMain(INTP_DATA *intpDataPtr);
void intpIdle(INTP_DATA *intpDataPtr);
int intpPre(INTP_DATA* intpDataPtr);
int intpExe(INTP_DATA* intpDataPtr);
int intpRtFunction(INTP_DATA* intpDataPtr);
int intpG04Wait(INTP_DATA* intpDataPtr);
int intpM30ProgramEnd(INTP_DATA* intpDataPtr);
int intpError(INTP_DATA* intpDataPtr);

//coordinate transform
void forwardKinematicTransform(INTP_DATA *intpDataPtr);
void inverseKinematicTransform(INTP_DATA *intpDataPtr);

//functions in slope
int slopeInit(SLOPE_DATA *slopeDataPtr);
void feedScheduling(INTP_DATA *intpDataPtr);
void slope(SLOPE_DATA *slopeDataPtr);

void slopeAcce(SLOPE_DATA *slopeDataPtr);
void slopeConst(SLOPE_DATA *slopeDataPtr);
void slopePeak(SLOPE_DATA *slopeDataPtr);
void slopeDece(SLOPE_DATA *slopeDataPtr);
void slopeEnd(SLOPE_DATA *slopeDataPtr);
void slopeHold(SLOPE_DATA *slopeDataPtr);

//functions for INTP RESET
int intpResetPre(INTP_DATA* intpDataPtr);
int intpResetExe(INTP_DATA* intpDataPtr);
int intpResetClose(INTP_DATA* intpDataPtr);
