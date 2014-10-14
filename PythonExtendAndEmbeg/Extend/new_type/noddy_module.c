#include <Python.h>
#include <structmember.h>

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
    PyObject *first;
    PyObject *last;
    int number;
} Noddy;

static void Noddy_dealloc(Noddy *self)
{
    Py_XDECREF(self->first);
    Py_XDECREF(self->last);
    self->ob_type->tp_free( (PyObject *)self );
}

static PyObject *Noddy_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Noddy *self;
    self = (Noddy *)type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->first = PyString_FromString("");
        if (self->first == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }

        self->last = PyString_FromString("");
        if (self->last == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }

        self->number = 0;
    }
    return (PyObject *)self;
}


static int Noddy_init(Noddy *self, PyObject *args, PyObject *kwds)
{
    PyObject *first = NULL, *last = NULL, *tmp;
    
    static char *kwlist[] = {"first", "last", "number", NULL};
    if ( ! PyArg_ParseTupleAndKeywords(args, kwds, "|SSi", kwlist, &first, &last, &last, &self->number) )
        return -1;

    if (first)
    {
        tmp = self->first;
        Py_INCREF(first);
        self->first = first;
        Py_XDECREF(tmp);
    }

    if (last)
    {
        tmp = self->last;
        Py_INCREF(last);
        self->last = last;
        Py_XDECREF(tmp);
    }

    return 0;
}

static PyObject *Noddy_getfirst(Noddy *self, void *closure)
{
    Py_INCREF(self->first);
    return self->first;
}

static int Noddy_setfirst(Noddy *self, PyObject *value, void *closure)
{
    if (NULL == value){
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }
    
    if (! PyString_Check(value) ){
        PyErr_SetString(PyExc_TypeError, "The first attribute value must be a string");
        return -1;
    }

    Py_DECREF(self->first);
    Py_INCREF(value);
    self->first = value;

    return 0;
}

static PyObject *Noddy_getlast(Noddy *self, void *closure)
{
    Py_INCREF(self->last);
    return self->last;
}

static int Noddy_setlast(Noddy *self, PyObject *value, void *closure)
{
    if (NULL == value){
        PyErr_SetString(PyExc_TypeError, "Cannot delete the last attribute");
        return -1;
    }

    if (! PyString_Check(value) ){
        PyErr_SetString(PyExc_TypeError, "The last attribute value must be a string");
        return -1;
    }

    Py_DECREF(self->last);
    Py_INCREF(value);
    self->last = value;

    return 0;
}

static PyObject *Noddy_name(Noddy *self)
{
    static PyObject *format = NULL;
    PyObject *args, *result;

    if (format == NULL){
        format = PyString_FromString("%s %s");
        if (format == NULL)
            return NULL;
    }

    if (self->first == NULL){
        PyErr_SetString(PyExc_AttributeError, "first");
        return NULL;
    }
    
    if (self->last == NULL){
        PyErr_SetString(PyExc_AttributeError, "last");
        return NULL;
    }

    args = Py_BuildValue("OO", self->first, self->last);
    if (args == NULL){
        return NULL;
    }

    result = PyString_Format(format, args);
    Py_DECREF(args);
    Py_DECREF(format);
    
    return result;
}

static int Noddy_traverse(Noddy *self, visitproc visit, void *arg)
{
    Py_VISIT(self->first);
    Py_VISIT(self->last);

    return 0;
}

static int Noddy_clear(Noddy *self)
{
    Py_CLEAR(self->first);
    Py_CLEAR(self->last);

    return 0;
}

static PyGetSetDef Noddy_getseters[] = {
    {"first", (getter)Noddy_getfirst, (setter)Noddy_setfirst, "first name", NULL},
    {"last", (getter)Noddy_getlast, (setter)Noddy_setlast, "last name", NULL},
    {NULL}
};

static PyMethodDef Noddy_methods[] = {
    {"name", (PyCFunction)Noddy_name, METH_NOARGS, 
        "Return the name, combining the first and last name"},
    {NULL}
};

static PyMemberDef Noddy_members[] = {
    {"first", T_OBJECT_EX, offsetof(Noddy, first), 0, "first name"},
    {"last", T_OBJECT_EX, offsetof(Noddy, last), 0, "last name"},
    {"number", T_INT, offsetof(Noddy, number), 0, "noddy number"},
    {NULL}  /* Sentinel */
};

static PyTypeObject noddy_NoddyType = {
PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "noddy.Noddy",             /*tp_name*/
    sizeof(Noddy),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)Noddy_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC, /*tp_flags*/
    "Noddy objects",           /* tp_doc */
    (traverseproc)Noddy_traverse,   /* tp_traverse */
    (inquiry)Noddy_clear,           /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    Noddy_methods,             /* tp_methods */
    Noddy_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Noddy_init,      /* tp_init */
    0,                         /* tp_alloc */
    Noddy_new,                 /* tp_new */
    
};

static PyMethodDef module_methods[] = {
    (NULL)   /* sentinel */
};

/* #ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif*/
PyMODINIT_FUNC
initnoddy(void) 
{
    PyObject* m;

    /* noddy_NoddyType.tp_new = PyType_GenericNew; */
    if (PyType_Ready(&noddy_NoddyType) < 0)
        return;

    m = Py_InitModule3("noddy", module_methods,
                       "Example module that creates an extension type.");

    if (NULL == m){
        return ;
    }

    Py_INCREF(&noddy_NoddyType);
    PyModule_AddObject(m, "Noddy", (PyObject *)&noddy_NoddyType);
}

