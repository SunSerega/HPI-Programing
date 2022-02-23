#pragma once
#include <string>
#include <iostream>

class CarInRepair {
    std::string_view brand;
    double cost;
    int days;
    std::string numberplate;

public:
    CarInRepair(std::string_view brand, double cost, int days, std::string numberplate)
        : brand{ brand }
        , cost{ cost }
        , days{ days }
        , numberplate{ numberplate }
    {}

    static void Generate(CarInRepair& data);

    auto& brand_prop() { return brand; }
    auto& cost_prop() { return cost; }
    auto& days_prop() { return days; }
    auto& numberplate_prop() { return numberplate; }

    friend bool operator<(const CarInRepair& d1, const CarInRepair& d2);

    friend std::ostream& operator<<(std::ostream& otp, const CarInRepair& data);

};


