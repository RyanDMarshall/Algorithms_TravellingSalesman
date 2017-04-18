#include "PathGraph.h"


using namespace std;
enum Include { Inc, Exc, NoRest };

PathGraph::PathGraph() : path_length(0) { }
PathGraph::PathGraph(vector<pair<size_t, size_t> > & positions, vector<size_t> & luum_in)
: best_path(luum_in) {

    for (size_t i = 0; i < positions.size(); ++i) {
        Vertex v(positions[i].first, positions[i].second);
        graph.push_back(v);
    }

    adj_matrix.reserve(graph.size());
    for (size_t i = 0; i < graph.size(); ++i) {
        vector<double> curr(graph.size());
        adj_matrix.push_back(curr);
    }

    calc_adj_matrix(adj_matrix);
    path_length = find_path_length(best_path);
}

void PathGraph::TSP() {

    find_path();

    cout << "The total campaign length is: " << path_length << '\n';
    cout << "Lukefahr should visit each state in the following order: \n";
    for (size_t i = 0; i < best_path.size(); ++i) {
        cout << best_path[i] << '\n';
    }

    return;
}

void PathGraph::find_path() {

    deque<size_t> unused;
    vector<size_t> path;
    for (size_t i = 1; i < adj_matrix.size(); ++i) {
        unused.push_back(i);
    }
    path.push_back(0);
    find_path_helper(unused, path);
    return;
}

void PathGraph::find_path_helper(deque<size_t> & unused, vector<size_t> & path) {

    if (unused.empty()) {
        double length = find_path_length(path);
        if (length < path_length) {
            path_length = length;
            best_path = path;
        }
    }
    if (!promising(unused, path)) {
        return;
    }
    for (size_t i = 0; i < unused.size(); ++i) {
        path.push_back(unused.front());
        unused.pop_front();
        find_path_helper(unused, path);
        unused.push_back(path.back());
        path.pop_back();
    }

    return;
}

void PathGraph::calc_adj_matrix(vector<vector<double> > & adj_matrix) {

    for (size_t i = 0; i < graph.size(); ++i) {
        for (size_t j = i; j < graph.size(); ++j) {
            if (i != j) {
                adj_matrix[i][j] = dist(graph[i], graph[j]);
                adj_matrix[j][i] = adj_matrix[i][j];
            }
            else {
                adj_matrix[i][j] = DBL_MAX;
            }
        }
    }

    return;
}
bool PathGraph::promising(deque<size_t> & unused, vector<size_t> & path) {

    double length = find_path_length(path);
    length -= (adj_matrix[path[0]][path[path.size() - 1]]);
    if (path_length < length) {
        return false;
    }
    vector<pair<size_t, size_t> > mst_vec;
    vector<bool> mst_diff(unused.size(), false);
    for (size_t i = 0; i < unused.size(); ++i) {
        mst_vec.push_back(make_pair(graph[unused[i]].x, graph[unused[i]].y));
    }

    MSTGraph unused_mst(0, mst_diff, mst_vec);
    double unused_size = unused_mst.Prim_d();
    double min_dist_0 = DBL_MAX;
    double min_dist_n = DBL_MAX;
    double current_0, current_n;
    for (size_t i = 0; i < unused.size(); ++i) {
        current_0 = adj_matrix[unused[i]][path[0]];
        current_n = adj_matrix[unused[i]][path[path.size() - 1]];


        //current_0 = dist(unused_mst.graph[i], graph[path[0]]);
        //current_n = dist(unused_mst.graph[i], graph[path[path.size() - 1]]);
        if (current_0 < min_dist_0) {
            min_dist_0 = current_0;
        }
        if (current_n < min_dist_n) {
            min_dist_n = current_n;
        }
    }

    length += (unused_size + min_dist_0 + min_dist_n);

    if (path_length < length) {
        return false;
    }

    /*size_t current = 0;
    size_t min1 = 0;
    size_t min2 = 1;
    for (size_t i = 0; i < unused.size(); ++i) {
        current = unused.front();
        unused.pop_front();
        for (size_t j = 0; j < adj_matrix.size(); ++j) {
            if (adj_matrix[current][j] < adj_matrix[current][min2]) {
                if (adj_matrix[current][j] < adj_matrix[current][min1]) {
                    min2 = min1;
                    min1 = j;
                }
                else {
                    min2 = j;
                }
            }
        }
        length += (adj_matrix[current][min1] + adj_matrix[current][min2]);
        unused.push_back(current);
    }
    if (path_length < (0.5 * length)) {
        return false;
    }*/

    return true;
}

double PathGraph::find_path_length(vector<size_t> & path) {
    double sum = 0;
    if (path.size() > 1) {
        for (size_t i = 0; i + 1 < path.size(); ++i) {
            sum += adj_matrix[path[i]][path[i + 1]];
        }
        sum += adj_matrix[path[0]][path[path.size() - 1]];
    }
    return sum;
}
double PathGraph::dist(Vertex a, Vertex b) {
    return (sqrt(((double(b.x) - double(a.x))*(double(b.x) - double(a.x)))
               + ((double(b.y) - double(a.y))*(double(b.y) - double(a.y)))));
}
double PathGraph::dist(MSTGraph::Vertex a, Vertex b) {
    return (sqrt(((double(b.x) - double(a.x))*(double(b.x) - double(a.x)))
        + ((double(b.y) - double(a.y))*(double(b.y) - double(a.y)))));
}