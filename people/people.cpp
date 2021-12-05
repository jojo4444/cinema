//
// Created by jojo on 03.12.2021.
//

#include "people.h"

people::Person::Person(Age a, std::string city, double cf_active) : a_(a), city_(city), cf_active_(cf_active) {
    ///
}

double people::Person::getCf(Age a) {
    switch (a) {
        case Age::Middle:
            return 1. / 30;
        case Age::Senior:
            return 1. / 90;
        case Age::Child:
            return 1. / 10;
        case Age::naa:
            return 0;
    }
    return 0;
}

bool people::Person::tryCinema() const {
    return thrnd::Thrand::singleton->bernoulli(cf_active_);
}

std::string people::Person::getCity() const {
    return city_;
}

double people::Person::goCinema() {
    return cnet::CinemaNetwork::singleton->buySeat(city_);
}

people::Child::Child(std::string city) : Person(Age::Child, city, Person::getCf(Age::Child)) {
    ///
}

double people::Child::rating(double quality) const {
    if (std::abs(quality) < 1e-5) {
        return 0; /// no cinema :(
    }
    return 1; /// all time -> super!
}

people::Midd::Midd(std::string city) : Person(Age::Child, city, Person::getCf(Age::Child)) {
    ///
}

double people::Midd::rating(double quality) const {
    if (std::abs(quality) < 1e-5) {
        return 0; /// no cinema :(
    }
    return thrnd::Thrand::singleton->uniformReal(1 - quality / 10, 1); /// optimism
}

people::Senior::Senior(std::string city) : Person(Age::Child, city, Person::getCf(Age::Child)) {
    ///
}

double people::Senior::rating(double quality) const {
    if (std::abs(quality) < 1e-5) {
        return 0; /// no cinema :(
    }
    return thrnd::Thrand::singleton->uniformReal(0, quality); /// pessimism
}