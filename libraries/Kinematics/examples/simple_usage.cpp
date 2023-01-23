//
// Created by Сергей Тяпкин on 22.01.2023.
//
#include "../Kinematics.h"
#include <iostream>
using namespace std;


int main() {
  const pos currentPoses[JOINTS_COUNT] = {512, 512, 0, 0, 2048, 2048};
  pos newPoses[JOINTS_COUNT];
  getAnglesByTargetPoint(200, 300, 200, currentPoses, newPoses);

  FOR_JOINTS_IDX(i) {
    cout << "[" << i << "]:" << newPoses[i] << " ";
  }
  cout << endl;

  return 0;
}
