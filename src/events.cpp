#include "lgf/events.h"

unsigned int LGF::Events::Events::operator+=(const std::function<void()>& other) {
    this->func[lastID] = other;
    return lastID++;
}

void LGF::Events::Events::operator-=(unsigned int ID) {
    this->func.erase(ID);
}

void LGF::Events::Events::trigger() {
    for (auto& [k, v] : this->func) {
        v();
    }
}