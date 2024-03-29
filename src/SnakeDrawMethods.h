#include "SnakeShared.h"
#include "InitData.h"

#ifndef SnakeDrawMethods_h
#define SnakeDrawMethods_h


void _snakeDrawHead(Point point, byte color){
    DRAW_RECT_UPDATE(oled,
                    SNAKE_CELL_UL_X(point.X),
                    SNAKE_CELL_UL_Y(point.Y),
                    SNAKE_CELL_DR_X(point.X),
                    SNAKE_CELL_DR_Y(point.Y),
                    color);
}
INLINE void SnakeDrawHead(Point point, bool color){
    _snakeDrawHead(point, color ? OLED_STROKE : OLED_CLEAR);
}
void SnakeHeadToSegmentEditor(Point point){
    _snakeDrawHead(point, OLED_FILL);
}
void SnakeDrawFood(Point point, byte color){
    oled.circle(
        SNAKE_CELL_CENTER_X(point.X),
        SNAKE_CELL_CENTER_Y(point.Y),
        SNAKE_CELL_MARGIN,
        color);
    oled.update(
        SNAKE_RECT_PARAMS(point.X, point.Y));
}
INLINE void SnakeFieldDraw(){
  DRAW_RECT_UPDATE(oled,
    SNAKE_INNER_POS_UL_X - 1,
    SNAKE_INNER_POS_UL_Y - 1,
    SNAKE_INNER_POS_DR_X + 1,
    SNAKE_INNER_POS_DR_Y + 1,
    OLED_STROKE
  );
}
INLINE void SnakeDrawScore(uint8_t value){
    oled.setCursorXY(SNAKE_SCORE_POS_X, SNAKE_SCORE_POS_Y);
    char buffer[4];
    snprintf(buffer,sizeof(buffer), "%03d", value);
    oled.print(buffer);
    oled.update(
        SNAKE_SCORE_POS_X,
        SNAKE_SCORE_POS_Y,
        SNAKE_SCORE_POS_X + 24,
        SNAKE_SCORE_POS_Y + 8
    );
}
void SnakeDrawBarrier(Barrier barrier, bool color){
    DRAW_RECT_UPDATE(oled,
        border.UL.X,
        border.UL.Y,
        border.DR.X,
        border.DR.Y,
        color ? OLED_FILL : OLED_CLEAR
    );
}
#endif