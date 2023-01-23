#include "Kinematics.h"
//#include <iostream>
using namespace std;


#define L0 100
#define L1 200
#define L2 300
#define L3 100

#define POS_MIN 0.0
#define POS_MAX 2048.0

const float minPoses[JOINTS_COUNT] = {
    POS_MIN, // min 1
    POS_MIN, // min 2
    POS_MIN, // min 3
    POS_MIN, // min 4
    POS_MIN, // min 5
    POS_MIN, // min 6
};
const float maxPoses[JOINTS_COUNT] = {
    POS_MAX, // max 1
    POS_MAX, // max 2
    POS_MAX, // max 3
    POS_MAX, // max 4
    POS_MAX, // max 5
    POS_MAX, // max 6
};
#define MIN_POS(idx) minPoses[idx]
#define MAX_POS(idx) maxPoses[idx]


float _delta(const float* phi, float x0, float y0, float z0) {
  #define F(idx) (phi[(idx - 1)] / POS_MAX * 2 * M_PI)
//  cout << phi[0] / POS_MAX << " ___ " << F(1) << " ___ " << cos(phi[0] / POS_MAX * 2 * M_PI) << endl;

  const float x = L1 * cos(F(1)) * cos(F(2)) + L2 * cos(F(1)) * cos(F(2) + F(3)) + L2 * cos(F(1)) * cos(F(2) + F(3) + F(4));
  const float y = L1 * cos(F(1)) * sin(F(2)) + L2 * cos(F(1)) * sin(F(2) + F(3)) + L2 * cos(F(1)) * sin(F(2) + F(3) + F(4));
  const float z = L0 + L1 * sin(F(2)) + L2 * sin(F(2) + F(3)) + L3 * sin(F(2) + F(3) + F(4));
  const float res = pow((x - x0), 2) + pow((y - y0), 2) + pow((z - z0), 2);
//  cout << "Delta: " << res << endl;
  return res;
}
float _penaltySum(const float* phi) {
  float res = 0;
  FOR_JOINTS_IDX(i) {
    if (phi[i] < MIN_POS(i))
      res += MIN_POS(i) - phi[i];
    else if (phi[i] > MAX_POS(i))
      res += phi[i] - MAX_POS(i);
  }
//  cout << "Penalty: " << res << endl;
  return res;
}
float _targetFunction(const float* phi, float x0, float y0, float z0) {
  return _delta(phi, x0, y0, z0) + _penaltySum(phi);
}

// ------

void getAnglesByTargetPoint(float x, float y, float z, const pos* currentPoses, pos* listToExportPoses) {
  float H = 0.01;

  float prevPoses[JOINTS_COUNT];
  float newPoses[JOINTS_COUNT];
  float tmpPoses[JOINTS_COUNT];
  FOR_JOINTS_IDX(i) {
//    cout << "[" << i << "]:" << currentPoses[i] << " ";
    prevPoses[i] = newPoses[i] = tmpPoses[i] = float(currentPoses[i]);
  }
//  cout << endl;

  // Реализуем поиск минимума через градиентный метод с дроблением шага
  const float targetEps = 0.1;
  float currentDelta = targetEps + 1;
  float currentDerivations[JOINTS_COUNT];
  float derivationsNorma = 0;
  while(currentDelta > targetEps) {  // Если смещение на прошлом шаге было достаточно велико, чтобы продолжить
    float deltaOfPosToFindDerivation = 1;  // для нахождения частной производной в точке нужно определить какое-то малое приращение
    FOR_JOINTS_IDX(i) {  // Для каждого параметра-угла поворота
      tmpPoses[i] = prevPoses[i] + deltaOfPosToFindDerivation;  // ищем производную - то есть добавляем небольшое смещение по этому прараметру

      const float derivation = (_targetFunction(prevPoses, x, y, z) - _targetFunction(tmpPoses, x, y, z)) / deltaOfPosToFindDerivation;  // и ищем численно производную
//      cout << i << ": " << derivation << endl;
      currentDerivations[i] = derivation;  // сохраняем производную
      derivationsNorma += pow(derivation, 2);

      tmpPoses[i] = prevPoses[i]; // теперь tmpPoses = prevPoses
    }
    derivationsNorma = sqrt(derivationsNorma);

    const float coeffTolerance = 0.1;
    const float coeffDecreasing = 0.95;
    FOR_JOINTS_IDX(i) {  // вычисляем новые значения для каждого параметра
      // дробление шага
      const float newPos = prevPoses[i] - H * currentDerivations[i];
      if (newPos > prevPoses[i] - coeffTolerance * H * derivationsNorma) {
//        H *= coeffDecreasing; // дробление шага отключено
      }
      newPoses[i] = prevPoses[i] - H * currentDerivations[i];  // шаг определен, спускаемся по градиенту. Если бы был +, то поднимались бы
    }

    currentDelta = 0;
    FOR_JOINTS_IDX(i) {
//      cout << "[" << i << "]:" << newPoses[i] << " ";
      currentDelta += newPoses[i] - prevPoses[i];  // Ищем общее смещение как сумму смещений по каждому из параметров
      prevPoses[i] = tmpPoses[i] = newPoses[i];  // Копируем новые значения в старые, чтобы начать новую итерацию
    }
//    cout << endl;
  }

  FOR_JOINTS_IDX(i) {
    listToExportPoses[i] = pos(newPoses[i]);
  }
}


void getPointByAngles(const pos* angles, float &exportX, float &exportY, float &exportZ) {
  #define F(idx) (angles[(idx - 1)])
  exportX =
    L1 * cos(F(1)) * cos(F(2)) +
    L2 * cos(F(1)) * cos(F(2) + F(3)) +
    L3 * cos(F(1)) * cos(F(2) + F(3) + F(4));
  exportY =
    L1 * cos(F(1)) * sin(F(2)) +
    L2 * cos(F(1)) * sin(F(2) + F(3)) +
    L3 * cos(F(1)) * sin(F(2) + F(3) + F(4));
  exportZ =
    L0 +
    L1 * sin(F(2)) +
    L2 * sin(F(2) + F(3)) +
    L3 * sin(F(2) + F(3) + F(4));
}
