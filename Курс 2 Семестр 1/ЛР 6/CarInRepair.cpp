#include "CarInRepair.h"
#include <algorithm>
#include <iomanip>
using namespace std;

constexpr string_view brands[] = { "Tesla", "Mazda", "Toyota", "Honda", "Jaguar", "Mitsubishi", "Volvo", "Lexus" };
constexpr auto brands_len = ranges::size(brands);
constexpr auto brands_max_w = ranges::max_element(brands, {}, [](const auto& s) { return s.length(); })->length();

constexpr char numberplate_chars[] = {
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
};
constexpr auto numberplate_chars_len = ranges::size(numberplate_chars) - 1;

//__declspec(noinline)
void CarInRepair::Generate(CarInRepair& data)
{

    auto brand = brands[ rand() % brands_len ];

    auto cost = 100.0 + rand() % 30;

    auto days = 15 + rand() % 5;

    auto numberplate_len = 6;
    string numberplate{};
    numberplate.reserve(numberplate_len);
    while (numberplate_len--)
        numberplate += numberplate_chars[ rand() % numberplate_chars_len ];

    new(&data) CarInRepair(move(brand), move(cost), move(days), move(numberplate));
}

bool operator<(const CarInRepair& d1, const CarInRepair& d2) {
    return d1.brand < d2.brand;
}

std::ostream& operator<<(std::ostream& otp, const CarInRepair& data) {
    return otp << "CarInRepair[brand=" << setw(brands_max_w) << data.brand << ", cost=" << data.cost << ", days=" << data.days << ", numberplate=" << data.numberplate << "]";
}
