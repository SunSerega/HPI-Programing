#include <iostream>
#include <io.h>
#include <fcntl.h>

//ToDo Другие заголовки

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin ), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    //ToDo Тело программы

}