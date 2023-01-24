#include <Basics.h>
#include <Kinematics.h>
#include <math.h>


#define OBSTACLES_COUNT 1
const float obstacles[OBSTACLES_COUNT][2][3] = {{{50, 0, 0}, {100, 50, 400}}};


const float startCoords[] = {50, 100, 20};
const float endCoords[] = {100, -100, 100};
pos startPositions[JOINTS_COUNT] = {};
pos endPositions[JOINTS_COUNT] = {};

float** trajectory;
unsigned trajectoryKeypointsCount;
pos** anglesTrajectory;

void setup() {
  SETUP();
  
  generateWay(startCoords, endCoords,  trajectory, trajectoryKeypointsCount);
  anglesTrajectory = (pos**)malloc(trajectoryKeypointsCount * sizeof(pos*));
  for (unsigned i = 0; i < trajectoryKeypointsCount; i++) {
    anglesTrajectory[i] = (pos*)calloc(JOINTS_COUNT, sizeof(pos));
    getAnglesByTargetPoint(trajectory[i][0], trajectory[i][1], trajectory[i][2], anglesTrajectory[i], anglesTrajectory[i]);
  }
  
  FOR_JOINTS_IDX(i) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(anglesTrajectory[0][i]);
  }
  Serial.println("[[[1]]]: Is that ok? Press Enter to go to this position");
}


unsigned nextTrajectoryPointIdx = 0;
void loop() {
  LOOP_PRINT_STATS();
  
  ENTER_SECTION {
    ON_ANY_ENTER {
      setAllJointsPositions(anglesTrajectory[nextTrajectoryPointIdx]);

      nextTrajectoryPointIdx++;
      if (nextTrajectoryPointIdx >= trajectoryKeypointsCount) {
        Serial.print(nextTrajectoryPointIdx - 1);
        Serial.println(" was the last trajectory keypoint");
        exit(0);
      }
      
      FOR_JOINTS_IDX(i) {
        Serial.print(i);
        Serial.print(": ");
        Serial.print(anglesTrajectory[nextTrajectoryPointIdx][i]);
      }
      Serial.print("[[[");
      Serial.print(nextTrajectoryPointIdx);
      Serial.println("]]]: Is that ok? Press Enter to go to this position");
    }
  }
  
  delay(20);
}
