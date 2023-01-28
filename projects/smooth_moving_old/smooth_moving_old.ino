#include <Basics.h>
#include <Kinematics.h>
#include <math.h>

#define MAX_SPEED_DPS 60.0
#define ACCELERATION 30.0


void getSmoothMovingPosition(const pos* start, const pos* end, float currentTime, pos* exportPositions) {
  FOR_JOINTS_IDX(i) {
    const float L = end[i] - start[i];
    const float T1 = min(MAX_SPEED_DPS / ACCELERATION, sqrt(L / ACCELERATION));
    const float speedAfterT1 = T1 * ACCELERATION;
    const float L1 = ACCELERATION * pow(T1, 2) / 2;
    const float T2 = T1 + (L - L1 * 2) / MAX_SPEED_DPS;
    const float L2 = L - L1;
    const float Tmax = T2 + T1;
    
    if (currentTime <= T1) {
      // Starting
      exportPositions[i] = start[i] + ACCELERATION * pow(currentTime, 2) / 2;
    } else if (currentTime <= T2) {
      // Center on max speed
      exportPositions[i] = start[i] + L1 + (currentTime - T1) * MAX_SPEED_DPS;
    } else if (currentTime <= Tmax) {
      // Stopping
      exportPositions[i] = start[i] + L2 + ((currentTime - T2) * speedAfterT1 - ACCELERATION * pow(currentTime - T2, 2) / 2);
    } else {
      // No movement. Must be in end pos
      exportPositions[i] = end[i];
    }
  }
}

void getSmoothMovingSpeedDPS(const pos* start, const pos* end, float currentTime, pos* exportPositions) {
  FOR_JOINTS_IDX(i) {
    const float L = end[i] - start[i];
    const float T1 = min(MAX_SPEED_DPS / ACCELERATION, sqrt(L / ACCELERATION));
    const float speedAfterT1 = T1 * ACCELERATION;
    const float L1 = ACCELERATION * pow(T1, 2) / 2;
    const float T2 = T1 + (L - L1 * 2) / MAX_SPEED_DPS;
    const float L2 = L - L1;
    const float Tmax = T2 + T1;

//    Serial.print("L: ");
//    Serial.println(L);
//
//    Serial.print("T1: ");
//    Serial.println(T1);
//
//    Serial.print("T2: ");
//    Serial.println(T2);
//
//    Serial.print("Tmax: ");
//    Serial.println(Tmax);
//
//    Serial.print("L1: ");
//    Serial.println(L1);
//
//    Serial.print("L2: ");
//    Serial.println(L2);
    
    if (currentTime <= T1) {
      // Starting
      exportSpeedsDPS[i] = ACCELERATION * currentTime;
      exportPositions[i] = start[i] + ACCELERATION * pow(currentTime, 2) / 2;
    } else if (currentTime <= T2) {
      // Center on max speed
      exportSpeedsDPS[i] = MAX_SPEED_DPS;
      exportPositions[i] = start[i] + L1 + (currentTime - T1) * MAX_SPEED_DPS;
    } else if (currentTime <= Tmax) {
      // Stopping
      exportSpeedsDPS[i] = speedAfterT1 - ACCELERATION * (currentTime - T2);
      exportPositions[i] = start[i] + L2 + ((currentTime - T2) * speedAfterT1 - ACCELERATION * pow(currentTime - T2, 2) / 2);
    } else {
      // No movement. Must be in end pos
      exportSpeedsDPS[i] = 1;
      exportPositions[i] = end[i];
    }
  }
}


pos defaultPositions[JOINTS_COUNT] = {180, 180, 180, 180, 180, 170};
pos startPositions[JOINTS_COUNT] = {};
pos endPositions[JOINTS_COUNT] = {};
void setup() {
  SETUP();
  setAllJointsPositions(defaultPositions);
  delay(1000);

  getAnglesByTargetPoint(25, 25, 3, defaultPositions, startPositions);
  getAnglesByTargetPoint(-25, -25, 60, defaultPositions, endPositions);

  setAllJointsPositions(startPositions);
  delay(1000);
}


float currentSpeeds[JOINTS_COUNT];
pos currentPositions[JOINTS_COUNT];
void loop() {
  LOOP_PRINT_CHANGED_STATS();
  
  static bool movingEnabled = false;
  static float currentTime;
  
  ENTER_SECTION {
    ON_ENTER(1) {
      movingEnabled = true;
      currentTime = 0;
      setAllJointsPositions(endPositions);
    }
  }

  if (movingEnabled) {
    currentTime += 0.050;
    getSmoothMovingSpeedPosition(startPositions, endPositions, currentTime, currentSpeeds, currentPositions);

    float x, y, z;
    getPointByAngles(actualJointsPositions, x, y, z);
    Serial.print("Point: ");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(", ");
    Serial.println(z);
    Serial.print("Speed: ");
    Serial.println(currentSpeeds[0]);
    Serial.print("Progress: ");
    Serial.println(currentPositions[0] / (endPositions[0] - startPositions[0]));
    Serial.print("Time: ");
    Serial.println(currentTime);
    
    //setAllJointsSpeedsDPS(currentSpeeds);
    setAllJointsPositions(currentPositions);
  }
  
  delay(50);
}
