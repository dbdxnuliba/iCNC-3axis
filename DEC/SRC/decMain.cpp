#pragma once
#include <StdAfx.h>
#include <stdio.h>
#include <stdlib.h>
#include "..\INC\dec.h"

int decMain(DEC_DATA *decDataPtr)
{
	
	switch (decDataPtr->decState)
	{
		case Dec_Idle:
			decIdle(decDataPtr);
			break;
		case Dec_Open_File:
			decOpenFile(decDataPtr);
			break;
		case Dec_Pre:
			decPre(decDataPtr);
			break;
		case Dec_Line_Loop:
			decLineLoop(decDataPtr);
			break;
		case Dec_End_Of_Line:
			decEndOfLine(decDataPtr);
			break;
		case Dec_Write_Fifo:
			decWriteFifo(decDataPtr);
			break;
		case Dec_M30_End:
			decM30End(decDataPtr);
			break;
		case Dec_Error:
			decDataPtr->linkDataPtr->errorData.decLineNumber = decError(decDataPtr);
			break;
	
	}
	
	return 1;
}//decMain

//**********************************************************
//decMain functions
//**********************************************************
int decIdle(DEC_DATA  *decDataPtr)
{  // check if order comes,
   // check data in LINK_DATA if order comes
	//   data exchange between module A and B
	//   module A check the bool variable
	//         if false, write the data, set the bool variable
	//   module B check the bool variable, 
	//         if true, read data, clear the bool variable

		// MEM/MDI mode, and get ncFile successful 
	if (decDataPtr->linkDataPtr->sysToDecBlockValid == true 
		&& decDataPtr->linkDataPtr->sysToDecBlock.ncFileValid == true
		&& (decDataPtr->linkDataPtr->systemOpMode == MEM_Mode || decDataPtr->linkDataPtr->systemOpMode == MDI_Mode
		 || decDataPtr->linkDataPtr->systemOpMode == HOME_Mode || decDataPtr->linkDataPtr->systemOpMode == JOG_Mode 
		 || decDataPtr->linkDataPtr->systemOpMode == WHEEL_Mode )) 
	{
	   decDataPtr->decState = Dec_Open_File;
	   decDataPtr->linkDataPtr->sysToIntpBlockValid = false;
	}
	else
	{
	   decDataPtr->decState = Dec_Idle;
	}
   return 1;
} // decIdle()

int decOpenFile(DEC_DATA  *decDataPtr)
{	// open NC file, if success, 
	// open for read/ "w" for write, "r" 這種型別用以讀取文字檔
	fopen_s(&decDataPtr->filePtr, decDataPtr->linkDataPtr->sysToDecBlock.ncFileName,"r");
	
	if (decDataPtr->filePtr == NULL)
	{
		decDataPtr->decState = Dec_Error;
		return -1;
	}
	else
	{  // open file succeed
		decDataPtr->decState = Dec_Pre;
		decDataPtr->linkDataPtr->sysToDecBlock.ncFileValid = false;  // data exchange finish, clear for next exchange
		decDataPtr->linkDataPtr->sysToDecBlockValid = false; // BlockValid clear
		return 1;
	}
	// change to state DEC_PRE for preperation of NC file

  
} // decOpenFile()


int decPre(DEC_DATA  *decDataPtr)
{
	// start to process the NC file line by line
	if ( fgets(decDataPtr->line, 200, decDataPtr->filePtr) == NULL)
	{
		decDataPtr->decState = Dec_Error;

		return -1;
	}
	else
	{
		decDataPtr->decState = Dec_Line_Loop;	
		decDataPtr->asciiPtr = decDataPtr->line;  // point at the first char
		return 1;
	}
	
   
} // decPre()

int decLineLoop(DEC_DATA  *decDataPtr)
{

	bool    loop = true;
	for ( ; loop == true;)
	{
		switch (*decDataPtr->asciiPtr)
		{
		case ' ':
			decDataPtr->asciiPtr++;
			break;
		case 'N':
			nFunction(decDataPtr);
			break;	  
		case 'G':
			gFunction(decDataPtr);
			break;
		case 'F':
			fFunction(decDataPtr);
			break;
		case 'X':
		case 'Y':
		case 'Z':
		case 'U':
		case 'V':
		case 'W':
		case 'I':
		case 'J':
		case 'K':
			coordFunction(decDataPtr);
			break;
		case 'M': // pause of motion or end of file
			mFunction(decDataPtr);

			break;
		case '\0':  // end of string/line
		case '\n':
			decDataPtr->asciiPtr++;
			loop = false;
			decDataPtr->decState = Dec_End_Of_Line;
			break;
		default:
			decDataPtr->decState = Dec_Error;
			loop = false;
			break;
		}//switch
	
	}//for

	return 1;
} // decLoop()


int decEndOfLine(DEC_DATA *decDataPtr)
{	
	decDataPtr->decToIntpBlock.lineNumber = decDataPtr->lineNumber;
	if (decDataPtr->ncCmd == Null_Cmd  && decDataPtr->markM == true 
		&& (decDataPtr->M00 == true || decDataPtr->M01 == true))
	{	// M01 or M00 single appearance
		decDataPtr->ncCmd = Function_Cmd;
	}
	decDataPtr->decToIntpBlock.ncCmd = decDataPtr->ncCmd;
	switch(decDataPtr->decToIntpBlock.ncCmd)
	{	
	case Null_Cmd:
		decDataPtr->decState = Dec_Error;
		break;
	case G00_Motion:
	case G01_Motion:
		decDataPtr->decToIntpBlock.motionFunction.motion.relAbs = decDataPtr->relAbs;
		decDataPtr->decToIntpBlock.motionFunction.motion.M00 = decDataPtr->M00;
		decDataPtr->decToIntpBlock.motionFunction.motion.M01 = decDataPtr->M01;
		if (decDataPtr->feedRateValid == true)
		{
			decDataPtr->decToIntpBlock.motionFunction.motion.feedRate = decDataPtr->feedRate;
		}
		decDataPtr->decToIntpBlock.motionFunction.motion.feedRateValid = decDataPtr->feedRateValid;
		for(int i=0; i<3; i++)
		{
			if(decDataPtr->xyzValid[i] == true)
			{
				decDataPtr->decToIntpBlock.motionFunction.motion.xyzCoordinate[i] = decDataPtr->xyzCoordinate[i];
			}	
			decDataPtr->decToIntpBlock.motionFunction.motion.xyzValid[i] = decDataPtr->xyzValid[i];

			if(decDataPtr->uvwValid[i] == true)
			{
				decDataPtr->decToIntpBlock.motionFunction.motion.uvwCoordinate[i] = decDataPtr->uvwCoordinate[i];
			}
			decDataPtr->decToIntpBlock.motionFunction.motion.uvwValid[i] = decDataPtr->uvwValid[i];
		}
		decDataPtr->decToIntpBlockValid = true;
		break;
	case G02_Motion:
	case G03_Motion:
		decDataPtr->decToIntpBlock.motionFunction.motion.relAbs = decDataPtr->relAbs;
		decDataPtr->decToIntpBlock.motionFunction.motion.M00 = decDataPtr->M00;
		decDataPtr->decToIntpBlock.motionFunction.motion.M01 = decDataPtr->M01;
		decDataPtr->decToIntpBlock.motionFunction.motion.feedRate = decDataPtr->feedRate;
		decDataPtr->decToIntpBlock.motionFunction.motion.feedRateValid = decDataPtr->feedRateValid;
		for(int i=0; i<3; i++)
		{
			if(decDataPtr->xyzValid[i] == true)
			{
				decDataPtr->decToIntpBlock.motionFunction.motion.xyzCoordinate[i] = decDataPtr->xyzCoordinate[i];
			}	
			decDataPtr->decToIntpBlock.motionFunction.motion.xyzValid[i] = decDataPtr->xyzValid[i];

			if(decDataPtr->uvwValid[i] == true)
			{
				decDataPtr->decToIntpBlock.motionFunction.motion.uvwCoordinate[i] = decDataPtr->uvwCoordinate[i];
			}
			decDataPtr->decToIntpBlock.motionFunction.motion.uvwValid[i] = decDataPtr->uvwValid[i];

			if(decDataPtr->ijkValid[i] == true)
			{
				decDataPtr->decToIntpBlock.motionFunction.motion.circleInput.ijkCenter[i] = decDataPtr->ijkCenter[i];
				decDataPtr->decToIntpBlock.motionFunction.motion.circleInput.ijkValid[i] = decDataPtr->ijkValid[i];
			}
		}
		decDataPtr->decToIntpBlockValid = true;
		break;

	case G04_Dwell_Time:
		decDataPtr->decToIntpBlock.motionFunction.rtFunction.dwellTime.value = decDataPtr->xyzCoordinate[0]; //X__
		decDataPtr->decToIntpBlockValid = true;
		break;
	case Function_Cmd: //for M00,M01 single appearance
		decDataPtr->decToIntpBlock.motionFunction.motion.M00 = decDataPtr->M00;
		decDataPtr->decToIntpBlock.motionFunction.motion.M01 = decDataPtr->M01;
//		decDataPtr->decToIntpBlock.motionFunction.motion.XXX = decDataPtr->XXX;
		decDataPtr->decToIntpBlockValid = true;
		break;
	case M30_End:
		decDataPtr->decToIntpBlock.motionFunction.rtFunction.programEnd.programNumber = decDataPtr->programNumber;
		break;
	default:
		break;
	}
	decDataPtr->decState = Dec_Write_Fifo;
	return 1;
} // decEndOfLine()


int decWriteFifo(DEC_DATA *decDataPtr)
{
	//writing, check if succeeded (true)
	if (writeFifo(&decDataPtr->linkDataPtr->decToIntpFifo.fifoHead, &decDataPtr->decToIntpBlock) == true) 
	{
		//after writing check if M30
		if(decDataPtr->ncCmd == M30_End)
		{
//			delByteSpace((char *)decDataPtr, sizeof(DEC_DATA));
			decDataPtr->decState = Dec_M30_End;
		}
		else
		{	//clear data to prepare for decoding next line
			char lineTemp[500];
			FILE *filePtrTemp;
			DEC_TO_INTP_FIFO decToIntpFifoTemp;
			
			strcpy_s(lineTemp, decDataPtr->line);
			filePtrTemp = decDataPtr->filePtr;
			decToIntpFifoTemp = decDataPtr->linkDataPtr->decToIntpFifo;
			
			//clear dec data except filePtr, line[] & decToIntpFifo
			decInit(decDataPtr);

			strcpy_s(decDataPtr->line, lineTemp);
			decDataPtr->filePtr = filePtrTemp;
			decDataPtr->linkDataPtr->decToIntpFifo = decToIntpFifoTemp;
			decDataPtr->decState = Dec_Pre;
		}
	}
	else	//possibly FIFO full
	{ 
		decDataPtr->decState = Dec_Write_Fifo;
		//keep waiting for writing
		
	}

	return 1;
}//int decWriteFifo()


int decM30End(DEC_DATA   *decDataPtr)
{	//wait for intp end
	fclose(decDataPtr->filePtr);
	if (decDataPtr->linkDataPtr->M30ProgramEnd == true)
	{
		decInit(decDataPtr);
		decDataPtr->decState = Dec_Idle;
	}
	else
		decDataPtr->decState = Dec_M30_End;
	return 1;
}//decM30End()


int decError(DEC_DATA  *decDataPtr)
{
	return decDataPtr->lineNumber;
} // decError()

//**********************************************************
//functions of decoder
//**********************************************************
int nFunction(DEC_DATA  *decDataPtr)
{     
	decDataPtr->markN = true;
	passChar(decDataPtr);  
	passSpace(decDataPtr);

	decDataPtr->lineNumber = atoi(decDataPtr->asciiPtr);

	passInt(decDataPtr);
	passSpace(decDataPtr);

	return decDataPtr->lineNumber;
} // nFunction()
/* 
1. skip letters if meeting letters 
2. skip spaces if meeting spaces 
3. store the number after that into lineNumber(atoi does not move ascii) 
4. since ascii still stay at the first char in the number, use function to pass it.
*/

int gFunction(DEC_DATA  *decDataPtr)
{	//Only when ncCmd = Null_Cmd can G01 02 03 04 work
	//Use markG90G91 to check G90 91 once only
	int      gCmd;
	passChar(decDataPtr);
	passSpace(decDataPtr);
	gCmd = atoi(decDataPtr->asciiPtr);
	switch (gCmd)
	{
	case 00:
		if(decDataPtr->ncCmd == Null_Cmd)
		{
			decDataPtr->ncCmd = G00_Motion;//rapid move
			passInt(decDataPtr);
			passSpace(decDataPtr);
			decDataPtr->markF = true;
		}
		else decDataPtr->decState = Dec_Error;
		break;
	case 01:
		if(decDataPtr->ncCmd == Null_Cmd)
		{
			decDataPtr->ncCmd = G01_Motion;//linear move
			passInt(decDataPtr);
			passSpace(decDataPtr);
		}
		else decDataPtr->decState = Dec_Error;
		break;
	case 02: 
		if(decDataPtr->ncCmd == Null_Cmd)
		{
			decDataPtr->ncCmd = G02_Motion;//CW
			decDataPtr->direction = CW;
			passInt(decDataPtr);
			passSpace(decDataPtr);
		}
		else decDataPtr->decState = Dec_Error;
		break;
	case 03: 
		if(decDataPtr->ncCmd == Null_Cmd)
		{
			decDataPtr->ncCmd = G03_Motion;//CCW
			decDataPtr->direction = CCW;
			passInt(decDataPtr);
			passSpace(decDataPtr);
		}
		else decDataPtr->decState = Dec_Error;
		break;
	case 04:
		if(decDataPtr->ncCmd == Null_Cmd)
		{
			decDataPtr->ncCmd = G04_Dwell_Time;// pause time

			passInt(decDataPtr);
			passSpace(decDataPtr);
			decDataPtr->dwellTime = atoi(decDataPtr->asciiPtr); //ex: G04 1000， pointer is at 1
		}
		else decDataPtr->decState = Dec_Error;
		break;

	case 17:
		if(decDataPtr->ncCmd == G02_Motion ||decDataPtr->ncCmd == G03_Motion)
		{
			decDataPtr->plane = XY;
			passInt(decDataPtr);
			passSpace(decDataPtr);
		}
		else
		{
			decDataPtr->decState = Dec_Error;
		}
		break;
	case 18:
		if(decDataPtr->ncCmd == G02_Motion || decDataPtr->ncCmd == G03_Motion)
		{
			decDataPtr->plane = YZ;
			passInt(decDataPtr);
			passSpace(decDataPtr);
		}
		else
		{
			decDataPtr->decState = Dec_Error;
		}
		break;
	case 19:
		if(decDataPtr->ncCmd == G02_Motion || decDataPtr->ncCmd == G03_Motion)
		{
			decDataPtr->plane = XZ;
			passInt(decDataPtr);
			passSpace(decDataPtr);
		}
		else
		{
			decDataPtr->decState = Dec_Error;
		}
		break;

	case 90: // G90 relAbs = false; absolute
		if(decDataPtr->markG90G91 == true)
		{
			decDataPtr->decState = Dec_Error;
		}
		else //OK
		{
			decDataPtr->markG90G91 = true;
			decDataPtr->relAbs = false; //G90 abs
			passInt(decDataPtr);
			passSpace(decDataPtr);
		}
		break;

	case 91: // G91 relAbs= true; relative
		if(decDataPtr->markG90G91 == true)
		{
			decDataPtr->decState = Dec_Error;
		}
		else //OK
		{
			decDataPtr->markG90G91 = true;
			decDataPtr->relAbs = true; //G91 rel
			passInt(decDataPtr);
			passSpace(decDataPtr);
		}		
		break;


	default:
		decDataPtr->decState = Dec_Error;
		passChar(decDataPtr);
		passSpace(decDataPtr);
		passFloat(decDataPtr);
		passSpace(decDataPtr);
		break;
	}//switch

	passInt(decDataPtr);
	passSpace(decDataPtr);
	return 1;
} // gFunction() waiting for revise


int fFunction(DEC_DATA  *decDataPtr)
{
	if(decDataPtr->markF == true)
	{
		decDataPtr->decState = Dec_Error;
		passChar(decDataPtr);
		passSpace(decDataPtr);
		passFloat(decDataPtr);
		passSpace(decDataPtr);

	}
	else //OK
	{
		decDataPtr->markF = true;  
		passChar(decDataPtr);
		passSpace(decDataPtr);
		decDataPtr->feedRate = atof(decDataPtr->asciiPtr);
		decDataPtr->feedRateValid = true;
		passFloat(decDataPtr);
		passSpace(decDataPtr);
	}
	return 1;
} // fFunction()
//用來讀取浮點數的speed


int coordFunction(DEC_DATA  *decDataPtr)
{     // pass the first char 'X or Y'

	switch(*decDataPtr->asciiPtr)
	{
	case 'X':
		if (decDataPtr->markX == true)
		{
			decDataPtr->decState = Dec_Error;
			passChar(decDataPtr);
			passSpace(decDataPtr);
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		else
		{
			decDataPtr->markX = true;
			passChar(decDataPtr);
		    passSpace(decDataPtr);
			decDataPtr-> xyzCoordinate[0] = atof(decDataPtr->asciiPtr); //[0] x
			decDataPtr-> xyzValid[0] = true;
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		break;
	case 'Y':
		if (decDataPtr->markY == true)
		{
			decDataPtr->decState = Dec_Error;
			passChar(decDataPtr);
			passSpace(decDataPtr);
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		else
		{
			decDataPtr->markY = true;
			passChar(decDataPtr);
			passSpace(decDataPtr);
			decDataPtr-> xyzCoordinate[1] = atof(decDataPtr->asciiPtr);//[1] y
			decDataPtr-> xyzValid[1] = true;
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		break;
	case 'Z':
		if (decDataPtr->markZ == true)
		{
			decDataPtr->decState = Dec_Error;
			passChar(decDataPtr);
			passSpace(decDataPtr);
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		else
		{
			decDataPtr->markZ = true;
			passChar(decDataPtr);
			passSpace(decDataPtr);
			decDataPtr-> xyzCoordinate[2] = atof(decDataPtr->asciiPtr);//[2] z
			decDataPtr-> xyzValid[2] = true;
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		break;
	case 'U':
		if (decDataPtr->markU == true)
		{
			decDataPtr->decState = Dec_Error;
			passChar(decDataPtr);
			passSpace(decDataPtr);
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		else
		{
			decDataPtr->markU = true;
			passChar(decDataPtr);
			passSpace(decDataPtr);
			decDataPtr-> uvwCoordinate[0] = atof(decDataPtr->asciiPtr);//[0] u
			decDataPtr-> uvwValid[0] = true;
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		break;
	case 'V':
		if (decDataPtr->markV == true)
		{
			decDataPtr->decState = Dec_Error;
			passChar(decDataPtr);
			passSpace(decDataPtr);
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		else
		{
			decDataPtr->markV = true;
			passChar(decDataPtr);
			passSpace(decDataPtr);
			decDataPtr-> uvwCoordinate[1] = atof(decDataPtr->asciiPtr);//[1] v
			decDataPtr-> uvwValid[1] = true;
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		break;
	case 'W':
		if (decDataPtr->markW == true)
		{
			decDataPtr->decState = Dec_Error;
			passChar(decDataPtr);
			passSpace(decDataPtr);
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		else
		{
			decDataPtr->markW = true;
			passChar(decDataPtr);
			passSpace(decDataPtr);
			decDataPtr-> uvwCoordinate[2] = atof(decDataPtr->asciiPtr);//[2] w
			decDataPtr-> uvwValid[2] = true;
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		break;
	case 'I':
		if(decDataPtr->ncCmd == G02_Motion || decDataPtr->ncCmd == G03_Motion)
		{
			passChar(decDataPtr);
			passSpace(decDataPtr);
			decDataPtr-> ijkCenter[0] = atof(decDataPtr->asciiPtr);//iCenter
			decDataPtr-> ijkValid[0] = true;
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		break;
	case 'J':
		if(decDataPtr->ncCmd == G02_Motion || decDataPtr->ncCmd == G03_Motion)
		{
			passChar(decDataPtr);
			passSpace(decDataPtr);
			decDataPtr-> ijkCenter[1] = atof(decDataPtr->asciiPtr);//jCenter
			decDataPtr-> ijkValid[1] = true;
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		break;
	case 'K':
		if(decDataPtr->ncCmd == G02_Motion || decDataPtr->ncCmd == G03_Motion)
		{
			passChar(decDataPtr);
			passSpace(decDataPtr);
			decDataPtr-> ijkCenter[2] = atof(decDataPtr->asciiPtr);//kCenter
			decDataPtr->ijkValid[2] = true;//kValid
			passFloat(decDataPtr);
			passSpace(decDataPtr);
		}
		break;
	default:
		decDataPtr->decState = Dec_Error;
		passChar(decDataPtr);
		passSpace(decDataPtr);
		passFloat(decDataPtr);
		passSpace(decDataPtr);
		break;
	}//switch X/Y/Z/ U/V/W/ I/J/K
	return 1;
} // coordFunction()


int mFunction(DEC_DATA  *decDataPtr)
{	// 首先讀到M 進入 mfunction，
	passChar(decDataPtr);
	passSpace(decDataPtr);
	int mNumber =atoi(decDataPtr->asciiPtr);
	switch(mNumber)
	{
	case 00:
		decDataPtr->M00 = true;
		passInt(decDataPtr);
		passSpace(decDataPtr);
		return 0;
		break;
	case 01:
		decDataPtr->M01 = true;
		passInt(decDataPtr);
		passSpace(decDataPtr);
		return 1;
		break;
	case 30:
		decDataPtr->ncCmd = M30_End;
		passInt(decDataPtr);
		passSpace(decDataPtr);
		return 30;
		break;

    default:
		decDataPtr->decState = Dec_Error;
		passChar(decDataPtr);
		passSpace(decDataPtr);
		passFloat(decDataPtr);
		passSpace(decDataPtr);
		return -1;
		break;
	}

}//mFunction()


//**********************************************************
//pass functions
//**********************************************************
int passSpace(DEC_DATA  *decDataPtr)
{
	while(*decDataPtr->asciiPtr == ' ')
	{
		decDataPtr->asciiPtr++; 
	}
	return 1;
}//passspace

int passChar(DEC_DATA  *decDataPtr)
{
	decDataPtr->asciiPtr++;
	return 1;
}//passchar

int passInt(DEC_DATA  *decDataPtr)
{
	while ((*decDataPtr->asciiPtr >= '0')&& (*decDataPtr->asciiPtr <= '9')||(*decDataPtr->asciiPtr =='-'))
	{ 
  
		// begins with dot
		decDataPtr->asciiPtr++;// pass dot
		// ...

	}
	return 1;
}//passPosInt

int passFloat(DEC_DATA  *decDataPtr)
{ 
	while((*decDataPtr->asciiPtr >= '0' && *decDataPtr->asciiPtr <= '9' )||(*decDataPtr->asciiPtr =='.')||(*decDataPtr->asciiPtr =='-')) 
	{  

		// begins with dot
		decDataPtr->asciiPtr++;// pass dot
		// ...

	}//while
	return 1;
}//passPosFloat


