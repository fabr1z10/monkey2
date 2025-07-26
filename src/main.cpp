#include <pybind11/stl.h>
#include <iostream>
#include "functions.h"
#include "room.h"
#include "text.h"
#include "camera.h"
#include "primitives/prim.h"
#include "models/linemodel.h"
#include "model.h"
#include "components/keyboard.h"
#include "components/camcontrol3d.h"
#include "directionallight.h"
#include "linebatch.h"
#include "script.h"
#include "script.h"
#include "scheduler.h"
#include "sprite.h"
#include "tilemap.h"
#include "vec.h"
#include "multisprite.h"

//#include "skeletal/mesh.h"
//#include "skeletal/skeletalmodel.h"

#include "adventure/walkarea.h"
#include "adventure/mousecontroller.h"
#include "adventure/obstacle.h"
#include "components/follow.h"

#include "actions/walk.h"
#include "actions/animate.h"
#include "actions/delay.h"
#include "actions/callfunc.h"
#include "actions/waitclick.h"
#include "actions/move.h"

#include "components/depthscale.h"
#include "components/collider.h"
#include "components/hotspot.h"
#include "components/npc.h"
#include "components/controller.h"

#include "shapes/line.h"
#include "shapes/polygon.h"
#include "shapes/rect.h"
#include "shapes/point.h"
#include "shapes/aabb.h"
#include "tiledsprite.h"
#include "models/tilemodel.h"
#include "models/linemodel.h"
#include "models/quad.h"
#include "models/treemodel.h"

#include "agi/agiroom.h"
#include "agi/agiobject.h"

using namespace adventure;
using namespace shapes;

namespace py = pybind11;

PYBIND11_MODULE(monkey2, m) {

    py::module_ mAdv = m.def_submodule("adventure");
    py::module_ mAct = m.def_submodule("actions");
    py::module_ mSha = m.def_submodule("shapes");
	py::module_ mAGI = m.def_submodule("agi");

	py::enum_<HAlign>(m, "Alignment")
		.value("LEFT", HAlign::LEFT)
		.value("RIGHT", HAlign::RIGHT)
		.value("CENTER", HAlign::CENTER)
		.export_values();

    py::enum_<ModelType>(m, "ModelType")
        .value("WIRE", ModelType::WIREFRAME)
        .value("SOLID", ModelType::SOLID)
        .export_values();

	py::enum_<CharacterDirection>(m, "Direction")
			.value("NORTH", CharacterDirection::NORTH)
			.value("SOUTH", CharacterDirection::SOUTH)
			.value("EAST", CharacterDirection::EAST)
			.value("WEST", CharacterDirection::WEST)

			.export_values();
    // gets node with given id
    m.def("getNode", &getNode, py::arg("id"), py::return_value_policy::reference);

    //m.def("fromHex", &fromHex, py::arg("color"));

    m.def("loadAsset", &loadAsset, py::arg("id"), py::arg("filename"), py::arg("cam"), py::arg("tex") = {});


	//m.def("getSheet", &getSheet, py::arg("id"));

	m.def("game", &game, py::return_value_policy::reference, "Gets the engine");

	m.def("closeRoom", &closeRoom);


    py::class_<Vec2>(m, "Vec2")
        .def(py::init<>())
        .def(py::init<float, float>())
        .def(py::init<std::vector<float>>())
        .def_readwrite("x", &Vec2::x)  // Expose member variables
        .def_readwrite("y", &Vec2::y)
        .def("__add__", &Vec2::operator+)  // Addition operator
        .def("__mul__", &Vec2::operator*)  // Multiplication by scalar
        .def("length", &Vec2::length);   // Length method

    py::class_<Vec3>(m, "Vec3")
        .def(py::init<>())
        .def(py::init<float, float, float>())
        .def(py::init<std::vector<float>>())
        .def_readwrite("x", &Vec3::x)  // Expose member variables
        .def_readwrite("y", &Vec3::y)
        .def_readwrite("z", &Vec3::z)
        .def("__add__", &Vec3::operator+)  // Addition operator
        .def("__mul__", &Vec3::operator*)  // Multiplication by scalar
        .def("length", &Vec3::length);   // Length method

    py::class_<Vec4>(m, "Vec4")
        .def(py::init<>())
        .def(py::init<float, float, float, float>())
        .def(py::init<std::vector<float>>())
        .def_readwrite("x", &Vec4::x)  // Expose member variables
        .def_readwrite("y", &Vec4::y)
        .def_readwrite("z", &Vec4::z)
        .def_readwrite("w", &Vec4::w)
        .def("__add__", &Vec4::operator+)  // Addition operator
        .def("__mul__", &Vec4::operator*)  // Multiplication by scalar
        .def("length", &Vec4::length);   // Length method

    py::class_<Color>(m, "Color")
        .def(py::init<const std::string&>())
        .def(py::init<float, float, float, float>())
        .def_readwrite("r", &Color::r)  // Expose member variables
        .def_readwrite("g", &Color::g)
        .def_readwrite("b", &Color::b)
        .def_readwrite("a", &Color::a);


	// Add static named colors:
	m.attr("Color").attr("WHITE") = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m.attr("Color").attr("BLACK") = Color(0.0f, 0.0f, 0.0f, 1.0f);
	m.attr("Color").attr("RED") = Color(1.0f, 0.0f, 0.0f, 1.0f);


	py::class_<models::Animation>(m, "Animation")
		.def(py::init<>())
		.def("addFrame", &models::Animation::addFrame);

    py::class_<IVec2>(m, "IVec2")
        .def(py::init<>())
        .def(py::init<float, float>())
        .def_readwrite("x", &IVec2::x)  // Expose member variables
        .def_readwrite("y", &IVec2::y)
        .def("__add__", &IVec2::operator+)  // Addition operator
        .def("__mul__", &IVec2::operator*)  // Multiplication by scalar
        .def("length", &IVec2::length);   // Length method

	py::class_<IVec3>(m, "IVec3")
			.def(py::init<>())
			.def(py::init<int, int, int>())
			.def_readwrite("x", &IVec3::x)  // Expose member variables
			.def_readwrite("y", &IVec3::y)
			.def_readwrite("z", &IVec3::z)
			.def("__add__", &IVec3::operator+)  // Addition operator
			.def("__mul__", &IVec3::operator*)  // Multiplication by scalar
			.def("length", &IVec3::length);   // Length method

	py::class_<IVec4>(m, "IVec4")
			.def(py::init<>())
			.def(py::init<int, int, int, int>())
			.def_readwrite("x", &IVec4::x)  // Expose member variables
			.def_readwrite("y", &IVec4::y)
			.def_readwrite("z", &IVec4::z)
			.def("__add__", &IVec4::operator+)  // Addition operator
			.def("__mul__", &IVec4::operator*)  // Multiplication by scalar
			.def("length", &IVec4::length);   // Length method




    py::class_<Game>(m, "Game")
		.def("start", &Game::start)
		.def("run", &Game::run)
		.def("room", &Game::getRoom, py::return_value_policy::reference)
        .def("makeCurrent", &Game::makeCurrent);


	py::class_<Camera, std::shared_ptr<Camera>>(m, "camera")
			.def("setPosition", &Camera::setPosition)
			.def("setBounds", &Camera::setBounds)
			.def("move", &Camera::move);

	py::class_<IBatch, std::shared_ptr<IBatch>>(m, "IBatch")
			.def_property_readonly("id", &IBatch::getId);

    py::class_<Room, std::shared_ptr<Room>>(m, "Room")
		.def(py::init<>())
		.def("addCamera", &Room::addCamera)
		.def("getCamera", &Room::getCamera, py::return_value_policy::reference)
		.def("root", &Room::getRoot, py::return_value_policy::reference)
		.def("setClearColor", &Room::setClearColor)
        .def ("setStartUpFunction", &Room::setStartUpFunction)
		.def("addBatch", &Room::addBatch)
        .def_property("collisionEngine", &Room::getCollisionEngine, &Room::setCollisionEngine)
        .def_property("hotSpotManager", &Room::getHotSpotManager, &Room::setHotSpotManager);



	py::class_<Component, std::shared_ptr<Component>> component(m, "Component");
	py::class_<Node, std::shared_ptr<Node>> node(m, "Node");
	py::class_<IModel, std::shared_ptr<IModel>>(m, "Y2")
		.def("setPalette", &IModel::setPalette);

	m.def("getModel", &getModel, py::arg("batch"),  py::arg("id"), py::return_value_policy::reference);

	py::class_<models::LineModel, IModel, std::shared_ptr<models::LineModel>>(m, "LineModel")
			.def(py::init<int, std::vector<float>, Color>(), py::arg("batch"), py::arg("data"), py::arg("color"));
	py::class_<models::Quad, IModel, std::shared_ptr<models::Quad>>(m, "Quad")
			.def(py::init<int, int, int, int, int, int>(), py::arg("batch"), py::arg("x"), py::arg("y"), py::arg("width"), py::arg("height"), py::arg("pal") = 0);
	py::class_<models::TileMap, IModel, std::shared_ptr<models::TileMap>>(m, "TileMap")
			.def(py::init<int, int>(), py::arg("batch"), py::arg("quads"))
			.def("addAnimation", &models::TileMap::addAnimation);
	py::class_<models::TreeModel, IModel, std::shared_ptr<models::TreeModel>>(m, "TreeModel")
			.def(py::init<>())
			.def("addAnimation", &models::TreeModel::addAnimation, py::arg("id"), py::arg("node"), py::arg("anim"))
			.def("addSprite", &models::TreeModel::addSprite, py::arg("id"),
				 py::arg("sprite"), py::arg("parent"), py::arg("bone"));

	py::class_<Shape, std::shared_ptr<Shape>>(m, "Shape")
			.def("toModel", &Shape::makeModel);
	mSha.def("fromImage", &shapeFromImage);

	node
        .def(py::init<>())
        .def_property_readonly("id", &Node::id)
        .def_property_readonly("id", &Node::id)
        .def_property_readonly("x", &Node::x)
        .def_property_readonly("y", &Node::y)
        .def_property("userData", &Node::getUserData, &Node::setUserData, py::return_value_policy::reference)
        .def_property("active", &Node::active, &Node::setActive)
        .def_property("show", &Node::show, &Node::setShow)
        .def_property("animation", &Node::getAnimation, &Node::setAnimation)
        .def("getChildren", &Node::getChildren, py::return_value_policy::reference)
        .def("getComponent", &Node::getComponentById)
		.def("model", &Node::getModel, py::return_value_policy::reference)
        .def("setModel", &Node::setModel, py::arg("model"))
        //.def("setTransform", &Node::setTransform)
        .def("add", &Node::add)
        .def("remove", &Node::remove)
        .def("addComponent", &Node::addComponent)
        .def("getPosition", &Node::getWorldPosition)
        .def("setPosition", &Node::setPosition)
        .def("setMultiplyColor", &Node::setMultiplyColor)
        .def("flipX", &Node::flipHorizontal)
        .def("clear", &Node::clear);

//    py::class_<MultiSprite, Node, std::shared_ptr<MultiSprite>>(m, "MultiSprite")
//        .def(py::init<>())
//        .def("addNode", &MultiSprite::addNode)
//        .def("addAnimation", &MultiSprite::addAnimation);

	py::class_<SpriteSheet, std::shared_ptr<SpriteSheet>>(m, "SpriteSheet")
		.def("makeSprite", &SpriteSheet::makeSprite);
    
	component
        .def_property_readonly("node", &Component::getNode, py::return_value_policy::reference)
		.def_property("id", &Component::getId, &Component::setId)
		.def("keepAlive", &HotSpot::setPySelf);

	
	py::class_<Keyboard, Component, std::shared_ptr<Keyboard>>(m, "Keyboard")
	    .def(py::init<>())
	    .def("add", &Keyboard::addFunction);

    py::class_<Follow, Component, std::shared_ptr<Follow>>(m, "Follow")
        .def(py::init<int>(), py::arg("camId"));

    py::class_<DepthScale, Component, std::shared_ptr<DepthScale>>(m, "DepthScale")
        .def(py::init<float, float, int>(), py::arg("y0"), py::arg("y1"), py::arg("mask"));

    py::class_<Collider, Component, std::shared_ptr<Collider>>(m, "Collider")
        .def(py::init<std::shared_ptr<Shape>, int, int, const std::string&, int>(),
                py::arg("shape"), py::arg("flag"), py::arg("mask"), py::arg("tag"), py::arg("batch")=-1);

	py::class_<Controller, Component, std::shared_ptr<Controller>>(m, "Controller")
		.def(py::init<float, int>());

    py::class_<HotSpot, PyHotSpot, Component, std::shared_ptr<HotSpot>>(m, "HotSpot")
        .def(py::init<std::shared_ptr<Shape>, int, int, int>())
        .def("setShape", &HotSpot::setShape);
//		.def(py::init([](std::shared_ptr<Shape> shape, int priority, int camera) {
//			auto instance = std::make_shared<PyHotSpot>(shape, priority, camera);
//			py::object self = py::cast(instance);
//			instance->setPySelf(self);
//			return instance;
//		}));

	py::class_<WalkArea, Node, std::shared_ptr<WalkArea>>(mAdv, "WalkArea")
			.def(py::init<const std::vector<float>&, int, Color>(),
				 py::arg("poly"), py::arg("batch") = -1, py::arg("color"))
			.def("addHole", &WalkArea::addHole, py::arg("points"), py::arg("node")=nullptr)
			.def("addLine", &WalkArea::addLine, py::arg("points"), py::arg("node")=nullptr);


    py::class_<NPC, Component, std::shared_ptr<NPC>>(m, "NPC")
        .def(py::init<adventure::WalkArea*, float, float, pybind11::function>());

    py::class_<MouseListener, std::shared_ptr<MouseListener>>(m, "_MouseListener");

	py::class_<CamControl3D, Component, std::shared_ptr<CamControl3D>>(m, "CamControl3D")
        .def(py::init<int, float, float>());

	py::class_<OrthoCamera, Camera, std::shared_ptr<OrthoCamera>>(m, "CamOrtho")
		.def(py::init<float, float>(), py::arg("width"), py::arg("height"))
        .def(py::init<float, float, Vec4>(), py::arg("width"), py::arg("height"), py::arg("viewport"));

	py::class_<PerspectiveCamera, Camera, std::shared_ptr<PerspectiveCamera>>(m, "CamPerspective")
		.def(py::init<>())
        .def(py::init<Vec4, float, float, float>(), py::arg("viewport"), py::arg("fov"), py::arg("near"), py::arg("far"));

	/****************************************
	 * batches
	 ***************************************/
	// base class - not instantiable

	    //.def("addLight", &IBatch::addLight);

	py::class_<LineBatch, IBatch, std::shared_ptr<LineBatch>>(m, "LineBatch")
		.def(py::init<int, int>(), py::arg("maxLines"), py::arg("camera"));

	py::class_<QuadBatchPalette, IBatch, std::shared_ptr<QuadBatchPalette>>(m, "QuadBatchPalette")
		.def(py::init<int, int, const std::string&>(), py::arg("maxQuads"), py::arg("camera"), py::arg("sheetFile"));
	/****************************************
	 * models
	 ***************************************/
    // base model - not instantiable

//	py::class_<Model<primitives::Line>, IModel, std::shared_ptr<Model<primitives::Line>>>(m, "LineModel")
//		.def(py::init<const std::vector<float>&>());
//
//    py::class_<Model<primitives::Triangle>, IModel, std::shared_ptr<Model<primitives::Triangle>>>(m, "TriangleModel")
//        .def(py::init<const std::vector<float>&>());
//
//    py::class_<Model<primitives::TriangleNormal>, IModel, std::shared_ptr<Model<primitives::TriangleNormal>>>(m, "TriangleNormalModel")
//        .def(py::init<const std::vector<float>&>());
//
//    py::class_<Model<primitives::Quad>, IModel, std::shared_ptr<Model<primitives::Quad>>>(m, "QuadModel");
//	py::class_<Model<primitives::QuadPalette>, IModel, std::shared_ptr<Model<primitives::QuadPalette>>>(m, "QuadModelPal");
//        //.def(py::init<const std::vector<float>&>());
//
//    py::class_<Quad, Model<primitives::Quad>, std::shared_ptr<Quad>>(m, "Quad")
//        .def(py::init<const std::vector<float>&, int, int>());
//
//	py::class_<PalQuad, Model<primitives::QuadPalette>, std::shared_ptr<PalQuad>>(m, "QuadPalette")
//		.def(py::init<IVec4, IVec2, int, int, IVec2>(), py::arg("textureCoords"), py::arg("anchor"), py::arg("pal"), py::arg("batch"), py::arg("size") = IVec2());
//
//    py::class_<Sprite, Model<primitives::Quad>, std::shared_ptr<Sprite>>(m, "Sprite")
//        .def(py::init<const std::vector<float>&, int>())
//        .def(py::init<const std::vector<float>&, int, int>(), py::arg("data"), py::arg("batchId"), py::arg("texId"))
//        //.def("add", &Sprite::add)
//        .def_property("defaultAnimation", &Sprite::getDefaultAnimation, &Sprite::setDefaultAnimation);
//
//    py::class_<TileMap, Node, std::shared_ptr<TileMap>>(m, "TileMap")
//        .def(py::init<int, int, int>(), py::arg("tileSize"),
//			 py::arg("batchId"), py::arg("textureId"))
//		.def("define", &TileMap::define);

    //py::class_<TiledSprite, Model<primitives::Quad>, std::shared_ptr<TiledSprite>>(m, "TiledSprite")
    //    .def(py::init<int, int, int>())
    //    .def("addFrame", &TiledSprite::addFrame);

    // base light - not instantiable
//    py::class_<Light, std::shared_ptr<Light>>(m, "__Light");
//
//    py::class_<alLight, Light, std::shared_ptr<DirectionalLight>>(m, "DirectionalLight")
//        .def(py::init<glm::vec3, glm::vec3, float>(), py::arg("direction"), py::arg("color"), py::arg("ambient"));

    // ------------ skeletal
//    py::class_<Mesh, std::shared_ptr<Mesh>>(m, "Mesh")
//        .def(py::init<const std::vector<float>&, const std::vector<unsigned>&, const std::string&>());
//
//    py::class_<SkeletalModel, IModel, std::shared_ptr<SkeletalModel>>(m, "SkeletalModel")
//        .def(py::init<>())
//        .def("addJoint", &SkeletalModel::addJoint)
//        .def("setMesh", &SkeletalModel::setMesh)
//        .def("setDefaultAnimation", &SkeletalModel::setDefaultAnimation)
//        .def("addAnimation", &SkeletalModel::addAnimation);
//
//    py::class_<KeyFrame, std::shared_ptr<KeyFrame>>(m, "KeyFrame")
//        .def(py::init<float>())
//        .def("addJoint", &KeyFrame::addJointInfo);
//
//    py::class_<SkeletalAnimation, std::shared_ptr<SkeletalAnimation>>(m, "SkeletalAnimation")
//        .def(py::init<>())
//        .def("addKeyFrame", &SkeletalAnimation::addKeyFrame);

    py::class_<Action, std::shared_ptr<Action>>(m, "_Action");

    py::class_<Script, std::shared_ptr<Script>>(m, "Script")
        .def(py::init<const std::string&>(), py::arg("id") ="")
        .def("setLoop", &Script::setLoop)
        .def("addAction", &Script::addAction, py::arg("action"), py::arg("after")=-2)
        .def("addEdge", &Script::addEdge);

    py::class_<Scheduler, Node, std::shared_ptr<Scheduler>>(m, "Scheduler")
        .def(py::init<>())
        .def("play", &Scheduler::play);

    py::class_<CollisionResponse, PyCollisionResponse, std::shared_ptr<CollisionResponse>>(m, "CollisionResponse")
        .def(py::init<>())  // Allow Python subclasses
        .def("onStart", &CollisionResponse::onStart)
        .def("onEnd", &CollisionResponse::onEnd);


    py::class_<ICollisionEngine, std::shared_ptr<ICollisionEngine>>(m, "ICollisionEngine")
        .def("addResponse", &ICollisionEngine::addResponse, py::arg("tag1"), py::arg("tag2"), py::arg("response"));
        //.def(py::init<>());
	py::class_<SpatialHashingCollisionEngine, ICollisionEngine, std::shared_ptr<SpatialHashingCollisionEngine>>(m, "CollisionEngine")
		.def(py::init<float, float>());
    /* Adventure
     */
    py::class_<Text, Node, std::shared_ptr<Text>>(m, "Text")
		.def(py::init<int, const std::string&, const std::string&, int, HAlign, float, Vec2>(),
			py::arg("batch"), py::arg("font"), py::arg("text"), py::arg("palette"),
			py::arg("align"), py::arg("width"),
			py::arg("anchor"))
        .def("updateText" , &Text::updateText)
		.def_property_readonly("size", &Text::getSize);


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

	py::class_<WalkableCharacter, Node, std::shared_ptr<WalkableCharacter>>(mAdv, "WalkableCharacter")
		.def(py::init<int, const std::string&, const std::string&, const std::string&>(),
		        py::arg("batch"), py::arg("legs_sprite"), py::arg("body_sprite"), py::arg("head_sprite"))
		.def("getDirection", &WalkableCharacter::getDirection)
		.def("setDirection", &WalkableCharacter::setDirection)
		.def("setTalking", &WalkableCharacter::setTalking);
    /*
     * Actions
     */
    py::class_<actions::Animate, Action, std::shared_ptr<actions::Animate>>(mAct, "Animate")
		.def(py::init<Node*, const std::string&>());

    py::class_<actions::MoveTo, Action, std::shared_ptr<actions::MoveTo>>(mAct, "MoveTo")
        .def(py::init<Node*, Vec2, float>(), py::arg("node"), py::arg("pos"), py::arg("speed"));

    py::class_<actions::WalkTo, Action, std::shared_ptr<actions::WalkTo>>(mAct, "Walk")
        //.def(py::init<glm::vec2, py::float_>());
        .def(py::init<Node*, WalkArea*, Vec2, py::float_>(), py::arg("node"), py::arg("walkarea"),
            py::arg("position"), py::arg("speed"));

    py::class_<actions::Delay, Action, std::shared_ptr<actions::Delay>>(mAct, "Delay")
        .def(py::init<float>(), py::arg("time"));

    py::class_<actions::CallFunc, Action, std::shared_ptr<actions::CallFunc>>(mAct, "CallFunc")
        .def(py::init<const pybind11::function&>(), py::arg("callback"));

	py::class_<actions::WaitForMouseClick, Action, std::shared_ptr<actions::WaitForMouseClick>>(mAct, "WaitForMouseClick")
		.def(py::init<pybind11::function, pybind11::function>());

	py::class_<Line, Shape, std::shared_ptr<Line>>(mSha, "Line")
			.def(py::init<Vec2, Vec2>(), py::arg("a"), py::arg("b"));


	py::class_<shapes::AABB, Shape, std::shared_ptr<shapes::AABB>>(mSha, "AABB")
			.def(py::init<float, float>(), py::arg("width"), py::arg("height"))
			.def(py::init<float, float, Vec2>(), py::arg("width"), py::arg("height"), py::arg("anchor"));

	py::class_<Rect, Shape, std::shared_ptr<Rect>>(mSha, "Rect")
			.def(py::init<float, float, Vec2>(), py::arg("width"), py::arg("height"), py::arg("anchor"));

	py::class_<PolyLine, Shape, std::shared_ptr<PolyLine>>(mSha, "PolyLine")
			.def(py::init<const std::vector<float>&>(), py::arg("points"));

	py::class_<Polygon, Shape, std::shared_ptr<Polygon>>(mSha, "Polygon")
			.def(py::init<const std::vector<float>&>(), py::arg("points"));

	py::class_<Point, Shape, std::shared_ptr<Point>>(mSha, "Point")
			.def(py::init<>());

	/****************
	 * AGI
	 */
	py::class_<agi::AGIRoom, Room, std::shared_ptr<agi::AGIRoom>>(mAGI, "Room")
		.def(py::init<const std::string&, const std::string&>());

	py::class_<agi::AGIObject, Node, std::shared_ptr<agi::AGIObject>>(mAGI, "Object")
		.def(py::init<const std::string&, int, int, float>());


}
