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


char const* greet()
{
	return "hello, world";
}

BOOST_PYTHON_MODULE(BoostPython_ext)
{
	Py_Initialize();
	np::initialize();
	
	// Write the bindings here
	bp::def("greet", greet);
}