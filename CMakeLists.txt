cmake_minimum_required(VERSION 3.20)
project(cinema)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(cinema main.cpp CinemaNetwork/cinemaNetwork.h CinemaNetwork/cinemaNetwork.cpp CinemaNetwork/cinema.h CinemaNetwork/cinema.cpp miniCLI/cli.h miniCLI/cli.cpp miniCLI/cursor.h miniCLI/cursor.cpp people/people.h people/people.cpp thrandom/rnd.h thrandom/rnd.cpp)
