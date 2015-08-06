#ifndef __CHECK_WATER_H
#define __CHECK_WATER_H

#include "bsp.h"

typedef void  (*pfunc)(int);

#define Power_Up      1
#define Power_Down    0

extern uint8_t Get_Status(pfunc delay, uint16_t ms);
extern void Check_Water(void);

#endif 
