//
// Created by jojo on 27.11.2021.
//

#include "cinema.h"

cinema::Hall::Hall() {
    name_ = DEFAULT_NAME;
}

void cinema::Hall::ReadFromJSON(nlohmann::json &j)  {
    name_ = j["name"].get<std::string>();
    rows_ = j["rows"].get<std::vector<int> >();
}

nlohmann::json cinema::Hall::GetJSON() {
    nlohmann::json j;
    j["name"] = name_;
    for (int& r : rows_) {
        j["rows"].push_back(r);
    }
    return j;
}

cinema::Cinema::Cinema() {
    city_ = DEFAULT_CITY;
}

void cinema::Cinema::ReadFromJSON(std::ifstream& inf) {
    nlohmann::json j;

    inf >> j;
    city_ = j["city"].get<std::string>();

    for (auto& el : j["halls"]) {
        cinema::Hall h;
        h.ReadFromJSON(el);
        halls_.push_back(h);
    }
}

nlohmann::json cinema::Cinema::GetJSON() {
    nlohmann::json j;
    j["city"] = city_;
    for (cinema::Hall& h : halls_) {
        j["halls"].push_back(h.GetJSON());
    }
    return j;
}

void cinema::Cinema::Write() {
    std::cout << GetJSON() << "\n";
}