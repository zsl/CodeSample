

#ifndef PY_CALLBACK__
#define PY_CALLBACK__

#include "consoleutil.h"
#include <boost/python.hpp>

namespace bpy = boost::python;

class PyCallback{
public:

    PyCallback(PyObject *obj){ m_funcObj = obj; }

    bool operator()(CmdReader &reader)
    {
        return bpy::call<bool>(m_funcObj, reader);
    }
private:
    PyObject *m_funcObj;
};

#endif
