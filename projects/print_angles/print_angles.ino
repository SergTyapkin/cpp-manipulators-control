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
  KEYS_SECTION;
  
  someDoes();

  delay(20);
}


// ------- Computer keyboard handling --------
void someDoes() {
  
}
