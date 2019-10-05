#pragma once
#include <StdAfx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "..\INC\intp.h"


int slopeInit(SLOPE_DATA *slopeDataPtr)
{
	slopeDataPtr->slopeState = Slope_Null;
	slopeDataPtr->slopeType = Type_Null;
	slopeDataPtr->acceleration = acc;

	slopeDataPtr->totalLength = 0; //sum+residual
	slopeDataPtr->sumLength = 0; 
	slopeDataPtr->residualLength = 0;
	slopeDataPtr->extraLength = 0;
	slopeDataPtr->feedLength = 0;
	slopeDataPtr->peakVb = 0;
	slopeDataPtr->extraVb = 0;

	slopeDataPtr->feed = 0; // F
	slopeDataPtr->momtVb = 0; // velocity

	slopeDataPtr->vbStep = 0 ;
	slopeDataPtr->stepLength = 0; // vbStep* cyclingTime int	acceCounter;
	slopeDataPtr->acceCounter = 0; 
	slopeDataPtr->deceCounter = 0;
	slopeDataPtr->feedCounter = 0;
	slopeDataPtr->intpCounter = 0; 

	slopeDataPtr->acceStep = 0;
	slopeDataPtr->deceStep = 0;
	slopeDataPtr->feedStep = 0;

	slopeDataPtr->extraValid = false;
	slopeDataPtr->resetHold = false;
	slopeDataPtr->intpEnd = false;

	return 1;
}// slopeInit()

void feedScheduling(INTP_DATA *intpDataPtr)
{
	intpDataPtr->slopeData.vbStep = intpDataPtr->slopeData.acceleration * intpDataPtr->cyclingTime;
	int N = (int) (intpDataPtr->slopeData.feed / intpDataPtr->slopeData.vbStep); //acce. step number to feedRate
	double R = intpDataPtr->slopeData.totalLength - N * (N+1) * intpDataPtr->slopeData.vbStep * intpDataPtr->cyclingTime;

	if (intpDataPtr->slopeData.totalLength <= intpDataPtr->slopeData.vbStep * intpDataPtr->cyclingTime){ //oneStep
		intpDataPtr->slopeData.slopeType = Type_One_Step;
		//One_Step
		intpDataPtr->slopeData.peakVb = intpDataPtr->slopeData.totalLength / intpDataPtr->cyclingTime;

	} // One_Step

	else if (intpDataPtr->slopeData.vbStep >= intpDataPtr->slopeData.feed){ //constFeed
		intpDataPtr->slopeData.slopeType = Type_Const_Feed;
		//Const_Feed 
		intpDataPtr->slopeData.acceStep = 0;
		intpDataPtr->slopeData.deceStep = 0;
		intpDataPtr->slopeData.feedStep = (int) (intpDataPtr->slopeData.totalLength / intpDataPtr->slopeData.feed / intpDataPtr->cyclingTime);

		//calculate extra Vb
		intpDataPtr->slopeData.extraLength = intpDataPtr->slopeData.totalLength
												- intpDataPtr->slopeData.feed * intpDataPtr->slopeData.feedCounter;
		intpDataPtr->slopeData.extraVb = intpDataPtr->slopeData.extraLength / intpDataPtr->cyclingTime;
		intpDataPtr->slopeData.extraValid = intpDataPtr->slopeData.extraVb == 0? false:true;

		intpDataPtr->slopeData.slopeState = Slope_Const;
	}//Const_Feed

	else if (R <= 0){//speed can NOT reach feedRate
		intpDataPtr->slopeData.slopeType = Type_Triangle;
		//Triangle
		int ADstep = 1;
		//calculate Acce and Dece step
		while ( (ADstep) * (ADstep + 1)
			* intpDataPtr->slopeData.vbStep * intpDataPtr->cyclingTime < intpDataPtr->slopeData.totalLength)
		{
			ADstep ++;
		}
		ADstep --;
		intpDataPtr->slopeData.acceStep = ADstep;
		intpDataPtr->slopeData.deceStep = ADstep;
		intpDataPtr->slopeData.feedStep = 0;
		//calculate extraLength
		intpDataPtr->slopeData.extraLength = intpDataPtr->slopeData.totalLength 
			- ((ADstep) * (ADstep + 1) * intpDataPtr->slopeData.vbStep * intpDataPtr->cyclingTime);

		//peakVb = Va + VbStep (temporary)
		intpDataPtr->slopeData.peakVb = intpDataPtr->slopeData.vbStep * (intpDataPtr->slopeData.acceStep + 1);

		//check if one more acce (peakVb) and revise extraLength 
		if (intpDataPtr->slopeData.extraLength >= intpDataPtr->slopeData.peakVb * intpDataPtr->cyclingTime)
		{	//one more acce 
			intpDataPtr->slopeData.acceStep ++;
			//revise extraLength
			intpDataPtr->slopeData.extraLength = intpDataPtr->slopeData.extraLength
													- intpDataPtr->slopeData.peakVb * intpDataPtr->cyclingTime;
		}// if (check one more acce)
		
		if (intpDataPtr->slopeData.extraLength
				>= intpDataPtr->slopeData.vbStep * intpDataPtr->slopeData.deceStep * intpDataPtr->cyclingTime)
		{//peakVb => extra in the middle
			intpDataPtr->slopeData.peakVb = intpDataPtr->slopeData.extraLength / intpDataPtr->cyclingTime;
			intpDataPtr->slopeData.extraLength = 0;
		}// if (check peak)
		else {//normal
			intpDataPtr->slopeData.peakVb = 0;
		}
		//calculate extra Vb
		intpDataPtr->slopeData.extraVb = intpDataPtr->slopeData.extraLength / intpDataPtr->cyclingTime;
		intpDataPtr->slopeData.extraValid = intpDataPtr->slopeData.extraVb == 0? false:true;

		intpDataPtr->slopeData.slopeState = Slope_Acce;
	} // Triangle

	else if (R > 0){
		intpDataPtr->slopeData.slopeType = Type_Trapezoid;
		//Trapezoid
		intpDataPtr->slopeData.acceStep = (int) (intpDataPtr->slopeData.feed / intpDataPtr->slopeData.vbStep);
		intpDataPtr->slopeData.deceStep = (int) (intpDataPtr->slopeData.feed / intpDataPtr->slopeData.vbStep);

		intpDataPtr->slopeData.feedLength = intpDataPtr->slopeData.totalLength 
										- (intpDataPtr->slopeData.acceStep) * (intpDataPtr->slopeData.acceStep + 1)
										* intpDataPtr->slopeData.vbStep * intpDataPtr->cyclingTime;
		//calculate step number for feed rate
		intpDataPtr->slopeData.feedStep = 
						(int) ( intpDataPtr->slopeData.feedLength / (intpDataPtr->slopeData.feed * intpDataPtr->cyclingTime) );
		//calculate extra Vb
		intpDataPtr->slopeData.extraLength = intpDataPtr->slopeData.feedLength
									- intpDataPtr->slopeData.feedStep * intpDataPtr->slopeData.feed * intpDataPtr->cyclingTime;
		intpDataPtr->slopeData.extraVb = intpDataPtr->slopeData.extraLength / intpDataPtr->cyclingTime;
		intpDataPtr->slopeData.extraValid = intpDataPtr->slopeData.extraVb == 0? false:true;
		intpDataPtr->slopeData.peakVb = 0;

		intpDataPtr->slopeData.slopeState = Slope_Acce;
	}
	else{
		intpDataPtr->intpState = Intp_Error;
	}//Trapezoid

}// feedScheduling()

void slope(SLOPE_DATA *slopeDataPtr)
{
	//calculate velocity magnitude
	switch (slopeDataPtr->slopeType){
	case Type_Null:
		
		break;
	case Type_One_Step:
		slopeDataPtr->momtVb = slopeDataPtr->peakVb;
		slopeDataPtr->intpEnd = true;
		break;
	case Type_Const_Feed:
	case Type_Triangle:
	case Type_Trapezoid:
	case Type_Reset:
		switch(slopeDataPtr->slopeState){
		case Slope_Acce:
			slopeAcce(slopeDataPtr);
			break;
		case Slope_Const:
			slopeConst(slopeDataPtr);
			break;
		case Slope_Dece:
			slopeDece(slopeDataPtr);
			break;
		case Slope_Peak:
			slopePeak(slopeDataPtr);
			break;		
		case Slope_End:
			slopeEnd(slopeDataPtr);
			break;
		case Slope_Hold:
			slopeHold(slopeDataPtr);
			//bla bla bla
			break;
		}
		break;
	}// case Slope_Type

}// slope()



void slopeAcce(SLOPE_DATA *slopeDataPtr)
{
	slopeDataPtr->acceCounter ++; //counter from 0 on, momtVb from 1 on
	slopeDataPtr->momtVb = slopeDataPtr->acceCounter * slopeDataPtr->vbStep;
	if (slopeDataPtr->acceCounter >= slopeDataPtr->acceStep){
		if (slopeDataPtr->peakVb != 0)
		{
			slopeDataPtr->slopeState = Slope_Peak;
		}
		else if (slopeDataPtr->feedStep == 0)
		{ //for triangle, next state: Dece
			slopeDataPtr->slopeState = Slope_Dece;
		}
		else //next state: Const
			slopeDataPtr->slopeState = Slope_Const;
	}
}// slopeAcce()
 
void slopeConst(SLOPE_DATA *slopeDataPtr)
{
	slopeDataPtr->feedCounter ++;
	slopeDataPtr->momtVb = slopeDataPtr->feed;

	if (slopeDataPtr->feedCounter >= slopeDataPtr->feedStep)
	{ //next state : Dece
		slopeDataPtr->slopeState = Slope_Dece;
	}

}// slopeConst()

void slopePeak(SLOPE_DATA *slopeDataPtr)
{
	slopeDataPtr->momtVb = slopeDataPtr->peakVb;
	slopeDataPtr->slopeState = Slope_Dece;
}// slopePeak()

void slopeDece(SLOPE_DATA *slopeDataPtr) //extra Vb included
{
	if ( slopeDataPtr->extraVb > ((slopeDataPtr->deceStep - slopeDataPtr->deceCounter)* slopeDataPtr->vbStep)
		&& 	slopeDataPtr->extraValid == true)
	{//calculate where extra Vb should be interpolated-->check if (momtVb - extraVb < Vbstep)
		slopeDataPtr->momtVb = slopeDataPtr->extraVb;
		slopeDataPtr->extraValid = false;
	}
	else 
	{
		slopeDataPtr->momtVb = (slopeDataPtr->deceStep - slopeDataPtr->deceCounter)* slopeDataPtr->vbStep;
		slopeDataPtr->deceCounter ++;
	
		if (slopeDataPtr->deceCounter >= slopeDataPtr->deceStep)
		{
			slopeDataPtr->slopeState = Slope_End;

		}
	}
}// slopeDece()


void slopeHold(SLOPE_DATA *slopeDataPtr)
{
	slopeDataPtr->momtVb -=  slopeDataPtr->vbStep;


	if (slopeDataPtr->momtVb <= 0)
	{
		slopeDataPtr->momtVb = 0;
		slopeDataPtr->slopeState = Slope_End;

	}

}// slopeHold

void slopeEnd(SLOPE_DATA *slopeDataPtr)
{
	slopeDataPtr->momtVb = 0;
	slopeDataPtr->intpEnd = true;

}// slopeEnd()