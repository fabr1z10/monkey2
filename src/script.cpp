#include "script.h"
#include <iostream>
#include <sstream>

long Script::_scriptId = 0;

void Action::start() {
    _completed=false;
}

Script::Script(const std::string& id) : _done(false), _id(id), _loop(false), _lastAdded(-1) {
    if (_id.empty()) {
        std::stringstream s;
        s << "_AUTO" << _scriptId++;
        _id = s.str();

    }

}

void Script::setLoop()
{
    _loop =true;
}

void Script::start() {
    // initialize current
    for (size_t i = 0; i < _previous.size(); i++) {
        if (_previous[i].empty()) {
            _current.push_back(i);
            // start i-th action
            _actions[i]->start();
        }
    }
}

void Script::kill() {

}

void Script::addEdge(size_t i , size_t j) {
    _next[i].push_back(j);
    _previous[j].push_back(i);
}

size_t Script::addAction(const std::shared_ptr<Action>& a, int after) {
	if (after == -2) {
		after = _lastAdded;
	}
    int id = _actions.size();
    _actions.push_back(a);
    _next.emplace_back();
    if (after == -1) {
        _previous.emplace_back();
    } else {
        _previous.push_back({after});
        _next[after].push_back({id});
    }
	_lastAdded = id;
    return id;


}


void Script::update(double dt) {
    // is there anything to process? do it otherwise do nothing
    //std::cout << " -- processing script " << _scriptId << "\n";

    std::vector<int> complete;
    for (auto it = _current.begin(); it != _current.end();) {
        // execute
        int currentId = *it;
        auto& action = _actions[currentId];

        if (action->isCompleted()) {
            it = _current.erase(it);
            // test for next
            for (auto n : _next[currentId]) {
                bool add{true};
                for (auto p : _previous[n]) {
                    if (!_actions[p]->isCompleted()) {
                        add = false;
                        break;
                    }
                }
                if (add) {
                    _actions[n]->start();
                    _current.push_back(n);
                }
            }
        } else {
            action->run(dt);
            it++;
        }
    }

    if (_current.empty()) {
        if (_loop) {
            start();
        } else {
            _done = true;
        }
    }

}
