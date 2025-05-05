#ifndef EVENT_HPP
#define EVENT_HPP

#include <cmath>

/*
  Event times are just specific times when nodes or edges appear or disappear
  We define a specific class for this in order to redefine comparison operators:
  We would then like to know whether a node is present between two event times
  or not.
*/
class Event {
public:
  Event(float val) : _val(val), _eps(1e-7) {}
  float getVal() { return _val; }

  bool operator==(const Event &other) const {
    return std::fabs(_val - other._val) < _eps;
  }

  bool operator!=(const Event &other) const { return not(*this == other); }

  bool operator<(const Event &other) const { return _val < other._val; }

  bool operator<=(const Event &other) const {
    return _val < other._val || *this == other;
  }

  bool operator>(const Event &other) const { return not(*this <= other); }

  bool operator>=(const Event &other) const { return not(*this < other); }

private:
  float _val;
  float _eps;
};

bool eventComp(Event a, Event b);

#endif // EVENT_HPP