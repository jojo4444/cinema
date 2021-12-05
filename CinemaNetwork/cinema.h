//
// Created by jojo on 27.11.2021.
//

#ifndef CINEMA_CINEMA_H
#define CINEMA_CINEMA_H

#include <fstream>
#include <iostream>
#include <mutex>

#include "../nlohmann/json.hpp"
#include "../thrandom/rnd.h"

namespace cinema {
    const std::string DEFAULT_NAME = "bfu-cinema";
    const std::string DEFAULT_CITY = "Ozerki";

    enum class Seat {
        Free,
        Buy,
        Lock,
    };

    enum class Order {
        Success,
        Taken,
        Incorrect,
    };

    class Hall {
    public:
        Hall();

        void clear();

        std::string getName() const;

        Order buySeat(int row, int col);

        double buyOptimum();

        nlohmann::json getJSON() const;

        void readFromJSON(nlohmann::json &j);

    private:
        void covidLock(int row, int col);

        double maxDistToCenter_;
        std::string name_;
        std::vector<int> rows_;
        std::vector<std::vector<Seat> > status_;
    };

    class Cinema {
    public:
        Cinema();

        Cinema(const Cinema &other);

        void clear();

        double buySeat(); /// return min dist to center

        std::string getCity() const;

        void readFromJSON(std::ifstream &inf);

        nlohmann::json getJSON() const;

        void write() const;

    private:
        mutable std::mutex mu_;
        std::string name_, city_;
        std::vector<Hall> halls_;
    };
}

#endif //CINEMA_CINEMA_H
