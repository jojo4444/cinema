//
// Created by jojo on 03.12.2021.
//

#ifndef CINEMA_PEOPLE_H
#define CINEMA_PEOPLE_H

#include <string>
#include <utility>

#include "../thrandom/rnd.h"
#include "../CinemaNetwork/cinemaNetwork.h"

namespace people {
    enum class Age {
        Child,
        Middle,
        Senior,
        naa /// not a age
    };

    class Person {
    public:
        Person(Age a, std::string city, double cf_active);

        static double getCf(Age a);

        std::string getCity() const;

        double goCinema(); /// return quality [0; 1], 1 - best

        virtual double rating(double quality) const = 0; /// [0; 1], 0 -> oh vay..., 1 -> fantastic

    protected:
        Age a_;
        std::string city_;
        double cf_active_;
    };

    class Child : public Person {
    public:
        explicit Child(std::string city = DEFAULT_CITY);

        double rating(double quality) const override;
    };

    class Midd : public Person {
    public:
        explicit Midd(std::string city = DEFAULT_CITY);

        double rating(double quality) const override;
    };

    class Senior : public Person {
    public:
        explicit Senior(std::string city = DEFAULT_CITY);

        double rating(double quality) const override;
    };
}

#endif //CINEMA_PEOPLE_H
