#include <stdio.h>
#include <stdlib.h>

#define DAY 7

#define RUN_DAY() GOL_CONCAT(day,DAY)(DAY)

#if DAY == 1
#include "day1.h"
#elif DAY == 2
#include "day2.h"
#elif DAY == 3
#include "day3.h"
#elif DAY == 4
#include "day4.h"
#elif DAY == 5
#include "day5.h"
#elif DAY == 6
#include "day6.h"
#elif DAY == 7
#include "day7.h"
#endif

int main() 
{ 
	RUN_DAY();
	system("pause");
}

