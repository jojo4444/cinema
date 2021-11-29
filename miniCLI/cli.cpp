//
// Created by jojo on 28.11.2021.
//

#include "cli.h"

static void console::shiftCol(int dc) {
    if (dc > 0) {
        std::cout << "\033[" + std::to_string(dc) + "C" << std::flush;
    } else if (dc < 0) {
        std::cout << "\033[" + std::to_string(-dc) + "D" << std::flush;
    }
}

static void console::shiftRow(int dr) {
    if (dr > 0) {
        std::cout << "\033[" + std::to_string(dr) + "A" << std::flush;
    } else if (dr < 0) {
        std::cout << "\033[" + std::to_string(-dr) + "B" << std::flush;
    }
}

static std::string console::Color(int r, int g, int b) {
    std::stringstream ss;
    ss << "\033[38;2;" << r << ";" << g << ";" << b << "m";
    return ss.str();
}

console::Metric::Metric(const std::string &name) {
    name_ = name;
    prob_ = 0;

    if (name_.size() > console::LEN_METRIC) {
        name_ = "bad name (very long)";
    }

    /// leftSize + name_.size() + rightSize == LEN_METRIC
    /// |leftSize - rightSize| <= 1

    int leftSize = (console::LEN_METRIC - (int) name_.size()) / 2;
    int rightSize = console::LEN_METRIC - (int) name_.size() - leftSize;

    const std::string boardSetLeft[3] = {"┌", "│", "└"};
    const std::string boardSetRight[3] = {"┐", "│", "┘"};

    std::stringstream ss[3];
    for (int i = 0; i < 3; ++i) {
        ss[i] << boardSetLeft[i];
        if (i == 0) {
            while (leftSize--) {
                ss[i] << "─";
            }
            for (char c: name_) {
                ss[i] << std::string(1, c);
            }
            while (rightSize--) {
                ss[i] << "─";
            }
        } else {
            for (int j = 0; j < console::LEN_METRIC; ++j) {
                ss[i] << (i == 1 ? " " : "─");
            }
        }
        ss[i] << boardSetRight[i];
        lines_[i] = ss[i].str();
    }
}

void console::Metric::setProb(double p) {
    prob_ = std::min(1., std::max(0., p));
    gauge();
}

double console::Metric::getProb() const {
    return prob_;
}

void console::Metric::write() const {
    int it = 0;
    for (const auto &line: lines_) {
        std::cout << line;
        ++it;
        if (it < 3) {
            std::cout << "\n";
        }
        std::cout << std::flush;
    }
}

void console::Metric::gauge() {
    std::stringstream ss;
    int cnt = prob_ * console::LEN_METRIC;

    ss << "│";
    for (int i = 0; i < console::LEN_METRIC; ++i) {
        ss << console::Color(0, 255 * i / (double)console::LEN_METRIC, 0) << (i < cnt ? "█" : " ");
    }
    ss << fDefault << "│";
    lines_[1] = ss.str();

    shiftRow(1);
    std::cout << lines_[1] << std::flush;
    shiftCol(-(2 + console::LEN_METRIC));
    shiftRow(-1);
}

console::Cli::Cli() = default;

void console::Cli::add(const std::string &nameMetric) {
    metrics_.emplace_back(nameMetric, Metric(nameMetric));
}

void console::Cli::write() const {
    // todo : clear all metrics and write items
    for (const auto &m: metrics_) {
        m.second.write();
        std::cout << std::endl;
    }
    shiftCol(-(2 + console::LEN_METRIC));
}

void console::Cli::setProb(const std::string &metric, double p) {
    for (int i = 0; i < metrics_.size(); ++i) {
        auto& m = metrics_[i];
        if (m.first == metric) {
            int dr = ((int)metrics_.size() - i - 1) * 3 + 1;
            shiftRow(dr);
            m.second.setProb(p);
            shiftRow(-dr);
        }
    }
}