#pragma once


#include "../component.h"
#include "../shape.h"
#include "../vec.h"

class HotSpot : public Component {
public:
    HotSpot(std::shared_ptr<Shape> shape, int priority, int cam, int batchId=-1);

	void setShape(std::shared_ptr<Shape>);

	virtual ~HotSpot();

    void start() override;

	bool isInside(glm::vec2);

    virtual void onEnter() = 0;

    virtual void onLeave() = 0;

    virtual void onClick(Vec2) = 0;

    int getPriority() const;

	int getCamera() const;

	void dispose() override;


private:
	void redraw();
    int _priority;
	int _cam;
    std::shared_ptr<Shape> _shape;
	int _batchId;
	Node* _debugDrawNode;
};

inline int HotSpot::getCamera() const {
	return _cam;
}

inline int HotSpot::getPriority() const {
    return _priority;
}
// Step 2: Define a trampoline class inheriting from Base
class PyHotSpot : public HotSpot {
public:
	using HotSpot::HotSpot; // Inherit constructors

	void onEnter() override {
		PYBIND11_OVERRIDE_PURE(
				void,       // Return type
				HotSpot,    // Parent class
				onEnter     // Function name
		);
	}

	void onLeave() override {
		PYBIND11_OVERRIDE_PURE(
				void,    // Return type
				HotSpot,    // Parent class
				onLeave // Function name
		);
	}

    void onClick(Vec2 pos) override {
		PYBIND11_OVERRIDE_PURE(
				void,    // Return type
				HotSpot,    // Parent class
				onClick, // Function name,
				pos
		);
	}

};
