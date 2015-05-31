/* [Amazon] http://www.careercup.com/question?id=5199356015673344

Given set of job schedules with start and end time, write a function that returns indexes of overlapping sets. 

for ex.:
input -> [1,2][5,6][1,5][7,8][1,6] 
return -> [0,1,2,4]
*/

#pragma once

#include <algorithm>
#include <vector>
#include <set>

namespace {

    struct Event {
        size_t JobId;
        size_t Time;
        bool Start; // Finish if false

        bool operator<(const Event& rhs) const {
            return Time < rhs.Time;
        }
    };

    vector<size_t> FindOverlappingJobs(const vector<pair<size_t, size_t>>& jobs) {
        vector<Event> events;
        for (size_t i = 0; i < jobs.size(); ++i) {
            events.emplace_back(Event{i, jobs[i].first, true});
            events.emplace_back(Event{i, jobs[i].second, false});
        }
        sort(events.begin(), events.end());

        set<size_t> runningJobs;
        set<size_t> overlappingJobs;
        for (const auto& i : events) {
            if (!i.Start) {
                runningJobs.erase(i.JobId);
                continue;
            }

            if (runningJobs.empty()) {
                runningJobs.insert(i.JobId);
                continue;
            }

            if (runningJobs.size() == 1)
                overlappingJobs.insert(*runningJobs.begin());

            runningJobs.insert(i.JobId);
            overlappingJobs.insert(i.JobId);
        }

        vector<size_t> res;
        for (auto i : overlappingJobs) {
            res.push_back(i);
        }
        return res;
    }
}

void TestFindOverlappingJobs() {
    vector<vector<pair<size_t, size_t>>> v = { { {1, 2}, {5, 6}, {1, 5}, {7, 8}, {1, 6} }
                                             , {} };
    vector<vector<size_t>> ans = { {0, 1, 2, 4}
                                 , {} };

    util::TestOne("case 0", FindOverlappingJobs(v[0]), ans[0]);
    util::TestOne("case 1", FindOverlappingJobs(v[1]), ans[1]);
}