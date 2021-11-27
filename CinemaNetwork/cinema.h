//
// Created by jojo on 27.11.2021.
//

#ifndef CINEMA_CINEMA_H
#define CINEMA_CINEMA_H

#include <fstream>
#include <iostream>

#include "../nlohmann/json.hpp"

namespace cinema {
    const std::string DEFAULT_NAME = "jojo";
    const std::string DEFAULT_CITY = "Ozerki";

    enum Seat {
        Free,
        Buy,
        Lock,
    };

    enum Order {
        Success,
        Taken,
        Incorrect,
    };

    class Hall {
    public:
        Hall();

        Order BuySeat(int row, int col);
        nlohmann::json GetJSON();
        void ReadFromJSON(nlohmann::json& j);
    private:
        void CovidLock(int row, int col);

        std::string name_;
        std::vector<int> rows_;
        std::vector<std::vector<Seat> > status_;
    };

    class Cinema {
    public:
        Cinema();

        void Write();
        nlohmann::json GetJSON();
        void ReadFromJSON(std::ifstream& inf);
    private:
        std::string city_;
        std::vector<Hall> halls_;
    };
}
#endif //CINEMA_CINEMA_H
