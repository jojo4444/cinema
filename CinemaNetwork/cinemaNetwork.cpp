//
// Created by jojo on 27.11.2021.
//

#include "cinemaNetwork.h"

cnet::CinemaNetwork* cnet::CinemaNetwork::singleton = nullptr;
std::mutex cnet::CinemaNetwork::mu_single_;

void cnet::CinemaNetwork::unable() {
    const std::lock_guard<std::mutex> lock(mu_single_);
    if (singleton == nullptr) {
        singleton = new cnet::CinemaNetwork();
    }
}

void cnet::CinemaNetwork::disable() {
    const std::lock_guard<std::mutex> lock(mu_single_);
    delete singleton; /// nullptr -> no effect
}

cnet::CinemaNetwork::CinemaNetwork() {
    thrnd::Thrand::unable();
    console::Cli::unable();

    const std::filesystem::path cinemas = std::filesystem::current_path().parent_path() / "cinemas";
    for (const auto &item: std::filesystem::directory_iterator(cinemas)) {
        std::string name = std::filesystem::path(item.path());
        std::string suffix = ".json";
        if (name.size() <= suffix.size() || name.substr(name.size() - suffix.size(), suffix.size()) != suffix) {
            continue;
        }

        auto inf = std::ifstream(item.path());
        Cinema c;
        c.readFromJSON(inf);

        cinemas_[c.getCity()].push_back(c);
        console::Cli::singleton->add(c.getCity());

        inf.close();
    }

    write();
    console::Cli::singleton->prewrite();
}

cnet::CinemaNetwork::~CinemaNetwork() {
    thrnd::Thrand::disable();
    console::Cli::disable();
}

void cnet::CinemaNetwork::clearAll() {
    for (auto& cs : cinemas_) {
        for (auto& c : cs.second) {
            c.clear();
        }
    }
}

double cnet::CinemaNetwork::buySeat(const std::string &city) {
    auto ct = cinemas_.find(city);
    if (ct == cinemas_.end()) {
        return 0;
    }

    if (ct->second.empty()) {
        return 0;
    }

    /// rand cinema in city
    int id = thrnd::Thrand::singleton->uniformInt(0, ct->second.size() - 1);

    return 1 - ct->second[id].buySeat();
}

bool cnet::CinemaNetwork::isRunning() const {
    return console::Cli::singleton->isRunning();
}

void cnet::CinemaNetwork::setDays(int days) const {
    console::Cli::singleton->setDays(days);
}

void cnet::CinemaNetwork::write() const {
    std::cout << getJSON().dump(2) << "\n" << std::flush;
}

nlohmann::json cnet::CinemaNetwork::getJSON() const {
    nlohmann::json j;
    for (const auto &pr: cinemas_) {
        for (const Cinema &c: pr.second) {
            j["cinema"][c.getCity()].push_back(c.getJSON());
        }
    }
    return j;
}