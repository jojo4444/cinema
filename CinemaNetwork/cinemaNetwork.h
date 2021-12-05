//
// Created by jojo on 27.11.2021.
//

#ifndef CINEMA_CINEMANETWORK_H
#define CINEMA_CINEMANETWORK_H

#include <filesystem>
#include <map>
#include <vector>

#include "cinema.h"
#include "../miniCLI/cli.h"
#include "../thrandom/rnd.h"

using namespace cinema;

namespace cnet {
    class CinemaNetwork {
    public:
        CinemaNetwork();

        ~CinemaNetwork();

        void clearAll();

        /// return relative distance [0; 1] to center (center is 1) [special metrics] (if allin -> 0)
        double buySeat(const std::string &city);

        bool isRunning() const;

        void setDays(int days) const;

        void write() const;

        static CinemaNetwork *singleton;

        static void unable();

        static void disable();

    private:
        static std::mutex mu_single_;

        nlohmann::json getJSON() const;

        std::map<std::string, std::vector<Cinema> > cinemas_;
    };
}

#endif //CINEMA_CINEMANETWORK_H
