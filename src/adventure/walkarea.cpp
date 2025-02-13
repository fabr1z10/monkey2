#include "walkarea.h"
#include "../geometry.h"
#include "../error.h"
#include <iostream>
#include "../util.h"
#include "../model.h"
#include <queue>


using namespace adventure;

void WalkArea::addPoly(const std::vector<float> & data) {
    M_Assert(data.size() >= 6 && data.size() % 2 == 0, "To specify a polygon, provide an even number of coordinates (provide at least 3 points).");
    size_t offset = _polygon.size();
    std::vector<glm::vec2> p;
    for (size_t i = 0; i < data.size(); i += 2) {
        p.emplace_back(data[i], data[i+1]);
    }
    M_Assert(signedArea(p) > 0, "Polygon in walkarea must be specified in ccw order.");
    _polygon.insert(_polygon.end(), p.begin(), p.end());
    auto np = data.size() / 2;
    for (size_t i = 0; i < np; ++i) {
        _nodeWalls.insert({offset+i, offset+(i+1) % np});
        _nodeWalls.insert({offset+(i+1) % np, offset+i});
    }
    // add poly info (for each polygon: start index and number of points)
    _polyInfo.push_back({offset, np});

}


WalkArea::WalkArea(const std::vector<float> & data, int batchId, glm::vec4 color) : Node(), _batchId(batchId), _color(color) {
    addPoly(data);
}

void WalkArea::addHole(const std::vector<float> & data) {

    addPoly(data);
}

void WalkArea::start() {
    Node::start();
    recalculatePoints();

}

void WalkArea::processPolygon(const std::vector<glm::vec2> & data, std::vector<float>& debugModelData) {
    size_t n = data.size();
    // here we need to find vertices -- i.e nodes in the graph
    // these will be concave poly nodes and convex hole points
    for (size_t k = 0; k < _polyInfo.size(); ++k) {
        auto l = _polyInfo[k].second;
        auto s = _polyInfo[k].first;
        for (size_t i = 0; i < l; ++i) {
            auto Pc = _polygon[s+i];
            auto Pp = _polygon[s+(i-1)%l];
            auto Pn = _polygon[s+(i+1)%l];
            auto cross = cross2D(Pc - Pp, Pn - Pc);
            if (k > 0) cross *= -1.f;
            if (cross < 0) {
                _vertexToPolygonNode[_vertices.size()] = s+i;
                _vertices.push_back(Pc);
            }
            auto f = {Pc.x, Pc.y, 0.f, Pn.x, Pn.y, 0.f, _color.r, _color.g, _color.b, _color.a};
            debugModelData.insert(debugModelData.end(), f.begin(), f.end());
        }
    }
}

void WalkArea::recalculatePoints() {


    // go through all points one by one
    auto n = _polygon.size();
    std::vector<float> debugModelData;

    //_nodeWalls.insert({0, n-1});
    //_walls.emplace_back(_polygon.front(), _polygon.back());
    int offset = 0;
    processPolygon(_polygon, debugModelData);
//    offset += _polygon.size();
//    for (const auto& hole : _holes) {
//        processPolygon(hole, debugModelData, true, offset);
//        offset += hole.size();
//    }
//    for (size_t i = 0 ; i < n; ++i) {
//        auto P1 = _polygon[i];
//        auto P0 = _polygon[(i-1+n) % n];
//        auto P2 = _polygon[(i+1) % n];
//        auto cross = cross2D(P1 - P0, P2 - P1);
//        if (cross < 0) {
//            _vertexToPolygonNode[_vertices.size()] = i;
//            _vertices.push_back(P1);
//        }
//        auto f = {P1.x, P1.y, 0.f, P2.x, P2.y, 0.f, _color.r, _color.g, _color.b, _color.a};
//        debugModelData.insert(debugModelData.end(), f.begin(), f.end());
//        _walls.emplace_back(P1, P2);
//        _nodeWalls.insert({i, (i+1) % n});
//    }
    if (_batchId != -1) {
        auto model = std::make_shared<Model<primitives::Line>>(debugModelData);
        setModel(model, _batchId);
    }

    // find visibility edges. For each pair of nodes AB, determine if
    for (size_t i = 0; i < _vertices.size(); ++i) {
        for (size_t j = i+1; j < _vertices.size(); ++j) {
            addVisibilityEdges(i, j);
        }
    }

    std::cerr << " -- vertices:\n";
    for (const auto& v : _vertices) {
        std::cerr << v << "\n";

    }
    std::cerr << " -- edges:\n";
    for (const auto& e : _visibilityEdges) {
        std::cerr << e.first << ": ";
        for (const auto& m : e.second) {
            std::cerr << m << ", ";

        }
        std::cerr << "\n";
    }

}

int WalkArea::getPolygonNodeIndex(int vertexId) const {
    auto it= _vertexToPolygonNode.find(vertexId);
    if (it == _vertexToPolygonNode.end()) {
        return -1;
    }
    return it->second;
}

void WalkArea::addVisibilityEdges(int i, int j) {
    // requesting if vertex_i connects to vertex_j
    auto ni = getPolygonNodeIndex(i);
    auto nj = getPolygonNodeIndex(j);
    if (_nodeWalls.count({ni, nj}) > 0) {
        // in this case nodes are visible
        _visibilityEdges[i].insert(j);
        _visibilityEdges[j].insert(i);
        return;
    }
    bool hitsWall = false;

    glm::vec2 P ;
    for (const auto& w : _nodeWalls) {

        if (w.first == ni || w.second == ni || w.first == nj || w.second == nj) {
            continue;
        }
        if (seg2seg(_vertices[i], _vertices[j], _polygon[w.first], _polygon[w.second], P)) {
            hitsWall = true;
            break;
        }
    }
    if (!hitsWall) {
        // if we get here, it means: P_i and P_j are connected. However we also
        // want to check mid point; for instance, if we have a square hole, we don't
        // want to insert diagonal!
        glm::vec2 midPoint = 0.5f * (_vertices[i] + _vertices[j]);
        if (isPointInWalkArea(midPoint)) {
            _visibilityEdges[i].insert(j);
            _visibilityEdges[j].insert(i);
        }
    }
}

void WalkArea::pushNode(glm::vec2 node) {
    _vertices.push_back(node);
    // loop through all vertices except of course last one which is itself
    glm::vec2 P;
    auto j = _vertices.size()-1;
    // check if node matches with an existing point in polygon

    for (size_t i = 0; i < _polygon.size(); i++) {
        if (isApproxEqual(node, _polygon[i])) {
            auto index = static_cast<int>(i);  // Store the index of the matching vertex
            _vertexToPolygonNode[j] = index;
        }
    }
    for (size_t i = 0; i < j; ++i)  {
        // checks visibility between node (j) and node i.
        // I also pass index so to avoid checking walls starting or ending at index
        addVisibilityEdges(j, i);
    }

}

void WalkArea::popNode() {
    auto n = _vertices.size()-1;
    _vertexToPolygonNode.erase(n);
    for (const auto& e : _visibilityEdges[n]) {
        _visibilityEdges[e].erase(n);
    }
    _visibilityEdges.erase(n);
    _vertices.pop_back();
}

bool WalkArea::isPointInWalkArea(glm::vec2 P) {
    int i {0};
    for (const auto& p : _polyInfo) {
        auto inside = pnpoly(_polygon, P, p.first, p.second);
        if ((i == 0 && !inside) || (i > 0 && inside)) {
            return false;
        }
        i++;
    }
    return true;
}

std::vector<glm::vec2> WalkArea::dijkstraShortestPath(glm::vec2 start, glm::vec2 goal) {

    auto isStartInWalkArea = isPointInWalkArea(start);
    auto isEndInWalkArea = isPointInWalkArea(goal);
    if (isStartInWalkArea && isEndInWalkArea) {

        // add start and goal to the graph
        pushNode(start);
        pushNode(goal);
        auto n = _vertices.size();
        std::vector<float> dist(n, std::numeric_limits<float>::infinity());
        std::vector<int> prev(n, -1);
        std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<>> pq;
        int startIndex = n-2;
        int endIndex = n-1;
        dist[startIndex] = 0;
        pq.push({0, startIndex});
        while (!pq.empty()) {
            auto [cost, u] = pq.top();
            pq.pop();

            if (cost > dist[u]) continue; // Skip outdated entries

            for (auto v : _visibilityEdges[u]) {
                float newCost = dist[u] + 1;

                if (newCost < dist[v]) {
                    dist[v] = newCost;
                    prev[v] = u;
                    pq.push({newCost, v});
                }
            }
        }

        // Reconstruct path from `goal` to `start`
        std::vector<glm::vec2> path;
        for (int at = endIndex; at != -1; at = prev[at]) {
            path.push_back(_vertices[at]);
        }
        std::reverse(path.begin(), path.end());
        popNode();
        popNode();
        return path;

    }
    return {};


}
