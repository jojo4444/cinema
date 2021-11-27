//
// Created by jojo on 27.11.2021.
//

#include "cinemaNetwork.h"

cnet::CinemaNetwork::CinemaNetwork() = default;

void cnet::CinemaNetwork::Init() {
    const std::filesystem::path cinemas = std::filesystem::current_path().parent_path() / "cinemas";
    for(const auto& dir: std::filesystem::directory_iterator(cinemas)) {
        auto inf = std::ifstream(dir.path());
        Cinema c;
        c.ReadFromJSON(inf);
        cinemas_.push_back(c);
        inf.close();
    }
}

nlohmann::json cnet::CinemaNetwork::GetJSON() const {
    nlohmann::json j;
    for (const Cinema& c : cinemas_) {
        j["cinema"].push_back(c.GetJSON());
    }
    return j;
}

void cnet::CinemaNetwork::Write() const {
    std::cout << GetJSON().dump(2) << "\n";
}