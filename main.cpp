#include <chrono>
#include <thread>

#include "CinemaNetwork/cinemaNetwork.h"
#include "miniCLI/cli.h"

using namespace std::chrono_literals;

int main() {
    cnet::CinemaNetwork cn;

    cn.init();

    cn.write();

    console::Cli cli;
    cli.add("SPB");
    cli.add("Kaliningrad");
    cli.write();

    int S = 60;
    for (int i = 0; i <= S; ++i) {
        std::this_thread::sleep_for(100ms);
        cli.setProb("SPB", i / (double)S);
    }

    return 0;
}
