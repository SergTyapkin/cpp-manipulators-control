#include <Basics.h>
#include <Kinematics.h>

#define SIDE 25  // TRAJECTORY_SQUARE_SIDE_LENGTH
#define TRAJECTORY_KEYPOINTS_COUNT 8

pos trajectory[TRAJECTORY_KEYPOINTS_COUNT][JOINTS_COUNT];
void setup() {
  SETUP();

  pos positions[JOINTS_COUNT] = {180, 180, 180, 180, 180, 170};
  setAllJointsPositions(positions);
  delay(20);

  getAnglesByTargetPoint( SIDE,  SIDE, 3,  positions, trajectory[0]);
  getAnglesByTargetPoint( 0   ,  SIDE, 30, positions, trajectory[1]);
  getAnglesByTargetPoint(-SIDE,  SIDE, 3,  positions, trajectory[2]);
  getAnglesByTargetPoint(-SIDE,  0   , 30, positions, trajectory[3]);
  getAnglesByTargetPoint(-SIDE, -SIDE, 3,  positions, trajectory[4]);
  getAnglesByTargetPoint( 0   , -SIDE, 30, positions, trajectory[5]);
  getAnglesByTargetPoint( SIDE, -SIDE, 3,  positions, trajectory[6]);
  getAnglesByTargetPoint( SIDE,  0   , 30, positions, trajectory[7]);
  for (unsigned i = 0; i < TRAJECTORY_KEYPOINTS_COUNT; i++) {
    Serial.print("----[[[");
    Serial.print(i);
    Serial.println("]]]----");
    FOR_JOINTS_IDX(c) {
      Serial.print("[");
      Serial.print(c);
      Serial.print("]: ");
      Serial.print(trajectory[i][c]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
  Serial.println("Is that ok? Press Enter to go on this trajectory");
}


unsigned nextTrajectoryPointIdx = 0;
void loop() {
  LOOP_PRINT_STATS();
  
  ENTER_SECTION {
    ON_ANY_ENTER {
      Serial.println("PRESSED KEY ");
      Serial.println(nextTrajectoryPointIdx);
      setAllJointsPositions(trajectory[nextTrajectoryPointIdx]);
      nextTrajectoryPointIdx++;
      if (nextTrajectoryPointIdx >= TRAJECTORY_KEYPOINTS_COUNT) {
        nextTrajectoryPointIdx -= TRAJECTORY_KEYPOINTS_COUNT;
      }
    }
  }
  
  delay(20);
}
