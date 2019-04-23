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

struct Relevance {
    int id;
    double val;

    Relevance() {}

    Relevance(int id, double val) {
        this->id = id;
        this->val = val;
    }

    bool operator<(const Relevance &str) const {
        return (val < str.val);
    }
};
