#include <Arduino.h>
#include "InitData.h"
#include "Snake.h"
#include "Sound.h"
#include "FrameData.h"

uint64_t previousMillis = 0;

INLINE void initButtons(){
  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);
  pinMode(BUTTON_LEFT, INPUT);
  pinMode(BUTTON_RIGHT, INPUT);
  pinMode(BUTTON_ENTER, INPUT);
}

void setup() {
  Serial.begin(9600);
  oled.init();
  oled.clear();
  SoundInit();
  initButtons();
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

INLINE bool SnakeEventGameEnd(bool isWin){
  SoundStart(isWin ? 0 : 1);
  oled.clear();
  oled.drawBitmap(0, 0, isWin ? youWin_128x64 : gameOver_128x64, 128, 64, BITMAP_NORMAL, BUF_ADD);
  oled.update();
  while(!CLICK(BUTTON_ENTER)) SoundTick();
  return true;
}