#pragma once
#include <math.h>
#include <stdio.h>
#include "..\..\LINK\INC\link.h"
#include "..\..\DRV\INC\switch.h"
#include "..\..\DRV\INC\stepMotorDRV.h"
#define PI 3.141592653589793238462643383279502884197169399375105820974944
#define acc 15 //(mm/s2)
#define timeInterval 0.02 //0.05sec = 50ms
#define DEFAULT_SPEED 10
#define INIT_Z_POSITION 100

int delByteSpace(char  *bytePtr, int  size);

void initFifo(FIFO_HEAD *fifoHeadPtr, FIFO_TYPE fifoType);
bool writeFifo(FIFO_HEAD *fifoHeadPtr, void *blockPtr);

bool readFifo(FIFO_HEAD *fifoHeadPtr, void* blockPtr);
//void resetFifo(FIFO_HEAD *FIFO_HEADPtr, FIFO_TYPE fifoType);

double degToRad(double angleD);
double radToDeg(double angleR);

double angleCycle(double angle);
double angleDiffReal(double angleDiff);

double arcCosProtect(double value);