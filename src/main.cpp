#include <Arduino.h>
#include <U8g2lib.h>
#include <ArduinoJson.h>

//I2C
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
/*
0是时钟模式
1是显示监视信息
2是动图
*/
char modeChar = '0';
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
String comData;
DynamicJsonDocument doc(128);
void setup()
{
  Serial.begin(1500000);
  while (!Serial)
  {
  }
  Serial.println("Serial Opened");
  Serial.write("ESP_ONLINE\n");
  u8g2.begin();
  u8g2.enableUTF8Print();
}
void loop()
{
  comData.clear();
  while (Serial.available() > 0)
  {
    comData += char(Serial.read());
    delay(1);
  }
  u8g2.clearBuffer(); // clear the internal memory
  DeserializationError error = deserializeJson(doc, comData);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  switch (modeChar)
  {
    case '0':
      modeChar = doc.getMember("m");
      Serial.println("No Data Recive");
      u8g2.setFont(u8g2_font_wqy16_t_gb2312a);
      u8g2.drawStr(8, 10, "Hello World!"); // write something to the internal memory
      break;
    case '1':
      if (!comData.isEmpty())
      {
        JsonObject obj = doc.as<JsonObject>();
        Serial.print(comData);
        const char *lStr = obj["l"];
        const char *vStr = obj["v"];
        u8g2.setFont(u8g2_font_wqy12_t_gb2312a);
        u8g2.drawStr(5, 20, lStr); // write something to the internal memory
        u8g2.setFont(u8g2_font_wqy16_t_gb2312a);
        u8g2.drawStr(5, 40, vStr); // write something to the internal memory
      }
      break;
    case '2':
      break;
  }
  u8g2.sendBuffer();
  delay(1);
}
