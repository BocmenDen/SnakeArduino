#include "SnakeDrawMethods.h"
#include "SnakeShared.h"

#ifndef Snake_h
#define Snake_h

#pragma region Локальные переменные 
Direction _snakeCurrendDirection = Direction::None;
Point _snakeCurrentOffset;
Point _snakeFood;
uint16_t _snakeSize = 0;

Barrier _snakeBarriers[1 + SNAKE_COUNT_BARRIERS] = {};

Point _snakeStack[SNAKE_COUNT_SEGMENT];
int16_t _snakeHeadPointer = 0;
#pragma endregion
#pragma region Работа с "стеком" хранения сегментов змейки. Запись происходит анологично кольцевому буферу
INLINE void SnakeClearStack(){
    memset((void *)&_snakeStack, 0, SNAKE_COUNT_SEGMENT * sizeof(Point)); // TODO наверное можно удалить
    _snakeHeadPointer = 0;
}
INLINE Point SnakeHead(){
    return _snakeStack[_snakeHeadPointer];
}
void SnakePushStack(Point point){
    _snakeHeadPointer = CYCLE_CONSTRAINT(_snakeHeadPointer + 1, 0, SNAKE_COUNT_SEGMENT - 1);
    _snakeStack[_snakeHeadPointer] = point;
}
Point SnakeGetValStack(uint8_t index){
    int16_t tmp = _snakeHeadPointer - index;
    if (tmp < 0)
        tmp += SNAKE_COUNT_SEGMENT;
    return _snakeStack[tmp];
}
#pragma endregion
#pragma region Внутренние методы 
bool SnakeCheckColisionCell(Point segment){
    Point tmp;
    for (size_t i = 0; i < _snakeSize; i++)
    {
        tmp = SnakeGetValStack(i);
        if (segment.X == tmp.X && segment.Y == tmp.Y)
            return true;
    }
    return false;
}
bool SnakeIsPintInBarrier(uint8_t x, uint8_t y, Barrier barrier){
    if (x >= barrier.UL.X &&
        y >= barrier.UL.Y &&
        x <= barrier.DR.X &&
        y <= barrier.DR.Y)
    {
        return true;
    }
    return false;
}
bool SnakeCheckColisionBarriers(uint8_t x, uint8_t y){
    for (size_t i = 0; i < (1 + SNAKE_COUNT_BARRIERS); i++)
        if(SnakeIsPintInBarrier(x, y, _snakeBarriers[i])) return true;
    return false;
}
Point SnakeRandomPoint()
{
    Point p;
    do{
        p = (Point){random(0, SNAKE_COLUMN_COUNT), random(0, SNAKE_ROW_COUNT)};
    }while(SnakeCheckColisionCell(p) || SnakeCheckColisionBarriers(SNAKE_CENTER_PARAMS(p.X, p.Y)));
    return p;
}
INLINE bool SnakeBorderIsNotValid(Barrier barrier){
    if(SnakeCheckColisionBarriers(barrier.UL.X, barrier.UL.Y)) return true;
    if(SnakeCheckColisionBarriers(barrier.DR.X, barrier.DR.Y)) return true;
    Point tmp = {barrier.DR.X, barrier.UL.Y};
    if(SnakeCheckColisionBarriers(tmp.X, tmp.Y)) return true;
    tmp = {barrier.UL.X, barrier.DR.Y};
    if(SnakeCheckColisionBarriers(tmp.X, tmp.Y)) return true;
    for (size_t i = 0; i < _snakeSize; i++)
    {
        tmp = SnakeGetValStack(i);
        if(SnakeIsPintInBarrier(SNAKE_CENTER_PARAMS(tmp.X, tmp.Y), barrier)) return true;
    }
    return false;
}
INLINE Barrier SnakeRandomBarrier(){
    Barrier b;
    do{
        uint8_t x, y;
        if(random() & 1){
            x = SNAKE_BARRIER_SIZE_X;
            y = SNAKE_BARRIER_SIZE_Y;
        }else{
            x = SNAKE_BARRIER_SIZE_Y;
            y = SNAKE_BARRIER_SIZE_X;
        }
        b.UL.X = random(SNAKE_INNER_POS_UL_X, SNAKE_INNER_POS_DR_X + 1 - x);
        b.UL.Y = random(SNAKE_INNER_POS_UL_Y, SNAKE_INNER_POS_DR_Y + 1 - y);
        b.DR.X = b.UL.X + x;
        b.DR.Y = b.UL.Y + y;
    }while(SnakeBorderIsNotValid(b));
    return b;
}
void SnakeGenerateBarriers(){
    for (size_t i = 0; i < SNAKE_COUNT_BARRIERS; i++)
    {
        SnakeDrawBarrier(_snakeBarriers[i], false);
        _snakeBarriers[i] = SnakeRandomBarrier();
        SnakeDrawBarrier(_snakeBarriers[i], true);
    }
}
INLINE Point SnakeGetNewHead(Point pointHead)
{
    switch (_snakeCurrendDirection)
    {
    case Direction::Up:
        return (Point){pointHead.X, CYCLE_CONSTRAINT(pointHead.Y - 1, 0, SNAKE_ROW_COUNT - 1)};
    case Direction::Down:
        return (Point){pointHead.X, CYCLE_CONSTRAINT((int8_t)pointHead.Y + 1, 0, SNAKE_ROW_COUNT - 1)};
    case Direction::Left:
        return (Point){CYCLE_CONSTRAINT((int8_t)pointHead.X - 1, 0, SNAKE_COLUMN_COUNT - 1), pointHead.Y};
    case Direction::Right:
        return (Point){CYCLE_CONSTRAINT(pointHead.X + 1, 0, SNAKE_COLUMN_COUNT - 1), pointHead.Y};
    }
}
void SnakeGenerateFood(){
    SnakeDrawFood(_snakeFood, OLED_CLEAR);
    _snakeFood = SnakeRandomPoint();
    SnakeDrawFood(_snakeFood, OLED_FILL);
}
#pragma endregion
#pragma region Методы нацеленные на вызов из вне
void SnakeSetDirection(Direction inputDirection){
    if(inputDirection == Direction::None ||
       abs(inputDirection) == abs(_snakeCurrendDirection)
    ) return;
    _snakeCurrendDirection = inputDirection;
}
INLINE void SnakeStart()
{
    oled.clear();
    SnakeFieldDraw();
    SnakeClearStack();
    SnakePushStack(SnakeRandomPoint());
    _snakeCurrendDirection = Direction::None;
    memset((void *)&_snakeBarriers, 255, SNAKE_COUNT_BARRIERS * 2 * sizeof(Point));
    _snakeFood = {255, 255};
    SnakeGenerateFood();
    SnakeGenerateBarriers();
    _snakeSize = 1;
    SnakeDrawScore(_snakeSize);
    SnakeEventStart();
    oled.update();
}
INLINE void SnakeTick()
{
    if(_snakeCurrendDirection == Direction::None) return;
    Point head = SnakeHead();
    Point nHead = SnakeGetNewHead(head);
    if (nHead.X == 255 && nHead.Y == 255)
        return;
    if (SnakeCheckColisionCell(nHead) ||
        SnakeCheckColisionBarriers(SNAKE_CENTER_PARAMS(nHead.X, nHead.Y)))
    {
        if(SnakeEventGameEnd(false, _snakeSize)) SnakeStart();
        return;
    }
    SnakePushStack(nHead);
    if (_snakeFood.X == nHead.X && _snakeFood.Y == nHead.Y)
    {
        _snakeSize++;
#if SNAKE_BARRIER_GENERATE_TICK
        SnakeGenerateBarriers();
#endif
        SnakeGenerateFood();
        SnakeDrawScore(_snakeSize);
        SnakeEventEditSize(_snakeSize);
        if (_snakeSize == (SNAKE_COUNT_SEGMENT - 1))
        {
            if(SnakeEventGameEnd(true, _snakeSize)) SnakeStart();
            return;
        }
    }
    else
    {
        SnakeDrawHead(SnakeGetValStack(_snakeSize), false);
    }
    SnakeDrawHead(nHead, true);
    if(_snakeSize > 1)
        SnakeHeadToSegmentEditor(head);
}
#pragma endregion

#endif