/* [Google] http://www.careercup.com/question?id=5079989210841088

Given an bunch of airline tickets with [from, to], for example [MUC, LHR], [CDG, MUC], [SFO, SJC], [LHR, SFO],
please reconstruct the itinerary in order, for example: [ CDG, MUC, LHR, SFO, SJC ].
//tickets can be represented as nodes
*/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace {
    using namespace std;

    struct Ticket {
        string From;
        string To;
    };
    vector<string> AirTicketsItinerary(const vector<Ticket>& tickets) {
        vector<string> res;

        if (tickets.empty())
            return res;

        unordered_map<string, string> m; // From -> To mapping
        unordered_set<string> s; // Destinations

        for (const auto& t : tickets) {
            m[t.From] = t.To;
            s.insert(t.To);
        }

        // Find starting point
        for (const auto& t : tickets) {
            if (s.find(t.From) == s.end()) {
                res.emplace_back(t.From);
                break;
            }
        }

        // Build itinerary
        for (auto i = m.find(res.back()); i != m.end(); i = m.find(res.back())) {
            res.emplace_back(i->second);
        }

        return res;
    }
}

void TestAirTicketsItinerary() {
    vector<vector<Ticket>> v = { { {"MUC", "LHR"}, {"CDG", "MUC"}, {"SFO", "SJC"}, {"LHR", "SFO"} }
                               , {}
                               , { {"MUC", "SFO"} } };
    vector<vector<string>> ans = { { "CDG", "MUC", "LHR", "SFO", "SJC" }
                                 , {}
                                 , { "MUC", "SFO" } };

    for (size_t i = 0; i < v.size(); ++i) {
        util::TestOne(i, AirTicketsItinerary(v[i]), ans[i]);
    }
}