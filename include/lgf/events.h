#pragma once
#include <functional>
#include <vector>
#include <unordered_map>

namespace LGF::Events {
        class Events {
            private:
            std::unordered_map<unsigned int, std::function<void()>> func;
            unsigned int lastID = 0;
            public:
            unsigned int operator+=(const std::function<void()>& other);
            void operator-=(unsigned int ID);
            void trigger();
        };
};