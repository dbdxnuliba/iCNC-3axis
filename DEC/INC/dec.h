#pragma once

#pragma once
#include <string.h>
#include <stdio.h>
#include "..\..\LINK\INC\link.h"
#include "..\..\UTIL\INC\util.h"

enum DEC_STATE {Dec_Null = 0, Dec_Idle, Dec_Open_File, Dec_Pre,
	Dec_Line_Loop, Dec_End_Of_Line, Dec_Write_Fifo, Dec_M30_End, Dec_Error };


struct DEC_DATA
{
	LINK_DATA	*linkDataPtr;

	DEC_STATE   decState;
	char        line[500];  // Line read from nc file
							// string end with '\0' or '\n'
	FILE        *filePtr;
	char        *asciiPtr;  // on the first char at the beginning
	bool        markN;// determine whether the lineNumer (N function) is valid 
	bool        markF;// same as above
	bool        markX;
	bool        markY;
	bool		markZ;
	bool		markU;
	bool		markV;
	bool		markW;
	bool		markM;
	bool		markG90G91;

	NC_BLOCK	decToIntpBlock;//for data packing
	bool		decToIntpBlockValid;
//	DEC_TO_INTP_BLOCK *decToIntpBlockPtr; 
	//NC_BLOCK
	int			lineNumber;  // N
	NC_CMD		ncCmd;     //G00, G06, G07, G04, M30
	//MOTION
	double		feedRate;
	bool		feedRateValid;
	bool		relAbs; //G91rel:true G90abs:false 
	bool		M00;
	bool		M01;

	double		xyzCoordinate[3];//0:X 1:Y 2:Z
	bool		xyzValid[3];//
	double		uvwCoordinate[3];//0:U 1:V 2:W
	bool		uvwValid[3];
	//CIRCLE INPUT
	PLANE		plane; //XY, YZ, XZ
	DIRECTION	direction; //CW, CCW
	double		ijkCenter[3];//circle center to x[0]i y[1]j z[2]k
	bool		ijkValid[3];
	//RT_FUNCTION
	double		dwellTime; //dwell time in sec
	char		programName[80];//program end
	int			programNumber;//program end
};//struct DEC_DATA

//functions in decCtl
void decInit(DEC_DATA *decDataPtr);
void decCtl(DEC_DATA *decDataPtr);
void decClose(DEC_DATA *decDataPtr);
void decReset(DEC_DATA *decDataPtr);

//functions in decMain
int decMain(DEC_DATA *decDataPtr);
int decIdle(DEC_DATA *decDataPtr);
int decOpenFile(DEC_DATA *decDataPtr);
int decLineLoop(DEC_DATA *decDataPtr);
int decPre(DEC_DATA *decDataPtr);
int decEndOfLine(DEC_DATA *decDataPtr);
int decWriteFifo(DEC_DATA *decDataPtr);
int decError(DEC_DATA *decDataPtr);
int decM30End(DEC_DATA *decDataPtr);

//functions of decoder
int nFunction(DEC_DATA  *decDataPtr);
int gFunction(DEC_DATA  *decDataPtr);
int fFunction(DEC_DATA  *decDataPtr);
int mFunction(DEC_DATA  *decDataPtr);
int coordFunction(DEC_DATA  *decDataPtr);

int passChar(DEC_DATA  *decDataPtr);
int passSpace(DEC_DATA  *decDataPtr);
int passInt(DEC_DATA  *decDataPtr);
int passFloat(DEC_DATA  *decDataPtr);