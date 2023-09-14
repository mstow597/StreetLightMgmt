#include <iostream>

class TrafficLight {
    public:
        TrafficLight();
        void setStraightColor(const std::string &color);
        void setLeftColor(const std::string &color);
    private:
        std::string colorForward;
        std::string colorLeft;
};