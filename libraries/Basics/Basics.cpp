#include "Basics.h"


Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
pos actualJointsPositions[JOINTS_COUNT];
float actualJointsVelocityDPS[JOINTS_COUNT];


// ------- Default function --------
void SETUP() {
  pinMode(LED_BUILTIN, OUTPUT);

  // --- Init serial port to log data in computer console
  Serial.begin(BAUDRATE);
  while (!Serial) {;}

  // --- Init Dinamyxel device
  dxl.begin(BAUDRATE);
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  // --- Init every joint
  bool result = false;
  FOR_JOINTS_ID(jointId) {
    result = dxl.ping(jointId);
    if (result == false) {
      Serial.print("Failed to ping joint ID: ");
      Serial.println(jointId);
      exit(ERR_INIT_JOINT);
    }
    Serial.print("Succeeded to ping. ");
    Serial.print("id: [");
    Serial.print(jointId);

    uint16_t modelNumber = dxl.getModelNumber(jointId);
    Serial.print("] ModelNumber : ");
    Serial.println(modelNumber);

    result = dxl.setOperatingMode(jointId, OP_POSITION);
    if (result == false) {
      Serial.println(F("Failed to set position mode"));
      exit(ERR_SET_JOINT_MODE);
    }

    // --- Limit the maximum velocity in Position Control Mode. Use 0 to Max speed
    float speed = MAX_JOINT_VELOCITY_PERCENT;
    if (jointId == 1)
      speed *= 2;
    result = dxl.setGoalVelocity(jointId, speed, UNIT_PERCENT);
    if (result == false) {
      Serial.print(F("Failed to set max velocity of joint ID: "));
      Serial.println(jointId);
      exit(ERR_SET_MAX_JOINT_VELOCITY);
    }
  }
  Serial.println();
}

void LOOP_PRINT_STATS() {
  readAllJointsPositions(actualJointsPositions);
  readAllJointsVelocityDPS(actualJointsVelocityDPS);
  printPositionsIfChanged(actualJointsPositions);
}


// ------- Set positions functions --------
void setJointPosition(id idx, pos position) {
  Serial.print("[SET POS][");
  Serial.print(idx);
  Serial.print("]: ");
  Serial.println(position);
  if (position < MIN_POS(idx - 1) || position > MAX_POS(idx - 1)) {
    Serial.print("Failed to set position. Position for this point must be between ");
    Serial.print(MIN_POS(idx-1));
    Serial.print(" and ");
    Serial.println(MAX_POS(idx-1));
    exit(ERR_SET_JOINT_POSITION);
  }
  bool result = dxl.setGoalPosition(idx, position, UNIT_DEGREE);
  if (result == false) {
    Serial.print("Failed to set position ");
    Serial.print(position);
    Serial.print(" on joint ID: ");
    Serial.println(idx);
    exit(ERR_SET_JOINT_POSITION);
  }
}

void setAllJointsPositions(pos* jointsPositions) {
  FOR_JOINTS_ID(jointId) {
    setJointPosition(jointId, jointsPositions[jointId - 1]);
  }
}

// ------- Set velocity functions --------
void setJointVelocity(id idx, float percents) {
  Serial.print("[SET VEL][");
  Serial.print(idx);
  Serial.print("]: ");
  Serial.print(percents);
  Serial.println("%");
  bool result = dxl.setGoalVelocity(idx, percents, UNIT_PERCENT);
  if (result == false) {
    Serial.print("Failed to set velocity ");
    Serial.print(percents);
    Serial.print(" on joint ID: ");
    Serial.println(idx);
    exit(ERR_SET_JOINT_VELOCITY);
  }
}
void setJointVelocityDPS(id idx, float dps) {
  Serial.print("[SET VEL][");
  Serial.print(idx);
  Serial.print("]: ");
  Serial.print(dps);
  Serial.print(" DPS = ");
  float rpm = dps / 6.0;
  if (rpm < MIN_RPM)
    rpm = MIN_RPM;
  Serial.print(rpm);
  Serial.print(" RPM");
  Serial.println();
  bool result = dxl.setGoalVelocity(idx, rpm, UNIT_RPM);
  if (result == false) {
    Serial.print("Failed to set velocity DPS = ");
    Serial.print(dps);
    Serial.print(" RPM = ");
    Serial.print(rpm);
    Serial.print(" on joint ID: ");
    Serial.println(idx);
//    exit(ERR_SET_JOINT_VELOCITY);
  }
}

void setAllJointsVelocity(float* jointsVelocity) {
  FOR_JOINTS_ID(jointId) {
    setJointVelocity(jointId, jointsVelocity[jointId - 1]);
  }
}
void setAllJointsVelocityDPS(float* jointsVelocity) {
  FOR_JOINTS_ID(jointId) {
    setJointVelocityDPS(jointId, jointsVelocity[jointId - 1]);
  }
}
// ------- Work with torque functions --------
void enable(id idx) {
  bool result = dxl.torqueOn(idx);
  if (result == false) {
    Serial.print("Failed to enable joint ID: ");
    Serial.println(idx);
    exit(ERR_ENABLE_JOINT);
  }
}
void disable(id idx) {
  bool result = dxl.torqueOff(idx);
  if (result == false) {
    Serial.print("Failed to disable joint ID: ");
    Serial.println(idx);
    exit(ERR_ENABLE_JOINT);
  }
}
void enableAll() {
  FOR_JOINTS_ID(jointId) {
    enable(jointId);
  }
}
void disableAll() {
  FOR_JOINTS_ID(jointId) {
    disable(jointId);
  }
}



// -------- Read stats --------
void readAllJointsPositions(pos* targetList) {
  FOR_JOINTS_ID(jointId) {
    pos jointPos = dxl.getPresentPosition(jointId, UNIT_DEGREE);
    targetList[jointId - 1] = jointPos;
  }
}
void readAllJointsVelocity(float* targetList) {
  FOR_JOINTS_ID(jointId) {
    float jointVelocity = dxl.getPresentVelocity(jointId, UNIT_PERCENT);
    targetList[jointId - 1] = jointVelocity;
  }
}
void readAllJointsVelocityDPS(float* targetList) {
  FOR_JOINTS_ID(jointId) {
    const float rpmVelocity = dxl.getPresentVelocity(jointId, UNIT_RPM);
    const float dpsVelocity = rpmVelocity * 6.0;
    targetList[jointId - 1] = dpsVelocity;
  }
}
void readAllJointsCurrent(float* targetList) {
  FOR_JOINTS_ID(jointId) {
    float jointCurrent = dxl.getPresentCurrent(jointId, UNIT_MILLI_AMPERE);
    targetList[jointId - 1] = jointCurrent;
  }
}


// ------- Print stats --------
void _printList(const pos* list) {
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
void printVelocity(const pos* list) {
  Serial.print("[VEL](%): ");
  _printList(list);
}
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
//      for (int i = 10000 - list[i]; i >= 1000; i /= 10)
//        Serial.print(" ");
      Serial.print("> ");
      prevPositions[i] = list[i];
      continue;
    }
    Serial.print(" ");
    Serial.print(list[i]);
//    for (int i = 10000 - list[i]; i >= 1000; i /= 10)
//      Serial.print(" ");
    Serial.print("  ");
  }
  Serial.println("");
}
