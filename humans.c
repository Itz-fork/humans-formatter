#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>


// func def: Time formatter
void FnTimeFormatter(
    int64_t milliseconds,
    int round_value,
    int compound,
    char *result,
    size_t buf_size)
{
    const int64_t SECOND = 1000;
    const int64_t MINUTE = 60 * SECOND;
    const int64_t HOUR   = 60 * MINUTE;
    const int64_t DAY    = 24 * HOUR;

    int negative = milliseconds < 0;
    uint64_t total_ms = negative ? (uint64_t)(-milliseconds) : (uint64_t)milliseconds;

    char sign[2] = "";
    if (negative)
        sign[0] = '-', sign[1] = '\0';

    if (compound)
    {
        uint64_t days    = total_ms / DAY;
        total_ms        %= DAY;

        uint64_t hours   = total_ms / HOUR;
        total_ms        %= HOUR;

        uint64_t minutes = total_ms / MINUTE;
        total_ms        %= MINUTE;

        uint64_t seconds = total_ms / SECOND;
        total_ms        %= SECOND;

        if (days > 0)
            snprintf(result, buf_size, "%s%" PRIu64 "d %" PRIu64 "h %" PRIu64 "m %" PRIu64 "s",
                     sign, days, hours, minutes, seconds);
        else if (hours > 0)
            snprintf(result, buf_size, "%s%" PRIu64 "h %" PRIu64 "m %" PRIu64 "s",
                     sign, hours, minutes, seconds);
        else if (minutes > 0)
            snprintf(result, buf_size, "%s%" PRIu64 "m %" PRIu64 "s",
                     sign, minutes, seconds);
        else if (seconds > 0)
            snprintf(result, buf_size, "%s%" PRIu64 "s",
                     sign, seconds);
        else
            snprintf(result, buf_size, "%s%" PRIu64 "ms",
                     sign, total_ms);

        return;
    }

    // Largest-unit mode
    uint64_t value = total_ms;
    const char *suffix = "ms";
    uint64_t divisor = 1;

    if (total_ms >= DAY)
        divisor = DAY, suffix = "d";
    else if (total_ms >= HOUR)
        divisor = HOUR, suffix = "h";
    else if (total_ms >= MINUTE)
        divisor = MINUTE, suffix = "m";
    else if (total_ms >= SECOND)
        divisor = SECOND, suffix = "s";

    if (round_value && divisor > 1)
        value = (total_ms + divisor / 2) / divisor;
    else
        value = total_ms / divisor;

    snprintf(result, buf_size, "%s%" PRIu64 "%s", sign, value, suffix);
}


static PyObject *py_FnTimeFormatter(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"milliseconds", "round", "compound", NULL};

    long long milliseconds;
    int round_value = 0;
    int compound = 0;

    if (!PyArg_ParseTupleAndKeywords(
            args,
            kwargs,
            "L|pp",
            kwlist,
            &milliseconds,
            &round_value,
            &compound))
        return NULL;

    char result[128];
    FnTimeFormatter((int64_t)milliseconds, round_value, compound, result, sizeof(result));

    return PyUnicode_FromString(result);
}


// func def: HumanBytes
void FnHumanBytes(int64_t size, char *result, size_t buf_size)
{
    const char *units[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB"};
    int max_units = (sizeof(units) / sizeof(units[0])) - 1;

    int negative = size < 0;
    uint64_t abs_size = negative ? (uint64_t)(-size) : (uint64_t)size;

    double value = (double)abs_size;
    int unit_index = 0;

    while (value >= 1024.0 && unit_index < max_units)
    {
        value /= 1024.0;
        unit_index++;
    }

    char sign[2] = "";
    if (negative)
        sign[0] = '-', sign[1] = '\0';

    if (unit_index == 0)
        snprintf(result, buf_size, "%s%.0f %s", sign, value, units[unit_index]);
    else
        snprintf(result, buf_size, "%s%.2f %s", sign, value, units[unit_index]);
}

// Wrapper
static PyObject *py_FnHumanBytes(PyObject *self, PyObject *args)
{
    long long size;

    if (!PyArg_ParseTuple(args, "L", &size))
        return NULL;

    char result[64];
    FnHumanBytes((int64_t)size, result, sizeof(result));

    return PyUnicode_FromString(result);
}


// Module def
static PyMethodDef HumansMethods[] = {
    {"human_time",
     (PyCFunction)py_FnTimeFormatter,
     METH_VARARGS | METH_KEYWORDS,
     "human_time(milliseconds: int, round=False, compound=False)\n"
     "Format milliseconds into human readable form."},

    {"human_bytes",
     py_FnHumanBytes,
     METH_VARARGS,
     "human_bytes(size: int)\n"
     "Convert byte count into human readable KiB/MiB/etc."},

    {NULL, NULL, 0, NULL}};

static struct PyModuleDef humansmodule = {
    PyModuleDef_HEAD_INIT,
    "humans",
    "Utility to convert time and bytes to human readable format",
    -1,
    HumansMethods};

PyMODINIT_FUNC PyInit_humans(void)
{
    return PyModule_Create(&humansmodule);
}



// I had to vibe code this a bit bruv i got absolutely roasted by chatgpt for all the flaws in my c code