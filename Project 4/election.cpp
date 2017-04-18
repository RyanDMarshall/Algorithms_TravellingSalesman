#include <iostream>
#include <iomanip>
#include <cassert>
#include <unordered_map>
#include <string>
#include <vector>
#include <set>
#include <getopt.h>
#include "CampGraph.h"
#include "PathGraph.h"
#include "Mode.h"

using namespace std;

Mode read_in(int argc, char* argv[]);
void campaign_read_in(double & budget, vector<double> & costs, vector<size_t> & votes);
void mst_read_in(double & diff, vector<pair<size_t, size_t> > & posts, vector<bool> & diff_set);
void path_read_in(vector<pair<size_t, size_t> > & posts, vector<size_t> & L);

int main(int argc, char* argv[]) {

    std::ios_base::sync_with_stdio(false);
    cout << std::setprecision(2);
    cout << std::fixed;
    
    Mode mode;

    try {
        mode = read_in(argc, argv);
    }
    catch (char const* exc) {
        cerr << exc << '\n';
        return 1;
    }

    switch (mode) {
    case Mode::CAMPAIGN: {
        double b = 0;
        vector<double> costs;
        vector<size_t> votes;

        try {
            campaign_read_in(b, costs, votes);
        }
        catch (char const* exc) {
            cerr << exc << '\n';
            return 1;
        }

        CampGraph graph(b, costs, votes);
        graph.KNAP();
    }
    break;
    case Mode::MST: {
        double d = 0.0;
        vector<pair<size_t, size_t> > posts;
        vector<bool> diff_set;

        try {
            mst_read_in(d, posts, diff_set);
        }
        catch (char const* exc) {
            cerr << exc << '\n';
            return 1;
        }

        MSTGraph graph(d, diff_set, posts);
        graph.Prim();
    }
    break;
    case Mode::PATH: {
        vector<pair<size_t, size_t> > posts;
        vector<size_t> luum;

        try {
            path_read_in(posts, luum);
        }
        catch (char const* exc) {
            cerr << exc << '\n';
            return 1;
        }

        PathGraph graph(posts, luum);
        graph.TSP();

    }
    break;
    default: 
        return 0;
    }

    return 0;
}

Mode read_in(int argc, char* argv[]) {

    opterr = true; // Give us help with errors
    int choice = 0;
    int option_index = 0;
    option long_options[] = {
        { "mode",   required_argument,   nullptr,  'm'  },
        { nullptr,  0,                   nullptr,  '\0' }
    };

    while ((choice = getopt_long(argc, argv, "m:", long_options, &option_index)) != -1) {
        switch (choice) {
        case 'm':
            if (optarg[0] == 'C') {
                return Mode::CAMPAIGN;
            }
            if (optarg[0] == 'M') {
                return Mode::MST;
            }
            if (optarg[0] == 'P') {
                return Mode::PATH;
            }
            break;
        default:
            throw "Error: invalid option.";
        }
    }

    assert(false);
    return Mode::CAMPAIGN;
}
void campaign_read_in(double & budget, vector<double> & costs, vector<size_t> & votes) {

    int n = 0;
    string junk;
    char junk2;
    cin >> junk >> junk >> junk >> junk >> n >> junk >> junk >> junk >> junk2 >> budget;
    if (n < 0) {
        throw "Number of states must be greater than 0.\n";
    }
    double cost;
    int vote;
    for (size_t i = 0; i < size_t(n); ++i) {
        cin >> junk2 >> cost >> vote;
        if (cost < 0.001 || vote <= 0) {
            throw "Cost and number of votes must be greater than 0\n";
        }
        costs.push_back(cost);
        votes.push_back(size_t(vote));
    }

    return;
}
void mst_read_in(double & diff, vector<pair<size_t, size_t> > & posts, vector<bool> & diff_set) {

    int n = 0;
    int m = 0;

    string junk;

    cin >> junk >> junk >> junk >> junk >> n;

    if (n <= 0) {
        throw "N must be a positive integer.\n";
    }

    diff_set.resize(n);

    size_t current_x, current_y;
    for (size_t i = 0; i < size_t(n); ++i) {
        cin >> current_x >> current_y;
        posts.push_back(make_pair(current_x, current_y));
    }
    cin >> junk >> junk >> junk >> m
        >> junk >> junk >> diff;
    if (m < 0) {
        throw "M must be a positive integer.\n";
    }
    if (diff < 0.001) {
        throw "D must be positive.\n";
    }

    int current;

    for (size_t i = 0; i < size_t(m); ++i) {
        cin >> current;
        if (current < 0 || current >= n) {
            throw "Each difficulty node must be between 0 and N.\n";
        }
        diff_set[current] = true;
    }

    return;
}
void path_read_in(vector<pair<size_t, size_t> > & posts, vector<size_t> & L) {

    int n;
    string junk;
    cin >> junk >> junk >> junk >> junk >> n;
    
    if (n < 0) {
        throw "N must be a non-negative integer.\n";
    }

    size_t current_x, current_y;

    for (size_t i = 0; i < size_t(n); ++i) {
        cin >> current_x >> current_y;
        posts.push_back(make_pair(current_x, current_y));
    }

    cin >> junk >> junk >> junk;
    size_t luum_count = 0;
    set<size_t> diff_set;

    int current = -1;

    while (cin >> current) {
        if (current < 0 || current >= n) {
            throw "The Luum path must visit each node exactly once.\n";
        }
        ++luum_count;
        L.push_back(size_t(current));
        if (!diff_set.insert(size_t(current)).second) {
            throw "The Luum path must visit each node exactly once.\n";
        }
    }
    if (luum_count != size_t(n)) {
        throw "The Luum path must visit each node exactly once.\n";
    }

    return;
}
