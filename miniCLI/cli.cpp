//
// Created by jojo on 28.11.2021.
//

#include "cli.h"

console::Cli *console::Cli::singleton = nullptr;
std::mutex console::Cli::mu_single_;

void console::Cli::unable() {
    const std::lock_guard<std::mutex> lock(mu_single_);
    if (singleton == nullptr) {
        singleton = new console::Cli();
    }
}

void console::Cli::disable() {
    const std::lock_guard<std::mutex> lock(mu_single_);
    delete singleton; /// nullptr -> no effect
}

console::Metric::Metric(const std::string &name) {
    active_ = false;
    name_ = name;
    prob_ = 0;
    cntReview_ = 0;

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

void console::Metric::addReview(double val, const Cursor &c) {
    prob_ += val;
    cntReview_++;
    gauge();
    write(c);
}

double console::Metric::getProb() const {
    return prob_;
}

double console::Metric::getCntReview() const {
    return cntReview_;
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

    int cnt = (cntReview_ ? prob_ / cntReview_ * LEN_METRIC : 0);

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
    days_ = 0;
    selectedID_ = 0;
    running_ = true;
    worker_ = std::thread(&Cli::worker, this);
}

console::Cli::~Cli() {
    running_ = false;
    c_.write("ZA VARDU TOKI WO TOMARE!\n");
    worker_.join();
}

bool console::Cli::isRunning() const {
    return running_;
}

void console::Cli::setDays(int days) {
    if (!running_) {
        return;
    }
    const std::lock_guard<std::mutex> lock(mu_);
    days_ = days;
    c_.shiftRow(1);
    c_.write("days passed: " + std::to_string(days_));
    c_.shiftRow(-1);
    c_.cursorToBegin();
}

void console::Cli::add(const std::string &nameMetric) {
    if (!running_) {
        return;
    }
    metrics_.emplace_back(nameMetric, Metric(nameMetric));
    if (metrics_.size() == 1) {
        metrics_.back().second.activate();
    }
}

void console::Cli::prewrite() const {
    if (!running_) {
        return;
    }
    std::string s(metrics_.size() * 3, '\n');
    c_.write(s);
    write();
}

void console::Cli::rewriteMetric(int id) const {
    if (!running_) {
        return;
    }
    const std::lock_guard<std::mutex> lock(mu_);
    if (id < 0 || id >= metrics_.size()) {
        return;
    }
    int dr = (metrics_.size() - id) * 3 + 1;
    c_.shiftRow(dr);
    metrics_[id].second.write(c_);
    c_.shiftRow(-dr);
}

void console::Cli::changeActivity(int id) {
    if (id < 0 || id >= metrics_.size() || !running_) {
        return;
    }
    metrics_[id].second.changeActivity();
    rewriteMetric(id);
}

void console::Cli::setProb(const std::string &metric, double p) {
    if (!running_) {
        return;
    }
    const std::lock_guard<std::mutex> lock(mu_);
    for (int i = 0; i < metrics_.size(); ++i) {
        auto &m = metrics_[i];
        if (m.first == metric) {
            int dr = ((int) metrics_.size() - i) * 3 + 1;
            c_.shiftRow(dr);
            m.second.setProb(p, c_);
            c_.shiftRow(-dr);
            break;
        }
    }
}

void console::Cli::updateMetric(const std::string &metric, double val) {
    if (!running_) {
        return;
    }
    const std::lock_guard<std::mutex> lock(mu_);
    for (int i = 0; i < metrics_.size(); ++i) {
        auto &m = metrics_[i];
        if (m.first == metric) {
            int dr = ((int) metrics_.size() - i) * 3 + 1;
            c_.shiftRow(dr);
            m.second.addReview(val, c_);
            c_.shiftRow(-dr);
            break;
        }
    }
}

void console::Cli::write() const {
    if (!running_) {
        return;
    }
    c_.shiftRow(metrics_.size() * 3 + 1);
    for (const auto &metric: metrics_) {
        metric.second.write(c_);
        c_.shiftRow(-3);
    }
    c_.write("days passed: " + std::to_string(days_) + "\n");
}

void console::Cli::worker() {
    while (running_) {
        int ch = console::Cursor::hgetch();

        if (!running_) {
            break;
        }

        if (ch == 'q' || ch == 'Q') {
            running_ = false;
            return;
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