#include "CinemaNetwork/cinemaNetwork.h"

int main() {
    auto inf = std::ifstream("../cinemas/Kaliningrad_1.json");

    cinema::Cinema c;
    c.ReadFromJSON(inf);
    c.Write();

    inf.close();

    return 0;
}
