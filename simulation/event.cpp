#include "include/event.hpp"

bool eventComp(Event a, Event b) { return a.val() <= b.val(); }
