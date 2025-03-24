#pragma once

#include <pybind11/pybind11.h>


template<typename T>
void addPyRef(std::shared_ptr<T> obj) {
    if (!obj) return;

    try {
        pybind11::gil_scoped_acquire gil;
        pybind11::object py_self = pybind11::cast(obj);
        obj->setPySelf(py_self);  // This works because all subclassable types inherit from PySubclassable
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to store Python reference: " << e.what() << std::endl;
    }

}

class PySubclassable {
public:
    virtual ~PySubclassable() {
        _py_self = pybind11::none();
    }

	void setPySelf(pybind11::object self) {
		_py_self = self;
	}

    void dispose() {
        _py_self = pybind11::none();
    }


private:
	pybind11::object _py_self;
};