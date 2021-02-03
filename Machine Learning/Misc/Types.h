/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#pragma once
#include <map>

struct Distances {
    int x, y, cX, cY;
    double val;

    Distances() {}

    Distances(int x, int y, double val, int cX = -1, int cY = -2) {
        this->x = x;
        this->y = y;
        this->val = val;
        this->cX = cX;
        this->cY = cY;
    }

    bool isIncluded(int id) {
        return id == x || id == y;
    }

    int getXY(int id) {
        if (id == x) {
            return y;
        } else if (id == y) {
            return x;
        } else {
            return -1;
        }
    }

    bool sameClass() {
        return cX == cY;
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

struct LocalSet {
    int id, idNE, category, cardinality;
    double radius;
    std::map<int, char> LS;

    LocalSet() {}

    LocalSet(int id, int idNE, double radius, int category) {
        this->id = id;
        this->idNE = idNE;
        this->category = category;
        this->radius = radius;
        this->cardinality = 1;
        this->LS[id] = 'v';
    }

    bool tryAppend(int id, double distance) {
        if (distance < radius) {
            this->LS[id] = 'v';
            this->cardinality++;
            return true;
        }
        return false;
    }

    bool isIn(int id) {
        return this->LS.count(id) > 0;
    }

    bool operator<(const LocalSet &str) const {
        return (cardinality < str.cardinality);
    }
};

template<typename M, typename V>
void MapToVec(const M &m, V &v) {
    for (typename M::const_iterator it = m.begin(); it != m.end(); ++it) {
        v.push_back(it->second);
    }
}
