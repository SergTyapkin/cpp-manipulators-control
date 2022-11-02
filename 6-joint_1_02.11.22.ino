#include <DynamixelWorkbench.h>



#define DEVICE_NAME "3"
#define BAUDRATE  1000000
#define DXL_ID_1    1
#define DXL_ID_2    2
#define DXL_ID_3    3
#define DXL_ID_4    4
#define DXL_ID_5    5
#define DXL_ID_6    6

typedef int32_t pos;

typedef uint8_t id;

DynamixelWorkbench dxl_wb;



const uint8_t handler_index_pos = 1;
pos goal_position[6] = {512,512,512,512,512,512};

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  //while(!Serial); // Open a Serial Monitor

  const char *log;
  bool result = false;

  uint16_t model_number = 0;

  result = dxl_wb.init(DEVICE_NAME, BAUDRATE, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to init");
  }
  else
  {
    Serial.print("Succeeded to init : ");
    Serial.println(BAUDRATE);  
  }
  //Serial.println("Test_1");
  
  for (int i=1; i<=6; i++)
  {
    uint8_t dxl_id = i;
    result = dxl_wb.ping(dxl_id, &model_number, &log);
    if (result == false)
    {
      Serial.println(log);
      Serial.println("Failed to ping");
    }
    else
    {
      Serial.println("Succeeded to ping");
      Serial.print("id: ");
      Serial.print(dxl_id);
      Serial.print(" model_number : ");
      Serial.println(model_number);
    }
  } 

  result = dxl_wb.addSyncWriteHandler(1, "Goal_Position", &log);
  result = dxl_wb.addSyncWriteHandler(2, "Goal_Position", &log);
  result = dxl_wb.addSyncWriteHandler(3, "Goal_Position", &log);
  result = dxl_wb.addSyncWriteHandler(4, "Goal_Position", &log);
  result = dxl_wb.addSyncWriteHandler(5, "Goal_Position", &log);
  result = dxl_wb.addSyncWriteHandler(6, "Goal_Position", &log);


  //Serial.println("Test_4");
  Set_Pos_all(goal_position);
  //Serial.println("Test_5");

 
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  pos newpos[]  = {2048, 2048, 2048, 2048, 512, 512};
  Set_Pos_all(newpos);
  delay(20);



}



void Set_Pos_all(pos *goal_position) {
  const char *log;
  bool result = false;
  result = dxl_wb.syncWrite(handler_index_pos, goal_position, &log);
  if (result == false)
    {
      Serial.println(log);
      Serial.println("Failed to set all joint`s positions");
    }
}

void Set_Pos_single(id idx, pos position) {
  const char *log;
  bool result = false;
  result = dxl_wb.goalPosition(idx, position, &log);
  if (result == false)
    {
      Serial.println(log);
      Serial.println("Failed to set posisiton of joint");
    }
}
