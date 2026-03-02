#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdint.h>

void FnTimeFormatter(double milliseconds, char *result, size_t buf_size)
{
    int64_t total_ms = (int64_t)milliseconds;

    const int64_t SECOND = 1000;
    const int64_t MINUTE = 60 * SECOND;
    const int64_t HOUR = 60 * MINUTE;
    const int64_t DAY = 24 * HOUR;

    if (total_ms >= DAY)
    {
        snprintf(result, buf_size, "%lldd", (long long)(total_ms / DAY));
    }
    else if (total_ms >= HOUR)
    {
        snprintf(result, buf_size, "%lldh", (long long)(total_ms / HOUR));
    }
    else if (total_ms >= MINUTE)
    {
        snprintf(result, buf_size, "%lldm", (long long)(total_ms / MINUTE));
    }
    else if (total_ms >= SECOND)
    {
        snprintf(result, buf_size, "%llds", (long long)(total_ms / SECOND));
    }
    else
    {
        snprintf(result, buf_size, "%lldms", (long long)total_ms);
    }
}

static PyObject *py_FnTimeFormatter(PyObject *self, PyObject *args)
{
    double milliseconds;
    if (!PyArg_ParseTuple(args, "d", &milliseconds))
        return NULL;

    char result[32]; // Sufficiently large for any 64-bit int + suffix
    FnTimeFormatter(milliseconds, result, sizeof(result));

    return PyUnicode_FromString(result);
}

void FnHumanBytes(double size, char *result, size_t buf_size)
{
    if (size <= 0)
    {
        snprintf(result, buf_size, "0 B");
        return;
    }

    const char *units[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB"};
    int n = 0;
    int max_units = (sizeof(units) / sizeof(units[0])) - 1;

    while (size >= 1024.0 && n < max_units)
    {
        size /= 1024.0;
        n++;
    }

    // Format: Integers for bytes, 2 decimal places for others
    if (n == 0)
    {
        snprintf(result, buf_size, "%.0f %s", size, units[n]);
    }
    else
    {
        snprintf(result, buf_size, "%.2f %s", size, units[n]);
    }
}

// Py
static PyObject *py_FnHumanBytes(PyObject *self, PyObject *args)
{
    double size;
    if (!PyArg_ParseTuple(args, "d", &size))
        return NULL;

    char result[64];
    FnHumanBytes(size, result, sizeof(result));

    return PyUnicode_FromString(result);
}
static PyMethodDef HumansMethods[] = {
    {"human_time", py_FnTimeFormatter, METH_VARARGS, "Format milliseconds to the largest relevant unit."},
    {"human_bytes", py_FnHumanBytes, METH_VARARGS, "Convert bytes to human readable KiB/MiB/etc."},
    {NULL, NULL, 0, NULL}};
static struct PyModuleDef humansmodule = {
    PyModuleDef_HEAD_INIT,
    "humans",
    "A module for human-readable data formatting.",
    -1,
    HumansMethods};

PyMODINIT_FUNC PyInit_humans(void)
{
    return PyModule_Create(&humansmodule);
}