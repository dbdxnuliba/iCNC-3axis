#pragma once

#include <string.h>
#include <stdio.h>

enum OP_MODE { MEM_Mode, MDI_Mode, JOG_Mode, WHEEL_Mode, TEACH_Mode, HOME_Mode };
//enum OP_MODE
enum NC_CMD { Null_Cmd = 0, G00_Motion, G01_Motion, G02_Motion, G03_Motion, Function_Cmd, G04_Dwell_Time, M30_End};
//enum NC_CMD

struct MMI_TO_SYS_BLOCK
{
	OP_MODE opMode;			//operation mode
	bool	opModeValid;	

	char	ncFileName[200];	//nc file path
	bool	ncFileValid;

	double	feedOverride;		//0-200%			
	bool	M01;
	bool	feedHold;
	bool	singleBlock;
	double	wheelRatio;

	bool	start;
	bool	startValid;
	bool	continuee;
	bool	continueValid;
	bool	reset;
	bool	resetValid;
	bool	teachStart;
	bool	teachStartValid;
	bool	record;
	bool	recordValid;
	bool	teachEnd;
	bool	teachEndValid;
	bool	emergencyStop;
	bool	emergencyStopValid;

	int		axisButton;		//[0]X+ [1]X- [2]Y+ [3]Y- [4]Z+ [5]Z-
	bool	axisButtonValid;
	bool	mouseUp;
	bool	mouseUpValid;
};//struct MMI TO SYS BLOCK

struct SYS_TO_MMI_BLOCK
{
	double	momtVb;
	double	xyzNew[3];
	double	uvwNew[3];
	double  totalLength, sumLength, residualLength;
	int		uvwIncrement[3];
//	bool	axisButtonValid[3];
};//struct SYS TO MMI BLOCK

struct SYS_TO_DEC_BLOCK
{
	char	ncFileName[200];
	bool	ncFileValid;

};//struct SYS_TO_DEC_BLOCK


struct INTP_TO_DEC_BLOCK
{
	bool	cmdValid;
	bool	m30End;
};//struct INTP TO DEC BLOCK

struct INTP_TO_SYS_BLOCK
{
	double	momtVb;
	double	xyzNew[3];
	double	uvwNew[3];
	double  totalLength, sumLength, residualLength;
	int		uvwIncrement[3];
//	bool	axisButtonValid[3];
};//struct INTP TO DEC BLOCK


struct ERROR_DATA
{
	int decLineNumber;
};

//**********************************************************
//DEC TO INTP DATA TRANSFORMATION
/****************************************Structure***********************************************
DEC_TO_INTP_FIFO -> NC_BLOCK  -> MOTION_FUNCTION(u) -> MOTION -> CIRCLE_INPUT & other elements

					Valid		 ncCmd,lineNumber	   RT_FUNCTION(u)-> PROGRAM_END & DWELL_TIME

		 (put ahead)FIFO_HEAD -> rdIndex, wrIndex... 	
************************************************************************************************/
enum FIFO_TYPE {DEC_TO_INTP, SYS_TO_INTP, OTHER_BLOCK};

struct FIFO_HEAD
{
	int wrIndex;
	int rdIndex;
//	int fifoLength;

	int sizeInBlock;
	int sizeInByte;

	FIFO_TYPE fifoType; //enum
};//struct FIFO_HEAD

struct PROGRAM_END
{
	int		programNumber;
	char	programName[80];

};// struct PROGRAM_END (M30)  ///NC_BLOCK

struct DWELL_TIME
{
	double value; //time in sec
};// struct DWELL_TIME ///NC_BLOCK

union RT_FUNCTION
{
	PROGRAM_END	programEnd;
	DWELL_TIME	dwellTime;
};//union RT_FUNCTION ///NC_BLOCK

enum PLANE { XY, YZ, XZ };
enum DIRECTION { CW, CCW };

struct CIRCLE_INPUT
{
	PLANE		plane; //XY, YZ, XZ
	DIRECTION	direction; //CW, CCW
	double		ijkCenter[3];//circle center to x[0]i y[1]j z[2]k
	bool		ijkValid[3]; //[0]i [1]j [2]k
};//struct CIRCLE_INPUT ///NC_BLOCK

struct MOTION
{
	double	feedRate;
	bool	feedRateValid;
	bool	relAbs; //G90abs:false G91rel:true
	bool	M00;
	bool	M01;
	
	double	xyzCoordinate[3];
	bool	xyzValid[3];
	double	uvwCoordinate[3];
	bool	uvwValid[3];
	CIRCLE_INPUT	circleInput;
};//struct MOTION ///NC_BLOCK

union MOTION_FUNCTION
{
	MOTION		motion;
	RT_FUNCTION	rtFunction;
};//struct MOTION_FUNCTION ///NC_BLOCK

struct NC_BLOCK
{
	MOTION_FUNCTION motionFunction;
	NC_CMD	ncCmd;
	int		lineNumber;
};//struct NC_block (orig. DEC_TO_INTP_BLOCK)


#define DEC_TO_INTP_FIFO_LENGTH 100
#define SYS_TO_INTP_FIFO_LENGTH 20

struct DEC_TO_INTP_FIFO
{
	FIFO_HEAD	fifoHead; // inheritance OOP for C
	NC_BLOCK	ncBlockInfoHolder[DEC_TO_INTP_FIFO_LENGTH];
	bool		ncBlockValid[DEC_TO_INTP_FIFO_LENGTH];
};//struct DEC_TO_INTP_FIFO

//**********************************************************
//SYS TO INTP DATA
//**********************************************************
struct SYS_TO_INTP_DATA
{
	double	feedOverride;		//0-200%			
	//for MEM / MDI
	bool	M01;
	bool	feedHold;
	bool	singleBlock;
	//for JOG / WHEEl
	int		axisButton;
	double	wheelRatio;
	bool	continuee;
	bool	continueValid;

};//struct SYS_TO_INTP_BLOCK

struct SYS_TO_INTP_FIFO
{
	FIFO_HEAD			fifoHead; // inheritance OOP for C
	SYS_TO_INTP_DATA	sysDataInfoHolder[DEC_TO_INTP_FIFO_LENGTH];
	bool				sysDataValid[DEC_TO_INTP_FIFO_LENGTH];
};//struct DEC_TO_INTP_FIFO

//**********************************************************
//OTHER BLOCK DATA TRANSFORMATION
//**********************************************************
struct OTHER_BLOCK_TRANSFORM
{
	int sthInThisBlock;


};//struct OTHER_BLOCK_TRANSFORM

#define OTHER_FIFO_LENGTH 100

struct OTHER_FIFO
{
	FIFO_HEAD			fifoHead; // inheritance OOP for C
	OTHER_BLOCK_TRANSFORM	otherBlockInfoHolder[OTHER_FIFO_LENGTH];
	bool				otherBlockValid[OTHER_FIFO_LENGTH];
};//struct OTHER_FIFO

//**********************************************************
//LINK_DATA
//**********************************************************

struct LINK_DATA
{
	int		linkState;
	OP_MODE	systemOpMode;
	bool	M30ProgramEnd;

	bool	systemReset;
	bool	decResetValid;
	bool	intpResetValid;
	bool	mmiResetValid;
//	bool	linkResetValid;//?
//	bool	pclResetValid;
	
	MMI_TO_SYS_BLOCK	mmiToSysBlock;
	bool				mmiToSysBlockValid;

	SYS_TO_DEC_BLOCK	sysToDecBlock;
	bool				sysToDecBlockValid;
	
	SYS_TO_MMI_BLOCK	sysToMmiBlock;
	bool				sysToMmiBlockValid;

	INTP_TO_DEC_BLOCK	intpToDecBlock;
	bool				intpToDecBlockValid;

	INTP_TO_SYS_BLOCK	intpToSysBlock;
	bool				intpToSysBlockValid;

	// the following are FIFOs
	NC_BLOCK			decToIntpBlock; //for data packing

	DEC_TO_INTP_FIFO	decToIntpFifo;

	SYS_TO_INTP_FIFO	sysToIntpFifo;

	SYS_TO_INTP_DATA	sysToIntpBlock; //for data packing
	bool				sysToIntpBlockValid;

	ERROR_DATA			errorData;
/*
	FORM_TO_INTP_BLOCK *formToIntpBlockPtr;
	FORM_TO_INTP_BLOCK  formToIntpBlock;
*/

};//struct LINK_DATA

int linkInit(LINK_DATA  *linkDataPtr);

