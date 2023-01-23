#include <Basics.h>
#include <Kinematics.h>

pos newPositions[JOINTS_COUNT];
void setup() {
  setup_basic();

  pos positions[JOINTS_COUNT] = {2048, 2048, 2048, 2048, 512, 512};
  setAllJointsPositions(positions);


  getAnglesByTargetPoint(200, 300, 200, positions, newPositions);
  FOR_JOINTS_IDX(i) {
    Serial.print(i, ": ", newPositions[i]);
  }
  Serial.println("Is that ok? Press Enter to set this positions");
}


void loop() {
  loop_readPrintJointsStats();
  
  ON_ENTER_SECTION {
    ON_ENTER(1) {
      setAllJointsPositions(newPositions); // set gotten positions
    }
  }
  
  delay(20);
}
