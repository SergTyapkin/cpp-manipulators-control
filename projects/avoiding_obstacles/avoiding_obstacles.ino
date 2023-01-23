#include <Basics.h>
#include <Kinematics.h>
#include <math.h>


#define OBSTACLES_COUNT 1
const float obstacles[][][] = {{{50, 0, 0}, {100, 50, 400}}};


const float* startCoords[] = {50, 100, 20};
const float* endCoords[] = {100, -100, 100};
pos startPositions[JOINTS_COUNT] = {};
pos endPositions[JOINTS_COUNT] = {};

float** trajectory;
unsigned trajectorylength;
float** anglesTrajectory;

void setup() {
  setup_basic();
  
  generateWay(startCoods, endCoords,  trajectory, trajectorylength);
  anglesTrajectory = (float**)malloc(trajectorylength * sizeof(float*));
  for (unsigned i = 0; i < trajectorylength; i++) {
    anglesTrajectory[i] = (float*)calloc(JOINTS_COUNT, sizeof(float));
    getAnglesByTargetPoint(trajectory[i][0], trajectory[i][1], trajectory[i][2], anglesTrajectory[i], anglesTrajectory[i]);
  }
  
  FOR_JOINTS_IDX(i)
    Serial.print(i, ": ", anglesTrajectory[0][i]);
  Serial.println("[1]: Is that ok? Press Enter to go to this position");
}


void goToPositionHandler(unsigned idx) {
  if (idx >= trajectorylength) {
    Serial.println(idx-1, " was the last trajectory keypoint");
    exit(0);
  }
  setAllJointsPositions(anglesTrajectory[idx]);
  FOR_JOINTS_IDX(i)
    Serial.print(i, ": ", anglesTrajectory[idx+1][i]);
  Serial.println("[", idx+1, "]: Is that ok? Press Enter to go to this position");
}
void loop() {
  loop_readPrintJointsStats();
  
  ON_ENTER_SECTION {
    ON_ENTER(1) {
      goToPositionHandler(0);
    }
    ON_ENTER(2) {
      goToPositionHandler(0);
    }
    ON_ENTER(3) {
      goToPositionHandler(0);
    }
    ON_ENTER(4) {
      goToPositionHandler(0);
    }
    ON_ENTER(5) {
      goToPositionHandler(0);
    }
    ON_ENTER(6) {
      goToPositionHandler(0);
    }
  }
  
  delay(20);
}
