#include "graph.hpp"

template <typename Location> <typename > class Walker {

    size_t id;
    Location position;

    Walker(size_t id) : id{id} {}

    size_t getId() const { return id; }
    Location getLoc() const { return position; }

    Location step(Graph& grid, float alpha);
};