#include <Basics.h>
#include <Kinematics.h>
#include <math.h>

#define MAX_SPEED_DPS 60.0
#define MIN_SPEED_DPS 0.0
#define MAX_ACCELERATION_DPS 30.0


void getFastestSmoothMovingPosition(const pos* start, const pos* end, const float currentTime, pos* exportPositions) {
  FOR_JOINTS_IDX(i) {
    const float L = end[i] - start[i];
    const float T1 = min(MAX_SPEED_DPS / MAX_ACCELERATION_DPS, sqrt(L / MAX_ACCELERATION_DPS));
    const float speedAfterT1 = T1 * MAX_ACCELERATION_DPS;
    const float L1 = MAX_ACCELERATION_DPS * pow(T1, 2) / 2;
    const float T2 = T1 + (L - L1 * 2) / MAX_SPEED_DPS;
    const float L2 = L - L1;
    const float Tmax = T2 + T1;
    
    if (currentTime <= T1) {
      // Starting
      exportPositions[i] = start[i] + MAX_ACCELERATION_DPS * pow(currentTime, 2) / 2;
    } else if (currentTime <= T2) {
      // Center on max speed
      exportPositions[i] = start[i] + L1 + (currentTime - T1) * MAX_SPEED_DPS;
    } else if (currentTime <= Tmax) {
      // Stopping
      exportPositions[i] = start[i] + L2 + ((currentTime - T2) * speedAfterT1 - MAX_ACCELERATION_DPS * pow(currentTime - T2, 2) / 2);
    } else {
      // No movement. Must be in end pos
      exportPositions[i] = end[i];
    }
  }
}

void getFastestSmoothMovingSpeedDPS(const pos* start, const pos* end, float* exportSpeedsDPS) {
  FOR_JOINTS_IDX(i) {
    const float currentL = actualJointsPositions[i] - start[i];
    const float L = end[i] - start[i];
    const float T1 = min(MAX_SPEED_DPS / MAX_ACCELERATION_DPS, sqrt(L / MAX_ACCELERATION_DPS));
    const float speedAfterT1 = T1 * MAX_ACCELERATION_DPS;
    const float L1 = MAX_ACCELERATION_DPS * pow(T1, 2) / 2;
    const float L2 = L - L1;
    
    if (currentL <= L1) {
      // Starting
      exportSpeedsDPS[i] = speedAfterT1 * ((currentL - 0) / L1);
    } else if (currentL <= L2) {
      // Center on max speed
      exportSpeedsDPS[i] = MAX_SPEED_DPS;
    } else if (currentL <= L) {
      // Stopping
      exportSpeedsDPS[i] = speedAfterT1 - speedAfterT1 * ((currentL - L2) / L1);
    } else {
      // No movement. Must be already in end pos
      exportSpeedsDPS[i] = MIN_SPEED_DPS;
    }
  }
}

void getTimedSmoothMovingSpeedDPS(const pos* start, const pos* end, float duration, const float currentTime, float* exportSpeedsDPS) {
  FOR_JOINTS_IDX(i) {
    const float L = end[i] - start[i];
    float T1 = duration / 2;
    float acceleration = L / pow(T1, 2);
    if (acceleration * T1 > MAX_SPEED_DPS) {
      Serial.println("MORE");
      T1 = duration - L / MAX_SPEED_DPS;
      acceleration = MAX_SPEED_DPS / T1;

      const float min_T1 = MAX_SPEED_DPS / MAX_ACCELERATION_DPS;
      if (T1 <= min_T1) {
        duration = -T1 + min_T1 * 2;
        T1 = min_T1;
        acceleration = MAX_ACCELERATION_DPS;
      }
    }
    
    const float currentL = actualJointsPositions[i] - start[i];
    const float speedAfterT1 = T1 * acceleration;
    const float L1 = acceleration * pow(T1, 2) / 2;
    const float L2 = L - L1;
    const float T2 = duration - T1;

    Serial.print("-[");
    Serial.print(i);
    Serial.println("]-");
    
    Serial.print("L: ");
    Serial.println(L);
    
    Serial.print("T1: ");
    Serial.println(T1);

    Serial.print("Acceleration: ");
    Serial.println(acceleration);

    Serial.print("SpeedAfterT1: ");
    Serial.println(speedAfterT1);


    Serial.print("L1: ");
    Serial.println(L1);

    Serial.print("L2: ");
    Serial.println(L2);

    Serial.print("currentL: ");
    Serial.println(currentL);

    float targetL;
    if (currentTime <= T1) {
      // Starting
      exportSpeedsDPS[i] = speedAfterT1 * ((currentL - 0) / L1);
      targetL = acceleration * pow(currentTime, 2) / 2;
    } else if (currentTime <= T2) {
      // Center on max speed
      exportSpeedsDPS[i] = speedAfterT1;
      targetL = L1 + (currentTime - T1) * MAX_SPEED_DPS;
    } else if (currentTime <= duration) {
      // Stopping
      exportSpeedsDPS[i] = speedAfterT1 - speedAfterT1 * ((currentL - L2) / L1);
      targetL = L2 + ((currentTime - T2) * speedAfterT1 - acceleration * pow(currentTime - T2, 2) / 2);
    } else {
      // No movement. Must be already in end pos
      exportSpeedsDPS[i] = MIN_SPEED_DPS;
      targetL = L;
    }
    Serial.print("targetL: ");
    Serial.println(targetL);

    exportSpeedsDPS[i] += (targetL - currentL) * 1;

    Serial.println();
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
float currentTime;
void loop() {
  LOOP_PRINT_CHANGED_STATS();
  
  static bool movingEnabled = false;
  
  KEYS_SECTION {
    ON_ENTER(1) {
      movingEnabled = true;
      currentTime = 0;
      setAllJointsPositions(endPositions);
    }
  }

  if (movingEnabled) {
    getTimedSmoothMovingSpeedDPS(startPositions, endPositions, 1, currentTime, currentSpeeds);
    
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
    
    setAllJointsSpeedsDPS(currentSpeeds);

    currentTime += 0.050;
  }

  delay(50);
}
