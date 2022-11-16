#include <DynamixelWorkbench.h>


#define DEVICE_NAME "3"
#define JOINTS_COUNT 6
#define BAUDRATE  1000000
#define DXL_ID_1    1
#define DXL_ID_2    2
#define DXL_ID_3    3
#define DXL_ID_4    4
#define DXL_ID_5    5
#define DXL_ID_6    6

#define ERR_INIT_DEVICE            -11
#define ERR_INIT_JOINT             -12
#define ERR_SET_JOINT_MODE         -13
#define ERR_SET_MAX_JOINT_VELOCITY -14
#define ERR_READ_JOINT_POS         -15


typedef int32_t pos;
typedef uint8_t index;


DynamixelWorkbench dxl;

pos actualJointsPositions[JOINTS_COUNT]; // READONLY!! Always contains current joints positions

void setup() {
  // --- Init serial port to log data in computer console
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  const char* log;
  bool result = false;


  // --- Init Dinamyxel device
  result = dxl.init(DEVICE_NAME, BAUDRATE, &log);
  if (result == false) {
    Serial.println(log);
    Serial.println("Failed to init");
    exit(ERR_INIT_DEVICE);
  }
  Serial.print("Succeeded to init : ");
  Serial.println(BAUDRATE);


  // --- Init every joint
  for (index jointIdx = 1; jointIdx <= JOINTS_COUNT; jointIdx++) {
    uint16_t modelNumber;
    result = dxl.ping(jointIdx, &modelNumber, &log);
    if (result == false) {
      Serial.println(log);
      Serial.println("Failed to ping");
      exit(ERR_INIT_JOINT);
    }
    Serial.println("Succeeded to ping");
    Serial.print("id: ");
    Serial.print(jointIdx);
    Serial.print(" modelNumber : ");
    Serial.println(modelNumber);

    result = dxl.addSyncWriteHandler(jointIdx, "Goal_Position", &log);
    if (result == false) {
      Serial.println(log);
      Serial.print("Failed to set mode of joint ");
      Serial.println(jointIdx);
      exit(ERR_SET_JOINT_MODE);
    }

    // --- Limit the maximum velocity in Position Control Mode. Use 0 for Max speed
    result = dxl.writeControlTableItem(PROFILE_VELOCITY, jointIdx, 10);
    if (result == false) {
      Serial.println(log);
      Serial.print("Failed to set max velocity of joint ");
      Serial.println(jointIdx);
      exit(ERR_SET_MAX_JOINT_VELOCITY);
    }
  }

  pos positions[] = {2048, 2048, 2048, 2048, 512, 512};
  setJointsPositions(positions);
}


void loop() {
  // --- Led blinking
  //  digitalWrite(LED_BUILTIN, HIGH);
  //  delay(1000);
  //  digitalWrite(LED_BUILTIN, LOW);
  //  delay(1000);

  readAllJointsPositions(actualJointsPositions);
  printPositionsIfChanged(actualJointsPositions);

  handleKeyboardControls();

  delay(20);
}


// ------- Set positions functions --------
void setJointsPositions(pos* jointsPositions) {
  static const uint8_t handlerIndexPos = 1;
  const char* log;
  bool result = false;
  result = dxl.syncWrite(handlerIndexPos, jointsPositions, &log);
  if (result == false) {
    Serial.println(log);
    Serial.println("Failed to set all joint`s positions");
  }
}

void setJointPosition(index idx, pos position) {
  const char* log;
  bool result;
  result = dxl.goalPosition(idx, position, &log);
  if (result == false) {
    Serial.println(log);
    Serial.println("Failed to set posisiton of joint");
  }
}


// -------- Read positions functions --------
void readAllJointsPositions(pos* targetList) {
  const char* log;
  bool result;
  result = dxl.syncRead(jointIdx, &log);
  if (result == false) {
    Serial.println(log);
    Serial.println("Failed to read position of joints");
    exit(ERR_READ_JOINT_POS);
  }

  result = dxl.getSyncReadData(jointIdx, targetList, &log);
  if (result == false) {
    Serial.println(log);
    Serial.println("Failed to read position of joints");
    exit(ERR_READ_JOINT_POS);
  }
}


// ------- Print positions functions --------
void printPositions(const pos* list) {
  Serial.print("[ POS ]: ");
  for (index i = 0, l = list; i < JOINTS_COUNT; i++, l++) {
    Serial.print(*l);
    Serial.print(" ");
  }
  Serial.println("");
}

void printPositionsIfChanged(const pos* list) {
  static pos* prevPositions[JOINTS_COUNT];
  static bool isInitialized = false;

  // copy actual positions to "prevPositions". Only 1 time
  if (!isInitialized) {
    for (index i = 0, l = list, p = prevPositions; i < JOINTS_COUNT; i++, l++, p++) {
      *p = *l;
    }
    isInitialized = true;
    return;
  }

  // check if some position changed
  bool isChanged = false;
  for (index i = 0, l = list, p = prevPositions; i < JOINTS_COUNT; i++, l++, p++) {
    if (*p != *l) {
      isChanged = true;
      break;
    }
  }

  // if not changed - not print
  if (!isChanged)
    return;

  // print positions and copy changed
  Serial.print("[ POS ]: ");
  for (index i = 0, l = list, p = prevPositions; i < JOINTS_COUNT; i++, l++, p++) {
    if (*p != *l) {
      Serial.print("<");
      Serial.print(*l);
      Serial.print("> ");
      *p = *l;
      continue;
    }
    Serial.print(*l);
    Serial.print(" ");
  }
  Serial.println("");
}


// ------- Computer keyboard handling --------
void handleKeyboardControls() {
  const STEP = 5;

  char key;
  static bool isLedLighting = false;
  static index selectedJoint = 1;

  if (Serial.available()) {
    key = Serial.read(); // get pressed key from serial port

    switch (key) {
      case 'l': // toggle LED state
        if (isLedLighting)
          digitalWrite(13, LOW);
        else
          digitalWrite(13, HIGH);
        isLedLighting = !isLedLighting;
        break;
      case 'w': // move joint in '+' direction
        pos currentJointPos = actualJointsPositions[selectedJoint - 1];
        setJointPosition(selectedJoint, currentJointPos + STEP);
        break;
      case 's': // move joint in '-' direction
        pos currentJointPos = actualJointsPositions[selectedJoint - 1];
        setJointPosition(selectedJoint, currentJointPos - STEP);
        break;
      default:
        if (key > '0' && key <= '0' + JOINTS_COUNT) { // select joint by index
          selectedJoint = key - '0';
          Serial.print("You selected joint ");
          Serial.println(selectedJoint);
        }
    }
  }
}
