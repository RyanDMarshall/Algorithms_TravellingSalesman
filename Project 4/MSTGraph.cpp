#include <unordered_map>
#include <algorithm>
#include <vector>
#include <set>
#include <math.h> 
#include "MSTGraph.h"

using namespace std;

MSTGraph::MSTGraph() : diff_mult(0) {assert(false);}
MSTGraph::MSTGraph(double diff_mult_in, vector<bool> & diff_set_in,
    vector<pair<size_t, size_t> > & positions)
    : diff_mult(diff_mult_in), diff(diff_set_in) {

    for (size_t i = 0; i < positions.size(); ++i) {
        Vertex v(positions[i].first, positions[i].second);
        graph.push_back(v);
    }
}

void MSTGraph::Prim() {

    unordered_map<size_t, pair<size_t, double> > not_visited;
    vector<size_t> previous(graph.size(), 0);

    double current_dist = 0.0;

    for (size_t i = 1; i < graph.size(); ++i) {
        current_dist = dist(graph[0], graph[i]);
        if (diff[0]) {
            current_dist *= diff_mult;
        }
        if (diff[i]) {
            current_dist *= diff_mult;
        }
        not_visited[i] = make_pair(0, current_dist);
    }
    
    double total_dist = 0;
    size_t last_insert = 0;

    while (!not_visited.empty()) {
        update_dist(not_visited, last_insert);
        pair<size_t, pair<size_t, double> > min = find_min(not_visited);
        last_insert = min.first;
        previous[min.first] = min.second.first;
        not_visited.erase(min.first);

        total_dist += min.second.second;
    }

    cout << "Total distance: " << total_dist << '\n' << "Trade routes:\n";
    for (size_t i = previous.size() - 1; i > 0; --i) {
        cout << min(i, previous[i]) << ' ' << max(i, previous[i]) << '\n';
    }
    return;
}
double MSTGraph::Prim_d() {

    unordered_map<size_t, pair<size_t, double> > not_visited;
    vector<size_t> previous(graph.size(), 0);

    double current_dist = 0.0;

    for (size_t i = 1; i < graph.size(); ++i) {
        current_dist = dist(graph[0], graph[i]);
        if (diff[0]) {
            current_dist *= diff_mult;
        }
        if (diff[i]) {
            current_dist *= diff_mult;
        }
        not_visited[i] = make_pair(0, current_dist);
    }

    double total_dist = 0;
    size_t last_insert = 0;

    while (!not_visited.empty()) {
        update_dist(not_visited, last_insert);
        pair<size_t, pair<size_t, double> > min = find_min(not_visited);
        last_insert = min.first;
        previous[min.first] = min.second.first;
        not_visited.erase(min.first);

        total_dist += min.second.second;
    }

    return total_dist;
}

double MSTGraph::dist(Vertex a, Vertex b) {
    return (sqrt(((b.x - a.x)*(b.x - a.x)) + ((b.y - a.y)*(b.y - a.y))));
}

pair<size_t, pair<size_t, double> > MSTGraph::find_min(std::unordered_map<size_t, pair<size_t, double> > & not_visited)
{
    auto current_min = not_visited.begin();
    for (auto iter = not_visited.begin(); iter != not_visited.end(); ++iter) {
        if ((iter->second).second < (current_min->second).second) {
            current_min = iter;
        }
    }

    return *current_min;
}
void MSTGraph::update_dist(std::unordered_map<size_t, std::pair<size_t, double> > & not_visited, size_t last_visited) {
    
    double new_dist = 0;

    for (auto iter = not_visited.begin(); iter != not_visited.end(); ++iter) {
        new_dist = dist(graph[iter->first], graph[last_visited]);
        if (diff[iter->first]) {
            new_dist *= diff_mult;
        }
        if (diff[last_visited]) {
            new_dist *= diff_mult;
        }
        if (new_dist < (iter->second).second) {
            (iter->second).first = last_visited;
            (iter->second).second = new_dist;
        }
    }

    return;
}