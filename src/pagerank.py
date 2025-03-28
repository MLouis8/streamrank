import numpy as np
import networkx as nx
import math


def create_transition_and_dangling_matrices(
    graph: nx.DiGraph,
) -> tuple[np.ndarray, np.ndarray]:
    """
    Computes the transition probability matrix and the dangling node vector
    from a directed weighted (on the edges) graph from networkx.
    """
    n = graph.number_of_nodes()
    h = np.zeros((n, n))
    a = np.zeros(n)
    edge_weights = nx.get_edge_attributes(graph, "weight", default=1)
    for u in graph:
        if list(graph.successors(u)) == []:
            a[u] = 1
        total_weight = sum([edge_weights[(u, v)] for v in graph.successors(u)])
        for v in graph.successors(u):
            h[u, v] = edge_weights[(u, v)] / total_weight
    return h, a


def pagerank(
    h_matrix: np.ndarray,
    a_vector: np.ndarray,
    alpha: float = 0.85,
    p_vector: np.ndarray | None = None,
    iter: int = 50,
    eps: float = 1e-5,
) -> np.ndarray:
    """
    PageRank implemented with the original power method.

    @parameters:
        h_matrix: hyperlink matrix (Markov matrix from original graph)
        a_vector: dangling node vector
        alpha:    probability of using h_matrix  / convergence ratio
        p_vector: personalization vector, if none then 1/n vector is taken
        iter:     number of iterations of the power method
        eps:      minimal convergence gap

    @returns:
        r: pagerank vector
    """
    n = len(h_matrix)
    if not isinstance(p_vector, np.ndarray):
        p_vector = np.ones((1, n)) * 1 / n
    r = p_vector
    for _ in range(iter):
        rr = alpha * r @ h_matrix + (alpha * r @ a_vector + 1 - alpha) * p_vector
        if np.linalg.norm(r - rr) < eps:
            return rr
        r = rr
    return rr