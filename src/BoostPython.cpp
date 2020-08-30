/*
 * Python Binding Wrapper
 *
*/
#define BOOST_PYTHON_STATIC_LIB
#define BOOST_NUMPY_STATIC_LIB

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace bp = boost::python;
namespace np = boost::python::numpy;

class MyClass
{
public:
	void set(std::string msg) { this->msg = msg; }
	std::string greet() { return msg; }
	std::string msg;
};

/* Expose module to Python */
BOOST_PYTHON_MODULE(pyBoostPython)
{
	bp::class_<MyClass>("MyClass")
		.def("greet", &MyClass::greet)
		.def("set", &MyClass::set)
		;
}