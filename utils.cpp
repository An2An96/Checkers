#include "utils.h"

int random(int min, int max)
{
    return (min + rand() % (max - min + 1));
}

bool IsEven(int num)	{
    return num % 2 ? false : true;
}
