int main() {
    size_t nbVertices;
    size_t nbWalkers;
    float probaEdge;
    float T;

    size_t nbSteps;
    float epsilon;

    Graph G = generateRDGraph(nbVertices, probaEdge);
    StreamGraph S = generateRDStreamGraph(nbVertices, probaEdge, T);

    vector<> res;
    res = simulatePRRW(nbWalkers, nbSteps, G);
    res = simulateCTRW(nbWalkers, nbSteps, S);
    res = simulateDTRW(nbWalkers, nbSteps, );
    res = simulateADTRW(nbWalkers, nbSteps);
    return 0;
}