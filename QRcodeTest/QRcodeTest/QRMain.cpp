#include <string.h>
#include <errno.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include "qrencode.h"   
#include "QR.h"

int main()
{
    QR::GetInteance()->Init();
    system("pause");
    return 0;
}
