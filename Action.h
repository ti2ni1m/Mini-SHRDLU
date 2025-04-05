#include <iostream>
using namespace std;
class Action{
    public:
    int destination, source;
    double heuristic;
    bool operator <(const Action a) const
    {
        return this->heuristic > a.heuristic;
    }
    static bool compare(Action n1, Action n2)
    {
        return n1.heuristic > n2.heuristic;
    }
};