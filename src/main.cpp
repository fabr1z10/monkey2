
#include "pyhelper.h"
#include <iostream>
#include "functions.h"
#include "room.h"
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
#include "components/depthscale.h"

#include "shapes/line.h"


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

	m.def("test", &test);

    m.def("fromHex", &fromHex, py::arg("color"));

	m.def("game", &game, py::return_value_policy::reference, "Gets the engine");

    py::class_<Shape, std::shared_ptr<Shape>>(m, "Shape");

    py::class_<Line, Shape, std::shared_ptr<Line>>(mSha, "Line")
        .def(py::init<glm::vec2, glm::vec2>(), py::arg("a"), py::arg("b"));

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
        .def_property("collisionEngine", &Room::getCollisionEngine, &Room::setCollisionEngine);


    py::class_<Camera, std::shared_ptr<Camera>>(m, "camera")
		.def("setPosition", &Camera::setPosition)
		.def("move", &Camera::move);

	py::class_<Node, std::shared_ptr<Node>>(m, "Node")
		.def(py::init<>())
		.def("setModel", &Node::setModel, py::arg("model"), py::arg("batch") = -1)
		.def("setTransform", &Node::setTransform)
		.def("add", &Node::add)
		.def("addComponent", &Node::addComponent)
        .def("getPosition", &Node::getWorldPosition)
        .def("setPosition", &Node::setPosition);

	py::class_<Component, std::shared_ptr<Component>>(m, "C");

	py::class_<Keyboard, Component, std::shared_ptr<Keyboard>>(m, "Keyboard")
	    .def(py::init<>())
	    .def("add", &Keyboard::addFunction);

    py::class_<Follow, Component, std::shared_ptr<Follow>>(m, "Follow")
        .def(py::init<int>(), py::arg("camId"));

    py::class_<DepthScale, Component, std::shared_ptr<DepthScale>>(m, "DepthScale")
        .def(py::init<float, float>(), py::arg("y0"), py::arg("y1"));

    py::class_<Collider, Component, std::shared_ptr<Collider>>(m, "Collider")
        .def(py::init<std::shared_ptr<Shape>>(), py::arg("shape"));

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
        .def(py::init<int, int, int, int, int>(), py::arg("size"), py::arg("cam"),
             py::arg("texWidth"), py::arg("texHeight"), py::arg("maxTextures"))
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

    py::class_<Sprite, Model<primitives::Quad>, std::shared_ptr<Sprite>>(m, "Sprite")
        .def(py::init<const std::vector<float>&, int>())
        .def(py::init<const std::vector<float>&, int, int>(), py::arg("data"), py::arg("batchId"), py::arg("texId"))
        .def("add", &Sprite::add)
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
        .def("addAction", &Script::addAction)
        .def("addEdge", &Script::addEdge);

    py::class_<Scheduler, Node, std::shared_ptr<Scheduler>>(m, "Scheduler")
        .def(py::init<>());

    py::class_<CollisionEngine, std::shared_ptr<CollisionEngine>>(m, "CollisionEngine")
        .def(py::init<>());
    /* Adventure
     */

    py::class_<WalkArea, Node, std::shared_ptr<WalkArea>>(mAdv, "WalkArea")
        .def(py::init<const std::vector<float>&, int, glm::vec4>(),
                py::arg("poly"), py::arg("batch") = -1, py::arg("color"))
        .def("addHole", &WalkArea::addHole)
        .def("addLine", &WalkArea::addLine);

    py::class_<MouseController, Node, MouseListener, std::shared_ptr<MouseController>>(mAdv, "MouseController")
        .def(py::init<int, WalkArea*, Node*, Scheduler*, float>());

    py::class_<Obstacle, Node, std::shared_ptr<Obstacle>>(mAdv, "Obstacle")
        .def(py::init<const std::string&, int, int, int, int, float>());
    /*
     * Actions
     */
    py::class_<actions::WalkTo, Action, std::shared_ptr<actions::WalkTo>>(mAct, "Walk")
        .def(py::init<Node*, WalkArea*, glm::vec2, float>(), py::arg("node"), py::arg("walkarea"),
             py::arg("position"), py::arg("speed"));
}
