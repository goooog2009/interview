/* [Google]

=== TASK 1 ===
http://www.careercup.com/question?id=5769432498438144

Given the interface below, implement a task scheduler.

interface Task {
    void Run();
    Set<Task> GetDependencies();
}
Additionally, the task scheduler should follow two rules. 

1. Each task may only be executed once. 
2. The dependencies of a task should be executed before the task itself.


=== TASK 2 ===
http://www.careercup.com/question?id=5753768215838720

Given the requirements of http://www.careercup.com/question?id=5769432498438144
implement the same task scheduler with parallel task execution.
*/

#pragma once

#include <string>
#include <vector>
#include <set>
#include <stack>
#include <thread>

namespace TaskScheduler {

using namespace std;

class ITask {
public:
    virtual void Run() = 0;
    virtual string GetName() const = 0;
    virtual const vector<ITask *> & GetDependencies() const = 0;
};

class Task : public ITask {
    string Name;
    vector<ITask *> Dependencies;
public:
    Task(const string & name, const vector<ITask *> & deps)
        : Name(name)
        , Dependencies(deps)
    {}

    string GetName() const {
        return Name;
    }

    virtual void Run() {}

    virtual const vector<ITask *> & GetDependencies() const {
        return Dependencies;
    }
};

void dfs(ITask * task, set<ITask *> & done, stack<ITask *> & exec) {
    if (done.find(task) != done.end())
        return;

    done.insert(task);

    exec.push(task);
    for (auto t : task->GetDependencies()) {
        dfs(t, done, exec);
    }
}

string RunTasks(vector<ITask *> tasks) {
    string result;
    set<ITask *> done;

    for (auto t : tasks) {
        if (!t)
            continue;

        stack<ITask *> exec;
        dfs(t, done, exec);

        while (!exec.empty()) {
            ITask * t = exec.top();
            t->Run();
            result += t->GetName();
            exec.pop();
        }
    }

    return result;
}

void RunThread(stack<ITask *> exec) {
    while (!exec.empty()) {
        ITask * t = exec.top();
        t->Run();
        cerr << t->GetName();
        exec.pop();
    }
}

void RunTasksMulti(vector<ITask *> tasks) {
    set<ITask *> done;

    vector<thread> threads;
    cerr << "Multithreaded: ";
    for (auto t : tasks) {
        if (!t)
            continue;

        stack<ITask *> exec;
        dfs(t, done, exec);

        threads.push_back(thread(RunThread, exec));
    }

    for (auto & t : threads)
        t.join();
    cerr << endl;
}


void CheckResult(const string & s1, const string & s2) {
    if (s1 != s2)
        cout << "ERROR! Result: " << s1 << ", Correct: " << s2 << endl;
    else
        cout << "OK: " << s1 << endl;
}

void Run() {
    cout << "TaskScheduler" << endl;

    Task a("a", {});
    CheckResult(RunTasks({&a}), "a");

    Task b("b", {&a});
    CheckResult(RunTasks({&b, &a}), "ab");

    Task c("c", {&b, &a});
    CheckResult(RunTasks({&c, &b, &a}), "abc");

    Task d("d", {});
    CheckResult(RunTasks({&c, &b, &d, &a}), "abcd");

    Task e("e", {&a, &d, &b});
    CheckResult(RunTasks({&c, &e, &b, &d, &a}), "abcde");

    Task f("f", {});
    Task g("g", {&f, &b});
    CheckResult(RunTasks({&c, &e, &b, &d, &a, &g}), "abcdefg");

    RunTasksMulti({&c, &e, &b, &d, &a, &g});
}


} // TaskScheduer