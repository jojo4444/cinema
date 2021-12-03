//
// Created by jojo on 28.11.2021.
//

#include "cli.h"

static std::string console::Color(int r, int g, int b) {
    std::stringstream ss;
    ss << "\033[38;2;" << r << ";" << g << ";" << b << "m";
    return ss.str();
}

console::Metric::Metric(const std::string &name) {
    name_ = name;
    prob_ = 0;

    if (name_.size() > LEN_METRIC) {
        name_ = "bad name (very long)";
    }

    /// leftSize + name_.size() + rightSize == LEN_METRIC
    /// |leftSize - rightSize| <= 1

    int leftSize = (LEN_METRIC - (int) name_.size()) / 2;
    int rightSize = LEN_METRIC - (int) name_.size() - leftSize;

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
            for (int j = 0; j < LEN_METRIC; ++j) {
                ss[i] << (i == 1 ? " " : "─");
            }
        }
        ss[i] << boardSetRight[i];
        lines_[i] = ss[i].str();
    }
}

void console::Metric::setProb(double p, const Cursor &c) {
    prob_ = std::min(1., std::max(0., p));
    gauge();
    write(c);
}

double console::Metric::getProb() const {
    return prob_;
}

void console::Metric::write(const Cursor &c) const {
    for (const auto &line: lines_) {
        c.write(line);
        c.cursorToBegin();
        c.shiftRow(-1);
    }
    c.shiftRow(3);
}

void console::Metric::gauge() {
    std::stringstream ss;
    int cnt = prob_ * LEN_METRIC;

    ss << "│";
    for (int i = 0; i < console::LEN_METRIC; ++i) {
        ss << console::Color(0, 85 + (255 - 85) * i / (double) LEN_METRIC, 0) << (i < cnt ? "█" : " ");
    }
    ss << fDefault << "│";
    lines_[1] = ss.str();
}

console::Cli::Cli() {
    running_ = true;
    worker_ = std::thread(&Cli::worker, this);
}

console::Cli::~Cli() {
    running_ = false;
    worker_.join();
}

void console::Cli::add(const std::string &nameMetric) {
    metrics_.emplace_back(nameMetric, Metric(nameMetric));
}

void console::Cli::prewrite() const {
    std::string s(metrics_.size() * 3, '\n');
    c_.write(s);
}

void console::Cli::write() const {
    c_.shiftRow(metrics_.size() * 3);
    for (const auto &m: metrics_) {
        m.second.write(c_);
        c_.shiftRow(-3);
    }
}

void console::Cli::setProb(const std::string &metric, double p) {
    for (int i = 0; i < metrics_.size(); ++i) {
        auto &m = metrics_[i];
        if (m.first == metric) {
            int dr = ((int) metrics_.size() - i) * 3;
            c_.shiftRow(dr);
            m.second.setProb(p, c_);
            c_.shiftRow(-dr);
        }
    }
}

void console::Cli::worker() {
    while (running_) {
        int ch = console::Cursor::hgetch();
        if (ch == 'q') {
            c_.write("exit!\n");
            std::exit(0);
        }
    }
}