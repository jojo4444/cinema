cmake_minimum_required(VERSION 3.20)
project(cinema)

set(CMAKE_CXX_STANDARD 17)

add_executable(cinema main.cpp CinemaNetwork/cinemaNetwork.h CinemaNetwork/cinemaNetwork.cpp CinemaNetwork/cinema.h CinemaNetwork/cinema.cpp)
