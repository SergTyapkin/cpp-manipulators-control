#include <Basics.h>


void setup() {
  SETUP();
  //disableAll();
  
  setJointSpeed(2, 2);
  setJointSpeed(3, 4);
  setJointSpeed(4, 3);
}


void loop() {
  LOOP_PRINT_CHANGED_STATS();
  ENTER_SECTION;
  
  someDoes();

  delay(20);
}


// ------- Computer keyboard handling --------
void someDoes() {
  #define BETWEEN 2500
  
  const float DOWN[3] = {148, 190, 130};
  const float UP[3] = {186, 273, 253};
  const unsigned ids[3] = {2, 3, 4};

  setJointPosition(ids[0], UP[0]); // -1 0 1
  setJointPosition(ids[2], DOWN[2]);
  delay(BETWEEN / 2);
  setJointPosition(ids[1], DOWN[1]); // 0 1 0
  delay(BETWEEN / 2);
  setJointPosition(ids[0], DOWN[0]); // 1 0 -1
  setJointPosition(ids[2], UP[2]);
  delay(BETWEEN / 2);
  setJointPosition(ids[1], UP[1]); // 0 -1 0
  delay(BETWEEN / 2);
}
