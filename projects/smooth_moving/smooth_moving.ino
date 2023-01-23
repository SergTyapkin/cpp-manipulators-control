#include <Basics.h>
#include <Kinematics.h>
#include <math.h>

#define MAX_SPEED_PERCENT 10.0
#define ACCELERATION  1.0


float getSmoothMovingSpeedPercent(pos start, pos end, float curentTime) {
  const float L = end - start;
  const float T1 = min(L / MAX_SPEED_PERCENT, MAX_SPEED_PERCENT / ACCELERATION);
  const float T2 = L / MAX_SPEED_PERCENT;
  const float Tmax = T2 + T1;

  if (curentTime <= T1)
    return ACCELERATION * currentTime;
  else if (currentTime >= T2)
    if (currentTime <= Tmax)
      return MAX_SPEED_PERCENT - ACCELERATION * (currentTime - T2);
     else
      return 0;
  return MAX_SPEED_PERCENT;
}


pos startPositions[JOINTS_COUNT] = {2048, 2048, 2048, 2048, 512, 512};
pos endPositions[JOINTS_COUNT];
void setup() {
  setup_basic();

  setAllJointsPositions(startPositions);

  getAnglesByTargetPoint(200, 300, 200, startPositions, endPositions);
  FOR_JOINTS_IDX(i) {
    Serial.print(i, ": ", endPositions[i]);
  }
  Serial.println("Is that ok? Press Enter to smooth go to this positions");
}


void loop() {
  static bool movingEnabled = false;
  static float currentTime;
  
  loop_readPrintJointsStats();
  
  ON_ENTER_SECTION {
    ON_ENTER(1) {
      movingEnabled = true;
      currentTime = 0;
    }
  }

  if (movingEnabled) {
    FOR_JOINTS_ID(jointId) {
      const unsigned i = jointId - 1;
      const float velocity = getSmoothMovingSpeedPercent(startPositions[i], endPositions[i], currentTime);
      if (jointId == 1) {
        Serial.println("Time: ", currentTime, ". Set speed for joint 1: ", velocity);
      }
      //setJointVelocity(jointId, velocity);
    }
    currentTime += 0.020;
  }
  
  delay(20);
}
