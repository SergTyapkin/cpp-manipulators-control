bool findBlockAndLineIntersections(const float* lineStart, const float* lineEnd, const float* blockStart, const float* blockEnd, float* exportPoint1, float* exportPoint2) {
  // Сортировка координат параллелепипеда
  const float blockXMin = min(blockStart[0], blockEnd[0]);
  const float blockYMin = min(blockStart[1], blockEnd[1]);
  const float blockZMin = min(blockStart[2], blockEnd[2]);
  const float blockXMax = max(blockStart[0], blockEnd[0]);
  const float blockYMax = max(blockStart[1], blockEnd[1]);
  const float blockZMax = max(blockStart[2], blockEnd[2]);

  const float deltaLength = 0.1;
  // Получение вектора из начала в конец линии
  float dx = (lineStart[0] - lineEnd[0]);
  float dy = (lineStart[1] - lineEnd[1]);
  float dz = (lineStart[2] - lineEnd[2]);
  // Нормирование вектора, чтобы его длина была равна deltaLength
  const float lineLength = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
  const float coeff = len / deltaLength;
  dx /= coeff;
  dy /= coeff;
  dz /= coeff;

  // Теперь просто идём по чуть-чуть из начала в конец линии, пока не найдем точку внутри прямоугольника. Это вход. Потом найдём точку вне прямоугольника. Это выходная точка.
  const unsigned iterationsCount = lineLength / deltaLength;
  bool isPoint1Found = false;
  for (unsigned i = 0; i < iterationsCount; i++) {
    float x = lineStart[0] + dx * i;
    float y = lineStart[1] + dy * i;
    float z = lineStart[2] + dz * i;

    // если точка внутри параллелепипеда
    if (
        (blockXMin <= x && x <= blockXMax) &&
        (blockYMin <= y && y <= blockYMax) &&
        (blockZMin <= z && z <= blockZMax)
      ) {
      // сохраняем точку между текущей и предыдущей, как входную или выходную
      if (!isPoint1Found) {
        exportPoint1[0] = x - dx / 2;
        exportPoint1[1] = y - dy / 2;
        exportPoint1[2] = z - dz / 2;
        isPoint1Found = true;
      } else {
        exportPoint2[0] = x - dx / 2;
        exportPoint2[1] = y - dy / 2;
        exportPoint2[2] = z - dz / 2;
      }
    }
  }
  return isPoint1Found; // ложь, если линия не пересекается с параллелепипедом
}


float** _allocateFloatArrayMem(size_t size) {
  float** arr = (float**)malloc(size * sizeof(float*));
  for (unsigned i = 0; i < size; i++) {
    arr[i] = (float*)malloc(3 * sizeof(float));
  }
}

float** generateWay(const float* start, const float* end, float** &way, unsigned &waypointsCount) {
  way = NULL;
  waypointsCount = 0;

  for (unsigned obstacleIdx = 0; obstacleIdx < OBSTACLES_COUNT; obstacleIdx++) {
    float point1[3];
    float point2[3];
    bool isIntersect = findBlockAndLineIntersections(start, end, obstacles[obstacleIdx][0], obstacles[obstacleIdx][1], point1, point2);

    if (!isIntersect)
      continue;

    // Если пересекается, нужно сместить эти точки в ближайшие к ним вершины параллелепипеда
    // Сортировка координат параллелепипеда
    const float xMin = min(obstacles[obstacleIdx][0][0], obstacles[obstacleIdx][1][0]);
    const float yMin = min(obstacles[obstacleIdx][0][1], obstacles[obstacleIdx][1][1]);
    const float zMin = min(obstacles[obstacleIdx][0][2], obstacles[obstacleIdx][1][2]);
    const float xMax = max(obstacles[obstacleIdx][0][0], obstacles[obstacleIdx][1][0]);
    const float yMax = max(obstacles[obstacleIdx][0][1], obstacles[obstacleIdx][1][1]);
    const float zMax = max(obstacles[obstacleIdx][0][2], obstacles[obstacleIdx][1][2]);

    const float vertices[8][3] = {
      {xMin, yMin, zMin}, {xMax, yMin, zMin}, {xMin, yMax, zMin}, {xMax, yMax, zMin},
      {xMin, yMin, zMax}, {xMax, yMin, zMax}, {xMin, yMax, zMax}, {xMax, yMax, zMax}
    };
    // Ищем индексы ближайших вершин к каждой из точек
    float minDist1 = 999999999;
    float minDist2 = 999999999;
    unsigned minVertexIdx1;
    unsigned minVertexIdx2;
    for (unsigned i = 0; i < 8; i++) {
      const float x = vectices[i][0];
      const float y = vectices[i][1];
      const float z = vectices[i][2];
      const float dist1 = sqrt(pow(x - point1[0], 2) + pow(y - point1[1], 2) + pow(z - point1[2], 2));
      const float dist2 = sqrt(pow(x - point2[0], 2) + pow(y - point2[1], 2) + pow(z - point2[2], 2));

      if (dist1 < minDist1) {
        minDist1 = dist1;
        minVertexIdx1 = i;
      }
      if (dist2 < minDist2) {
        minDist2 = dist2;
        minVertexIdx2 = i;
      }
    }

    #define i1 minVectexIdx1
    #define i2 minVectexIdx2
    #define i1Coord(dir_idx) vertices[minVectexIdx1][dir_idx]
    #define i2Coord(dir_idx) vertices[minVectexIdx2][dir_idx]
    #define indexesIs(idx1, idx2) (min(i1, i2) == idx1 && max(i1, i2) == idx2)
    // Есть три варианта.
    // 1. Это одна и та же точка. i1 = i2
    // 2. Вершины соединены ребром. i1 = i2 +- 1 || (0, 2) || (4, 6) || i1 = i2 +- 4
    // 3. Вершины находятся на одной грани. (0, 5) (1, 4) (1, 7) (3, 5) (3, 6) (2, 7) (2, 4) (0, 6) (0, 3) (1, 2) (4, 7) (5, 6)
    // 4. Вершины противоположны, на диагонали параллелепипеда (0, 7) (1, 6) (3, 4) (2, 5)
    if (i1 == i2) {  // 1-ый случай
      waypointsCount = 3;
      way = _allocateFloatArrayMem(waypointsCount);
      way[1][0] = i1Coord[0];
      way[1][1] = i1Coord[1];
      way[1][2] = i1Coord[2];
    } else if (indexesIs(0, 7) || indexesIs(1, 6) || indexesIs(3, 4) || indexesIs(2, 5)) {  // 4-ый случай
      waypointsCount = 5;
      way = _allocateFloatArrayMem(waypointsCount);
      way[1][0] = i1Coord[0];
      way[1][1] = i1Coord[1];
      way[1][2] = i1Coord[2];
      
      unsigned centralVerticeIdx;
      if (indexesIs(0, 7))
        centralVerticeIdx = 4;
      else if (indexesIs(1, 6)
        centralVerticeIdx = 5;
      else if (indexesIs(3, 4)
        centralVerticeIdx = 7;
      else // (2, 5)
        centralVerticeIdx = 6;
      way[2][0] = vertices[centralVerticeIdx][0];
      way[2][1] = vertices[centralVerticeIdx][1];
      way[2][2] = vertices[centralVerticeIdx][2];
      
      way[3][0] = i2Coord[0];
      way[3][1] = i2Coord[1];
      way[3][2] = i2Coord[2];
    } else { // 2-ой случай или 3-ий случай
      waypointsCount = 4;
      way = _allocateFloatArrayMem(waypointsCount);
      way[1][0] = i1Coord[0];
      way[1][1] = i1Coord[1];
      way[1][2] = i1Coord[2];
      
      way[2][0] = i2Coord[0];
      way[2][1] = i2Coord[1];
      way[2][2] = i2Coord[2];
    }
    
    // первая точка
    way[0][0] = start[0];
    way[0][1] = start[1];
    way[0][2] = start[2];
    // последняя точка
    way[waypointsCount-1][0] = start[0];
    way[waypointsCount-1][1] = start[1];
    way[waypointsCount-1][2] = start[2];
    return way;
  }

  // пересечений с объектами не найдено. Просто копируем начало и конец
  waypointsCount = 2;
  way = _allocateFloatArrayMem(waypointsCount);
  way[0][0] = start[0];
  way[0][1] = start[1];
  way[0][2] = start[2];
  
  way[1][0] = end[0];
  way[1][1] = end[1];
  way[1][2] = end[2];
  return way;
}


void freeWayMemory(float** &way, unsigned &waypointsCount) {
  for (unsigned i = 0; i < waypointsCount; i++) {
    free(way[i]);
  }
  free(way);
  way = NULL;
  waypointsCount = 0;
}
