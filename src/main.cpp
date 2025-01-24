
#include "pyhelper.h"
#include <iostream>
#include "functions.h"
#include "room.h"
#include "camera.h"
#include "primitives/prim.h"
#include "linemodel.h"


namespace py = pybind11;

void test() {
	std::cout << "viva la figa\n";
}

PYBIND11_MODULE(monkey2, m) {

	m.def("test", &test);

	m.def("game", &game, py::return_value_policy::reference, "Gets the engine");

	py::class_<Game>(m, "Game")
		.def("start", &Game::start)
		.def("run", &Game::run);

	py::class_<Room, std::shared_ptr<Room>>(m, "Room")
		.def(py::init<>())
		.def("addCamera", &Room::addCamera)
		.def("root", &Room::getRoot, py::return_value_policy::reference)
		.def("addBatch", &Room::addBatch);

	py::class_<Camera, std::shared_ptr<Camera>>(m, "camera")
		.def("setPosition", &Camera::setPosition)
		.def("move", &Camera::move);

	py::class_<Node, std::shared_ptr<Node>>(m, "Node")
		.def(py::init<>())
		.def("setModel", &Node::setModel)
		.def("add", &Node::add);

	py::class_<OrthoCamera, Camera, std::shared_ptr<OrthoCamera>>(m, "CamOrtho")
		.def(py::init<float, float, glm::vec4>(), py::arg("width"), py::arg("height"), py::arg("viewport") = glm::vec4(0.f));

	py::class_<PerspectiveCamera, Camera, std::shared_ptr<PerspectiveCamera>>(m, "CamPerspective")
		.def(py::init<glm::vec4, float, float, float>(), py::arg("viewport") = glm::vec4(0.f), py::arg("fov")=45.f, py::arg("near")=0.1f, py::arg("far")=100.f);

	// batches
	// base class - not instantiable
	py::class_<IBatch, std::shared_ptr<IBatch>>(m, "Y");

	py::class_<Batch<primitives::Line>, IBatch, std::shared_ptr<Batch<primitives::Line>> >(m, "LineBatch")
		.def(py::init<int, int>(), py::arg("size"), py::arg("cam"));

	// base mode l- not instantiable
	py::class_<IModel, std::shared_ptr<IModel>>(m, "Y2");

	py::class_<LineModel, IModel, std::shared_ptr<LineModel>>(m, "LineModel")
		.def(py::init<const std::vector<float>&, glm::vec4>());
}