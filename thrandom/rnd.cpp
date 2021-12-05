//
// Created by jojo on 04.12.2021.
//

#include "rnd.h"

thrnd::Thrand* thrnd::Thrand::singleton = nullptr;
std::mutex thrnd::Thrand::mu_single_;

void thrnd::Thrand::unable() {
    const std::lock_guard<std::mutex> lock(mu_single_);
    if (singleton == nullptr) {
        singleton = new thrnd::Thrand();
    }
}

void thrnd::Thrand::disable() {
    const std::lock_guard<std::mutex> lock(mu_single_);
    delete singleton; /// nullptr -> no effect
}