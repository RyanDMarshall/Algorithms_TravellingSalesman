#include <iostream>
#include <unordered_map>
#include <cassert>
#include <vector>
#include <set>

class MSTGraph {
private:
    class Vertex {
    public:
        Vertex() : x(0), y(0) {assert(false);}
        Vertex(size_t x_in, size_t y_in) : x(x_in), y(y_in) { }
    private:
        friend class MSTGraph;
        friend class PathGraph;
        size_t x;
        size_t y;
    };

    friend class PathGraph;

    double diff_mult;
    std::vector<bool> diff;
    std::vector<Vertex> graph;

    std::pair<size_t, std::pair<size_t, double> > find_min(std::unordered_map<size_t, std::pair<size_t, double> > & not_visited);
    void update_dist(std::unordered_map<size_t, std::pair<size_t, double> > & not_visited, size_t last_visited);
public:
    MSTGraph();
    MSTGraph(double diff_mult_in, std::vector<bool> & diff_set_in,
            std::vector<std::pair<size_t, size_t> > & positions);
    void Prim();
    double Prim_d();

    double dist(Vertex a, Vertex b);
};