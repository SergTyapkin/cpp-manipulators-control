#include <Basics.h>


#define STEP 20
#define SPEED_STEP 10.0  // DPS
#define SPEED_INIT 10.0  // DPS



void setup() {
  SETUP();

  FOR_JOINTS_ID(i) {
    setJointVelocityDPS(i, SPEED_INIT); 
  }
  delay(20);
  
  pos positions[] = {180, 180, 180, 180, 90, 170};
  setAllJointsPositions(positions);
  delay(20);
}


void loop() {
  LOOP_PRINT_STATS();

  handleKeyboardControls();

  delay(20);
}


// ------- Computer keyboard handling --------
void handleKeyboardControls() {
  #define STEP 20
  #define SPEED_STEP 1

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
    float currentJointSpeed = actualJointsVelocityDPS[selectedJoint - 1];
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
      case 'e': // enable joint torque
        enable(selectedJoint);
        break;
      case 'd': // disable joint torque
        disable(selectedJoint);
        break;
      case '=': // plus speed limit
        setJointVelocityDPS(selectedJoint, currentJointSpeed + SPEED_STEP);
        break;
      case '-': // minus speed limit
        setJointVelocityDPS(selectedJoint, currentJointSpeed - SPEED_STEP);
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
