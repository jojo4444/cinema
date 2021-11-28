//
// Created by jojo on 28.11.2021.
//

#ifndef CINEMA_CLI_H
#define CINEMA_CLI_H

/*
 ┌──────────────────────── name ────────────────────────────────────────────┐
 │█████████████████████████████████████                                     │
 └──────────────────────────────────────────────────────────────────────────┘
 */

#include <iostream>
#include <string>
#include <sstream>
#include <map>

namespace console {
    const int LEN_METRIC = 64;
    const std::string DEFAULT_NAME = "default metric";

    class Metric {
    public:
        explicit Metric(const std::string &name = DEFAULT_NAME);

        void setProb(double p);

        double getProb() const;

        void write() const;

    private:
        void gauge();

        std::string name_;
        std::string lines_[3];
        double prob_;
    };

    class Cli {
    public:
        Cli();

        void add(const std::string &nameMetric);

        void write() const;

        void setProb(const std::string &metric, double p);

    private:
        std::map<std::string, Metric> metrics_;
    };
}

#endif //CINEMA_CLI_H
