#include "CarInRepair.h"
#include <algorithm>
#include <iomanip>
using namespace std;

const string brands[] = { "Tesla", "Mazda", "Toyota", "Honda", "Jaguar", "Mitsubishi", "Volvo", "Lexus" };
const auto brands_len = sizeof(brands) / sizeof(string);
const auto brands_max_w = max_element(brands, brands + brands_len, [](const auto& s1, const auto& s2) { return s1.length() < s2.length(); })->length();

void CarInRepair::Generate(CarInRepair& data)
{

    data.brand = brands[ rand() % brands_len ];

    data.cost = 100.0 + rand() % 30;

    data.days = 15 + rand() % 5;

    static const char numberplate_valid_chars[] = {
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    };
    auto numberplate_len = 6;
    data.numberplate.reserve(numberplate_len);
    for (int i = 0; i < numberplate_len; ++i)
        data.numberplate += numberplate_valid_chars[rand() % (sizeof(numberplate_valid_chars) - 1)];

}

bool operator<(const CarInRepair& d1, const CarInRepair& d2) {
    return d1.brand < d2.brand;
}

std::ostream& operator<<(std::ostream& otp, const CarInRepair& data) {
    return otp << "CarInRepair[brand=" << setw(brands_max_w) << data.brand << ", cost=" << data.cost << ", days=" << data.days << ", numberplate=" << data.numberplate << "]";
}
