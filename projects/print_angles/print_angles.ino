#include <Basics.h>


void setup() {
  SETUP();
}


void loop() {
  LOOP_PRINT_STATS();

  handleKeyboardControls();

  delay(20);
}


// ------- Computer keyboard handling --------
void handleKeyboardControls() {
  #define STEP 20

  char key;
  static bool isLedLighting = false;
  static id selectedJoint = 1;

  if (Serial.available()) {
    key = Serial.read(); // get pressed key from serial port
    Serial.print("KEY: ");
    Serial.println(key);
    Serial.print("CODE: ");
    Serial.println(int(key));

    pos currentJointPos = actualJointsPositions[selectedJoint - 1];
    switch (key) {
      case 'l': // toggle LED state
        if (isLedLighting)
          digitalWrite(LED_BUILTIN, LOW);
        else
          digitalWrite(LED_BUILTIN, HIGH);
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
