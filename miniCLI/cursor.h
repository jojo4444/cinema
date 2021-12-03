//
// Created by jojo on 03.12.2021.
//

#ifndef CINEMA_CURSOR_H
#define CINEMA_CURSOR_H

#include <mutex>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <sstream>

namespace console {

    class Cursor {
    public:
        Cursor();

        static std::string colorForeground(int r, int g, int b);

        static std::string colorBackground(int r, int g, int b);

        static int hgetch();

        void shiftCol(int dc) const;

        void shiftRow(int dr) const;

        void cursorToBegin() const;

        void setActivatedMod(bool action) const;

        void setDeactivatedMod() const;

        void write(const std::string &s) const;

    private:
        mutable std::mutex mu_;
    };
}

#endif //CINEMA_CURSOR_H
