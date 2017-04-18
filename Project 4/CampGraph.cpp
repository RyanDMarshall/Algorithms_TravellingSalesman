#include "CampGraph.h"

using namespace std;

/*
    struct State {
        int votes;
        double cost;
        int idx;
}

    struct Configuration {
        int idx;
        double cost;
        int votes;
        vector<bool> used;
}

    class Knapsack {
        Knapsack(vector<State>)
            Sort states vectors by votes/cost descending
            Compute initial guess by Greedy Discrete Knapsack
        bool promising(Configuration* c)
            Compute best possible by Greedy Fractional Knapsack
        void search()
            stack<Configuration*>
            s.push(empty);
            while !empty()
            curr = s.pop()
            if (promising(curr))
            s.push(newConfig(curr))
            curr.cost += states[curr.idx - 1]
            curr.votes += states[curr.idx - 1]
            if (promising(curr))
            s.push(new Config(curr))
            delete curr
}

*/

CampGraph::CampGraph() : budget(0), cost(0), votes(0) { }
CampGraph::CampGraph(double & budget_in, vector<double> & costs_in, vector<size_t> & votes_in)
    : budget(budget_in), cost(0), votes(0) {

    for (size_t i = 0; i < costs_in.size(); ++i) {
        Vertex v(votes_in[i], i, costs_in[i]);
        graph.push_back(v);
    }

    sort_by_ratio();
}

void CampGraph::KNAP() {

    vector<size_t> states;
    deque<pair<size_t, bool> > s;
    greedy_disc(states);

    s.push_back(make_pair(0, false));
    KNAP_helper(states, s);
    s.pop_back();
    s.push_back(make_pair(0, true));
    KNAP_helper(states, s);
    s.pop_back();

    vector<size_t> final_vec;
    for (size_t i = 0; i < states.size(); ++i) {
        final_vec.push_back(graph[states[i]].index);
    }
    sort(final_vec.begin(), final_vec.end());

    cout << "Expected number of votes on Election day: " << votes << "\nSenator Lukefahr should campaign in the following states:\n";
    for (size_t i = 0; i < final_vec.size(); ++i) {
        cout << final_vec[i] << '\n';
    }

    return;
}

void CampGraph::KNAP_helper(vector<size_t> & states, deque<pair<size_t, bool> > & s) {

    /*for (size_t i = 0; i < s.size(); ++i) {
        pair<size_t, bool> curr;
        curr = s.front();
        s.pop_front();
        cout << curr.second << ' ';
        s.push_back(curr);
    }
    cout << endl;*/

    if (s.empty())
    {
        return;
    }
    else if (s.size() == graph.size()) {
        pair<size_t, double> s_vote_cost = calc_s(s);
        if ((s_vote_cost.first > votes && s_vote_cost.second < budget)|| (s_vote_cost.first == votes && s_vote_cost.second < cost)) {
            votes = s_vote_cost.first;
            cost = s_vote_cost.second;
            states.clear();
            pair<size_t, bool> s_current;
            for (size_t i = 0; i < s.size(); ++i) {
                s_current = s.front();
                s.pop_front();
                if (s_current.second) {
                    states.push_back(s_current.first);
                }
                s.push_back(s_current);
            }
        }
        return;
    }
    else if (!promising(s)) {
        return;
    }
    else {
        s.push_back(make_pair(s.size(), false));
        KNAP_helper(states, s);
        s.pop_back();
        s.push_back(make_pair(s.size(), true));
        KNAP_helper(states, s);
        s.pop_back();
    }
}

bool CampGraph::promising(deque<pair<size_t, bool> > & s) {

    size_t upper = greedy_fract(s);
    if (votes > upper) {
        return false;
    }
    return true;
}

size_t CampGraph::calc_votes(vector<size_t> & states) {
    size_t votes = 0;
    for (size_t i = 0; i < states.size(); ++i) {
        votes += graph[i].votes;
    }
    return votes;
}
pair<size_t, double> CampGraph::calc_s(deque<pair<size_t, bool> > & s) {
    pair<size_t, double> s_vote_cost(make_pair(0,0));
    pair<size_t, bool> s_current;
    for (size_t i = 0; i < s.size(); ++i) {
        s_current = s.front();
        s.pop_front();
        if (s_current.second) {
            s_vote_cost.first += graph[i].votes;
            s_vote_cost.second += graph[i].cost;
        }
        s.push_back(s_current);
    }
    return s_vote_cost;
}

void CampGraph::greedy_disc(vector<size_t> & states) {

    for (size_t i = 0; i < graph.size(); ++i) {
        if (cost + graph[i].cost < budget) {
            states.push_back(i);
            cost += graph[i].cost;
            votes += graph[i].votes;
        }
    }

    return;
}
size_t CampGraph::greedy_fract(deque<pair<size_t, bool> > & s) {

    size_t g_votes = 0;
    double g_cost = 0;
    double current_cost = 0;
    pair<size_t, bool> s_current;

    for (size_t i = 0; i < s.size(); ++i) {
        s_current = s.front();
        s.pop_front();
        if (s_current.second) {
            g_cost += graph[i].cost;
            g_votes += graph[i].votes;
            if (g_cost > budget) {
                s.push_back(s_current);
                return votes - 1;
            }
        }
        s.push_back(s_current);
    }
    if (g_cost > budget) {
        return votes - 1;
    }
    for (size_t i = s.size(); i < graph.size(); ++i) {
        if ((g_cost + graph[i].cost) < budget) {
            g_cost += graph[i].cost;
            g_votes += graph[i].votes;
        }
        else {
            g_votes += size_t(((graph[i].votes / graph[i].cost) * (budget - g_cost)));
            return g_votes;
        }
    }
    return g_votes;
}

void CampGraph::sort_by_ratio() {
    sort(graph.begin(), graph.end(), ratio_comp());
    return;
}