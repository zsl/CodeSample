#include <python.h>

static PyObject *spam_system(PyObject *self, PyObject *args){
	const char *command;
	int sts;

	if (!PyArg_ParseTuple(args, "s", &command))
		return NULL;
	sts = system(command);
	return Py_BuildValue("i", sts);
}


/*********************** Install Module **************************/

/* No functions in array module. */
static PyMethodDef a_methods[] = {
	{"system", spam_system, METH_VARARGS, "Execute a shell command"}  ,
	{NULL, NULL, 0, NULL}/* Sentinel */
};


PyMODINIT_FUNC
	initspam(void)
{
	(void) Py_InitModule("spam", a_methods);
	/* No need to check the error here, the caller will do that */
}