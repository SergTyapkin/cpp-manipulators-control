#include "Basics.h"


Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
pos actualJointsPositions[JOINTS_COUNT];


// ------- Default function --------
void setup_basic() {
  // --- Init serial port to log data in computer console
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(BAUDRATE);

  bool result = false;


  // --- Init Dinamyxel device
  dxl.begin(BAUDRATE);
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  // --- Init every joint
  FOR_JOINTS_ID(jointId) {
    uint16_t modelNumber = dxl.getModelNumber(jointId);
    result = dxl.ping(jointId);
    if (result == false) {
      Serial.println("Failed to ping");
      exit(ERR_INIT_JOINT);
    }
    Serial.println("Succeeded to ping");
    Serial.print("id: ");
    Serial.print(jointId);
    Serial.print("; modelNumber : ");
    Serial.println(modelNumber);

    result = dxl.setOperatingMode(jointId, OP_POSITION);
    if (result == false) {
      Serial.println(F("Failed to set position mode"));
      exit(ERR_SET_JOINT_MODE);
    }

//    // --- Limit the maximum velocity in Position Control Mode. Use 0 to Max speed
//    result = dxl.writeControlTableItem(PROFILE_VELOCITY, jointId, 10);
//    if (result == false) {
//      Serial.print(F("Failed to set max velocity of joint "));
//      Serial.println(jointId);
//      exit(ERR_SET_MAX_JOINT_VELOCITY);
//    }
  }
}


// ------- Set positions functions --------
void setJointPosition(id idx, pos position) {
  bool result;
  result = dxl.setGoalPosition(idx, position);
  if (result == false) {
    Serial.println("Failed to set posisiton of joint");
    exit(ERR_SET_JOINT_POSITION);
  }
}

void setAllJointsPositions(pos* jointsPositions) {
  FOR_JOINTS_ID(jointId) {
    setJointPosition(jointId, jointsPositions[jointId]);
  }
}

// ------- Set positions functions --------
void setJointVelocity(id idx, float percent) {
  bool result;
  result = dxl.setGoalVelocity(idx, percent, UNIT_PERCENT);
  if (result == false) {
    Serial.println("Failed to set velocity of joint");
    exit(ERR_SET_JOINT_VELOCITY);
  }
}

void setAllJointsVelocity(float* jointsPositions) {
  FOR_JOINTS_ID(jointId) {
    setJointVelocity(jointId, jointsPositions[jointId]);
  }
}

// -------- Read positions --------
void readAllJointsPositions(pos* targetList) {
  FOR_JOINTS_ID(jointId) {
    pos jointPos = dxl.getPresentPosition(jointId);
    targetList[jointId] = jointPos;
  }
}
// -------- Read velocity --------
void readAllJointsVelocity(float* targetList) {
  FOR_JOINTS_ID(jointId) {
    float jointVelocity = dxl.getPresentVelocity(jointId, UNIT_PERCENT);
    targetList[jointId] = jointVelocity;
  }
}
// -------- Read current --------
void readAllJointsCurrent(float* targetList) {
  FOR_JOINTS_ID(jointId) {
    float jointCurrent = dxl.getPresentCurrent(jointId, UNIT_MILLI_AMPERE);
    targetList[jointId] = jointCurrent;
  }
}


// ------- Print positions --------
void _printList(const pos* lost) {
  FOR_JOINTS_IDX(i) {
    Serial.print(list[i]);
    Serial.print(" ");
  }
  Serial.println("");
}
void printPositions(const pos* list) {
  Serial.print("[POS](raw): ");
  _printList(list);
}
// ------- Print velocity --------
void printVelocity(const pos* list) {
  Serial.print("[VEL](%): ");
  _printList(list);
}
// ------- Print current --------
void printCurrent(const pos* list) {
  Serial.print("[CUR](mA): ");
  _printList(list);
}

void printPositionsIfChanged(const pos* list) {
  static pos prevPositions[JOINTS_COUNT];
  static bool isInitialized = false;

  // copy actual positions to "prevPositions". Only 1 time
  if (!isInitialized) {
    FOR_JOINTS_IDX(i) {
      prevPositions[i] = list[i];
    }
    isInitialized = true;
    return;
  }

  // check if some position changed
  bool isChanged = false;
  FOR_JOINTS_IDX(i) {
    if (prevPositions[i] != list[i]) {
      isChanged = true;
      break;
    }
  }

  // if not changed - not print
  if (!isChanged)
    return;

  // print positions and copy changed
  Serial.print("[ POS ]: ");
  FOR_JOINTS_IDX(i) {
    if (prevPositions[i] != list[i]) {
      Serial.print("<");
      Serial.print(list[i]);
      Serial.print("> ");
      prevPositions[i] = list[i];
      continue;
    }
    Serial.print(list[i]);
    Serial.print(" ");
  }
  Serial.println("");
}


// ------- Loop function --------
void loop_readPrintJointsStats() {
  readAllJointsPositions(actualJointsPositions);
  printPositionsIfChanged(actualJointsPositions);
}
