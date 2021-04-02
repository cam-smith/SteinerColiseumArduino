
#include "U8glib.h"
#include <AccelStepper.h>

U8GLIB_ST7920_128X64_1X u8g(23, 17, 16);
int switchPin =3;
int encPin1 =31;
int encPin2 =33;
int encPin3 =35;


AccelStepper stepper1(AccelStepper::FULL4WIRE); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepper2(AccelStepper::FULL4WIRE, 6, 7, 8, 9);
AccelStepper stepper3(AccelStepper::FULL2WIRE, 10, 11);


uint8_t randNum = 0;
uint8_t randNum2 = 0;
int wallsTargets[]  =   { 0,  0,  0,  0,  0,  0};
int oldWallsTargets[] = { 0,  0,  0,  0,  0,  0};
int wallsAngles[]   = {-1, -1, -1, -1, -1, -1};
int buttonVal = 0;
int oldButtonVal =0;
long lastDebounceTime =0;
long debounceDelay=100;
int rolling = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  randomSeed(analogRead(0));

  pinMode(switchPin, INPUT);
}



void draw(void) {
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);


  //u8g.drawStr( 0, 0, "Hello World!" );
  if(rolling==0){
      drawDice(5, 10, randNum);
      if (randNum == 1 || randNum == 6) {
        drawDice(30, 10, randNum2);
      }else{
        randNum2=-1;
      }
      char msg[4];

      //u8g.drawStr(0, 50, itoa(randNum, msg, 10));

      drawWalls(wallsTargets);
  }else{
    drawDice(5,10, random(1,7));
    drawWalls(wallsTargets);
    rolling -=1;
  }

}

void drawWalls(int walls[]){
  
  u8g.setFont(u8g_font_unifont_67_75);
  for (int i = 0; i < 6; i++) {
    //u8g.drawXBMP(10*i, 60, 16,16,  (walls[i]==1?0x8b:0x93));
    if (walls[i]){
      if (randNum2-1==i){
        // Transition to Up
        u8g.drawStr(16 * i, 60, "\x51");
      }else{
        // already up
        u8g.drawStr(16 * i, 60, "\x11");
      }
    }else{
      if (randNum2-1==i){
        // transition down
        u8g.drawStr(16 * i, 60, "\x53");
      }else{
        // already down
        u8g.drawStr(16 * i, 60, "\x13");
      }      
    }
    
  }
  u8g.setFont(u8g_font_unifont);
}
void drawDice(uint8_t x, uint8_t y, uint8_t val) {

  u8g.drawRFrame(0 + x, 0 + y, 20, 19, 3);

  switch (val) {
    case 1:
      u8g.drawDisc(9 + x, 9 + y, 2);
      break;
    case 2:
      u8g.drawDisc(5 + x, 4 + y, 2);

      u8g.drawDisc(14 + x, 14 + y, 2);
      break;
    case 3:
      u8g.drawDisc(5 + x, 4 + y, 2);

      u8g.drawDisc(9 + x, 9 + y, 2);

      u8g.drawDisc(14 + x, 14 + y, 2);
      break;
    case 4:
      u8g.drawDisc(5 + x, 4 + y, 2);
      u8g.drawDisc(5 + x, 14 + y, 2);

      u8g.drawDisc(14 + x, 4 + y, 2);
      u8g.drawDisc(14 + x, 14 + y, 2);
      break;
    case 5:
      u8g.drawDisc(5 + x, 4 + y, 2);
      u8g.drawDisc(5 + x, 14 + y, 2);
      u8g.drawDisc(9 + x, 9 + y, 2);
      u8g.drawDisc(14 + x, 4 + y, 2);
      u8g.drawDisc(14 + x, 14 + y, 2);
      break;
    case 6:
      u8g.drawDisc(5 + x, 4 + y, 2);
      u8g.drawDisc(5 + x, 9 + y, 2);
      u8g.drawDisc(5 + x, 14 + y, 2);

      u8g.drawDisc(14 + x, 4 + y, 2);
      u8g.drawDisc(14 + x, 9 + y, 2);
      u8g.drawDisc(14 + x, 14 + y, 2);
      break;
  }

}



void changeWallStatus(){
  randNum = random(1, 7);
  if (randNum == 1 || randNum == 6) {
    randNum2 = random(1, 7);
    wallsTargets[randNum2 - 1] = !wallsTargets[randNum2 - 1];
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:




  buttonVal=digitalRead(switchPin);
  //Serial.print(buttonVal);
  // debounce
  if ((millis() - lastDebounceTime) > debounceDelay){
    
    // only trigger button logic if changed
    if (oldButtonVal!=buttonVal){
      if (!buttonVal){
        if (rolling==0){
        changeWallStatus();
        rolling=10;
        }
        
      }
      oldButtonVal=buttonVal;
    }
    
    lastDebounceTime = millis();
  }


  // Draw updated status.
  u8g.firstPage();
  do {
    draw();
  } while ( u8g.nextPage() );
  if (rolling>0)
    delay(200);
}
