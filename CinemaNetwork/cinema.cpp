//
// Created by jojo on 27.11.2021.
//

#include "cinema.h"

cinema::Hall::Hall() : name_(DEFAULT_NAME) {}

cinema::Order cinema::Hall::BuySeat(int row, int col) {
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
}

void cinema::Hall::covidLock(int row, int col) {
    if (0 <= row && row < rows_.size() && 0 <= col && col < rows_[row] && status_[row][col] == cinema::Seat::Free) {
        status_[row][col] = cinema::Seat::Lock;
    }
}

cinema::Cinema::Cinema() : city_(DEFAULT_CITY) {}

void cinema::Cinema::readFromJSON(std::ifstream &inf) {
    nlohmann::json j;

    inf >> j;
    city_ = j["city"].get<std::string>();

    for (auto &el: j["halls"]) {
        cinema::Hall h;
        h.readFromJSON(el);
        halls_.push_back(h);
    }
}

nlohmann::json cinema::Cinema::getJSON() const {
    nlohmann::json j;
    j["city"] = city_;
    for (const cinema::Hall &h: halls_) {
        j["halls"].push_back(h.getJSON());
    }
    return j;
}

void cinema::Cinema::write() const {
    std::cout << getJSON().dump(2) << "\n";
}