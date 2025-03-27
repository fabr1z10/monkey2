#pragma once

#include "node.h"
#include <list>
#include "script.h"


class Scheduler : public Node {
public:
    Scheduler() = default;

    void customUpdate(double) override;

    void play(std::shared_ptr<Script>);



private:
    std::list<std::shared_ptr<Script>> _scripts;

    std::vector<std::shared_ptr<Script>> _scriptBuffer;
    std::vector<std::list<std::shared_ptr<Script>>::iterator> _terminationBuffer;

    std::unordered_map<std::string, std::list<std::shared_ptr<Script>>::iterator > _scriptMap;
};