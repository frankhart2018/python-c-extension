#include <Python.h>

// Custom exception
static PyObject *StringTooShortError = NULL;

static PyObject *method_fputs(PyObject *self, PyObject *args) {
    char *str, *filename = NULL;
    int bytes_copied = -1;

    // Parse arguments
    if(!PyArg_ParseTuple(args, "ss", &str, &filename)) {
        return NULL;
    }

    // // If string has length less than 10 then throw ValueError
    // if(strlen(str) < 10) {
    //     PyErr_SetString(PyExc_ValueError, "String must be greater than 10!");
    //     return NULL;
    // }

    if(strlen(str) < 10) {
        // Passing custom exception
        PyErr_SetString(StringTooShortError, "String length must be greater than 10!");
        return NULL;
    }

    FILE *fp = fopen(filename, "w");
    bytes_copied = fputs(str, fp);
    fclose(fp);

    return PyLong_FromLong(bytes_copied);
}

static PyMethodDef FputsMethods[] = {
    {"fputs", method_fputs, METH_VARARGS, "Python interface for fputs C library function"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef fputsmodule = {
    PyModuleDef_HEAD_INIT,
    "fputs",
    "Python interface for fputs C library function",
    -1,
    FputsMethods
};

PyMODINIT_FUNC PyInit_fputs(void) {
    // Assign module value
    PyObject* module = PyModule_Create(&fputsmodule);

    // Initialize new exception object
    StringTooShortError = PyErr_NewException("fputs.StringTooShortError", NULL, NULL);

    // Add exception object to module
    PyModule_AddObject(module, "StringTooShortError", StringTooShortError);

    // Add int constant by name - Method 1 to define constant
    PyModule_AddIntConstant(module, "FPUTS_FLAG", 64);

    // Define int macro - Method 2 to define constant
    #define FPUTS_MACRO 256

    // Add macro to module
    PyModule_AddIntMacro(module, FPUTS_MACRO);

    return module;
}

/*
PyObject - This is an object structure that is used to define object types in Python. All other object types are derived from
this. PyObject tells the python interpreter to treat a pointer to an object as a n object. 

Setting the return type of the above function as PyObject defines the common fields that are required by the Python interpreter
in order to recognize this as valid Python type.

-------------------------------------------------------------------------------------------------------------------------------

PyArg_ParseTuple() - Parses the arguments we will receive from our python program into local variables.

ss is the format specifier that specifies the data type of the arguments to parse.

&str and &filename are pointers to local variables which the parsed values will be assigned.

PyArg_ParseTuple() evaluates to false on failure, if it fails the function will return NULL.

-------------------------------------------------------------------------------------------------------------------------------

PyLong_FromLong() - Returns a PyLongObject which represents an integer object in python.

-------------------------------------------------------------------------------------------------------------------------------

PyMethodDef - Used to tell the python interpreter about the methods defined. Since, there can be more than one module so
this is an array of PyMethodDef structs.

METH_VARARGS is a flag that tells the interpreter that the function will accept two arguments of type PyObject* - self (is 
the module object), and args (tuple containing the actual parameters for the function).

The final string is a value to represent the method docstring.

-------------------------------------------------------------------------------------------------------------------------------

PyModuleDef - Holds information about our module itself. It is a single structure. In total there are 9 members, but not all
are required.

PyModuleDef_HEAD_INIT - Member of type PyModuleDef_Base which is advised to have just this one value.

The string is for module's docstring. We can set it to NULL. If passed a const char* it is of type Py_ssize_t. We can also use
PyDoc_STRVAR() to define a docstring for our module.

-1 is the amount of memory needed to store program state. It can have the following values:-
    1) A negative value - Indicates that this module doesn't have support for sub interpreters.
    2) A non-negative value - Enables the re-initialization of module. It also specifies the memory requirement of our module
    to be allocated on each sub-interpreter session.

-------------------------------------------------------------------------------------------------------------------------------

PyMODINIT_FUNC - It does three things implicitly when stated as function return type:-
    1) It implicitly sets the return type of the function as PyObject*.
    2) It declares any special linkages.
    3) It declares the function as extern "C". In case we are using C++, it tells the C++ compiler not to do name-mangling
    on the symbols.

-------------------------------------------------------------------------------------------------------------------------------

PyModule_Create() - Returns a new module object of type PyObject*, the address of the argument is passed as an argument.

*/