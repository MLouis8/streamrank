# Streamrank

Most networks have a temporal nature: they evolve through time. Providing a
complete rigorous framework for the analysis of such networks has been a focal point for
much research in the complex systems community and beyond. In particular, a team led by
Matthieu Latapy in the LIP6 laboratory (CNRS/Sorbonne Université) has proposed a model
called stream graphs [[1]](#1). A proper definition for a PageRank [[2, 3]](#2) type of measure on stream
graphs remains largely an open question.
This project focus on a proposed method based on a continuous time random walk [[4,
5]](#4) adapted to the structure of a stream graph. This random walk combines jumps taken along
links with a probability proportional to the duration of links, and waiting times during which it
might jump anywhere in the stream graph. A discrete approximation of the temporal structure
may then lead to a directed acyclic graph where link weights correspond to the CTRW
measure — and from there to a simple definition of a measure similar to PageRank.

## References

<a id="1">[1]</a>
Latapy, M., Viard, T., & Magnien, C. (2018). Stream graphs and link streams for the modeling of interactions over time. Social Network Analysis and Mining, 8, 1-29.

<a id="2">[2]</a>
Rozenshtein, Polina, and Aristides Gionis. "Temporal pagerank." Machine Learning and Knowledge Discovery in Databases: European Conference, ECML PKDD 2016, Riva del Garda, Italy, September 19-23, 2016, Proceedings, Part II 16. Springer International Publishing, 2016.

<a id="3">[3]</a>
Langville, A. N., & Meyer, C. D. (2006). Google's PageRank and beyond: The science of search engine rankings. Princeton university press.

<a id="4">[4]</a>
Kenkre, Vasudev M., Elliot W. Montroll, and Michael F. Shlesinger. "Generalized master equations for continuous-time random walks." Journal of Statistical Physics 9 (1973): 45-50.

<a id="5">[5]</a>
Masuda, Naoki, Mason A. Porter, and Renaud Lambiotte. "Random walks and diffusion on networks." Physics reports 716 (2017): 1-58.
