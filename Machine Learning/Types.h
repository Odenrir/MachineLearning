#pragma once

struct Distances {
    int x, y;
    double val;

    Distances() {}

    Distances(int x, int y, double val) {
        this->x = x;
        this->y = y;
        this->val = val;
    }

    bool operator<(const Distances &str) const {
        return (val < str.val);
    }
};