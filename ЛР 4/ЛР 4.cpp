#include <iostream>
#include <io.h>
#include <fcntl.h>

#define RE_READ_ENCODED true

/// <summary>
/// Записывает count битов из числа bits в разряд from числа n
/// </summary>
inline void WriteBits(uint16_t& n, int bits, int from, int count)
{
    auto mask = (1 << count) - 1;
    n |= (bits & mask) << from;
}

/// <summary>
/// Вырезает и возвращается область из count бит числа source, начиная с разряда from
/// </summary>
inline int ReadBits(uint16_t source, int from, int count)
{
    auto mask = (1 << count) - 1;
    return (source >> from) & mask;
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin ), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    int oper_code, command_chain, data_chain, byte_count;

    wprintf(L"Код операции (0..31): ");
    wscanf_s(L"%d", &oper_code);

    wprintf(L"Признак цепочки команд (0..1): ");
    wscanf_s(L"%d", &command_chain);
    command_chain = command_chain ? 1 : 0;

    wprintf(L"Признак цепочки данных (0..1): ");
    wscanf_s(L"%d", &data_chain);
    data_chain = data_chain ? 1 : 0;

    wprintf(L"Количество байтов для передачи (0..511): ");
    wscanf_s(L"%d", &byte_count);

    uint16_t encoded{};
    WriteBits(encoded, byte_count, 0, 9);
    WriteBits(encoded, data_chain, 9, 1);
    WriteBits(encoded, command_chain, 10, 1);
    WriteBits(encoded, oper_code, 11, 5);
    wprintf(L"В зашифрованном виде: 0x%X\n", encoded);

    if (RE_READ_ENCODED)
    {
        wprintf(L"Введите зашифрованное состояние в 16СС: ");
        int temp_encoded;
        wscanf_s(L"%x", &temp_encoded);
        encoded = temp_encoded;
    }

    wprintf(L"Код операции: %d\n", ReadBits(encoded, 11, 5));
    wprintf(L"Признак цепочки команд: %d\n", ReadBits(encoded, 10, 1));
    wprintf(L"Признак цепочки данных: %d\n", ReadBits(encoded, 9, 1));
    wprintf(L"Количество байтов для передачи: %d\n", ReadBits(encoded, 0, 9));

}