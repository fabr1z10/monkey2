
#include "pyhelper.h"
#include <iostream>
#include "functions.h"
#include "room.h"
#include "text.h"
#include "camera.h"
#include "primitives/prim.h"
#include "linemodel.h"
#include "model.h"
#include "components/keyboard.h"
#include "components/camcontrol3d.h"
#include "directionallight.h"
#include "quadbatch.h"
#include "script.h"
#include "scheduler.h"
#include "sprite.h"
#include "tilemap.h"

#include "skeletal/mesh.h"
#include "skeletal/skeletalmodel.h"

#include "adventure/walkarea.h"
#include "adventure/mousecontroller.h"
#include "adventure/obstacle.h"
#include "components/follow.h"

#include "actions/walk.h"
#include "actions/animate.h"
#include "actions/delay.h"
#include "actions/callfunc.h"
#include "actions/waitclick.h"
#include "components/depthscale.h"
#include "components/hotspot.h"

#include "shapes/line.h"
#include "shapes/polygon.h"
#include "shapes/rect.h"
#include "shapes/point.h"

using namespace adventure;
using namespace shapes;

namespace py = pybind11;

void test() {
	std::cout << "viva la figa\n";
}

PYBIND11_MODULE(monkey2, m) {

    py::module_ mAdv = m.def_submodule("adventure");
    py::module_ mAct = m.def_submodule("actions");
    py::module_ mSha = m.def_submodule("shapes");

	py::enum_<HAlign>(m, "Alignment")
		.value("LEFT", HAlign::LEFT)
		.value("RIGHT", HAlign::RIGHT)
		.value("CENTER", HAlign::CENTER)
		.export_values();

    py::enum_<ModelType>(m, "ModelType")
        .value("WIRE", ModelType::WIREFRAME)
        .value("SOLID", ModelType::SOLID)
        .export_values();


	m.def("test", &test);

    // gets node with given id
    m.def("getNode", &getNode, py::arg("id"), py::return_value_policy::reference);

    m.def("fromHex", &fromHex, py::arg("color"));

    m.def("loadAsset", &loadAsset, py::arg("id"), py::arg("filename"), py::arg("cam"), py::arg("tex"));

    m.def("getModel", &getModel, py::arg("id"), py::return_value_policy::reference);

	m.def("game", &game, py::return_value_policy::reference, "Gets the engine");

	m.def("closeRoom", &closeRoom);

	mSha.def("fromImage", &shapeFromImage);



    py::class_<Shape, std::shared_ptr<Shape>>(m, "Shape")
		.def("toModel", &Shape::makeModel);

    py::class_<Line, Shape, std::shared_ptr<Line>>(mSha, "Line")
        .def(py::init<glm::vec2, glm::vec2>(), py::arg("a"), py::arg("b"));

	py::class_<Rect, Shape, std::shared_ptr<Rect>>(mSha, "Rect")
		.def(py::init<float, float, glm::vec2>(), py::arg("width"), py::arg("height"), py::arg("anchor")=glm::vec2(0.f));

    py::class_<PolyLine, Shape, std::shared_ptr<PolyLine>>(mSha, "PolyLine")
        .def(py::init<const std::vector<float>&>(), py::arg("points"));

    py::class_<Polygon, Shape, std::shared_ptr<Polygon>>(mSha, "Polygon")
        .def(py::init<const std::vector<float>&>(), py::arg("points"));

	py::class_<Point, Shape, std::shared_ptr<Point>>(mSha, "Point")
		.def(py::init<>());


    py::class_<Game>(m, "Game")
		.def("start", &Game::start)
		.def("run", &Game::run)
        .def("makeCurrent", &Game::makeCurrent);


    py::class_<Room, std::shared_ptr<Room>>(m, "Room")
		.def(py::init<>())
		.def("addCamera", &Room::addCamera)
		.def("root", &Room::getRoot, py::return_value_policy::reference)
		.def("setClearColor", &Room::setClearColor)
        .def ("setStartUpFunction", &Room::setStartUpFunction)
		.def("addBatch", &Room::addBatch)
        .def_property("collisionEngine", &Room::getCollisionEngine, &Room::setCollisionEngine)
        .def_property("hotSpotManager", &Room::getHotSpotManager, &Room::setHotSpotManager);


    py::class_<Camera, std::shared_ptr<Camera>>(m, "camera")
		.def("setPosition", &Camera::setPosition)
		.def("move", &Camera::move);

	py::class_<Node, std::shared_ptr<Node>>(m, "Node")
		.def(py::init<>())
        .def_property_readonly("id", &Node::id)
        .def_property_readonly("x", &Node::x)
        .def_property_readonly("y", &Node::y)
		.def_property("userData", &Node::getUserData, &Node::setUserData, py::return_value_policy::reference)
		.def_property("active", &Node::active, &Node::setActive)
		.def_property("show", &Node::show, &Node::setShow)
        .def_property("animation", &Node::getAnimation, &Node::setAnimation)
		.def("setModel", &Node::setModel, py::arg("model"), py::arg("batch") = -1)
		.def("setTransform", &Node::setTransform)
		.def("add", &Node::add)
		.def("remove", &Node::remove)
		.def("addComponent", &Node::addComponent)
        .def("getPosition", &Node::getWorldPosition)
        .def("setPosition", &Node::setPosition)
        .def("setMultiplyColor", &Node::setMultiplyColor)        
        .def("flipX", &Node::flipHorizontal);

	py::class_<Component, std::shared_ptr<Component>>(m, "C")
        .def_property_readonly("node", &Component::getNode, py::return_value_policy::reference)
		.def("keepAlive", &HotSpot::setPySelf);


	py::class_<Keyboard, Component, std::shared_ptr<Keyboard>>(m, "Keyboard")
	    .def(py::init<>())
	    .def("add", &Keyboard::addFunction);

    py::class_<Follow, Component, std::shared_ptr<Follow>>(m, "Follow")
        .def(py::init<int>(), py::arg("camId"));

    py::class_<DepthScale, Component, std::shared_ptr<DepthScale>>(m, "DepthScale")
        .def(py::init<float, float, int>(), py::arg("y0"), py::arg("y1"), py::arg("mask"));

    py::class_<Collider, Component, std::shared_ptr<Collider>>(m, "Collider")
        .def(py::init<std::shared_ptr<Shape>, int, int, const std::string&>(),
                py::arg("shape"), py::arg("flag"), py::arg("mask"), py::arg("tag"));

    py::class_<HotSpot, PyHotSpot, Component, std::shared_ptr<HotSpot>>(m, "HotSpot")
        .def(py::init<std::shared_ptr<Shape>, int, int>());
//		.def(py::init([](std::shared_ptr<Shape> shape, int priority, int camera) {
//			auto instance = std::make_shared<PyHotSpot>(shape, priority, camera);
//			py::object self = py::cast(instance);
//			instance->setPySelf(self);
//			return instance;
//		}));

    py::class_<MouseListener, std::shared_ptr<MouseListener>>(m, "_MouseListener");

	py::class_<CamControl3D, Component, std::shared_ptr<CamControl3D>>(m, "CamControl3D")
        .def(py::init<int, float, float>());

	py::class_<OrthoCamera, Camera, std::shared_ptr<OrthoCamera>>(m, "CamOrtho")
		.def(py::init<float, float, glm::vec4>(), py::arg("width"), py::arg("height"), py::arg("viewport") = glm::vec4(0.f));

	py::class_<PerspectiveCamera, Camera, std::shared_ptr<PerspectiveCamera>>(m, "CamPerspective")
		.def(py::init<glm::vec4, float, float, float>(), py::arg("viewport") = glm::vec4(0.f), py::arg("fov")=45.f, py::arg("near")=0.1f, py::arg("far")=100.f);

	// batches
	// base class - not instantiable
	py::class_<IBatch, std::shared_ptr<IBatch>>(m, "Y")
	    .def("addLight", &IBatch::addLight);

	py::class_<Batch<primitives::Line>, IBatch, std::shared_ptr<Batch<primitives::Line>> >(m, "LineBatch")
		.def(py::init<int, int>(), py::arg("size"), py::arg("cam"));

    py::class_<Batch<primitives::Triangle>, IBatch, std::shared_ptr<Batch<primitives::Triangle>> >(m, "TriangleBatch")
        .def(py::init<int, int>(), py::arg("size"), py::arg("cam"));

    py::class_<Batch<primitives::TriangleNormal>, IBatch, std::shared_ptr<Batch<primitives::TriangleNormal>> >(m, "TriangleNormalBatch")
        .def(py::init<int, int>(), py::arg("size"), py::arg("cam"));

    py::class_<QuadBatch, IBatch, std::shared_ptr<QuadBatch>>(m, "QuadBatch")
        .def(py::init<int, int, int>(), py::arg("size"), py::arg("cam"), py::arg("maxTextures"))
        .def("addTexture", &QuadBatch::addTexture);

    // base mode l- not instantiable
	py::class_<IModel, std::shared_ptr<IModel>>(m, "Y2");

	py::class_<Model<primitives::Line>, IModel, std::shared_ptr<Model<primitives::Line>>>(m, "LineModel")
		.def(py::init<const std::vector<float>&>());

    py::class_<Model<primitives::Triangle>, IModel, std::shared_ptr<Model<primitives::Triangle>>>(m, "TriangleModel")
        .def(py::init<const std::vector<float>&>());

    py::class_<Model<primitives::TriangleNormal>, IModel, std::shared_ptr<Model<primitives::TriangleNormal>>>(m, "TriangleNormalModel")
        .def(py::init<const std::vector<float>&>());

    py::class_<Model<primitives::Quad>, IModel, std::shared_ptr<Model<primitives::Quad>>>(m, "QuadModel");
        //.def(py::init<const std::vector<float>&>());

    py::class_<Quad, Model<primitives::Quad>, std::shared_ptr<Quad>>(m, "Quad")
        .def(py::init<const std::vector<float>&, int, int>());

    py::class_<Sprite, Model<primitives::Quad>, std::shared_ptr<Sprite>>(m, "Sprite")
        .def(py::init<const std::vector<float>&, int>())
        .def(py::init<const std::vector<float>&, int, int>(), py::arg("data"), py::arg("batchId"), py::arg("texId"))
        //.def("add", &Sprite::add)
        .def_property("defaultAnimation", &Sprite::getDefaultAnimation, &Sprite::setDefaultAnimation);

    py::class_<TileMap, Model<primitives::Quad>, std::shared_ptr<TileMap>>(m, "TileMap")
        .def(py::init<int, int, glm::ivec2>())
        .def("addFrame", &TileMap::addFrame);

    // base light - not instantiable
    py::class_<Light, std::shared_ptr<Light>>(m, "__Light");

    py::class_<DirectionalLight, Light, std::shared_ptr<DirectionalLight>>(m, "DirectionalLight")
        .def(py::init<glm::vec3, glm::vec3, float>(), py::arg("direction"), py::arg("color"), py::arg("ambient"));

    // ------------ skeletal
    py::class_<Mesh, std::shared_ptr<Mesh>>(m, "Mesh")
        .def(py::init<const std::vector<float>&, const std::vector<unsigned>&, const std::string&>());

    py::class_<SkeletalModel, IModel, std::shared_ptr<SkeletalModel>>(m, "SkeletalModel")
        .def(py::init<>())
        .def("addJoint", &SkeletalModel::addJoint)
        .def("setMesh", &SkeletalModel::setMesh)
        .def("setDefaultAnimation", &SkeletalModel::setDefaultAnimation)
        .def("addAnimation", &SkeletalModel::addAnimation);

    py::class_<KeyFrame, std::shared_ptr<KeyFrame>>(m, "KeyFrame")
        .def(py::init<float>())
        .def("addJoint", &KeyFrame::addJointInfo);

    py::class_<SkeletalAnimation, std::shared_ptr<SkeletalAnimation>>(m, "SkeletalAnimation")
        .def(py::init<>())
        .def("addKeyFrame", &SkeletalAnimation::addKeyFrame);

    py::class_<Action, std::shared_ptr<Action>>(m, "_Action");

    py::class_<Script, std::shared_ptr<Script>>(m, "Script")
        .def(py::init<const std::string&>(), py::arg("id") ="")
        .def("setLoop", &Script::setLoop)
        .def("addAction", &Script::addAction, py::arg("action"), py::arg("after")=-2)
        .def("addEdge", &Script::addEdge);

    py::class_<Scheduler, Node, std::shared_ptr<Scheduler>>(m, "Scheduler")
        .def(py::init<>())
        .def("play", &Scheduler::play);


    py::class_<ICollisionEngine, std::shared_ptr<ICollisionEngine>>(m, "ICollisionEngine")
		.def("addResponse", &ICollisionEngine::addResponse, py::arg("tag1"), py::arg("tag2"),
			 py::arg("onStart")=py::none(), py::arg("onEnd")=py::none());
        //.def(py::init<>());
	py::class_<SpatialHashingCollisionEngine, ICollisionEngine, std::shared_ptr<SpatialHashingCollisionEngine>>(m, "CollisionEngine")
		.def(py::init<float, float>());
    /* Adventure
     */
    py::class_<Text, Node, std::shared_ptr<Text>>(m, "Text")
        .def(py::init<const std::string&, const std::string&, glm::vec4, HAlign, float, glm::vec2>(),
                py::arg("font"), py::arg("text"), py::arg("color"),
				py::arg("align") = HAlign::LEFT, py::arg("width")=0.f,
				py::arg("anchor")=glm::vec2(0.f))
		.def_property_readonly("size", &Text::getSize);

    py::class_<WalkArea, Node, std::shared_ptr<WalkArea>>(mAdv, "WalkArea")
        .def(py::init<const std::vector<float>&, int, glm::vec4>(),
                py::arg("poly"), py::arg("batch") = -1, py::arg("color"))
        .def("addHole", &WalkArea::addHole, py::arg("points"), py::arg("node")=nullptr)
        .def("addLine", &WalkArea::addLine, py::arg("points"), py::arg("node")=nullptr);

    py::class_<MouseController, Node, MouseListener, std::shared_ptr<MouseController>>(mAdv, "MouseController")
        .def(py::init<float>())
		.def("activateCamera", &MouseController::activateCamera)
        .def("setCursor", &MouseController::setCursor)
        .def("addSequence", &MouseController::addCursorSequence)
        .def("addAction", &MouseController::setCursorAction)
		.def("setOnClick", &MouseController::setOnClick)
        .def("setSequence", &MouseController::setSequence, py::arg("seq"), py::arg("index")=0)
		.def("setOnRightClick", &MouseController::setOnRightClick);

    py::class_<Obstacle, Node, std::shared_ptr<Obstacle>>(mAdv, "Obstacle")
        .def(py::init<const std::string&, int, int, int, int, float>());
    /*
     * Actions
     */
    py::class_<actions::Animate, Action, std::shared_ptr<actions::Animate>>(mAct, "Animate")
		.def(py::init<Node*, const std::string&>());

	py::class_<actions::WalkTo, Action, std::shared_ptr<actions::WalkTo>>(mAct, "Walk")
        .def(py::init<Node*, WalkArea*, glm::vec2, float>(), py::arg("node"), py::arg("walkarea"),
             py::arg("position"), py::arg("speed"));

    py::class_<actions::Delay, Action, std::shared_ptr<actions::Delay>>(mAct, "Delay")
        .def(py::init<float>(), py::arg("time"));

    py::class_<actions::CallFunc, Action, std::shared_ptr<actions::CallFunc>>(mAct, "CallFunc")
        .def(py::init<const pybind11::function&>(), py::arg("callback"));

	py::class_<actions::WaitForMouseClick, Action, std::shared_ptr<actions::WaitForMouseClick>>(mAct, "WaitForMouseClick")
		.def(py::init<pybind11::function, pybind11::function>());


}
