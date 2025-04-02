#include "walkarea.h"
#include "../geometry.h"
#include "../error.h"


#include "../model.h"
#include <queue>


using namespace adventure;


// this is for area
void WalkArea::addPoly(const std::vector<float> & data, PolyType type, Node* ref) {
    M_Assert(data.size() >= (type == PolyType::AREA ? 6 : 4) && data.size() % 2 == 0, "To specify a polygon, provide an even number of coordinates (provide at least 3 points).");
    // localPolygon holds *all* vertices of the walkarea (main and holes)
    size_t offset = _localPolygon.size();
    std::vector<glm::vec2> p;
    for (size_t i = 0; i < data.size(); i += 2) {
        p.emplace_back(data[i], data[i+1]);
    }
    if (type == PolyType::AREA) {
        M_Assert(signedArea(p) > 0, "Polygon in walkarea must be specified in ccw order.");
    }
    _localPolygon.insert(_localPolygon.end(), p.begin(), p.end());
    auto np = data.size() / 2;

    // add poly info (for each polygon: start index and number of points)
    _polyInfo.push_back(PolyInfo(offset, np, type, ref));

}


WalkArea::WalkArea(const std::vector<float> & data, int batchId, Color color) : Node(), _batchId(batchId), _color(color) {
    addPoly(data, PolyType::AREA, nullptr);
}

void WalkArea::addLine(const std::vector<float>& data, Node* ref) {
    addPoly(data, PolyType::LINE, ref);

}

void WalkArea::addHole(const std::vector<float> & data, Node* ref) {

    addPoly(data, PolyType::AREA, ref);
}

void WalkArea::start() {
    _polygon.resize(_localPolygon.size());
    recalculatePoints();
	Node::start();

}

void WalkArea::processPolygon(const std::vector<glm::vec2> & data, std::vector<float>& debugModelData) {

}

void WalkArea::recalculatePoints() {


    // go through all points one by one
    auto n = _polygon.size();
    std::vector<float> debugModelData;

    //_nodeWalls.insert({0, n-1});
    //_walls.emplace_back(_polygon.front(), _polygon.back());

    //processPolygon(_polygon, debugModelData);

    // here we need to find vertices -- i.e nodes in the graph
    // these will be concave poly nodes and convex hole points
    _nodeWalls.clear();
    _vertices.clear();
    int k = 0;
    for (const auto& p : _polyInfo) {
        if (p.active) {
            int s = p.offset;
            int l = p.length;
            glm::vec2 offset(0.f);
            if (p.ref != nullptr) {
                offset = p.ref->getWorldPosition().toGlm();
            }
            // trasnform poly points in world coords
            for (size_t i = 0; i < l; ++i) {
                _polygon[s+i] = offset + _localPolygon[s+i];
            }
            if (p.type == PolyType::AREA) {
                for (int i = 0; i < l; ++i) {
                    auto iCurr = s + i;
                    auto iPrev = s + (i-1+l) % l;
                    auto iNext = s + (i+1) % l;
                    auto Pc = _polygon[iCurr];
                    auto Pp = _polygon[iPrev];
                    auto Pn = _polygon[iNext];
                    auto cross = cross2D(Pc - Pp, Pn - Pc);
                    float hadj = k > 0 ? -1.f : 1.f;
                    _vertexInUnitVec[iCurr] = hadj * glm::normalize(Pp + Pn - 2.0f * Pc);
                    glm::vec2 normal(-(Pn.y - Pc.y), Pn.x - Pc.x);
                    _wallInUnitVec[{iCurr, iNext}] = hadj * glm::normalize(normal);
                    cross *= hadj;
                    if (cross < 0) {
                        _vertexToPolygonNode[_vertices.size()] = s+i;
						_polygonToVertex[s+i] = _vertices.size();
                        _vertices.push_back(Pc);
                    }
                    auto f = {Pc.x, Pc.y, 0.f, Pn.x, Pn.y, 0.f};
                    debugModelData.insert(debugModelData.end(), f.begin(), f.end());
                    _nodeWalls.insert({iCurr, iNext});
                    
                    //_nodeWalls.insert({iNext, iCurr});
                }
            } else {
                _vertexToPolygonNode[_vertices.size()] = s;
				_polygonToVertex[s] = _vertices.size();
                _vertices.push_back(_polygon[s]);
                _vertexToPolygonNode[_vertices.size()] = s + l - 1;
				_polygonToVertex[s+l-1] = _vertices.size();
                _vertices.push_back(_polygon[s + l - 1]);
                for (size_t i = 0; i < l-1; ++i) {
                    auto Pc = _polygon[s+i];
                    auto Pn = _polygon[s+i+1];
                    auto f = {Pc.x, Pc.y, 0.f, Pn.x, Pn.y, 0.f};
                    debugModelData.insert(debugModelData.end(), f.begin(), f.end());
                    _nodeWalls.insert({s+i, s+i+1});
                    _nodeWalls.insert({s+i+1, s+i});

                }
            }
        }
        k++;
    }

    if (_batchId != -1) {
        auto model = std::make_shared<Model<primitives::Line>>(debugModelData);
        setModel(model, _batchId);
    //    this->setMultiplyColor(_color);
    }

    // find visibility edges. For each pair of nodes AB, determine if
    for (size_t i = 0; i < _vertices.size(); ++i) {
        for (size_t j = i+1; j < _vertices.size(); ++j) {
            addVisibilityEdges(i, j);
        }
    }

    /*std::cerr << " -- vertices:\n";
    for (const auto& v : _vertices) {
        std::cerr << v << "\n";

    }
    std::cerr << " -- edges:\n";
    for (const auto& e : _visibilityEdges) {
        //std::cerr << e.first << ": ";
        for (const auto& m : e.second) {
            //std::cerr << m.first << "(" << m.second << ")" << ", ";

        }
        std::cerr << "\n";
    }
    */
}

int WalkArea::getPolygonNodeIndex(int vertexId) const {
    auto it= _vertexToPolygonNode.find(vertexId);
    if (it == _vertexToPolygonNode.end()) {
        return -1;
    }
    return it->second;
}


void WalkArea::addEdge(int i, int j) {
    auto length = glm::length(_vertices[i] - _vertices[j]);
    _visibilityEdges[i][j] = length;
    _visibilityEdges[j][i] = length;

}

void WalkArea::addVisibilityEdges(int i, int j) {
    // requesting if vertex_i connects to vertex_j
    auto ni = getPolygonNodeIndex(i);
    auto nj = getPolygonNodeIndex(j);
    if (_nodeWalls.count({ni, nj}) > 0 || _nodeWalls.count({nj, ni}) > 0) {
        // in this case nodes are visible
        addEdge(i, j);
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
            addEdge(i, j);
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

	// check if ndoe belongs to a wall
	int iu = -1;
	int iv = -1;
	for (const auto& [u,v] : _nodeWalls) {
		if (pointInSegment(_polygon[u], _polygon[v], node)) {
            //std::cout << " NODE " << node << " IN SEG [" << _polygon[u] << ", " << _polygon[v] << "]\n";
			auto it = _polygonToVertex.find(u);
			if (it != _polygonToVertex.end()) iu = it->second;

			it = _polygonToVertex.find(v);
			if (it != _polygonToVertex.end()) iv = it->second;

			break;
			// if P is in uv, then u and v are visible
		}
	}
    for (size_t i = 0; i < j; ++i)  {
        // checks visibility between node (j) and node i.
        // I also pass index so to avoid checking walls starting or ending at index
		if (i == iu || i == iv) {
			addEdge(i, j);
		} else {
			addVisibilityEdges(j, i);
		}
    }

}

void WalkArea::popNode() {
    auto n = _vertices.size()-1;
    _vertexToPolygonNode.erase(n);
    for (auto& e : _visibilityEdges) {
        // remove edge a->n for a in all vertices
        e.second.erase(n);
        //_visibilityEdges[e].erase(n);
    }
    _visibilityEdges.erase(n);
    _vertices.pop_back();
}

bool WalkArea::isPointInWalkArea(glm::vec2 P) {
    int i {0};
    for (const auto& p : _polyInfo) {
        if (p.type == PolyType::AREA) {
            auto inside = pnpoly(_polygon, P, p.offset, p.length);
            if ((i == 0 && !inside) || (i > 0 && inside)) {
                // automatically compute closest point
                return false;
            }
        }
        i++;
    }
    return true;
}

glm::vec2 WalkArea::getClosestPoint(glm::vec2 P)
{
    int i {0};
    for (const auto& p : _polyInfo) {        
        if (p.type == PolyType::AREA) {
            auto inside = pnpoly(_polygon, P, p.offset, p.length);
            if ((i == 0 && !inside) || (i > 0 && inside)) {
                // loop through all edges
                glm::vec2 best;
                float dbest = std::numeric_limits<float>::infinity();
                for (int i = 0; i < p.length; ++i) {
                    int j = (i + 1) % p.length;
                    int i0 = p.offset + i;
                    int i1 = p.offset + j;
                    glm::vec2 A = _polygon[i0];
                    glm::vec2 B = _polygon[i1];
                    float t = glm::dot(P - A, B - A) / glm::dot(B-A, B-A);
                    glm::vec2 Q = (t < 0) ? A : (t > 1 ? B : A + t*(B-A));
                    float d2 = glm::dot(P-Q, P-Q);
                    if (d2 < dbest) {
                        dbest = d2;
                        glm::vec2 inUnitVec;
                        if (t < 0) {
                            inUnitVec = _vertexInUnitVec[i0];
						}
						else if (t > 1) {
							inUnitVec = _vertexInUnitVec[i1];
						}
						else {
                            inUnitVec = _wallInUnitVec[{i0, i1}];
						}
                        best = Q + 0.1f * inUnitVec;
                    }
                }
                return best;
            }
        }
        i++;
    }
    return P;




}

std::vector<glm::vec2> WalkArea::dijkstraShortestPath(glm::vec2 start, glm::vec2 goal) {


    start = getClosestPoint(start);
    goal = getClosestPoint(goal);

	if (glm::length(goal - start) < 0.01) {
		return {};

	}
    // auto isStartInWalkArea = isPointInWalkArea(start);
    // auto isEndInWalkArea = isPointInWalkArea(goal);
    // if (!isStartInWalkArea) {
    //     // move to closest point

    // }

    //if (isStartInWalkArea && isEndInWalkArea) {

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
                float newCost = dist[u] + v.second;

                if (newCost < dist[v.first]) {
                    dist[v.first] = newCost;
                    prev[v.first] = u;
                    pq.push({newCost, v.first});
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

    //}
    return {};


}
