#include <iostream>
#include <cassert>
#include <math.h>
#include <float.h>
#include <queue>
#include <vector>
#include <deque>
#include <set>
#include "MSTGraph.h"

class PathGraph {
private:
    class Vertex {
    public:
        Vertex() : x(0), y(0) { assert(false); }
        Vertex(size_t x_in, size_t y_in) : x(x_in), y(y_in) { }
    private:
        friend class PathGraph;
        size_t x;
        size_t y;
    };

    void find_path_helper(std::deque<size_t> & unused, std::vector<size_t> & path);

    std::vector<Vertex> graph;
    std::vector<size_t> best_path;
    std::vector<std::vector<double> > adj_matrix;
    double path_length;
public:
    PathGraph();
    PathGraph(std::vector<std::pair<size_t, size_t> > & positions, std::vector<size_t> & luum_in);

    void TSP();
    void find_path();

    void calc_adj_matrix(std::vector<std::vector<double> > & adj_matrix);
    bool promising(std::deque<size_t> & unused, std::vector<size_t> & path);
    double find_path_length(std::vector<size_t> & path);
    double dist(Vertex a, Vertex b);
    double dist(MSTGraph::Vertex a, Vertex b);
};