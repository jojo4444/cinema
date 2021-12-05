//
// Created by jojo on 03.12.2021.
//

#ifndef CINEMA_RND_H
#define CINEMA_RND_H

#include <chrono>
#include <random>
#include <mutex>
#include <algorithm>

namespace thrnd {
    class Thrand {
    public:
        Thrand() {
            eng_.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        }

        bool bernoulli(double p) {
            if (p > 1 || p <= 0) {
                return true;
            }
            const std::lock_guard<std::mutex> lock(mu_);
            std::bernoulli_distribution d(p);
            return d(eng_);
        }

        int uniformInt(int l, int r) {
            if (l > r) {
                std::swap(l, r);
            }
            const std::lock_guard<std::mutex> lock(mu_);
            std::uniform_int_distribution<> d(l, r);
            return d(eng_);
        }

        double uniformReal(double l, double r) {
            if (l > r) {
                std::swap(l, r);
            }
            const std::lock_guard<std::mutex> lock(mu_);
            std::uniform_real_distribution<> d(l, r);
            return d(eng_);
        }

        static Thrand *singleton;

        static void unable();

        static void disable();

    private:
        static std::mutex mu_single_;

        std::mutex mu_;
        std::mt19937 eng_;
    };
}
#endif //CINEMA_RND_H
