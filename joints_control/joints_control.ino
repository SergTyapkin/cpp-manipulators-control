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

#define ERR_INIT_DEVICE    -11
#define ERR_INIT_JOINT     -12
#define ERR_SET_JOINT_MODE -13
#define ERR_READ_JOINT_POS -14


typedef int32_t pos;
typedef uint8_t index;


DynamixelWorkbench WorkBench;


void setup() {
  // --- Init serial port to log data in computer console
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  const char *log;
  bool result = false;


  // --- Init Dinamyxel device
  result = WorkBench.init(DEVICE_NAME, BAUDRATE, &log);
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
    result = WorkBench.ping(jointIdx, &modelNumber, &log);
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

    result = WorkBench.addSyncWriteHandler(jointIdx, "Goal_Position", &log);
    if (result == false) {
      Serial.println(log);
      Serial.print("Failed to set mode of joint ");
      Serial.println(jointIdx);
      exit(ERR_SET_JOINT_MODE);
    }
  }
}


void loop() {
  // --- Led blinking
//  digitalWrite(LED_BUILTIN, HIGH);
//  delay(1000);
//  digitalWrite(LED_BUILTIN, LOW);
//  delay(1000);

  pos newpos[] = {2048, 2048, 2048, 2048, 512, 512};
  SetJointsPositions(newpos);
  delay(20);
}


// ------- Set positions functions --------
void setAllJointsPos(pos* jointsPositions) {
  static const uint8_t handlerIndexPos = 1;
  const char *log;
  bool result = false;
  result = WorkBench.syncWrite(handlerIndexPos, jointsPositions, &log);
  if (result == false) {
    Serial.println(log);
    Serial.println("Failed to set all joint`s positions");
  }
}

void setJointPos(id idx, pos position) {
  const char *log;
  bool result = false;
  result = WorkBench.goalPosition(idx, position, &log);
  if (result == false) {
    Serial.println(log);
    Serial.println("Failed to set posisiton of joint");
  }
}
