//
// Created by jojo on 27.11.2021.
//

#include "cinema.h"

cinema::Hall::Hall() : maxDistToCenter_(0), name_(DEFAULT_NAME) {}

void cinema::Hall::clear() {
    for (auto& r : status_) {
        std::fill(r.begin(), r.end(), Seat::Free);
    }
}

std::string cinema::Hall::getName() const {
    return name_;
}

cinema::Order cinema::Hall::buySeat(int row, int col) {
    if (row < 0 || row >= rows_.size()) {
        return cinema::Order::Incorrect;
    }
    if (col < 0 || col >= rows_[row]) {
        return cinema::Order::Incorrect;
    }
    if (status_[row][col] == cinema::Seat::Free) {
        status_[row][col] = cinema::Seat::Buy;
        covidLock(row, col - 1);
        covidLock(row, col + 1);
        return cinema::Order::Success;
    }
    if (status_[row][col] == cinema::Seat::Lock) {
        return cinema::Order::Incorrect;
    }
    return cinema::Order::Taken;
}

double cinema::Hall::buyOptimum() {
    if (rows_.empty()) {
        return 1;
    }

    int rowOpt = -1, colOpt = -1;
    double distOpt = maxDistToCenter_;

    int row = rows_.size() / 2;
    int col = rows_[row] / 2;

    for (int r = 0; r < rows_.size(); ++r) {
        for (int c = 0; c < rows_[r]; ++c) {
            if (status_[r][c] == Seat::Free) {
                double dist = std::hypot(row - r, col - c);
                if (distOpt > dist) {
                    distOpt = dist;
                    rowOpt = r;
                    colOpt = c;
                }
            }
        }
    }

    Order result = buySeat(rowOpt, colOpt);
    return distOpt / maxDistToCenter_;
}

nlohmann::json cinema::Hall::getJSON() const {
    nlohmann::json j;
    j["name"] = name_;
    for (int r: rows_) {
        j["rows"].push_back(r);
    }
    return j;
}

void cinema::Hall::readFromJSON(nlohmann::json &j) {
    name_ = j["name"].get<std::string>();
    rows_ = j["rows"].get<std::vector<int> >();

    status_.clear();
    status_.resize(rows_.size());
    for (size_t i = 0; i < rows_.size(); ++i) {
        status_[i].resize(rows_[i], Seat::Free);
    }

    int row = rows_.size() / 2, col = rows_[row] / 2;
    for (int r = 0; r < rows_.size(); ++r) {
        for (int c = 0; c < rows_[r]; ++c) {
            maxDistToCenter_ = std::max(maxDistToCenter_, std::hypot(col - c, row - r));
        }
    }
}

void cinema::Hall::covidLock(int row, int col) {
    if (0 <= row && row < rows_.size() && 0 <= col && col < rows_[row] && status_[row][col] == cinema::Seat::Free) {
        status_[row][col] = cinema::Seat::Lock;
    }
}

cinema::Cinema::Cinema() : name_(DEFAULT_CITY) {}

cinema::Cinema::Cinema(const Cinema &other) {
    city_ = other.city_;
    name_ = other.name_;
    halls_ = other.halls_;
}

void cinema::Cinema::clear() {
    const std::lock_guard<std::mutex> lock(mu_);
    for (auto& h : halls_) {
        h.clear();
    }
}

double cinema::Cinema::buySeat() {
    const std::lock_guard<std::mutex> lock(mu_);
    double best = 1; /// max

    if (halls_.empty()) {
        return best;
    }

    /// rand hall in cinema
    int id = thrnd::Thrand::singleton->uniformInt(0, halls_.size() - 1);
    best = halls_[id].buyOptimum();

    return best;
}

std::string cinema::Cinema::getCity() const {
    return city_;
}

void cinema::Cinema::readFromJSON(std::ifstream &inf) {
    nlohmann::json j;

    inf >> j;
    city_ = j["city"].get<std::string>();
    name_ = j["name"].get<std::string>();

    for (auto &el: j["halls"]) {
        cinema::Hall h;
        h.readFromJSON(el);
        halls_.push_back(h);
    }
}

nlohmann::json cinema::Cinema::getJSON() const {
    nlohmann::json j;
    j["city"] = city_;
    j["name"] = name_;
    for (const cinema::Hall &h: halls_) {
        j["halls"].push_back(h.getJSON());
    }
    return j;
}

void cinema::Cinema::write() const {
    std::cout << getJSON().dump(2) << "\n" << std::flush;
}