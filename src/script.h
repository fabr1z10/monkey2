#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <list>
#include <string>


class Action {
public:
    Action() : _completed(false) {}

    virtual ~Action() = default;

    virtual void start();

    virtual void run(double) = 0;

    bool isCompleted() const;

protected:
    bool _completed;
};

inline bool Action::isCompleted() const {
    return _completed;
}

class Script {
public:
    Script(const std::string& id = std::string());

    void setLoop();

    bool done() const;

    size_t addAction(const std::shared_ptr<Action>&, int = -2);

    void addEdge(size_t, size_t);

    void start();

    void kill();

    std::string id() const;

    void update(double);
private:
    bool _loop;
    std::string _id;
    static long _scriptId;
    std::list<int> _current;
    bool _done;
    // nodes
    std::vector<std::shared_ptr<Action>> _actions;
    // edges
    std::vector<std::vector<int>> _next;
    std::vector<std::vector<int>> _previous;
	int _lastAdded;
};

inline std::string Script::id() const {
    return _id;
}
inline bool Script::done() const {
    return _done;
}
