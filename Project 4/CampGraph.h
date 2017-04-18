#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <vector>
#include <deque>
#include <set>

class CampGraph {
private:
    class Vertex {
    private:
        friend class CampGraph;
        size_t votes;
	size_t index;
        double cost;
    public:
        Vertex() : votes(0), index(0), cost(0) { assert(false); };
        Vertex(size_t votes_in, size_t index_in, double cost_in)
            : votes(votes_in), index(index_in), cost(cost_in) { };
    };

    double budget;
    double cost;
    size_t votes;
    std::vector<Vertex> graph;
public:
    struct ratio_comp {
        bool operator() (const Vertex & v1, const Vertex & v2) {
            return (double(v1.votes)/v1.cost) > (double(v2.votes)/v2.cost);
        }
    };
    CampGraph();
    CampGraph(double & budget_in, std::vector<double> & costs_in, std::vector<size_t> & votes_in);

    void KNAP();
    void KNAP_helper(std::vector<size_t> & states, std::deque<std::pair<size_t, bool> > & s);
    bool promising(std::deque<std::pair<size_t, bool> > & s);
    size_t calc_votes(std::vector<size_t> & states);
    std::pair<size_t, double> calc_s(std::deque<std::pair<size_t, bool> > & s);

    void greedy_disc(std::vector<size_t> & states);
    size_t greedy_fract(std::deque<std::pair<size_t, bool> > & s);
    void sort_by_ratio();
};
