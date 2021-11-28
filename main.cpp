#include "CinemaNetwork/cinemaNetwork.h"
#include "miniCLI/cli.h"

int main() {
    cnet::CinemaNetwork cn;

    cn.init();

    cn.write();

    console::Cli cli;
    cli.add("SPB");
    cli.add("Kaliningrad");
    cli.setProb("SPB", 0.3);
    cli.write();

    return 0;
}
