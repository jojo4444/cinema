//
// Created by jojo on 28.11.2021.
//

#include "cli.h"

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
    for (const auto &line: lines_) {
        std::cout << line << std::endl;
    }
}

void console::Metric::gauge() {
    std::stringstream ss;
    int cnt = console::LEN_METRIC * prob_;

    ss << "│";
    for (int i = 0; i < console::LEN_METRIC; ++i) {
        ss << (i < cnt ? "█" : " ");
    }
    ss << "│";
    lines_[1] = ss.str();
}

console::Cli::Cli() = default;

void console::Cli::add(const std::string &nameMetric) {
    metrics_[nameMetric] = Metric(nameMetric);
}

void console::Cli::write() const {
    for (const auto &m: metrics_) {
        m.second.write();
    }
}

void console::Cli::setProb(const std::string &metric, double p) {
    if (metrics_.count(metric)) {
        metrics_[metric].setProb(p);
    }
}