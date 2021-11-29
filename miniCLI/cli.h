//
// Created by jojo on 28.11.2021.
//

#ifndef CINEMA_CLI_H
#define CINEMA_CLI_H

/*
 ┌────────────────────────name───────────────────────────┐
 │███████████████████████████████████                    │
 └───────────────────────────────────────────────────────┘
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#define fBlack "\033[30m"
#define fRed "\033[31m"
#define fGreen "\033[32m"
#define fDefault "\033[39m"

#define bBlack "\033[40m"
#define bRed "\033[41m"
#define bGreen "\033[42m"
#define bDefault "\033[49m"

namespace console {
    const int LEN_METRIC = 64;
    const std::string DEFAULT_NAME = "default metric";

    static std::string Color(int r, int g, int b);

    static void shiftCol(int dc);

    static void shiftRow(int dr);

    class Metric {
    public:
        explicit Metric(const std::string &name = DEFAULT_NAME);

        void setProb(double p);

        double getProb() const;

        // todo : add rewrite methods (if cursor invariant)
        void write() const;

    private:
        void gauge();

        std::string name_;
        std::string lines_[3];
        double prob_;

        /// invariant cursor: local left down [0, 0]
    };

    class Cli {
    public:
        Cli();

        void add(const std::string &nameMetric);

        // todo : add rewrite method
        void write() const;

        void setProb(const std::string &metric, double p);

    private:
        /// invariant cursor: row_ -> down, col -> left
        std::vector<std::pair<std::string, Metric> > metrics_;
    };
}

#endif //CINEMA_CLI_H
