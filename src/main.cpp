#include <stdio.h>
#include <stdlib.h>



#define DAY 3

#define RUN_DAY(_day) int main() { day##_day(_day); system("pause"); }

#if DAY == 1
#include "day1.h"
RUN_DAY(1);

#elif DAY == 2
#include "day2.h"
RUN_DAY(2);

#elif DAY == 3
#include "day3.h"
RUN_DAY(3);

#endif
