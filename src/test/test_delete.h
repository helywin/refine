//
// Created by jiang.wenqiang on 2018/7/12.
//

#ifndef CORE_TEST_DELETE_H
#define CORE_TEST_DELETE_H

#include <iostream>

using std::cout;
using std::endl;

class TestDelete {
public:
    int a;

    TestDelete() : a(1) {};

    virtual void print() {
        cout << "Hello World!" << endl;
    }
};

class TestDeleteChild : public TestDelete {
public:
    void print() final = delete;
};

#endif //CORE_TEST_DELETE_H
