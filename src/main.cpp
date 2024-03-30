#include <Arduino.h>
#include "InitData.h"
#include "Snake.h"
#include "Sound.h"
#include "FrameData.h"

uint64_t previousMillis = 0;

INLINE void InitButtons(){
  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);
  pinMode(BUTTON_LEFT, INPUT);
  pinMode(BUTTON_RIGHT, INPUT);
  pinMode(BUTTON_ENTER, INPUT);
}

INLINE void HelpInitMessage(){
  oled.fill(255);
  oled.invertText(true);
  oled.setCursor(5, 1);
  oled.setScale(2);
  oled.print(F("SNAKE GAME"));
  oled.setScale(1);
  oled.setCursor(20, 4);
  oled.print(F("After starting"));
  oled.setCursor(8, 5);
  oled.print(F("press the direction"));
  oled.setCursor(20, 6);
  oled.print(F("button to start"));
  oled.invertText(false);
  oled.update();
}

void setup() {
  oled.init();
  Wire.setClock(800000L);
  SoundInit();
  InitButtons();
  HelpInitMessage();
  delay(2000);
  SnakeStart();
}

INLINE void SnakeEventEditSize(uint8_t sizeSnake){
  SoundStart(0);
}

INLINE void SnakeEventStart(){
  SoundStart(0);
}

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > SCREEN_UPDATE) {
    SnakeTick();
    previousMillis = currentMillis;
  }
  if(CLICK(BUTTON_UP)){
    SnakeSetDirection(Direction::Up);
  }else if(CLICK(BUTTON_DOWN)){
    SnakeSetDirection(Direction::Down);
  }else if(CLICK(BUTTON_LEFT)){
    SnakeSetDirection(Direction::Left);
  }else if(CLICK(BUTTON_RIGHT)){
    SnakeSetDirection(Direction::Right);
  }
  SoundTick(currentMillis);
}

INLINE bool SnakeEventGameEnd(bool isWin, uint8_t score){
  SoundStart(isWin ? 0 : 1);
  oled.clear();
  oled.drawBitmap(0, 0, isWin ? youWin_128x64 : gameOver_128x64, 128, 64, BITMAP_NORMAL, BUF_ADD);
  oled.invertText(true);
  oled.setCursorXY(8, 32);
  oled.print(F("Click center button"));
  oled.setCursorXY(40, 48);
  oled.print(F("Score: "));
  oled.print(score);
  oled.update();
  oled.invertText(false);
  while(!CLICK(BUTTON_ENTER)) SoundTick();
  return true;
}