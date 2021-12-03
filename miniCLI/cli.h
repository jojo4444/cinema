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
#include <thread>
#include <atomic>
#include <chrono>

#include "cursor.h"

#define fBlack "\033[30m"
#define fRed "\033[31m"
#define fGreen "\033[32m"
#define fDefault "\033[39m"

#define bBlack "\033[40m"
#define bRed "\033[41m"
#define bGreen "\033[42m"
#define bDefault "\033[49m"

using namespace std::chrono_literals;

namespace console {
    const int LEN_METRIC = 80;
    const std::string DEFAULT_NAME = "default metric";

    class Metric {
    public:
        explicit Metric(const std::string &name = DEFAULT_NAME);

        void setProb(double p, const Cursor &c);

        double getProb() const;

        void activate();

        void deactivate();

        void changeActivity();

        void write(const Cursor &c) const;

    private:
        void gauge();

        bool active_;
        std::string name_;
        std::string lines_[3];
        double prob_;

        /// invariant cursor: local -> left-up
    };

    class Cli {
    public:
        Cli();

        ~Cli();

        void add(const std::string &nameMetric);

        void prewrite() const;

        void write() const;

        void rewriteMetric(int id) const;

        void changeActivity(int id);

        void setProb(const std::string &metric, double p);

    private:
        void worker();

        int selectedID_; /// if metrics empty -> 0
        std::vector<std::pair<std::string, Metric> > metrics_;

        std::thread worker_;
        std::atomic_bool running_;

        /// invariant cursor: row -> down, col -> left
        Cursor c_;
    };
}

#endif //CINEMA_CLI_H
