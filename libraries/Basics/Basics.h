#ifndef INCLUDE_BASICS_
#define INCLUDE_BASICS_


//#include <Dynamixel2Arduino.h>
//using namespace ControlTableItem;


#define JOINTS_COUNT 6
#define BAUDRATE  57600

#define ERR_INIT_DEVICE            -11
#define ERR_INIT_JOINT             -12
#define ERR_SET_JOINT_MODE         -13
#define ERR_READ_JOINT_POS         -14
#define ERR_SET_MAX_JOINT_VELOCITY -15
#define ERR_SET_JOINT_POSITION     -16
#define ERR_SET_JOINT_VELOCITY     -17


typedef int pos;
typedef unsigned id;

//
//#define DXL_SERIAL Serial
//#define DXL_DIR_PIN 22
//
//#define DXL_PROTOCOL_VERSION 1.0

// --- MACROSes
#define FOR_JOINTS_IDX(name_of_var) for (id name_of_var = 0; name_of_var < JOINTS_COUNT; name_of_var++)
#define FOR_JOINTS_ID(name_of_var) for (id name_of_var = 1; name_of_var <= JOINTS_COUNT; name_of_var++)

#define ON_ENTER_SECTION static unsigned __entersPressed = 0; if (Serial.available() == 0 || Serial.read() != '\n') { __entersPressed += 1; } if (Serial.available() == 0 || Serial.read() != '\n')
#define ON_ENTER(number) if (entersPressed == number)
//
//// --- Global vars
//extern Dynamixel2Arduino dxl;
//extern pos actualJointsPositions[JOINTS_COUNT]; // READONLY!! Always contains current joints positions
//
//// ------- Default function --------
//void setup_basic();
//
//
//// ------- Set positions functions --------
//void setJointPosition(id idx, pos position);
//
//void setAllJointsPositions(pos* jointsPositions);
//
//// -------- Read positions --------
//void readAllJointsPositions(pos* targetList);
//
//
//// ------- Print positions --------
//void printPositions(const pos* list);
//
//void printPositionsIfChanged(const pos* list);
//
//
//// ------- Loop function --------
//void loop_readPrintJointsStats();


#endif //  INCLUDE_BASICS_
