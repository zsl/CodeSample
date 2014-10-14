#include <python.h>

extern float volumecone(float radius, float height);
extern float rawprint(char *string);

static PyObject *testex_volumecone(PyObject *self, PyObject *args)
{
    float volume, radius, height;
    if ( !PyArg_ParseTuple(args, "ff", &radius, &height) )
    {
        PyErr_BadArgument();
        return NULL;
    }

    volume = volumecone(radius, height);
    return Py_BuildValue("f", volume);
}

static PyObject *testex_rawprint(PyObject *self, PyObject *args)
{
    char *string;
    if ( !PyArg_ParseTuple(args, "s", &string) )
    {
        PyErr_BadArgument();
        return NULL;
    }

    rawprint(string);

    Py_INCREF(Py_None);
    
    return Py_None;
}

static PyMethodDef testexmethods[] = {
    {
        "volumecone", testex_volumecone, METH_VARARGS, 
        "compute the volume of a cone given the radius and height."
    },
    {
        "rawprint", testex_rawprint, METH_VARARGS, "print a raw string"
    },
    {
        NULL, NULL
    },
};


void inittestex(void)
{
    Py_InitModule("testex", testexmethods);
}
