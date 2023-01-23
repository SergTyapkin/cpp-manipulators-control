#include <basics.h>


void setup() {
  setup_basic();

  pos positions[] = {2048, 2048, 2048, 2048, 512, 512};
  setAllJointsPositions(positions);
}


void loop() {
  loop_readPrintJointsStats();

  handleKeyboardControls();

  delay(20);
}


// ------- Computer keyboard handling --------
void handleKeyboardControls() {
  #define STEP 5

  char key;
  static bool isLedLighting = false;
  static index selectedJoint = 1;

  if (Serial.available()) {
    key = Serial.read(); // get pressed key from serial port

    pos currentJointPos = actualJointsPositions[selectedJoint - 1];
    switch (key) {
      case 'l': // toggle LED state
        if (isLedLighting)
          digitalWrite(13, LOW);
        else
          digitalWrite(13, HIGH);
        isLedLighting = !isLedLighting;
        break;
      case 'w': // move joint in '+' direction
        setJointPosition(selectedJoint, currentJointPos + STEP);
        break;
      case 's': // move joint in '-' direction
        setJointPosition(selectedJoint, currentJointPos - STEP);
        break;
      default:
        if (key > '0' && key <= '0' + JOINTS_COUNT) { // select joint by index
          selectedJoint = key - '0';
          Serial.print(F("You select joint "));
          Serial.println(selectedJoint);
        }
    }
  }
}
