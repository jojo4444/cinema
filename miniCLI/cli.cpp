//
// Created by jojo on 28.11.2021.
//

#include "cli.h"

console::Metric::Metric(const std::string &name) {
    active_ = false;
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

void console::Metric::activate() {
    active_ = true;
}

void console::Metric::deactivate() {
    active_ = false;
}

void console::Metric::changeActivity() {
    active_ = !active_;
}

void console::Metric::write(const Cursor &c) const {
    c.setActivatedMod(active_);
    for (const auto &line: lines_) {
        c.write(line);
        c.cursorToBegin();
        c.shiftRow(-1);
    }
    c.shiftRow(3);
    c.setDeactivatedMod();
}

void console::Metric::gauge() {
    std::stringstream ss;
    int cnt = prob_ * LEN_METRIC;

    ss << "│";
    for (int i = 0; i < console::LEN_METRIC; ++i) {
        ss << console::Cursor::colorForeground(0, 85 + (255 - 85) * i / (double) LEN_METRIC, 0)
           << (i < cnt ? "█" : " ");
    }
    ss << fDefault << "│";
    lines_[1] = ss.str();
}

console::Cli::Cli() {
    running_ = true;
    selectedID_ = 0;
    worker_ = std::thread(&Cli::worker, this);
}

console::Cli::~Cli() {
    running_ = false;
    worker_.join();
}

void console::Cli::add(const std::string &nameMetric) {
    metrics_.emplace_back(nameMetric, Metric(nameMetric));
    if (metrics_.size() == 1) {
        metrics_.back().second.activate();
    }
}

void console::Cli::prewrite() const {
    std::string s(metrics_.size() * 3, '\n');
    c_.write(s);
    write();
}

void console::Cli::write() const {
    c_.shiftRow(metrics_.size() * 3);
    for (const auto &m: metrics_) {
        m.second.write(c_);
        c_.shiftRow(-3);
    }
}

void console::Cli::rewriteMetric(int id) const {
    if (id < 0 || id >= metrics_.size()) {
        return;
    }
    int dr = (metrics_.size() - id) * 3;
    c_.shiftRow(dr);
    metrics_[id].second.write(c_);
    c_.shiftRow(-dr);
}

void console::Cli::changeActivity(int id) {
    if (id < 0 || id >= metrics_.size()) {
        return;
    }
    metrics_[id].second.changeActivity();
    rewriteMetric(id);
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
            std::exit(0);
        }
        if (ch == 'w') {
            if (selectedID_ > 0) {
                changeActivity(selectedID_);
                changeActivity(--selectedID_);
            }
        }
        if (ch == 's') {
            if (selectedID_ + 1 < metrics_.size()) {
                changeActivity(selectedID_);
                changeActivity(++selectedID_);
            }
        }
    }
}