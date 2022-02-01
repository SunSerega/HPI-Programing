#pragma once
#include <string>
#include <iostream>

class CarInRepair {
    std::string brand;
    double cost;
    int days;
    std::string numberplate;

public:
    CarInRepair()
        : brand{}
        , cost{}
        , days{}
        , numberplate{}
    {}

    CarInRepair(std::string brand, double cost, int days, std::string numberplate)
        : brand{ brand }
        , cost{ cost }
        , days{ days }
        , numberplate{ numberplate }
    {}

    static void Generate(CarInRepair& data);

    friend bool operator<(const CarInRepair& d1, const CarInRepair& d2);

    friend std::ostream& operator<<(std::ostream& otp, const CarInRepair& data);

};


