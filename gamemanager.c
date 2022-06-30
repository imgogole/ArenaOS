#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utilities.h"
#include "gamemanager.h"

void Clear(int _OS)
{
    switch (_OS)
    {
        case 0 :
            system("cls");
            break;
        case 1 :
            system("clear");
            break;
        default:
            // Do nothing
            break;
    }
}