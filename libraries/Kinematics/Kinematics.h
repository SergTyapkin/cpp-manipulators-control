#ifndef INCLUDE_REVERSE_KINEMATICS_
#define INCLUDE_REVERSE_KINEMATICS_

#include "../Basics/Basics.h"
#include <math.h>

#define _L0   16.0
#define _L1_1 20.0
#define _L1_2 6.0
#define _L2   13.5
#define _L3   17.8


float _delta(const float* phi, float x0, float y0, float z0);
float _penaltySum(const float* phi);
float _targetFunction(const float* phi, float x0, float y0, float z0);
// ------
void getAnglesByTargetPoint(float x, float y, float z, const pos* currentPoses, pos* listToExportPoses);
void getPointByAngles(const pos* angles, float &exportX, float &exportY, float &exportZ);

#endif //  INCLUDE_REVERSE_KINEMATICS_
