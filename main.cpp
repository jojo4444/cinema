#include "CinemaNetwork/cinemaNetwork.h"
#include "people/people.h"

#include <condition_variable>

using namespace std::chrono_literals;

const std::chrono::duration ONE_DAY_MS = 50ms;

std::vector<std::string> cities = {
        "Saint Petersburg",
        "Kaliningrad",
        "Moscow"
};

template<typename T>
void makePeople(std::vector<T> &p, int n) {
    p.resize(n);
    std::generate(p.begin(), p.end(), []() {
        int id = thrnd::Thrand::singleton->uniformInt(0, cities.size() - 1);
        return T(cities[id]);
    });
}

/// 4 + 10 is magic...
const int P = 14;

std::mutex mu_;
std::condition_variable cv;

std::atomic_bool running;
int day;

template<typename T>
void distribution(std::vector<T> &p, int start) {
    int localDay = 0;
    while (running) {
        std::unique_lock<std::mutex> lk(mu_);
        cv.wait(lk, [localDay]{return day > localDay || !running;});

        if (!running) {
            break;
        }

        for (int i = start; i < p.size(); i += P) {
            double review = p[i].goCinema();
            console::Cli::singleton->updateMetric(p[i].getCity(), review);
        }

        ++localDay;
    }
}

int main() {
    day = 0;
    running = true;

    cnet::CinemaNetwork::unable();

    std::vector<people::Child> children;
    std::vector<people::Midd> middles;
    std::vector<people::Senior> seniors;

    makePeople(children, 25);
    makePeople(middles, 70);
    makePeople(seniors, 15);

    std::vector<std::thread> jobs[3];
    for (int j = 0; j < P; ++j) {
        jobs[0].emplace_back(distribution<people::Child>, std::ref(children), j);
        jobs[1].emplace_back(distribution<people::Midd>, std::ref(middles), j);
        jobs[2].emplace_back(distribution<people::Senior>, std::ref(seniors), j);
    }

    while (cnet::CinemaNetwork::singleton->isRunning()) {
        std::this_thread::sleep_for(ONE_DAY_MS);
        ++day;
        cnet::CinemaNetwork::singleton->clearAll();
        cv.notify_all();
        cnet::CinemaNetwork::singleton->setDays(day);
    }

    running = false;
    cv.notify_all();

    for (auto &job: jobs) {
        for (auto &j: job) {
            j.join();
        }
    }

    cnet::CinemaNetwork::disable();

    return 0;
}
