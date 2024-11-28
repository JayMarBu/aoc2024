#include "AocUtils.h"

#include "gol/FileSystem.h"
#include <stdio.h>

void day1()
{
	gol::File file = AoC::LoadData(1, true);

	char buff[32];
	if (file.ReadTextLine(buff, 32))
		printf("%s\n", buff);
	else
		printf("file failed to load :(\n");

	file.Close();
}
