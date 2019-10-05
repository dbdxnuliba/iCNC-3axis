#include <StdAfx.h>
#include <stdio.h>
#include "..\INC\util.h"

int delByteSpace(char  *bytePtr, int  size)
{
	for (int i = 0; i < size; i++)//local variable (only used within this function)
	{
		*bytePtr = 0; // byte for byte to clear memory
		bytePtr++;
	}
	return 1;
} // delByteSpace()
// clear used comments


void initFifo(FIFO_HEAD *fifoHeadPtr, FIFO_TYPE fifoType)
{
	fifoHeadPtr->rdIndex = 0;
	fifoHeadPtr->wrIndex = 0;
	fifoHeadPtr->fifoType = fifoType;

	switch (fifoType)
	{
	case FIFO_TYPE::DEC_TO_INTP:
		fifoHeadPtr->sizeInByte = sizeof(NC_BLOCK);
		fifoHeadPtr->sizeInBlock = DEC_TO_INTP_FIFO_LENGTH;

		for(int i = 0; i < DEC_TO_INTP_FIFO_LENGTH; i++)
		{
			((DEC_TO_INTP_FIFO*)fifoHeadPtr)->ncBlockValid[i] = false;
		}
		break;
	case FIFO_TYPE::SYS_TO_INTP:
		fifoHeadPtr->sizeInByte = sizeof(SYS_TO_INTP_DATA);
		fifoHeadPtr->sizeInBlock = 100;

		for(int i = 0; i < SYS_TO_INTP_FIFO_LENGTH; i++)
		{
			((SYS_TO_INTP_FIFO*)fifoHeadPtr)->sysDataValid[i] = false;
		}
		break;
	case FIFO_TYPE::OTHER_BLOCK:
		fifoHeadPtr->sizeInByte = sizeof(OTHER_BLOCK_TRANSFORM);
		fifoHeadPtr->sizeInBlock = 100;

		for(int i = 0; i < OTHER_FIFO_LENGTH; i++)
		{
			((OTHER_FIFO*)fifoHeadPtr)->otherBlockValid[i] = false;
		}
		break;
	default:
		break;
	}// case (fifoType)
} // initFifo()

bool writeFifo(FIFO_HEAD *fifoHeadPtr, void* blockPtr)
{
	switch(fifoHeadPtr->fifoType)
	{
	case FIFO_TYPE::DEC_TO_INTP:
		//check whether FIFO is full
		if(((DEC_TO_INTP_FIFO*)fifoHeadPtr)->ncBlockValid[fifoHeadPtr->wrIndex] == false)
		{
			//struct memory allocation, alignment
			((DEC_TO_INTP_FIFO*)fifoHeadPtr)->ncBlockInfoHolder[fifoHeadPtr->wrIndex] = *(NC_BLOCK*)blockPtr;
			((DEC_TO_INTP_FIFO*)fifoHeadPtr)->ncBlockValid[fifoHeadPtr->wrIndex] = true;
			fifoHeadPtr->wrIndex++;

			//prevent index out of bound exception
			if (fifoHeadPtr->wrIndex >= fifoHeadPtr->sizeInBlock)
			{
				fifoHeadPtr->wrIndex = 0;
			}
			return true;
		}
		else
			return false;
		break;
	case FIFO_TYPE::SYS_TO_INTP:
		if(((SYS_TO_INTP_FIFO*)fifoHeadPtr)->sysDataValid[fifoHeadPtr->wrIndex] == false)
		{
			//struct memory allocation, alignment
			((SYS_TO_INTP_FIFO*)fifoHeadPtr)->sysDataInfoHolder[fifoHeadPtr->wrIndex] = *(SYS_TO_INTP_DATA*)blockPtr;
			((SYS_TO_INTP_FIFO*)fifoHeadPtr)->sysDataValid[fifoHeadPtr->wrIndex] = true;
			fifoHeadPtr->wrIndex++;

			//prevent index out of bound exception
			if (fifoHeadPtr->wrIndex >= fifoHeadPtr->sizeInBlock)
			{
				fifoHeadPtr->wrIndex = 0;
			}
			return true;
		}//if not full
		else
			return false;
		break;
	case FIFO_TYPE::OTHER_BLOCK:
		if(((OTHER_FIFO*)fifoHeadPtr)->otherBlockValid[fifoHeadPtr->wrIndex] == false)
		{
			//struct memory allocation, alignment
			((OTHER_FIFO*)fifoHeadPtr)->otherBlockInfoHolder[fifoHeadPtr->wrIndex] = *(OTHER_BLOCK_TRANSFORM*)blockPtr;
			((OTHER_FIFO*)fifoHeadPtr)->otherBlockValid[fifoHeadPtr->wrIndex] = true;
			fifoHeadPtr->wrIndex++;

			//prevent index out of bound exception
			if (fifoHeadPtr->wrIndex >= fifoHeadPtr->sizeInBlock)
			{
				fifoHeadPtr->wrIndex = 0;
			}
			return true;
		}//if not full
		else
			return false;
		break;
	default:
		return false;
		break;
	}
} // writeFifo()

bool readFifo(FIFO_HEAD *fifoHeadPtr, void* blockPtr)
{
	switch(fifoHeadPtr->fifoType)
	{
	case FIFO_TYPE::DEC_TO_INTP:
		if (((DEC_TO_INTP_FIFO*)fifoHeadPtr)->ncBlockValid[fifoHeadPtr->rdIndex] == true)
		{
			*((NC_BLOCK*)blockPtr) = ((DEC_TO_INTP_FIFO*)fifoHeadPtr)->ncBlockInfoHolder[fifoHeadPtr->rdIndex];
			((DEC_TO_INTP_FIFO*)fifoHeadPtr)->ncBlockValid[fifoHeadPtr->rdIndex] = false;
			fifoHeadPtr->rdIndex++;
			
			if(fifoHeadPtr->rdIndex >= fifoHeadPtr->sizeInBlock)
			{
				fifoHeadPtr->rdIndex = 0;
			}
			return true;
		}
		else
			return false;
		break;
	case FIFO_TYPE::SYS_TO_INTP:
		if (((SYS_TO_INTP_FIFO*)fifoHeadPtr)->sysDataValid[fifoHeadPtr->rdIndex] == true)
		{
			*((SYS_TO_INTP_DATA*)blockPtr) = ((SYS_TO_INTP_FIFO*)fifoHeadPtr)->sysDataInfoHolder[fifoHeadPtr->rdIndex];
			((SYS_TO_INTP_FIFO*)fifoHeadPtr)->sysDataValid[fifoHeadPtr->rdIndex] = false;
			fifoHeadPtr->rdIndex++;

			if(fifoHeadPtr->rdIndex >= fifoHeadPtr->sizeInBlock)
			{
				fifoHeadPtr->rdIndex = 0;
			}
			return true;
		}
		else
			return false;
		break;
	case FIFO_TYPE::OTHER_BLOCK:
		if (((OTHER_FIFO*)fifoHeadPtr)->otherBlockValid[fifoHeadPtr->rdIndex] == true)
		{
			*((OTHER_BLOCK_TRANSFORM*)blockPtr) = ((OTHER_FIFO*)fifoHeadPtr)->otherBlockInfoHolder[fifoHeadPtr->rdIndex];
			((OTHER_FIFO*)fifoHeadPtr)->otherBlockValid[fifoHeadPtr->rdIndex] = false;
			fifoHeadPtr->rdIndex++;

			if(fifoHeadPtr->rdIndex >= fifoHeadPtr->sizeInBlock)
			{
				fifoHeadPtr->rdIndex = 0;
			}
			return true;
		}
		else
			return false;
		break;
	default:
		return false;
	}
} // readFifo()


double degToRad(double angleD)
{
	return angleD*PI/180;
}// deg2rad()
double radToDeg(double angleR)
{
	return angleR*180/PI;
}// rad2deg()

double angleCycle(double angle)
{
	while (angle >= 2*PI)
	{
		angle -= 2*PI;
	}
	while (angle < 0)
		angle += 2*PI;
	return angle;
}

double angleDiffReal(double angleDiff)
{
	if (abs(angleDiff) > PI)
		return 2*PI - abs(angleDiff);
	else
		return abs(angleDiff);
}

double arcCosProtect(double value)
{
	if (value > 1)
		return 1;
	else if (value < -1)
		return -1;
	else
		return value;
}