#include "scheduler.h"

void Scheduler::update(double dt) {
    // terminate scripts
    if (!_terminationBuffer.empty()) {
        for (auto it: _terminationBuffer) {
            _scripts.erase(it);
        }
        _terminationBuffer.clear();
    }

    // run all scripts
    for (auto it = _scripts.begin(); it != _scripts.end();) {
        (*it)->update(dt);
        if ((*it)->done()) {
            //m_scriptMap.erase((*it)->getId());
            _scriptMap.erase((*it)->id());
            it = _scripts.erase(it);

        } else {
            it++;
        }
    }

    for (auto& s : _scriptBuffer) {
        s->start();
        _scripts.push_back(s);
        _scriptMap[s->id()] = std::prev(_scripts.end());
    }
    _scriptBuffer.clear();
}

void Scheduler::add(std::shared_ptr<Script> s) {
    _scriptBuffer.push_back(s);

    // if a script has the same name of the new script, kill it
    auto it = _scriptMap.find(s->id());
    if (it != _scriptMap.end()) {
        _terminationBuffer.push_back(it->second);
    }
}