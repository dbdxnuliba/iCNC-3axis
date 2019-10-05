#pragma once
#include <StdAfx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "..\INC\intp.h"
#include "..\..\LINK\INC\link.h"

void intpMain(INTP_DATA *intpDataPtr)
{
	readFifo(&intpDataPtr->linkDataPtr->sysToIntpFifo.fifoHead, &intpDataPtr->sysToIntpBlock);
	// state machine design methodology
	switch(intpDataPtr->intpState)
	{ //intp_IDLE, intp_OPEN_FILE, intp_PRE, intp_LINE_LOOP, ....
	case Intp_Idle:
		intpIdle(intpDataPtr);
		break;
	case Intp_Pre:
		intpPre(intpDataPtr);
		break;
	case Intp_Exe:
		intpExe(intpDataPtr);
		break;
	case Intp_Close:
		intpClose(intpDataPtr);
		break;	
	case Intp_RT_Function:
		intpRtFunction(intpDataPtr);
		break;
	case Intp_G04_Wait:
		intpG04Wait(intpDataPtr);
		break;
	case Intp_Hold:
		//intpHold(intpDataPtr);
		break;
	case Intp_M30_Program_End:
		intpM30ProgramEnd(intpDataPtr);
		break;
	case Intp_Reset:
		intpReset(intpDataPtr);
		break;
	case Intp_Error:
		intpError(intpDataPtr);
		break;
	}
	//   break to here

	return;
}

void intpIdle(INTP_DATA *intpDataPtr)
{
/*
	read FIFO
	if true, check ncCommand in DEC_TO_INTP_BLOCK
	if function command: execute function, or init RT_Function parameters and change state to RT_Functions
	if motion command, change state to INTP_PRE
*/

	//readFifo dec to intp
	if (readFifo(&intpDataPtr->linkDataPtr->decToIntpFifo.fifoHead, &intpDataPtr->ncBlock)){
		intpDataPtr->intpState = Intp_Pre;
		intpDataPtr->linkDataPtr->M30ProgramEnd = false;
	}
	/*
	//get block from sys to intp
	if (intpDataPtr->linkDataPtr->sysToIntpBlockValid == true)
	{
		intpDataPtr->sysToIntpBlock = intpDataPtr->linkDataPtr->sysToIntpBlock;
		intpDataPtr->linkDataPtr->sysToIntpBlockValid = false;
	}
	*/
}// intpIdle()

int intpPre(INTP_DATA* intpDataPtr)
{

	//coordination management
	switch(intpDataPtr->linkDataPtr->systemOpMode)
	{
	case MEM_Mode:
	case MDI_Mode:
	case HOME_Mode:
	case WHEEL_Mode:
	case JOG_Mode:

		switch(intpDataPtr->ncBlock.ncCmd)
		{
		case G00_Motion:
		case G01_Motion:
			//Start point known
			//Calculate End, Relative, totalLength, linearFactor
			for (int i=0; i<3; i++){
				if(    intpDataPtr->ncBlock.motionFunction.motion.xyzValid[0] 
					|| intpDataPtr->ncBlock.motionFunction.motion.xyzValid[1] 
					|| intpDataPtr->ncBlock.motionFunction.motion.xyzValid[2]){ // input xyz coordinate

					if(intpDataPtr->ncBlock.motionFunction.motion.xyzValid[i]) { //check xyz input valid
						if (intpDataPtr->ncBlock.motionFunction.motion.relAbs == true){ //relative
							intpDataPtr->xyzEnd[i] = intpDataPtr->xyzStart[i] 
													+ intpDataPtr->ncBlock.motionFunction.motion.xyzCoordinate[i];
						}
						else{ //absolute
							intpDataPtr->xyzEnd[i] = intpDataPtr->ncBlock.motionFunction.motion.xyzCoordinate[i];
						}
						intpDataPtr->xyzRelative[i] = intpDataPtr->xyzEnd[i] - intpDataPtr->xyzStart[i];

					} 
					else{ // xyz invalid
						intpDataPtr->xyzRelative[i] = 0;
						intpDataPtr->xyzEnd[i] = intpDataPtr->xyzStart[i];
					}

				} // input xyz coordinate

				else {// input uvw coordinate

					if(intpDataPtr->ncBlock.motionFunction.motion.uvwValid[i]) { //check uvw input valid
						if (intpDataPtr->ncBlock.motionFunction.motion.relAbs == true){ //relative
							intpDataPtr->uvwEnd[i] = intpDataPtr->uvwStart[i] 
													+ intpDataPtr->ncBlock.motionFunction.motion.uvwCoordinate[i];
							intpDataPtr->uvwRelative[i] = intpDataPtr->ncBlock.motionFunction.motion.uvwCoordinate[i];
						}
						else{ //absolute
							intpDataPtr->uvwEnd[i] = intpDataPtr->ncBlock.motionFunction.motion.uvwCoordinate[i];
							intpDataPtr->uvwRelative[i] = intpDataPtr->uvwEnd[i] - intpDataPtr->uvwStart[i];
						}
					} 
					else{ // uvw invalid
						intpDataPtr->uvwRelative[i] = 0;
						intpDataPtr->uvwEnd[i] = intpDataPtr->uvwStart[i];
					}
					//forward kinematic transform
					intpDataPtr->xyzEnd[0] = intpDataPtr->l1 * cos(angleCycle(degToRad(intpDataPtr->uvwEnd[0])) )
											+ intpDataPtr->l2 * cos(angleCycle(degToRad(intpDataPtr->uvwEnd[1])) );
					intpDataPtr->xyzEnd[1] = intpDataPtr->l1 * sin(angleCycle(degToRad(intpDataPtr->uvwEnd[0])) ) 
											+ intpDataPtr->l2 * sin(angleCycle(degToRad(intpDataPtr->uvwEnd[1])) );
					intpDataPtr->xyzEnd[2] = intpDataPtr->uvwEnd[2];
					for (int i=0; i<3; i++)
						intpDataPtr->xyzRelative[i] = intpDataPtr->xyzEnd[i] - intpDataPtr->xyzStart[i];
					//forward kinematic transform
				
				} // input uvw coordinate
			}
			intpDataPtr->slopeData.totalLength = sqrt(  pow(intpDataPtr->xyzRelative[0],2) 
													  + pow(intpDataPtr->xyzRelative[1],2)
													  + pow(intpDataPtr->xyzRelative[2],2) );
			for (int i=0; i<3; i++)
			{
				intpDataPtr->linearFactor[i] = intpDataPtr->xyzRelative[i] / intpDataPtr->slopeData.totalLength;
			} // linearFactor

			//velocity assign
			if (intpDataPtr->ncBlock.motionFunction.motion.feedRateValid)
				intpDataPtr->slopeData.feed = intpDataPtr->ncBlock.motionFunction.motion.feedRate
											 * intpDataPtr->sysToIntpBlock.feedOverride/100;
			else
				intpDataPtr->slopeData.feed = intpDataPtr->g00Speed * intpDataPtr->sysToIntpBlock.feedOverride/100;

			intpDataPtr->intpState = Intp_Exe;
			break;
		case G02_Motion:
		case G03_Motion:

			intpDataPtr->intpState = Intp_Exe;
			break;
		case G04_Dwell_Time:
			intpDataPtr->g04Counter = 0;
			intpDataPtr->dwellTime = intpDataPtr->ncBlock.motionFunction.rtFunction.dwellTime.value;
			intpDataPtr->intpState = Intp_G04_Wait;
			break;
		case Function_Cmd:
			intpDataPtr->intpState = Intp_RT_Function;
			break;
		case M30_End:
			intpDataPtr->intpState = Intp_M30_Program_End;
			break;
		}
	default:
		break;

	}

	//feed scheduling
	feedScheduling(intpDataPtr);
	return 1;
}// intpPre()

int intpExe(INTP_DATA* intpDataPtr)
{
	//momtVb calculate
	slope(intpDataPtr->slopeDataPtr);
	//Length management
	intpDataPtr->slopeData.stepLength = intpDataPtr->slopeData.momtVb * intpDataPtr->cyclingTime;
	intpDataPtr->slopeData.sumLength += intpDataPtr->slopeData.stepLength;
	intpDataPtr->slopeData.residualLength = intpDataPtr->slopeData.totalLength - intpDataPtr->slopeData.sumLength;

	//velocity to xyz coordinate update
	for (int i = 0; i < 3; i++)
	{
		intpDataPtr->xyzNew[i] = intpDataPtr->xyzOld[i]
					+ intpDataPtr->slopeData.momtVb * intpDataPtr->cyclingTime * intpDataPtr->linearFactor[i];
	}

	//inverse (xyz to uvw) coordinate transform
	inverseKinematicTransform(intpDataPtr);
	//uvwNew & uvwOld were calculated

	//machine output magnitude
	for (int i = 0 ; i < 3 ; i++)
	{	
		intpDataPtr->uvwIncrement[i] = (int) ((intpDataPtr->uvwNew[i] - intpDataPtr->uvwOld[i]) / intpDataPtr->uvwResolution[i]);

		// send to motion card when we use HD_TIMER
#if HD_TIMER
		putAxisValue(i, intpDataPtr->uvwIncrement[i]);
#endif

	}//end for

	//update momtVb and coordination to SYS
	intpDataPtr->linkDataPtr->intpToSysBlock.momtVb = intpDataPtr->slopeData.momtVb;
	for (int i = 0; i < 3; i++)
	{
		intpDataPtr->linkDataPtr->intpToSysBlock.xyzNew[i] = intpDataPtr->xyzNew[i];
		intpDataPtr->linkDataPtr->intpToSysBlock.uvwNew[i] = intpDataPtr->uvwNew[i];
		intpDataPtr->linkDataPtr->intpToSysBlockValid = true;
		//clear uvwIncrement
		intpDataPtr->uvwIncrement[i] = 0;
		//update old coord
		intpDataPtr->xyzOld[i] = intpDataPtr->xyzNew[i];
		intpDataPtr->uvwOld[i] = intpDataPtr->uvwNew[i];

	}


	//next: EXE or Idle
	if (intpDataPtr->slopeData.intpEnd)
	{
		for (int i = 0; i < 3; i++)
		{
			intpDataPtr->xyzNew[i] = intpDataPtr->xyzEnd[i];
			intpDataPtr->xyzStart[i] = intpDataPtr->xyzEnd[i];
		}
		bool motionPause =  (intpDataPtr->ncBlock.motionFunction.motion.M01 && intpDataPtr->sysToIntpBlock.M01) 
							|| intpDataPtr->ncBlock.motionFunction.motion.M00 || intpDataPtr->sysToIntpBlock.singleBlock;
		if (motionPause == true)
		{
			intpDataPtr->intpState = Intp_RT_Function;
		}
		else
			intpDataPtr->intpState = Intp_Idle;

		slopeInit(intpDataPtr->slopeDataPtr);
	}
	return 1;
}// intpExe()

//**************************************
//Coordination Transformation
//**************************************


void inverseKinematicTransform(INTP_DATA *intpDataPtr)
{
	double alphaNew, betaNew, gammaNew;
	double uvwNewA[2], uvwNewB[2];
	//New
	alphaNew = atan2(intpDataPtr->xyzNew[1], intpDataPtr->xyzNew[0]); //ArcTan y/x, rad
	double cosineBetaNew = arcCosProtect((pow(intpDataPtr->l1,2) + pow(intpDataPtr->l2,2) - pow(intpDataPtr->xyzNew[0],2) - pow(intpDataPtr->xyzNew[1],2))
										  /	(2 * intpDataPtr->l1 * intpDataPtr->l2));	
	betaNew = acos(cosineBetaNew); //ArcCos ( ++-+/*** ), rad, 0-PI
	gammaNew = (PI - betaNew) /2; // rad

	intpDataPtr->uvwNew[2] = intpDataPtr->xyzNew[2]; // W = Z
	//A: case 1
	uvwNewA[0] = angleCycle(alphaNew - gammaNew);
	uvwNewA[1] = angleCycle(alphaNew + gammaNew);
	//B: case 2
	uvwNewB[0] = angleCycle(alphaNew + gammaNew);
	uvwNewB[1] = angleCycle(alphaNew - gammaNew);

	//judgment variables AAA & BBB
	double AAA = pow(angleDiffReal(uvwNewA[0] - intpDataPtr->uvwOld[0]),2) + pow(angleDiffReal(uvwNewA[1] - intpDataPtr->uvwOld[1]),2);
	double BBB = pow(angleDiffReal(uvwNewB[0] - intpDataPtr->uvwOld[0]),2) + pow(angleDiffReal(uvwNewB[1] - intpDataPtr->uvwOld[1]),2);
	if (AAA < BBB){ //use A: case 1
		intpDataPtr->uvwNew[0] = radToDeg(uvwNewA[0]);
		intpDataPtr->uvwNew[1] = radToDeg(uvwNewA[1]);
		if (pow(angleDiffReal(intpDataPtr->uvwNew[0] - intpDataPtr->uvwOld[0]) , 2) > 100)
		{   //prevent jumping
			intpDataPtr->uvwNew[0] = radToDeg(uvwNewB[0]);
			intpDataPtr->uvwNew[1] = radToDeg(uvwNewB[1]);
		}
	}
	else{ // use B: case 2
		intpDataPtr->uvwNew[0] = radToDeg(uvwNewB[0]);
		intpDataPtr->uvwNew[1] = radToDeg(uvwNewB[1]);
		if ( pow(angleDiffReal(intpDataPtr->uvwNew[0] - intpDataPtr->uvwOld[0]) , 2) > 100)
		{   //prevent jumping
			intpDataPtr->uvwNew[0] = radToDeg(uvwNewA[0]);
			intpDataPtr->uvwNew[1] = radToDeg(uvwNewA[1]);
		}

	}
}// inverseCoordTransform()

void forwardKinematicTransform(INTP_DATA *intpDataPtr)
{

}// forwardCoordTransform()


int intpRtFunction(INTP_DATA* intpDataPtr)
{
	//wait for continuee (to be revised)
	if (intpDataPtr->sysToIntpBlock.continuee == true && intpDataPtr->sysToIntpBlock.continueValid == true)
	{
		intpDataPtr->intpState = Intp_Idle;
	}
	else{
		intpDataPtr->intpState = Intp_RT_Function;
	}
	return 1;
}// intpRtFunction()

int intpG04Wait(INTP_DATA* intpDataPtr)
{
	intpDataPtr->g04Counter ++;
	if (intpDataPtr->g04Counter * intpDataPtr->cyclingTime >= intpDataPtr->dwellTime)
	{
		intpDataPtr->g04Counter = 0;
		intpDataPtr->intpState = Intp_Idle;
	} 
	else
	{
		intpDataPtr->intpState = Intp_G04_Wait;
	}

	return 1;
}// intpG04Wait()

int intpM30ProgramEnd(INTP_DATA* intpDataPtr)
{
	intpDataPtr->linkDataPtr->M30ProgramEnd = true;
	intpDataPtr->intpState = Intp_Idle;
	return 1;
}// intpM30ProgramEnd()

int intpError(INTP_DATA* intpDataPtr)
{
	
	return 1;
}// intpError()



//********************
//intpReset Function
//********************

int intpResetPre(INTP_DATA* intpDataPtr)
{
	if (intpDataPtr->intpState == Intp_Idle || intpDataPtr->intpState == Intp_RT_Function)
		intpDataPtr->intpResetState = Intp_Reset_Close;
	else{
	intpDataPtr->slopeData.totalLength = 0;
	intpDataPtr->slopeData.residualLength = 0;
	intpDataPtr->slopeData.sumLength = 0;

//	intpDataPtr->slopeData.deceStep = (int) (intpDataPtr->slopeData.momtVb / intpDataPtr->slopeData.vbStep);
//	intpDataPtr->slopeData.deceCounter = 0;
	intpDataPtr->slopeData.slopeType = Type_Reset;
	intpDataPtr->slopeData.slopeState = Slope_Hold;
	intpDataPtr->intpResetState = Intp_Reset_Exe;
	}
	return 1;
}// intpResetPre()

int intpResetExe(INTP_DATA* intpDataPtr)
{
	slope(intpDataPtr->slopeDataPtr);

	//Length management //no need
//	intpDataPtr->slopeData.stepLength = intpDataPtr->slopeData.momtVb * intpDataPtr->cyclingTime;
//	intpDataPtr->slopeData.sumLength += intpDataPtr->slopeData.stepLength;
//	intpDataPtr->slopeData.residualLength = intpDataPtr->slopeData.totalLength - intpDataPtr->slopeData.sumLength;

	//velocity to xyz coordinate update
	for (int i = 0; i < 3; i++)
	{
		intpDataPtr->xyzNew[i] = intpDataPtr->xyzOld[i]
		+ intpDataPtr->slopeData.momtVb * intpDataPtr->cyclingTime * intpDataPtr->linearFactor[i];
	}

	//inverse (xyz to uvw) coordinate transform
	inverseKinematicTransform(intpDataPtr);
	//uvwNew & uvwOld were calculated

	//machine output uvw increment
	for (int i = 0 ; i < 3 ; i++)
	{	
		intpDataPtr->uvwIncrement[i] = (int) ((intpDataPtr->uvwNew[i] - intpDataPtr->uvwOld[i]) / intpDataPtr->uvwResolution[i]);

		// send to motion card when we use HD_TIMER
#if HD_TIMER
		putAxisValue(i, intpDataPtr->uvwIncrement[i]);
#endif

	}//for (increment output)

	//update momtVb and coordination to SYS
	intpDataPtr->linkDataPtr->intpToSysBlock.momtVb = intpDataPtr->slopeData.momtVb;
	for (int i = 0; i < 3; i++)
	{
		intpDataPtr->linkDataPtr->intpToSysBlock.xyzNew[i] = intpDataPtr->xyzNew[i];
		intpDataPtr->linkDataPtr->intpToSysBlock.uvwNew[i] = intpDataPtr->uvwNew[i];
		intpDataPtr->linkDataPtr->intpToSysBlockValid = true;
		//update old coord
		intpDataPtr->xyzOld[i] = intpDataPtr->xyzNew[i];
		intpDataPtr->uvwOld[i] = intpDataPtr->uvwNew[i];

	}


	//next: resetEXE or Idle
	if (intpDataPtr->slopeData.intpEnd)
	{
		for (int i = 0; i < 3; i++)
		{
			intpDataPtr->xyzStart[i] = intpDataPtr->xyzNew[i];
		}
		intpDataPtr->intpResetState = Intp_Reset_Close;
		slopeInit(intpDataPtr->slopeDataPtr);

	}

	return 1;
}// intpResetExe()

int intpResetClose(INTP_DATA* intpDataPtr)
{
	intpDataPtr->linkDataPtr->intpResetValid = true;
	intpDataPtr->intpState = Intp_Idle;

	//things that should be conserved, e.g. xyzNew, uvwNew
	double xyzNewTemp[3], uvwNewTemp[3] ;
	for (int i = 0; i < 3; i++)
	{
		xyzNewTemp[i] = intpDataPtr->xyzNew[i];
		uvwNewTemp[i] = intpDataPtr->uvwNew[i];
	}
	
	intpInit(intpDataPtr);

	intpDataPtr->intpResetState = Intp_Reset_Pre;
	for (int i = 0; i < 3; i++)
	{
		intpDataPtr->xyzOld[i] = xyzNewTemp[i];
		intpDataPtr->uvwOld[i] = uvwNewTemp[i];
		intpDataPtr->xyzNew[i] = xyzNewTemp[i];
		intpDataPtr->uvwNew[i] = uvwNewTemp[i];
		intpDataPtr->xyzStart[i] = xyzNewTemp[i];
		intpDataPtr->uvwStart[i] = uvwNewTemp[i];
		intpDataPtr->xyzEnd[i] = xyzNewTemp[i];
		intpDataPtr->uvwEnd[i] = uvwNewTemp[i];
	}
	return 1;
}// intpResetClose

void intpUvwExe(INTP_DATA* intpDataPtr)
{
	
}