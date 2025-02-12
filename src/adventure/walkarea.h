#pragma once

#include <vector>
#include "../node.h"
#include "glm/glm.hpp"
#include "../hashpair.h"

namespace adventure {
    class WalkArea : public Node {
    public:
        WalkArea(const std::vector<float> &, int batchId = -1, glm::vec4 color = glm::vec4(1.f));

        void addHole(const std::vector<float>&);

        void recalculatePoints();

        void start() override;

        std::vector<glm::vec2> dijkstraShortestPath(glm::vec2 start, glm::vec2 goal);

        bool isPointInWalkArea(glm::vec2);
    private:
        void addPoly(const std::vector<float>&);
        void processPolygon(const std::vector<glm::vec2>&, std::vector<float>& debugModel);

        void pushNode(glm::vec2);

        void popNode();

        void addVisibilityEdges(int i, int j);

        int getPolygonNodeIndex(int) const;
        // geometry of walkarea
        std::vector<glm::vec2> _polygon;

        //std::vector<std::vector<glm::vec2>> _holes;

        //std::vector<std::pair<glm::vec2, glm::vec2>> _walls;

        std::unordered_set<std::pair<int, int>> _nodeWalls;

        // graph stuff
        std::vector<glm::vec2> _vertices;

        std::unordered_map<int, int> _vertexToPolygonNode;

        std::unordered_map<size_t, std::unordered_set<size_t>> _visibilityEdges;
        int _batchId;
        glm::vec4 _color;
        std::vector<std::pair<int, int>> _polyInfo;
    };
}