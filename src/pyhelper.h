#pragma once

#include <pybind11/pybind11.h>
#include <glm/glm.hpp>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>




namespace py = pybind11;

namespace PYBIND11_NAMESPACE { namespace detail {
	template <> struct type_caster<glm::vec2> {
		public:
			/**
			 * This macro establishes the name 'inty' in
			 * function signatures and declares a local variable
			 * 'value' of type inty
			 */
			PYBIND11_TYPE_CASTER(glm::vec2, const_name("vec2"));

			/**
			 * Conversion part 1 (Python->C++): convert a PyObject into a inty
			 * instance or return false upon failure. The second argument
			 * indicates whether implicit conversions should be applied.
			 */
			bool load(handle src, bool) {
				PyObject *source = src.ptr();

				/* Try converting into a Python integer value */
				PyObject *tmp = PyObject_GetItem(source, PyLong_FromLong(0));
				value.x = PyFloat_AsDouble(tmp);
				Py_DECREF(tmp);
				tmp = PyObject_GetItem(source, PyLong_FromLong(1));
				value.y = PyFloat_AsDouble(tmp);
				Py_DECREF(tmp);

				//
				/* Ensure return code was OK (to avoid out-of-range errors etc) */
				return true;
			}
			/**
			 * Conversion part 2 (C++ -> Python): convert an inty instance into
			 * a Python object. The second and third arguments are used to
			 * indicate the return value policy and parent object (for
			 * ``return_value_policy::reference_internal``) and are generally
			 * ignored by implicit casters.
			 */
			static handle cast(glm::vec2 src, return_value_policy /* policy */, handle /* parent */) {
				return PyTuple_Pack(2, PyFloat_FromDouble(src.x), PyFloat_FromDouble(src.y));// PyLong_FromLong(src.long_value);
			}
	};

	template <> struct type_caster<glm::vec3> {
		public:
			/**
			 * This macro establishes the name 'inty' in
			 * function signatures and declares a local variable
			 * 'value' of type inty
			 */
			PYBIND11_TYPE_CASTER(glm::vec3, const_name("vec3"));

			/**
			 * Conversion part 1 (Python->C++): convert a PyObject into a inty
			 * instance or return false upon failure. The second argument
			 * indicates whether implicit conversions should be applied.
			 */
			bool load(handle src, bool convert) {
				PyObject *source = src.ptr();

				/* Try converting into a Python integer value */
				PyObject *tmp = PyObject_GetItem(source, PyLong_FromLong(0));
				value.x = PyFloat_AsDouble(tmp);
				Py_DECREF(tmp);
				tmp = PyObject_GetItem(source, PyLong_FromLong(1));
				value.y = PyFloat_AsDouble(tmp);
				Py_DECREF(tmp);
				tmp = PyObject_GetItem(source, PyLong_FromLong(2));
				value.z = PyFloat_AsDouble(tmp);
				Py_DECREF(tmp);
				//
				/* Ensure return code was OK (to avoid out-of-range errors etc) */
				return true;
			}

			/**
			 * Conversion part 2 (C++ -> Python): convert an inty instance into
			 * a Python object. The second and third arguments are used to
			 * indicate the return value policy and parent object (for
			 * ``return_value_policy::reference_internal``) and are generally
			 * ignored by implicit casters.
			 */
			static handle cast(glm::vec3 src, return_value_policy /* policy */, handle /* parent */) {
				return PyTuple_Pack(3, PyFloat_FromDouble(src.x), PyFloat_FromDouble(src.y), PyFloat_FromDouble(src.z));// PyLong_FromLong(src.long_value);
			}
	};

	template <> struct type_caster<glm::vec4> {
		public:
			/**
			 * This macro establishes the name 'inty' in
			 * function signatures and declares a local variable
			 * 'value' of type inty
			 */
			PYBIND11_TYPE_CASTER(glm::vec4, const_name("vec4"));

			/**
			 * Conversion part 1 (Python->C++): convert a PyObject into a inty
			 * instance or return false upon failure. The second argument
			 * indicates whether implicit conversions should be applied.
			 */
			bool load(handle src, bool) {
				/* Extract PyObject from handle */
				//PyObject *source = src.ptr();
				PyObject *source = src.ptr();

				/* Try converting into a Python integer value */
				PyObject *tmp = PyObject_GetItem(source, PyLong_FromLong(0));
				value.x = PyFloat_AsDouble(tmp);
				Py_DECREF(tmp);
				tmp = PyObject_GetItem(source, PyLong_FromLong(1));
				value.y = PyFloat_AsDouble(tmp);
				Py_DECREF(tmp);
				tmp = PyObject_GetItem(source, PyLong_FromLong(2));
				value.z = PyFloat_AsDouble(tmp);
				Py_DECREF(tmp);
				tmp = PyObject_GetItem(source, PyLong_FromLong(3));
				value.w= PyFloat_AsDouble(tmp);
				Py_DECREF(tmp);
				//
				/* Ensure return code was OK (to avoid out-of-range errors etc) */
				return true;
			}

			/**
			 * Conversion part 2 (C++ -> Python): convert an inty instance into
			 * a Python object. The second and third arguments are used to
			 * indicate the return value policy and parent object (for
			 * ``return_value_policy::reference_internal``) and are generally
			 * ignored by implicit casters.
			 */
			static handle cast(glm::vec4 src, return_value_policy /* policy */, handle /* parent */) {
				return PyTuple_Pack(4, PyFloat_FromDouble(src.x), PyFloat_FromDouble(src.y), PyFloat_FromDouble(src.z), PyFloat_FromDouble(src.w));// PyLong_FromLong(src.long_value);
			}
	};


	template <> struct type_caster<glm::ivec2> {
		public:
			/**
			 * This macro establishes the name 'inty' in
			 * function signatures and declares a local variable
			 * 'value' of type inty
			 */
			PYBIND11_TYPE_CASTER(glm::ivec2, const_name("ivec2"));

			/**
			 * Conversion part 1 (Python->C++): convert a PyObject into a inty
			 * instance or return false upon failure. The second argument
			 * indicates whether implicit conversions should be applied.
			 */
			bool load(handle src, bool) {
				/* Extract PyObject from handle */
				//PyObject *source = src.ptr();
				auto tuple = src.cast<pybind11::tuple>();
				value.x = tuple[0].cast<int>();
				value.y = tuple[1].cast<int>();

				/* Ensure return code was OK (to avoid out-of-range errors etc) */
				return true;
			}

			/**
			 * Conversion part 2 (C++ -> Python): convert an inty instance into
			 * a Python object. The second and third arguments are used to
			 * indicate the return value policy and parent object (for
			 * ``return_value_policy::reference_internal``) and are generally
			 * ignored by implicit casters.
			 */
			static handle cast(glm::ivec2 src, return_value_policy /* policy */, handle /* parent */) {
				return PyTuple_Pack(2, src.x, src.y);// PyLong_FromLong(src.long_value);
			}
	};

	template <> struct type_caster<glm::ivec3> {
		public:
			/**
			 * This macro establishes the name 'inty' in
			 * function signatures and declares a local variable
			 * 'value' of type inty
			 */
			PYBIND11_TYPE_CASTER(glm::ivec3, const_name("ivec3"));

			/**
			 * Conversion part 1 (Python->C++): convert a PyObject into a inty
			 * instance or return false upon failure. The second argument
			 * indicates whether implicit conversions should be applied.
			 */
			bool load(handle src, bool) {
				/* Extract PyObject from handle */
				//PyObject *source = src.ptr();
				auto tuple = src.cast<pybind11::tuple>();
				value.x = tuple[0].cast<int>();
				value.y = tuple[1].cast<int>();
				value.z = tuple[2].cast<int>();
				/* Ensure return code was OK (to avoid out-of-range errors etc) */
				return true;
			}

			/**
			 * Conversion part 2 (C++ -> Python): convert an inty instance into
			 * a Python object. The second and third arguments are used to
			 * indicate the return value policy and parent object (for
			 * ``return_value_policy::reference_internal``) and are generally
			 * ignored by implicit casters.
			 */
			static handle cast(glm::ivec3 src, return_value_policy /* policy */, handle /* parent */) {
				return PyTuple_Pack(3, src.x, src.y, src.z);// PyLong_FromLong(src.long_value);
			}
	};

	template <> struct type_caster<glm::ivec4> {
		public:
			/**
			 * This macro establishes the name 'inty' in
			 * function signatures and declares a local variable
			 * 'value' of type inty
			 */
			PYBIND11_TYPE_CASTER(glm::ivec4, const_name("ivec4"));

			/**
			 * Conversion part 1 (Python->C++): convert a PyObject into a inty
			 * instance or return false upon failure. The second argument
			 * indicates whether implicit conversions should be applied.
			 */
			bool load(handle src, bool) {
				/* Extract PyObject from handle */
				//PyObject *source = src.ptr();
				auto tuple = src.cast<pybind11::tuple>();
				value.x = tuple[0].cast<int>();
				value.y = tuple[1].cast<int>();
				value.z = tuple[2].cast<int>();
				value.w = tuple[3].cast<int>();
				/* Ensure return code was OK (to avoid out-of-range errors etc) */
				return true;
			}

			/**
			 * Conversion part 2 (C++ -> Python): convert an inty instance into
			 * a Python object. The second and third arguments are used to
			 * indicate the return value policy and parent object (for
			 * ``return_value_policy::reference_internal``) and are generally
			 * ignored by implicit casters.
			 */
			static handle cast(glm::ivec4 src, return_value_policy /* policy */, handle /* parent */) {
				return PyTuple_Pack(4, src.x, src.y, src.z, src.w);// PyLong_FromLong(src.long_value);
			}
	};

	template <> struct type_caster<glm::mat4> {
		public:

			PYBIND11_TYPE_CASTER(glm::mat4, const_name("mat4"));

			/**
			 * Conversion part 1 (Python->C++): convert a PyObject into a inty
			 * instance or return false upon failure. The second argument
			 * indicates whether implicit conversions should be applied.
			 */
			bool load(handle src, bool) {
				/* Extract PyObject from handle */
				//PyObject *source = src.ptr();
				auto tuple = src.cast<pybind11::tuple>();
				value[0][0] = tuple[0].cast<float>();
				value[0][1] = tuple[1].cast<float>();
				value[0][2] = tuple[2].cast<float>();
				value[0][3] = tuple[3].cast<float>();

				value[1][0] = tuple[4].cast<float>();
				value[1][1] = tuple[5].cast<float>();
				value[1][2] = tuple[6].cast<float>();
				value[1][3] = tuple[7].cast<float>();

				value[2][0] = tuple[8].cast<float>();
				value[2][1] = tuple[9].cast<float>();
				value[2][2] = tuple[10].cast<float>();
				value[2][3] = tuple[11].cast<float>();

				value[3][0] = tuple[12].cast<float>();
				value[3][1] = tuple[13].cast<float>();
				value[3][2] = tuple[14].cast<float>();
				value[3][3] = tuple[15].cast<float>();

				/* Ensure return code was OK (to avoid out-of-range errors etc) */
				return true;
			}

			/**
			 * Conversion part 2 (C++ -> Python): convert an inty instance into
			 * a Python object. The second and third arguments are used to
			 * indicate the return value policy and parent object (for
			 * ``return_value_policy::reference_internal``) and are generally
			 * ignored by implicit casters.
			 */
			static handle cast(glm::mat4 src, return_value_policy /* policy */, handle /* parent */) {
				return PyTuple_Pack(16, src[0][0], src[0][1], src[0][2], src[0][3], src[1][0],src[1][1], src[1][2],
									src[1][3], src[2][0], src[2][1], src[2][2], src[2][3],
									src[3][0], src[3][1], src[3][2], src[3][3]);// PyLong_FromLong(src.long_value);
			}
	};

}} // namespace PYBIND11_NAMESPACE::detail

template<typename T>
T py_get(const pybind11::handle& obj, const std::string& key) {
	return obj.attr(key.c_str()).cast<T>();
}

template <typename T>
T py_get_dict(const pybind11::handle& obj, const std::string& key) {
	auto ox = obj[key.c_str()];
	auto value = py::cast<T>(ox);
	return obj[key.c_str()].cast<T>();
}


template<typename T>
T py_get(const pybind11::handle& obj, const std::string& key, T defaultValue) {
	try {
		auto value = py_get<T>(obj, key);
		return value;
	} catch (...) {
		return defaultValue;
	}
}

template<typename T>
T py_get_dict(const pybind11::handle& obj, const std::string& key, T defaultValue) {
	try {
		auto value = py_get_dict<T>(obj, key);
		return value;
	} catch (...) {
		return defaultValue;
	}
}



