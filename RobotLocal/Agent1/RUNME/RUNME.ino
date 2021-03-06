/*
Code Authored by Keegan Kelly
*/
#include "RobotControl.h"
#define id 1
String server = "http://192.168.1.2:3000";
void setup(void)
{
  Serial.begin(115200);
}

void loop(void)
{
  Robot robotA(0, 0, pi / 2, id, server);
  robotA.getPath(1);
  int idx = robotA.pathDoc["id"].as<int>();
  int total = robotA.pathDoc["total"].as<int>();
  robotA.localize();
  robotA.moveTo(robotA.pathDoc["path"][0][0].as<float>(), robotA.pathDoc["path"][0][1].as<float>());
  robotA.setReady();

  int Ready = 0;
  while (!Ready)
  {
    Ready = robotA.getReady();
  }

  while (idx <= total)
  {
    robotA.getPath(idx);
    int len = robotA.pathDoc["path"].size();
    Serial.println(len);
    for (int i = 0; i < len; i++)
    {
      float prevTime = millis();
      int success = 0;
      while (!success)
      {
        success = robotA.localize();
      }
      robotA.moveTo(robotA.pathDoc["path"][i][0].as<float>(), robotA.pathDoc["path"][i][1].as<float>());
      while ((millis() - prevTime) * 0.001 < robotA.pathDoc["dt"].as<float>())
      {
        // do nothing until next time step
      }
    }
    idx++;
  }
  delay(5000);
}
