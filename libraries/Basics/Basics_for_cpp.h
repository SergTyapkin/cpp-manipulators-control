#ifndef INCLUDE_BASICS_
#define INCLUDE_BASICS_


//#include <Dynamixel2Arduino.h>
//using namespace ControlTableItem;


#define JOINTS_COUNT 6
#define BAUDRATE  1000000

#define ERR_INIT_DEVICE            -11
#define ERR_INIT_JOINT             -12
#define ERR_SET_JOINT_MODE         -13
#define ERR_READ_JOINT_POS         -14
#define ERR_SET_MAX_JOINT_VELOCITY -15
#define ERR_SET_JOINT_POSITION     -16
#define ERR_SET_JOINT_VELOCITY     -17
#define ERR_ENABLE_JOINT           -18
#define ERR_DISABLE_JOINT          -19

#define MAX_JOINT_VELOCITY_PERCENT 5 // On first joint will be SPEED x2

#define MIN_RPM 1

typedef float pos;
typedef unsigned id;


//#define DXL_SERIAL Serial3
//const uint8_t DXL_DIR_PIN = 22;

//const float DXL_PROTOCOL_VERSION = 1.0;

// ---
#define POS_MIN 0.0
#define POS_MAX 360.0
const float minPoses[JOINTS_COUNT] = {
    POS_MIN, // min 1
    73, // min 2
    83, // min 3
    78, // min 4
    3,  // min 5
    120, // min 6
};
const float maxPoses[JOINTS_COUNT] = {
    POS_MAX, // max 1
    283, // max 2
    235, // max 3
    259, // max 4
    245, // max 5
    193, // max 6
};
#define MIN_POS(idx) minPoses[idx]
#define MAX_POS(idx) maxPoses[idx]

// --- MACROSes
#define FOR_JOINTS_IDX(name_of_var) for (id name_of_var = 0; name_of_var < JOINTS_COUNT; name_of_var++)
#define FOR_JOINTS_ID(name_of_var) for (id name_of_var = 1; name_of_var <= JOINTS_COUNT; name_of_var++)
//
//#define ENTER_SECTION \
//  static unsigned __entersPressed = 0; \
//  bool __isEnterPressedNow = false;    \
//  char __readedSymbol = '\0';          \
//  if (Serial.available()) {            \
//    __readedSymbol = Serial.read();    \
//    if (__readedSymbol == '\n') {      \
//      __entersPressed += 1;            \
//      __isEnterPressedNow = true;      \
//    }                                  \
//    if (__readedSymbol == 'Q') {       \
//      FOR_JOINTS_ID(i)                 \
//        setJointVelocity(i, 7);        \
//      static pos __null_pos_1[] = {135, 180, 155, 164, 180, 170}; \
//      setAllJointsPositions(__null_pos_1);                        \
//      delay(3000);                     \
//      FOR_JOINTS_ID(i)                 \
//        setJointVelocity(i, 3);        \
//      static pos __null_pos_2[] = {135, 155, 132, 164, 180, 170}; \
//      setAllJointsPositions(__null_pos_2);                        \
//      delay(500);                      \
//      FOR_JOINTS_ID(i)                 \
//        setJointVelocity(i, 1);        \
//      delay(2500);    \
//      disableAll();   \
//      exit(0);        \
//    }                 \
//  }
//#define ON_ENTER(number) if (__isEnterPressedNow && __entersPressed == number)
//#define ON_ANY_ENTER if (__isEnterPressedNow)
////
//// --- Global vars
//extern Dynamixel2Arduino dxl;
//extern pos actualJointsPositions[JOINTS_COUNT]; // READONLY!! Always contains current joints positions
//extern float actualJointsVelocityDPS[JOINTS_COUNT]; // READONLY!! Always contains current joints velocity
//
//// ------- Default function --------
//void SETUP();
//void LOOP_PRINT_STATS();
//
//// ------- Set positions functions --------
//void setJointPosition(id idx, pos position);
//void setAllJointsPositions(pos* jointsPositions);
//// ------- Set velocity functions --------
//void setJointVelocity(id idx, float percents);
//void setJointVelocityDPS(id idx, float dps);
//void setAllJointsVelocity(float* jointsVelocity);
//void setAllJointsVelocityDPS(float* jointsVelocity);
//// ------- Work with torque functions --------
//void enable(id idx);
//void disable(id idx);
//void enableAll();
//void disableAll();
//
//
//// -------- Read stats --------
//void readAllJointsPositions(pos* targetList);
//void readAllJointsVelocity(float* targetList);
//void readAllJointsVelocityDPS(float* targetList);
//void readAllJointsCurrent(float* targetList);
//// ------- Print stats --------
//void _printList(const pos* list);
//void printPositions(const pos* list);
//void printVelocity(const pos* list);
//void printCurrent(const pos* list);
//void printPositions(const pos* list);
//
//void printPositionsIfChanged(const pos* list);


#endif //  INCLUDE_BASICS_
