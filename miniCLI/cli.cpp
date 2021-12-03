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

    gauge();
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
    gauge();
}

void console::Metric::deactivate() {
    active_ = false;
    gauge();
}

void console::Metric::changeActivity() {
    active_ = !active_;
    gauge();
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
    /// leftSize + name_.size() + rightSize == LEN_METRIC
    /// |leftSize - rightSize| <= 1

    int leftSize = (LEN_METRIC - (int) name_.size()) / 2;
    int rightSize = LEN_METRIC - (int) name_.size() - leftSize;

    const std::string boardSetLeft[3] = {"┌", "│", "└"};
    const std::string boardSetRight[3] = {"┐", "│", "┘"};

    int cnt = prob_ * LEN_METRIC;

    const auto activeColorForeground = Cursor::colorForeground(40, 40, 40);
    const auto activeColorBackground = Cursor::colorBackground(200, 200, 200);

    std::stringstream ss[3];
    for (int i = 0; i < 3; ++i) {
        if (active_) {
            ss[i] << activeColorBackground << activeColorForeground;
        }
        ss[i] << boardSetLeft[i];
        if (i == 0) {
            while (leftSize--) {
                ss[i] << "─";
            }
            for (char c: name_) {
                ss[i] << std::string{c};
            }
            while (rightSize--) {
                ss[i] << "─";
            }
        } else {
            for (int j = 0; j < LEN_METRIC; ++j) {
                if (i == 1) {
                    int h = 50 + 180 * j / (double) LEN_METRIC;
                    std::string cl;

                    if (active_) {
                        cl = Cursor::colorForeground(h, 0, 0);
                    } else {
                        cl = Cursor::colorForeground(0, h, 0);
                    }

                    ss[i] << cl << (j < cnt ? "█" : " ");
                } else {
                    ss[i] << "─";
                }
            }
        }
        ss[i] << "\033[39m";
        if (active_) {
            ss[i] << activeColorForeground;
        }
        ss[i] << boardSetRight[i] << "\033[39m\033[49m";
        lines_[i] = ss[i].str();
    }
}

console::Cli::Cli() {
    running_ = true;
    selectedID_ = 0;
    worker_ = std::thread(&Cli::worker, this);
}

console::Cli::~Cli() {
    running_ = false;
    c_.write("press any key for exit\n");
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

void console::Cli::rewriteMetric(int id) const {
    const std::lock_guard<std::mutex> lock(mu_);
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
    const std::lock_guard<std::mutex> lock(mu_);
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

void console::Cli::write() const {
    c_.shiftRow(metrics_.size() * 3);
    for (const auto &m: metrics_) {
        m.second.write(c_);
        c_.shiftRow(-3);
    }
}

void console::Cli::worker() {
    while (running_) {
        int ch = console::Cursor::hgetch();

        if (!running_) {
            break;
        }

        if (ch == 'q' || ch == 'Q') {
            std::exit(0);
        }
        if (ch == 'w' || ch == 'W') {
            if (selectedID_ > 0) {
                changeActivity(selectedID_);
                changeActivity(--selectedID_);
            }
        }
        if (ch == 's' || ch == 'S') {
            if (selectedID_ + 1 < metrics_.size()) {
                changeActivity(selectedID_);
                changeActivity(++selectedID_);
            }
        }
    }
}