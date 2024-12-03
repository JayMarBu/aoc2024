#include <stdio.h>
#include <stdlib.h>

#define DAY 2

#define RUN_DAY(_day) int main() { day##_day(); system("pause"); }

#if DAY == 1
#include "day1.h"
RUN_DAY(1);

#elif DAY == 2
#include "day2.h"
RUN_DAY(2);

#endif
