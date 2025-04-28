Graph generateRDGraph(size_t n, float p, std::default_random_engine& eng)
{
    std::uniform_real_distribution<> distr(0., 1.);
    Graph G = Graph(n, []);
    for (size_t i=0; i<n; i++)
    {
        for (size_t j=0; j<n; j++)
        {
            if (distr(eng) <= p)
                G.addVertex(i, j);
        }
    }
    return G;
}