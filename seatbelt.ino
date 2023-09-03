#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

const float PRESS = 100.0; // 로드센서의 임계값. 이 값을 넘으면 사람이 있음을 의미함
const int reedSW[4] = { 10, 11, 12, 13 }; //벨트를 채우면 1 안채우면 0
int reedValue_curr[4] = { // 리드스위치
  0,
};

char _String[4][7] = { "Empty", "Exist", "Gone ", "Belt " };
char tx_data[6] = {'M', 'E', 'G', 'B'};

HX711 scale[4];

float Weight[4] = {
  0.0,
};

uint8_t currentState[4] = {
  0,
};

char dispString[30] = {
  0x00,
};


void setup() {
  Serial.begin(115200);

  scale[0].begin(2, 3);
  scale[1].begin(4, 5);
  scale[2].begin(6, 7);
  scale[3].begin(8, 9);

  for (int i = 0; i < 4 ; i++ )
  {
    scale[i].set_scale(2280.f); // this value is obtained by calibrating the scale with known weights; see the README for details
    scale[i].tare();
  }

  for (int i = 0; i < 4; i++) {
    pinMode(reedSW[i], INPUT_PULLUP);
  }

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("trust us");
  delay(500);
  lcd.clear();


}

void loop() {

  for (int i = 0; i < 4; i++) {
    reedValue_curr[i] = digitalRead(reedSW[i]);
    Weight[i] = scale[i].get_units();

    //    Serial.print(" i : ");
    //    Serial.print(reedValue_curr[i]);
    //    Serial.print(" ");
    //    Serial.print(Weight[i]);
    //    Serial.print(" ");
  }
  //  Serial.println();


  for (int i = 0; i < 4; i++) {
    if (reedValue_curr[i] && Weight[i] > PRESS ) currentState[i] = 1; //
    else if ( Weight[i] > PRESS ) currentState[i] = 3; //
    else if ( currentState[i] == 0 ) ;
    else {
      currentState[i] = 2;
    }
  }

  sprintf(dispString, "%d:%s %d:%s", 1, _String[currentState[0]], 2, _String[currentState[1]]);
  lcd.setCursor(0, 0);
  lcd.print(dispString);
  sprintf(dispString, "%d:%s %d:%s", 3, _String[currentState[2]], 4, _String[currentState[3]]);
  lcd.setCursor(0, 1);
  lcd.print(dispString);


  Serial.print('1');
  Serial.print( tx_data[currentState[0]]);
  Serial.print('2');
  Serial.print( tx_data[currentState[1]]);
  Serial.print('3');
  Serial.print( tx_data[currentState[2]]);
  Serial.print('4');
  Serial.print( tx_data[currentState[3]]);
 
  delay(100);
}
