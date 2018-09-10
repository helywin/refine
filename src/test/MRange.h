//
// Created by jiang.wenqiang on 2018/9/7.
//

#ifndef REFINE_RANGE_H
#define REFINE_RANGE_H


class MRange {
    class Iter {
    private:
        MRange *_p_vec;
        int _pos;
    public:
        Iter(MRange *p_vec, int pos) : _p_vec(p_vec), _pos(pos) {}
        bool operator!= (const Iter &other) const {
            return _pos != other._pos;
        }

        int &operator*() const {
            return _p_vec->get(_pos);
        }
        const Iter &operator++() {
            ++_pos;
            return *this;
        }
    };

private:
    int cell[20] = {0};

public:
    MRange() = default;
    explicit MRange(std::initializer_list<int> list) {
        Q_ASSERT(list.size() <= 20);
        int i = 0;
        for (const auto &iter : list) {
            cell[i] = iter;
            i += 1;
        }
    }
    Iter begin() {
        return Iter(this, 0);
    }

    Iter end() {
        return Iter(this, 20);
    }

    int &get(int index) {
        return cell[index];
    }
};

#endif //REFINE_RANGE_H
