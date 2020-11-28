#include <iostream>
#include <io.h>
#include <fcntl.h>

//ToDo Другие заголовки

int main()
{
    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1) return -1;
    if (_setmode(_fileno(stdin ), _O_U16TEXT) == -1) return -1;
    if (_setmode(_fileno(stderr), _O_U16TEXT) == -1) return -1;

    //ToDo Тело программы

}