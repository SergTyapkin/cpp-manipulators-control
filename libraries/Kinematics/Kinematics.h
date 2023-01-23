#ifndef INCLUDE_REVERSE_KINEMATICS_
#define INCLUDE_REVERSE_KINEMATICS_

#include "../Basics/Basics.h"
#include <math.h>


float _delta(const float* phi, float x0, float y0, float z0);
float _penaltySum(const float* phi);
float _targetFunction(const float* phi, float x0, float y0, float z0);
// ------
void getAnglesByTargetPoint(float x, float y, float z, const pos* currentPoses, pos* listToExportPoses);


#endif //  INCLUDE_REVERSE_KINEMATICS_
