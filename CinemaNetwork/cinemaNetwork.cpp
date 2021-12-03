//
// Created by jojo on 27.11.2021.
//

#include "cinemaNetwork.h"

cnet::CinemaNetwork::CinemaNetwork() = default;

void cnet::CinemaNetwork::init() {
    const std::filesystem::path cinemas = std::filesystem::current_path().parent_path() / "cinemas";
    for (const auto &item: std::filesystem::directory_iterator(cinemas)) {
        auto inf = std::ifstream(item.path());
        Cinema c;
        c.readFromJSON(inf);
        cinemas_.push_back(c);
        inf.close();
    }
}

nlohmann::json cnet::CinemaNetwork::getJSON() const {
    nlohmann::json j;
    for (const Cinema &c: cinemas_) {
        j["cinema"].push_back(c.getJSON());
    }
    return j;
}

void cnet::CinemaNetwork::write() const {
    std::cout << getJSON().dump(2) << "\n";
}