//
// Created by jojo on 03.12.2021.
//

#include "cursor.h"

console::Cursor::Cursor() = default;

std::string console::Cursor::colorForeground(int r, int g, int b) {
    std::stringstream ss;
    ss << "\033[38;2;" << r << ";" << g << ";" << b << "m";
    return ss.str();
}

std::string console::Cursor::colorBackground(int r, int g, int b) {
    std::stringstream ss;
    ss << "\033[48;2;" << r << ";" << g << ";" << b << "m";
    return ss.str();
}

int console::Cursor::hgetch() {
    int ch;
    termios old_termios{}, new_termios{};
    tcgetattr(STDIN_FILENO, &old_termios);
    new_termios = old_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
    return ch;
}

void console::Cursor::shiftCol(int dc) const {
    const std::lock_guard<std::mutex> lock(mu_);
    if (dc > 0) {
        std::cout << "\033[" + std::to_string(dc) + "C" << std::flush;
    } else if (dc < 0) {
        std::cout << "\033[" + std::to_string(-dc) + "D" << std::flush;
    }
}

void console::Cursor::shiftRow(int dr) const {
    const std::lock_guard<std::mutex> lock(mu_);
    if (dr > 0) {
        std::cout << "\033[" + std::to_string(dr) + "A" << std::flush;
    } else if (dr < 0) {
        std::cout << "\033[" + std::to_string(-dr) + "B" << std::flush;
    }
}

void console::Cursor::cursorToBegin() const {
    const std::lock_guard<std::mutex> lock(mu_);
    std::cout << "\033[" << std::numeric_limits<short>::max() << "D" << std::flush;
}

void console::Cursor::write(const std::string &s) const {
    const std::lock_guard<std::mutex> lock(mu_);
    std::cout << s << std::flush;
}