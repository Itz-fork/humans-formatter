// written by a professional
// please do not attempt to recreate this at home
// please don't laugh
#include <Python.h>
#include <stdio.h>

// Convert milliseconds in to string that can be understood by hoomans
void FnTimeFormatter(double milliseconds, char *result)
{
    int seconds = (int)milliseconds / 1000;
    milliseconds = seconds ? (int)milliseconds % 1000 : milliseconds;
    int minutes = seconds / 60;
    seconds %= 60;
    int hours = minutes / 60;
    minutes %= 60;
    int days = hours / 24;
    hours %= 24;

    if (minutes > 0)
    {
        sprintf(result, "%dm", minutes);
    }
    else if (seconds > 0)
    {
        sprintf(result, "%ds", seconds);
    }
    else if (hours > 0)
    {
        sprintf(result, "%dh", hours);
    }
    else if (days > 0)
    {
        sprintf(result, "%dd", days);
    }
    else
    {
        sprintf(result, "%dms", (int)milliseconds);
    }
}

static PyObject *py_FnTimeFormatter(PyObject *self, PyObject *args)
{
    double milliseconds;
    if (!PyArg_ParseTuple(args, "d", &milliseconds))
        return NULL;
    char result[20];
    FnTimeFormatter(milliseconds, result);
    return Py_BuildValue("s", result);
}

// Convert bytes (as in 1024) to string that can be understood by hoomans
char *FnHumanBytes(double size)
{
    if (size == 0)
    {
        return "";
    }
    int power = 1024;
    int n = 0;
    const char *pwrN[] = {" ", "Ki", "Mi", "Gi", "Ti"};
    while (size > power)
    {
        size /= power;
        n += 1;
    }
    static char result[20];
    sprintf(result, "%.2f %sB", size, pwrN[n]);
    return result;
}

static PyObject *py_FnHumanBytes(PyObject *self, PyObject *args)
{
    double size;
    if (!PyArg_ParseTuple(args, "d", &size))
        return NULL;
    char *result = FnHumanBytes(size);
    return Py_BuildValue("s", result);
}

// humans module
static PyMethodDef HumansMethods[] = {
    {"human_time", py_FnTimeFormatter, METH_VARARGS, "Format time in milliseconds to human readable format"},
    {"human_bytes", py_FnHumanBytes, METH_VARARGS, "Convert bytes to human readable format"},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef humansmodule = {
    PyModuleDef_HEAD_INIT, "humans", NULL, -1, HumansMethods};

PyMODINIT_FUNC PyInit_humans(void)
{
    return PyModule_Create(&humansmodule);
}