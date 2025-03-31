#pragma once

#include <vector>
#include "../node.h"
#include "../vec.h"
#include "../hashpair.h"

namespace adventure {

    enum class PolyType {
        AREA, LINE
    };

    class WalkArea : public Node {
    public:
        WalkArea(const std::vector<float> &, int batchId = -1, Color color = Color(1.f, 1.f, 1.f, 1.f));

        void addHole(const std::vector<float>&, Node* ref = nullptr);

        void addLine(const std::vector<float>&, Node* ref = nullptr);

        void recalculatePoints();

        void start() override;

        std::vector<glm::vec2> dijkstraShortestPath(glm::vec2 start, glm::vec2 goal);

        bool isPointInWalkArea(glm::vec2);

        glm::vec2 getClosestPoint(glm::vec2);
    private:
        struct PolyInfo {
            PolyInfo(size_t offset, size_t length, PolyType type, Node* ref)
                : offset(offset), length(length), type(type), active(true), ref(ref) {}
            size_t offset;
            size_t length;
            PolyType type;
            // a polygon/line can be associated to a node
            Node* ref;
            bool active;
        };
        void addEdge(int i, int j);
        void addPoly(const std::vector<float>&, PolyType, Node*);
        void processPolygon(const std::vector<glm::vec2>&, std::vector<float>& debugModel);

        void pushNode(glm::vec2);

        void popNode();

        void addVisibilityEdges(int i, int j);

        int getPolygonNodeIndex(int) const;
        // geometry of walkarea
        std::vector<glm::vec2> _localPolygon;

        std::vector<glm::vec2> _polygon;

        //std::vector<std::vector<glm::vec2>> _holes;

        //std::vector<std::pair<glm::vec2, glm::vec2>> _walls;

        std::unordered_set<std::pair<int, int>> _nodeWalls;

        // graph stuff
        std::vector<glm::vec2> _vertices;

        std::unordered_map<int, int> _vertexToPolygonNode;
		std::unordered_map<int, int> _polygonToVertex;

        std::unordered_map<size_t, std::unordered_map<size_t, float>> _visibilityEdges;
        int _batchId;
        Color _color;
        std::vector<PolyInfo> _polyInfo;
    };
}
