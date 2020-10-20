
#include <iostream>
#include <io.h>
#include <fcntl.h>

int main()
{
    // Переключение кодировки вывода
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin ), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    wchar_t name1[13], name2[13], name3[13];
    wchar_t manuf1[14], manuf2[14], manuf3[14];
    unsigned parts1, parts2, parts3;
    float cost1, cost2, cost3;

    wprintf(L"1. Введите: наименование, производителя, количество сост. частей, цену: ");
    wscanf_s(L"%s %s %d %f", name1, _countof(name1), manuf1, _countof(manuf1), &parts1, &cost1);
    wprintf(L"2. Введите: наименование, производителя, количество сост. частей, цену: ");
    wscanf_s(L"%s %s %d %f", name2, _countof(name2), manuf2, _countof(manuf2), &parts2, &cost2);
    wprintf(L"3. Введите: наименование, производителя, количество сост. частей, цену: ");
    wscanf_s(L"%s %s %d %f", name3, _countof(name3), manuf3, _countof(manuf3), &parts3, &cost3);

    wprintf(L"|------------------------------------------------------------------|\n");
    wprintf(L"| Офисные пакеты                                                   |\n");
    wprintf(L"|------------------------------------------------------------------|\n");
    wprintf(L"| Наименование | Производитель | Количество сост.частей | Цена ($) |\n");
    wprintf(L"|--------------|---------------|------------------------|----------|\n");

    wprintf(L"| %-12ls | %-13ls | %-22d | %-8.2f |\n", name1, manuf1, parts1, cost1);
    wprintf(L"| %-12ls | %-13ls | %-22d | %-8.2f |\n", name2, manuf2, parts2, cost2);
    wprintf(L"| %-12ls | %-13ls | %-22d | %-8.2f |\n", name3, manuf3, parts3, cost3);

    wprintf(L"|------------------------------------------------------------------|\n");
    wprintf(L"| Примечание: возможно бесплатно получить                          |\n");
    wprintf(L"| продукт StarOffice через Internet                                |\n");
    wprintf(L"|------------------------------------------------------------------|\n");

}

/* Пример вывода из задания:
*
* Офисные пакеты
* Наименование	Производитель	Количество сост.частей	Цена ($)
* Office	Microsoft	4	870
* SmartSute	Lotus	5	1020
* StarOffice	Sun	4	9
* Примечание: возможно бесплатно получить продукт StarOffice через Internet
*
*/