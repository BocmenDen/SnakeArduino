#include <GyverOLED.h>

#ifndef InitData_h
#define InitData_h

// =========================================================================== Пищалка
#define BUZZER 2
// =========================================================================== Параметры экрана
#define OLED_SIZE_X 128
#define OLED_SIZE_Y 64
// =========================================================================== Пины кнопок
#define BUTTON_UP 6
#define BUTTON_DOWN 5
#define BUTTON_LEFT 4
#define BUTTON_RIGHT 7
#define BUTTON_ENTER 8
#define BUTTON_HIGH 1
// =========================================================================== Параметры игры
#define SCREEN_UPDATE 200
#define SCREEN_UPDATE_PROGRESS_DECREMENT ((uint16_t)(((uint16_t)(SCREEN_UPDATE * 0.7)) / (SNAKE_COUNT_SEGMENT - 1)))

#define SNAKE_CELL_MARGIN 1
#define SNAKE_FIELD_POS_X 0
#define SNAKE_FIELD_POS_Y 0
#define SNAKE_COLUMN_COUNT 34
#define SNAKE_ROW_COUNT 20

#define SNAKE_COUNT_SEGMENT 30 /* Максимальное количество сегментов у змейки, также является максимальным значением счёта */
#define SNAKE_COUNT_BARRIERS 2 /* Кол-во препятствий на игровом поле, не рекомендуется указывать большие значения */

#define SNAKE_BARRIER_SIZE_X 10 /* Размеры барьера по X и Y, но генерация их также меняет местами */
#define SNAKE_BARRIER_SIZE_Y 5  /* для сохздание как горизонтальных так и вертикальных препятствий  */

#define SNAKE_BARRIER_GENERATE_TICK true /* Разрешить генерировать препятствия во время игры? (когда змейка съедает еду) */

#define SNAKE_SCORE_POS_X 108
#define SNAKE_SCORE_POS_Y 28
// =========================================================================== Макросы
#define SNAKE_CELL_SEIZE (SNAKE_CELL_MARGIN * 2 + 1)
#define INLINE inline __attribute__((always_inline))
#define CLICK(_BUTTON_) (digitalRead(_BUTTON_) == BUTTON_HIGH)
#define DRAW_RECT_UPDATE(_OLED_, x0, y0, x1, y1, fill) _OLED_.rect(x0, y0, x1, y1, fill);\
                                                       _OLED_.update(x0, y0, x1, y1)
#define CYCLE_CONSTRAINT(amt, low, high) ((amt) < (low) ? (high) : ((amt) > (high) ? (low) : (amt)))

GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

#endif