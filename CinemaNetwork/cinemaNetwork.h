//
// Created by jojo on 27.11.2021.
//

#ifndef CINEMA_CINEMANETWORK_H
#define CINEMA_CINEMANETWORK_H

#include <filesystem>
#include "cinema.h"

using namespace cinema;

namespace cnet {
    class CinemaNetwork {
    public:
        CinemaNetwork();

        void Init();
        nlohmann::json GetJSON() const;
        void Write() const;
    private:
        std::vector<Cinema> cinemas_;
    };
}

#endif //CINEMA_CINEMANETWORK_H
