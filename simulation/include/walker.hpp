#ifndef WALKER_HPP
#define WALKER_HPP

#include "network.hpp"
template <typename Location> class Walker {
public:
    Walker(int id, Location startingPos) { _id = id; _pos=startingPos; }
    int getId() const { return _id; }
    Location getLoc() { return _pos; }
    Location step(Network& net, float alpha);

private:
    int _id;
    Location _pos;
};

#endif // WALKER_HPP