#include <vector>

class tempoNetwork {
public:
    tempoNetwork(float tStart, float tEnd, int n): {}
    float getStartingTime() { return _tStart; }
    float getEndingTime() { return _tEnd; }
    int getSize() { return _n; }
    
    // methods to add
    // check if a time is valid for a node
    // check if a time is valid for an edge
    // get direct neighbours of a node
    // get future neighbours of a node
    // get t+ and t- for a node and for an edge

private:
    float _tStart;
    float _tEnd;
    int _n;
    std::vector<vector<float>> _W;
    std::map<> _E;
};

// function to add
// generate a random tempoNetwork
    