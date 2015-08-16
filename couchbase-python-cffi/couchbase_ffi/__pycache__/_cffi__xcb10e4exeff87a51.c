
#include <Python.h>
#include <stddef.h>

/* this block of #ifs should be kept exactly identical between
   c/_cffi_backend.c, cffi/vengine_cpy.py, cffi/vengine_gen.py */
#if defined(_MSC_VER)
# include <malloc.h>   /* for alloca() */
# if _MSC_VER < 1600   /* MSVC < 2010 */
   typedef __int8 int8_t;
   typedef __int16 int16_t;
   typedef __int32 int32_t;
   typedef __int64 int64_t;
   typedef unsigned __int8 uint8_t;
   typedef unsigned __int16 uint16_t;
   typedef unsigned __int32 uint32_t;
   typedef unsigned __int64 uint64_t;
   typedef __int8 int_least8_t;
   typedef __int16 int_least16_t;
   typedef __int32 int_least32_t;
   typedef __int64 int_least64_t;
   typedef unsigned __int8 uint_least8_t;
   typedef unsigned __int16 uint_least16_t;
   typedef unsigned __int32 uint_least32_t;
   typedef unsigned __int64 uint_least64_t;
   typedef __int8 int_fast8_t;
   typedef __int16 int_fast16_t;
   typedef __int32 int_fast32_t;
   typedef __int64 int_fast64_t;
   typedef unsigned __int8 uint_fast8_t;
   typedef unsigned __int16 uint_fast16_t;
   typedef unsigned __int32 uint_fast32_t;
   typedef unsigned __int64 uint_fast64_t;
   typedef __int64 intmax_t;
   typedef unsigned __int64 uintmax_t;
# else
#  include <stdint.h>
# endif
# if _MSC_VER < 1800   /* MSVC < 2013 */
   typedef unsigned char _Bool;
# endif
#else
# include <stdint.h>
# if (defined (__SVR4) && defined (__sun)) || defined(_AIX)
#  include <alloca.h>
# endif
#endif

#if PY_MAJOR_VERSION < 3
# undef PyCapsule_CheckExact
# undef PyCapsule_GetPointer
# define PyCapsule_CheckExact(capsule) (PyCObject_Check(capsule))
# define PyCapsule_GetPointer(capsule, name) \
    (PyCObject_AsVoidPtr(capsule))
#endif

#if PY_MAJOR_VERSION >= 3
# define PyInt_FromLong PyLong_FromLong
#endif

#define _cffi_from_c_double PyFloat_FromDouble
#define _cffi_from_c_float PyFloat_FromDouble
#define _cffi_from_c_long PyInt_FromLong
#define _cffi_from_c_ulong PyLong_FromUnsignedLong
#define _cffi_from_c_longlong PyLong_FromLongLong
#define _cffi_from_c_ulonglong PyLong_FromUnsignedLongLong

#define _cffi_to_c_double PyFloat_AsDouble
#define _cffi_to_c_float PyFloat_AsDouble

#define _cffi_from_c_int_const(x)                                        \
    (((x) > 0) ?                                                         \
        ((unsigned long long)(x) <= (unsigned long long)LONG_MAX) ?      \
            PyInt_FromLong((long)(x)) :                                  \
            PyLong_FromUnsignedLongLong((unsigned long long)(x)) :       \
        ((long long)(x) >= (long long)LONG_MIN) ?                        \
            PyInt_FromLong((long)(x)) :                                  \
            PyLong_FromLongLong((long long)(x)))

#define _cffi_from_c_int(x, type)                                        \
    (((type)-1) > 0 ? /* unsigned */                                     \
        (sizeof(type) < sizeof(long) ?                                   \
            PyInt_FromLong((long)x) :                                    \
         sizeof(type) == sizeof(long) ?                                  \
            PyLong_FromUnsignedLong((unsigned long)x) :                  \
            PyLong_FromUnsignedLongLong((unsigned long long)x)) :        \
        (sizeof(type) <= sizeof(long) ?                                  \
            PyInt_FromLong((long)x) :                                    \
            PyLong_FromLongLong((long long)x)))

#define _cffi_to_c_int(o, type)                                          \
    ((type)(                                                             \
     sizeof(type) == 1 ? (((type)-1) > 0 ? (type)_cffi_to_c_u8(o)        \
                                         : (type)_cffi_to_c_i8(o)) :     \
     sizeof(type) == 2 ? (((type)-1) > 0 ? (type)_cffi_to_c_u16(o)       \
                                         : (type)_cffi_to_c_i16(o)) :    \
     sizeof(type) == 4 ? (((type)-1) > 0 ? (type)_cffi_to_c_u32(o)       \
                                         : (type)_cffi_to_c_i32(o)) :    \
     sizeof(type) == 8 ? (((type)-1) > 0 ? (type)_cffi_to_c_u64(o)       \
                                         : (type)_cffi_to_c_i64(o)) :    \
     (Py_FatalError("unsupported size for type " #type), (type)0)))

#define _cffi_to_c_i8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[1])
#define _cffi_to_c_u8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[2])
#define _cffi_to_c_i16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[3])
#define _cffi_to_c_u16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[4])
#define _cffi_to_c_i32                                                   \
                 ((int(*)(PyObject *))_cffi_exports[5])
#define _cffi_to_c_u32                                                   \
                 ((unsigned int(*)(PyObject *))_cffi_exports[6])
#define _cffi_to_c_i64                                                   \
                 ((long long(*)(PyObject *))_cffi_exports[7])
#define _cffi_to_c_u64                                                   \
                 ((unsigned long long(*)(PyObject *))_cffi_exports[8])
#define _cffi_to_c_char                                                  \
                 ((int(*)(PyObject *))_cffi_exports[9])
#define _cffi_from_c_pointer                                             \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[10])
#define _cffi_to_c_pointer                                               \
    ((char *(*)(PyObject *, CTypeDescrObject *))_cffi_exports[11])
#define _cffi_get_struct_layout                                          \
    ((PyObject *(*)(Py_ssize_t[]))_cffi_exports[12])
#define _cffi_restore_errno                                              \
    ((void(*)(void))_cffi_exports[13])
#define _cffi_save_errno                                                 \
    ((void(*)(void))_cffi_exports[14])
#define _cffi_from_c_char                                                \
    ((PyObject *(*)(char))_cffi_exports[15])
#define _cffi_from_c_deref                                               \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[16])
#define _cffi_to_c                                                       \
    ((int(*)(char *, CTypeDescrObject *, PyObject *))_cffi_exports[17])
#define _cffi_from_c_struct                                              \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[18])
#define _cffi_to_c_wchar_t                                               \
    ((wchar_t(*)(PyObject *))_cffi_exports[19])
#define _cffi_from_c_wchar_t                                             \
    ((PyObject *(*)(wchar_t))_cffi_exports[20])
#define _cffi_to_c_long_double                                           \
    ((long double(*)(PyObject *))_cffi_exports[21])
#define _cffi_to_c__Bool                                                 \
    ((_Bool(*)(PyObject *))_cffi_exports[22])
#define _cffi_prepare_pointer_call_argument                              \
    ((Py_ssize_t(*)(CTypeDescrObject *, PyObject *, char **))_cffi_exports[23])
#define _cffi_convert_array_from_object                                  \
    ((int(*)(char *, CTypeDescrObject *, PyObject *))_cffi_exports[24])
#define _CFFI_NUM_EXPORTS 25

typedef struct _ctypedescr CTypeDescrObject;

static void *_cffi_exports[_CFFI_NUM_EXPORTS];
static PyObject *_cffi_types, *_cffi_VerificationError;

static int _cffi_setup_custom(PyObject *lib);   /* forward */

static PyObject *_cffi_setup(PyObject *self, PyObject *args)
{
    PyObject *library;
    int was_alive = (_cffi_types != NULL);
    (void)self; /* unused */
    if (!PyArg_ParseTuple(args, "OOO", &_cffi_types, &_cffi_VerificationError,
                                       &library))
        return NULL;
    Py_INCREF(_cffi_types);
    Py_INCREF(_cffi_VerificationError);
    if (_cffi_setup_custom(library) < 0)
        return NULL;
    return PyBool_FromLong(was_alive);
}

static int _cffi_init(void)
{
    PyObject *module, *c_api_object = NULL;

    module = PyImport_ImportModule("_cffi_backend");
    if (module == NULL)
        goto failure;

    c_api_object = PyObject_GetAttrString(module, "_C_API");
    if (c_api_object == NULL)
        goto failure;
    if (!PyCapsule_CheckExact(c_api_object)) {
        PyErr_SetNone(PyExc_ImportError);
        goto failure;
    }
    memcpy(_cffi_exports, PyCapsule_GetPointer(c_api_object, "cffi"),
           _CFFI_NUM_EXPORTS * sizeof(void *));

    Py_DECREF(module);
    Py_DECREF(c_api_object);
    return 0;

  failure:
    Py_XDECREF(module);
    Py_XDECREF(c_api_object);
    return -1;
}

#define _cffi_type(num) ((CTypeDescrObject *)PyList_GET_ITEM(_cffi_types, num))

/**********/



#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <libcouchbase/couchbase.h>
#include <libcouchbase/api3.h>
#include <libcouchbase/views.h>
#include <libcouchbase/n1ql.h>

void _Cb_set_key(void *cmd, const void *key, size_t nkey) {
    LCB_CMD_SET_KEY((lcb_CMDBASE*)cmd, key, nkey);
}
void _Cb_set_val(void *cmd, const void *val, size_t nval) {
    LCB_CMD_SET_VALUE((lcb_CMDSTORE*)cmd, val, nval);
}
void _Cb_do_callback(lcb_socket_t s, short events, lcb_ioE_callback cb, void *arg) {
    cb(s, events, arg);
}
LIBCOUCHBASE_API
lcb_error_t
lcb_n1p_synctok_for(lcb_N1QLPARAMS *params, lcb_t instance,
            const void *key, size_t nkey) { return LCB_SUCCESS; }


static void _cffi_check__lcb_ARITHCMDv0(lcb_ARITHCMDv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->exptime) << 1);
  (void)((p->create) << 1);
  (void)((p->delta) << 1);
  (void)((p->initial) << 1);
  { void const * *tmp = &p->hashkey; (void)tmp; }
  (void)((p->nhashkey) << 1);
}
static PyObject *
_cffi_layout__lcb_ARITHCMDv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_ARITHCMDv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_ARITHCMDv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_ARITHCMDv0, key),
    sizeof(((lcb_ARITHCMDv0 *)0)->key),
    offsetof(lcb_ARITHCMDv0, nkey),
    sizeof(((lcb_ARITHCMDv0 *)0)->nkey),
    offsetof(lcb_ARITHCMDv0, exptime),
    sizeof(((lcb_ARITHCMDv0 *)0)->exptime),
    offsetof(lcb_ARITHCMDv0, create),
    sizeof(((lcb_ARITHCMDv0 *)0)->create),
    offsetof(lcb_ARITHCMDv0, delta),
    sizeof(((lcb_ARITHCMDv0 *)0)->delta),
    offsetof(lcb_ARITHCMDv0, initial),
    sizeof(((lcb_ARITHCMDv0 *)0)->initial),
    offsetof(lcb_ARITHCMDv0, hashkey),
    sizeof(((lcb_ARITHCMDv0 *)0)->hashkey),
    offsetof(lcb_ARITHCMDv0, nhashkey),
    sizeof(((lcb_ARITHCMDv0 *)0)->nhashkey),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_ARITHCMDv0(0);
}

static void _cffi_check__lcb_ARITHRESPv0(lcb_ARITHRESPv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->value) << 1);
  (void)((p->cas) << 1);
  { lcb_SYNCTOKEN const * *tmp = &p->synctoken; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_ARITHRESPv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_ARITHRESPv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_ARITHRESPv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_ARITHRESPv0, key),
    sizeof(((lcb_ARITHRESPv0 *)0)->key),
    offsetof(lcb_ARITHRESPv0, nkey),
    sizeof(((lcb_ARITHRESPv0 *)0)->nkey),
    offsetof(lcb_ARITHRESPv0, value),
    sizeof(((lcb_ARITHRESPv0 *)0)->value),
    offsetof(lcb_ARITHRESPv0, cas),
    sizeof(((lcb_ARITHRESPv0 *)0)->cas),
    offsetof(lcb_ARITHRESPv0, synctoken),
    sizeof(((lcb_ARITHRESPv0 *)0)->synctoken),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_ARITHRESPv0(0);
}

static int _cffi_e__lcb_CALLBACKTYPE(PyObject *lib)
{
  if ((LCB_CALLBACK_DEFAULT) > 0 || (long)(LCB_CALLBACK_DEFAULT) != 0L) {
    char buf[64];
    if ((LCB_CALLBACK_DEFAULT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_DEFAULT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_DEFAULT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_DEFAULT", buf, "0");
    return -1;
  }
  if ((LCB_CALLBACK_GET) <= 0 || (unsigned long)(LCB_CALLBACK_GET) != 1UL) {
    char buf[64];
    if ((LCB_CALLBACK_GET) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_GET));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_GET));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_GET", buf, "1");
    return -1;
  }
  if ((LCB_CALLBACK_STORE) <= 0 || (unsigned long)(LCB_CALLBACK_STORE) != 2UL) {
    char buf[64];
    if ((LCB_CALLBACK_STORE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_STORE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_STORE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_STORE", buf, "2");
    return -1;
  }
  if ((LCB_CALLBACK_COUNTER) <= 0 || (unsigned long)(LCB_CALLBACK_COUNTER) != 3UL) {
    char buf[64];
    if ((LCB_CALLBACK_COUNTER) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_COUNTER));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_COUNTER));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_COUNTER", buf, "3");
    return -1;
  }
  if ((LCB_CALLBACK_TOUCH) <= 0 || (unsigned long)(LCB_CALLBACK_TOUCH) != 4UL) {
    char buf[64];
    if ((LCB_CALLBACK_TOUCH) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_TOUCH));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_TOUCH));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_TOUCH", buf, "4");
    return -1;
  }
  if ((LCB_CALLBACK_REMOVE) <= 0 || (unsigned long)(LCB_CALLBACK_REMOVE) != 5UL) {
    char buf[64];
    if ((LCB_CALLBACK_REMOVE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_REMOVE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_REMOVE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_REMOVE", buf, "5");
    return -1;
  }
  if ((LCB_CALLBACK_UNLOCK) <= 0 || (unsigned long)(LCB_CALLBACK_UNLOCK) != 6UL) {
    char buf[64];
    if ((LCB_CALLBACK_UNLOCK) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_UNLOCK));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_UNLOCK));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_UNLOCK", buf, "6");
    return -1;
  }
  if ((LCB_CALLBACK_STATS) <= 0 || (unsigned long)(LCB_CALLBACK_STATS) != 7UL) {
    char buf[64];
    if ((LCB_CALLBACK_STATS) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_STATS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_STATS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_STATS", buf, "7");
    return -1;
  }
  if ((LCB_CALLBACK_VERSIONS) <= 0 || (unsigned long)(LCB_CALLBACK_VERSIONS) != 8UL) {
    char buf[64];
    if ((LCB_CALLBACK_VERSIONS) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_VERSIONS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_VERSIONS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_VERSIONS", buf, "8");
    return -1;
  }
  if ((LCB_CALLBACK_VERBOSITY) <= 0 || (unsigned long)(LCB_CALLBACK_VERBOSITY) != 9UL) {
    char buf[64];
    if ((LCB_CALLBACK_VERBOSITY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_VERBOSITY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_VERBOSITY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_VERBOSITY", buf, "9");
    return -1;
  }
  if ((LCB_CALLBACK_FLUSH) <= 0 || (unsigned long)(LCB_CALLBACK_FLUSH) != 10UL) {
    char buf[64];
    if ((LCB_CALLBACK_FLUSH) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_FLUSH));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_FLUSH));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_FLUSH", buf, "10");
    return -1;
  }
  if ((LCB_CALLBACK_OBSERVE) <= 0 || (unsigned long)(LCB_CALLBACK_OBSERVE) != 11UL) {
    char buf[64];
    if ((LCB_CALLBACK_OBSERVE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_OBSERVE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_OBSERVE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_OBSERVE", buf, "11");
    return -1;
  }
  if ((LCB_CALLBACK_GETREPLICA) <= 0 || (unsigned long)(LCB_CALLBACK_GETREPLICA) != 12UL) {
    char buf[64];
    if ((LCB_CALLBACK_GETREPLICA) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_GETREPLICA));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_GETREPLICA));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_GETREPLICA", buf, "12");
    return -1;
  }
  if ((LCB_CALLBACK_ENDURE) <= 0 || (unsigned long)(LCB_CALLBACK_ENDURE) != 13UL) {
    char buf[64];
    if ((LCB_CALLBACK_ENDURE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_ENDURE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_ENDURE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_ENDURE", buf, "13");
    return -1;
  }
  if ((LCB_CALLBACK_HTTP) <= 0 || (unsigned long)(LCB_CALLBACK_HTTP) != 14UL) {
    char buf[64];
    if ((LCB_CALLBACK_HTTP) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_HTTP));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_HTTP));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_HTTP", buf, "14");
    return -1;
  }
  if ((LCB_CALLBACK_CBFLUSH) <= 0 || (unsigned long)(LCB_CALLBACK_CBFLUSH) != 15UL) {
    char buf[64];
    if ((LCB_CALLBACK_CBFLUSH) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_CBFLUSH));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_CBFLUSH));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_CBFLUSH", buf, "15");
    return -1;
  }
  if ((LCB_CALLBACK_OBSEQNO) <= 0 || (unsigned long)(LCB_CALLBACK_OBSEQNO) != 16UL) {
    char buf[64];
    if ((LCB_CALLBACK_OBSEQNO) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK_OBSEQNO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK_OBSEQNO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK_OBSEQNO", buf, "16");
    return -1;
  }
  if ((LCB_CALLBACK__MAX) <= 0 || (unsigned long)(LCB_CALLBACK__MAX) != 17UL) {
    char buf[64];
    if ((LCB_CALLBACK__MAX) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CALLBACK__MAX));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CALLBACK__MAX));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_CALLBACKTYPE: ", "LCB_CALLBACK__MAX", buf, "17");
    return -1;
  }
  return ((void)lib,0);
}

static void _cffi_check__lcb_CMDCOUNTER(lcb_CMDCOUNTER *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->cmdflags) << 1);
  (void)((p->exptime) << 1);
  (void)((p->cas) << 1);
  { lcb_KEYBUF *tmp = &p->key; (void)tmp; }
  { lcb_KEYBUF *tmp = &p->_hashkey; (void)tmp; }
  (void)((p->delta) << 1);
  (void)((p->initial) << 1);
  (void)((p->create) << 1);
}
static PyObject *
_cffi_layout__lcb_CMDCOUNTER(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_CMDCOUNTER y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_CMDCOUNTER),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_CMDCOUNTER, cmdflags),
    sizeof(((lcb_CMDCOUNTER *)0)->cmdflags),
    offsetof(lcb_CMDCOUNTER, exptime),
    sizeof(((lcb_CMDCOUNTER *)0)->exptime),
    offsetof(lcb_CMDCOUNTER, cas),
    sizeof(((lcb_CMDCOUNTER *)0)->cas),
    offsetof(lcb_CMDCOUNTER, key),
    sizeof(((lcb_CMDCOUNTER *)0)->key),
    offsetof(lcb_CMDCOUNTER, _hashkey),
    sizeof(((lcb_CMDCOUNTER *)0)->_hashkey),
    offsetof(lcb_CMDCOUNTER, delta),
    sizeof(((lcb_CMDCOUNTER *)0)->delta),
    offsetof(lcb_CMDCOUNTER, initial),
    sizeof(((lcb_CMDCOUNTER *)0)->initial),
    offsetof(lcb_CMDCOUNTER, create),
    sizeof(((lcb_CMDCOUNTER *)0)->create),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_CMDCOUNTER(0);
}

static void _cffi_check__lcb_CMDENDURE(lcb_CMDENDURE *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->cmdflags) << 1);
  (void)((p->exptime) << 1);
  (void)((p->cas) << 1);
  { lcb_KEYBUF *tmp = &p->key; (void)tmp; }
  { lcb_KEYBUF *tmp = &p->_hashkey; (void)tmp; }
  { lcb_SYNCTOKEN const * *tmp = &p->synctoken; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_CMDENDURE(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_CMDENDURE y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_CMDENDURE),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_CMDENDURE, cmdflags),
    sizeof(((lcb_CMDENDURE *)0)->cmdflags),
    offsetof(lcb_CMDENDURE, exptime),
    sizeof(((lcb_CMDENDURE *)0)->exptime),
    offsetof(lcb_CMDENDURE, cas),
    sizeof(((lcb_CMDENDURE *)0)->cas),
    offsetof(lcb_CMDENDURE, key),
    sizeof(((lcb_CMDENDURE *)0)->key),
    offsetof(lcb_CMDENDURE, _hashkey),
    sizeof(((lcb_CMDENDURE *)0)->_hashkey),
    offsetof(lcb_CMDENDURE, synctoken),
    sizeof(((lcb_CMDENDURE *)0)->synctoken),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_CMDENDURE(0);
}

static void _cffi_check__lcb_CMDGET(lcb_CMDGET *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->cmdflags) << 1);
  (void)((p->exptime) << 1);
  (void)((p->cas) << 1);
  { lcb_KEYBUF *tmp = &p->key; (void)tmp; }
  { lcb_KEYBUF *tmp = &p->_hashkey; (void)tmp; }
  (void)((p->lock) << 1);
}
static PyObject *
_cffi_layout__lcb_CMDGET(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_CMDGET y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_CMDGET),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_CMDGET, cmdflags),
    sizeof(((lcb_CMDGET *)0)->cmdflags),
    offsetof(lcb_CMDGET, exptime),
    sizeof(((lcb_CMDGET *)0)->exptime),
    offsetof(lcb_CMDGET, cas),
    sizeof(((lcb_CMDGET *)0)->cas),
    offsetof(lcb_CMDGET, key),
    sizeof(((lcb_CMDGET *)0)->key),
    offsetof(lcb_CMDGET, _hashkey),
    sizeof(((lcb_CMDGET *)0)->_hashkey),
    offsetof(lcb_CMDGET, lock),
    sizeof(((lcb_CMDGET *)0)->lock),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_CMDGET(0);
}

static void _cffi_check__lcb_CMDGETREPLICA(lcb_CMDGETREPLICA *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->cmdflags) << 1);
  (void)((p->exptime) << 1);
  (void)((p->cas) << 1);
  { lcb_KEYBUF *tmp = &p->key; (void)tmp; }
  { lcb_KEYBUF *tmp = &p->_hashkey; (void)tmp; }
  { lcb_replica_t *tmp = &p->strategy; (void)tmp; }
  (void)((p->index) << 1);
}
static PyObject *
_cffi_layout__lcb_CMDGETREPLICA(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_CMDGETREPLICA y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_CMDGETREPLICA),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_CMDGETREPLICA, cmdflags),
    sizeof(((lcb_CMDGETREPLICA *)0)->cmdflags),
    offsetof(lcb_CMDGETREPLICA, exptime),
    sizeof(((lcb_CMDGETREPLICA *)0)->exptime),
    offsetof(lcb_CMDGETREPLICA, cas),
    sizeof(((lcb_CMDGETREPLICA *)0)->cas),
    offsetof(lcb_CMDGETREPLICA, key),
    sizeof(((lcb_CMDGETREPLICA *)0)->key),
    offsetof(lcb_CMDGETREPLICA, _hashkey),
    sizeof(((lcb_CMDGETREPLICA *)0)->_hashkey),
    offsetof(lcb_CMDGETREPLICA, strategy),
    sizeof(((lcb_CMDGETREPLICA *)0)->strategy),
    offsetof(lcb_CMDGETREPLICA, index),
    sizeof(((lcb_CMDGETREPLICA *)0)->index),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_CMDGETREPLICA(0);
}

static void _cffi_check__lcb_CMDHTTP(lcb_CMDHTTP *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->cmdflags) << 1);
  (void)((p->exptime) << 1);
  (void)((p->cas) << 1);
  { lcb_KEYBUF *tmp = &p->key; (void)tmp; }
  { lcb_KEYBUF *tmp = &p->_hashkey; (void)tmp; }
  { lcb_http_type_t *tmp = &p->type; (void)tmp; }
  { lcb_http_method_t *tmp = &p->method; (void)tmp; }
  { char const * *tmp = &p->body; (void)tmp; }
  (void)((p->nbody) << 1);
  { struct lcb_http_request_st * * *tmp = &p->reqhandle; (void)tmp; }
  { char const * *tmp = &p->content_type; (void)tmp; }
  { char const * *tmp = &p->username; (void)tmp; }
  { char const * *tmp = &p->password; (void)tmp; }
  { char const * *tmp = &p->host; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_CMDHTTP(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_CMDHTTP y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_CMDHTTP),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_CMDHTTP, cmdflags),
    sizeof(((lcb_CMDHTTP *)0)->cmdflags),
    offsetof(lcb_CMDHTTP, exptime),
    sizeof(((lcb_CMDHTTP *)0)->exptime),
    offsetof(lcb_CMDHTTP, cas),
    sizeof(((lcb_CMDHTTP *)0)->cas),
    offsetof(lcb_CMDHTTP, key),
    sizeof(((lcb_CMDHTTP *)0)->key),
    offsetof(lcb_CMDHTTP, _hashkey),
    sizeof(((lcb_CMDHTTP *)0)->_hashkey),
    offsetof(lcb_CMDHTTP, type),
    sizeof(((lcb_CMDHTTP *)0)->type),
    offsetof(lcb_CMDHTTP, method),
    sizeof(((lcb_CMDHTTP *)0)->method),
    offsetof(lcb_CMDHTTP, body),
    sizeof(((lcb_CMDHTTP *)0)->body),
    offsetof(lcb_CMDHTTP, nbody),
    sizeof(((lcb_CMDHTTP *)0)->nbody),
    offsetof(lcb_CMDHTTP, reqhandle),
    sizeof(((lcb_CMDHTTP *)0)->reqhandle),
    offsetof(lcb_CMDHTTP, content_type),
    sizeof(((lcb_CMDHTTP *)0)->content_type),
    offsetof(lcb_CMDHTTP, username),
    sizeof(((lcb_CMDHTTP *)0)->username),
    offsetof(lcb_CMDHTTP, password),
    sizeof(((lcb_CMDHTTP *)0)->password),
    offsetof(lcb_CMDHTTP, host),
    sizeof(((lcb_CMDHTTP *)0)->host),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_CMDHTTP(0);
}

static void _cffi_check__lcb_CMDOBSEQNO(lcb_CMDOBSEQNO *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->cmdflags) << 1);
  (void)((p->exptime) << 1);
  (void)((p->cas) << 1);
  { lcb_KEYBUF *tmp = &p->key; (void)tmp; }
  { lcb_KEYBUF *tmp = &p->_hashkey; (void)tmp; }
  (void)((p->server_index) << 1);
  (void)((p->vbid) << 1);
  (void)((p->uuid) << 1);
}
static PyObject *
_cffi_layout__lcb_CMDOBSEQNO(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_CMDOBSEQNO y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_CMDOBSEQNO),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_CMDOBSEQNO, cmdflags),
    sizeof(((lcb_CMDOBSEQNO *)0)->cmdflags),
    offsetof(lcb_CMDOBSEQNO, exptime),
    sizeof(((lcb_CMDOBSEQNO *)0)->exptime),
    offsetof(lcb_CMDOBSEQNO, cas),
    sizeof(((lcb_CMDOBSEQNO *)0)->cas),
    offsetof(lcb_CMDOBSEQNO, key),
    sizeof(((lcb_CMDOBSEQNO *)0)->key),
    offsetof(lcb_CMDOBSEQNO, _hashkey),
    sizeof(((lcb_CMDOBSEQNO *)0)->_hashkey),
    offsetof(lcb_CMDOBSEQNO, server_index),
    sizeof(((lcb_CMDOBSEQNO *)0)->server_index),
    offsetof(lcb_CMDOBSEQNO, vbid),
    sizeof(((lcb_CMDOBSEQNO *)0)->vbid),
    offsetof(lcb_CMDOBSEQNO, uuid),
    sizeof(((lcb_CMDOBSEQNO *)0)->uuid),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_CMDOBSEQNO(0);
}

static void _cffi_check__lcb_CMDOBSERVE(lcb_CMDOBSERVE *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->cmdflags) << 1);
  (void)((p->exptime) << 1);
  (void)((p->cas) << 1);
  { lcb_KEYBUF *tmp = &p->key; (void)tmp; }
  { lcb_KEYBUF *tmp = &p->_hashkey; (void)tmp; }
  { unsigned short const * *tmp = &p->servers_; (void)tmp; }
  (void)((p->nservers_) << 1);
}
static PyObject *
_cffi_layout__lcb_CMDOBSERVE(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_CMDOBSERVE y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_CMDOBSERVE),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_CMDOBSERVE, cmdflags),
    sizeof(((lcb_CMDOBSERVE *)0)->cmdflags),
    offsetof(lcb_CMDOBSERVE, exptime),
    sizeof(((lcb_CMDOBSERVE *)0)->exptime),
    offsetof(lcb_CMDOBSERVE, cas),
    sizeof(((lcb_CMDOBSERVE *)0)->cas),
    offsetof(lcb_CMDOBSERVE, key),
    sizeof(((lcb_CMDOBSERVE *)0)->key),
    offsetof(lcb_CMDOBSERVE, _hashkey),
    sizeof(((lcb_CMDOBSERVE *)0)->_hashkey),
    offsetof(lcb_CMDOBSERVE, servers_),
    sizeof(((lcb_CMDOBSERVE *)0)->servers_),
    offsetof(lcb_CMDOBSERVE, nservers_),
    sizeof(((lcb_CMDOBSERVE *)0)->nservers_),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_CMDOBSERVE(0);
}

static void _cffi_check__lcb_CMDSTORE(lcb_CMDSTORE *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->cmdflags) << 1);
  (void)((p->exptime) << 1);
  (void)((p->cas) << 1);
  { lcb_KEYBUF *tmp = &p->key; (void)tmp; }
  { lcb_KEYBUF *tmp = &p->_hashkey; (void)tmp; }
  { lcb_VALBUF *tmp = &p->value; (void)tmp; }
  (void)((p->flags) << 1);
  (void)((p->datatype) << 1);
  { lcb_storage_t *tmp = &p->operation; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_CMDSTORE(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_CMDSTORE y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_CMDSTORE),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_CMDSTORE, cmdflags),
    sizeof(((lcb_CMDSTORE *)0)->cmdflags),
    offsetof(lcb_CMDSTORE, exptime),
    sizeof(((lcb_CMDSTORE *)0)->exptime),
    offsetof(lcb_CMDSTORE, cas),
    sizeof(((lcb_CMDSTORE *)0)->cas),
    offsetof(lcb_CMDSTORE, key),
    sizeof(((lcb_CMDSTORE *)0)->key),
    offsetof(lcb_CMDSTORE, _hashkey),
    sizeof(((lcb_CMDSTORE *)0)->_hashkey),
    offsetof(lcb_CMDSTORE, value),
    sizeof(((lcb_CMDSTORE *)0)->value),
    offsetof(lcb_CMDSTORE, flags),
    sizeof(((lcb_CMDSTORE *)0)->flags),
    offsetof(lcb_CMDSTORE, datatype),
    sizeof(((lcb_CMDSTORE *)0)->datatype),
    offsetof(lcb_CMDSTORE, operation),
    sizeof(((lcb_CMDSTORE *)0)->operation),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_CMDSTORE(0);
}

static void _cffi_check__lcb_CMDVERBOSITY(lcb_CMDVERBOSITY *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->cmdflags) << 1);
  (void)((p->exptime) << 1);
  (void)((p->cas) << 1);
  { lcb_KEYBUF *tmp = &p->key; (void)tmp; }
  { lcb_KEYBUF *tmp = &p->_hashkey; (void)tmp; }
  { char const * *tmp = &p->server; (void)tmp; }
  { lcb_verbosity_level_t *tmp = &p->level; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_CMDVERBOSITY(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_CMDVERBOSITY y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_CMDVERBOSITY),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_CMDVERBOSITY, cmdflags),
    sizeof(((lcb_CMDVERBOSITY *)0)->cmdflags),
    offsetof(lcb_CMDVERBOSITY, exptime),
    sizeof(((lcb_CMDVERBOSITY *)0)->exptime),
    offsetof(lcb_CMDVERBOSITY, cas),
    sizeof(((lcb_CMDVERBOSITY *)0)->cas),
    offsetof(lcb_CMDVERBOSITY, key),
    sizeof(((lcb_CMDVERBOSITY *)0)->key),
    offsetof(lcb_CMDVERBOSITY, _hashkey),
    sizeof(((lcb_CMDVERBOSITY *)0)->_hashkey),
    offsetof(lcb_CMDVERBOSITY, server),
    sizeof(((lcb_CMDVERBOSITY *)0)->server),
    offsetof(lcb_CMDVERBOSITY, level),
    sizeof(((lcb_CMDVERBOSITY *)0)->level),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_CMDVERBOSITY(0);
}

static void _cffi_check__lcb_CMDVIEWQUERY(lcb_CMDVIEWQUERY *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->cmdflags) << 1);
  { char const * *tmp = &p->ddoc; (void)tmp; }
  (void)((p->nddoc) << 1);
  { char const * *tmp = &p->view; (void)tmp; }
  (void)((p->nview) << 1);
  { char const * *tmp = &p->optstr; (void)tmp; }
  (void)((p->noptstr) << 1);
  { char const * *tmp = &p->postdata; (void)tmp; }
  (void)((p->npostdata) << 1);
  (void)((p->docs_concurrent_max) << 1);
  { void(* *tmp)(struct lcb_st *, int, lcb_RESPVIEWQUERY const *) = &p->callback; (void)tmp; }
  { struct lcbview_REQUEST_st * * *tmp = &p->handle; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_CMDVIEWQUERY(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_CMDVIEWQUERY y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_CMDVIEWQUERY),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_CMDVIEWQUERY, cmdflags),
    sizeof(((lcb_CMDVIEWQUERY *)0)->cmdflags),
    offsetof(lcb_CMDVIEWQUERY, ddoc),
    sizeof(((lcb_CMDVIEWQUERY *)0)->ddoc),
    offsetof(lcb_CMDVIEWQUERY, nddoc),
    sizeof(((lcb_CMDVIEWQUERY *)0)->nddoc),
    offsetof(lcb_CMDVIEWQUERY, view),
    sizeof(((lcb_CMDVIEWQUERY *)0)->view),
    offsetof(lcb_CMDVIEWQUERY, nview),
    sizeof(((lcb_CMDVIEWQUERY *)0)->nview),
    offsetof(lcb_CMDVIEWQUERY, optstr),
    sizeof(((lcb_CMDVIEWQUERY *)0)->optstr),
    offsetof(lcb_CMDVIEWQUERY, noptstr),
    sizeof(((lcb_CMDVIEWQUERY *)0)->noptstr),
    offsetof(lcb_CMDVIEWQUERY, postdata),
    sizeof(((lcb_CMDVIEWQUERY *)0)->postdata),
    offsetof(lcb_CMDVIEWQUERY, npostdata),
    sizeof(((lcb_CMDVIEWQUERY *)0)->npostdata),
    offsetof(lcb_CMDVIEWQUERY, docs_concurrent_max),
    sizeof(((lcb_CMDVIEWQUERY *)0)->docs_concurrent_max),
    offsetof(lcb_CMDVIEWQUERY, callback),
    sizeof(((lcb_CMDVIEWQUERY *)0)->callback),
    offsetof(lcb_CMDVIEWQUERY, handle),
    sizeof(((lcb_CMDVIEWQUERY *)0)->handle),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_CMDVIEWQUERY(0);
}

static int _cffi_e__lcb_COMPRESSOPTS(PyObject *lib)
{
  if ((LCB_COMPRESS_NONE) > 0 || (long)(LCB_COMPRESS_NONE) != 0L) {
    char buf[64];
    if ((LCB_COMPRESS_NONE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_COMPRESS_NONE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_COMPRESS_NONE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_COMPRESSOPTS: ", "LCB_COMPRESS_NONE", buf, "0");
    return -1;
  }
  if ((LCB_COMPRESS_IN) <= 0 || (unsigned long)(LCB_COMPRESS_IN) != 1UL) {
    char buf[64];
    if ((LCB_COMPRESS_IN) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_COMPRESS_IN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_COMPRESS_IN));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_COMPRESSOPTS: ", "LCB_COMPRESS_IN", buf, "1");
    return -1;
  }
  if ((LCB_COMPRESS_OUT) <= 0 || (unsigned long)(LCB_COMPRESS_OUT) != 2UL) {
    char buf[64];
    if ((LCB_COMPRESS_OUT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_COMPRESS_OUT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_COMPRESS_OUT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_COMPRESSOPTS: ", "LCB_COMPRESS_OUT", buf, "2");
    return -1;
  }
  if ((LCB_COMPRESS_INOUT) <= 0 || (unsigned long)(LCB_COMPRESS_INOUT) != 3UL) {
    char buf[64];
    if ((LCB_COMPRESS_INOUT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_COMPRESS_INOUT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_COMPRESS_INOUT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_COMPRESSOPTS: ", "LCB_COMPRESS_INOUT", buf, "3");
    return -1;
  }
  if ((LCB_COMPRESS_FORCE) <= 0 || (unsigned long)(LCB_COMPRESS_FORCE) != 4UL) {
    char buf[64];
    if ((LCB_COMPRESS_FORCE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_COMPRESS_FORCE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_COMPRESS_FORCE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_COMPRESSOPTS: ", "LCB_COMPRESS_FORCE", buf, "4");
    return -1;
  }
  return _cffi_e__lcb_CALLBACKTYPE(lib);
}

static int _cffi_e__lcb_DUMPFLAGS(PyObject *lib)
{
  if ((LCB_DUMP_VBCONFIG) <= 0 || (unsigned long)(LCB_DUMP_VBCONFIG) != 1UL) {
    char buf[64];
    if ((LCB_DUMP_VBCONFIG) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_DUMP_VBCONFIG));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_DUMP_VBCONFIG));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_DUMPFLAGS: ", "LCB_DUMP_VBCONFIG", buf, "1");
    return -1;
  }
  if ((LCB_DUMP_PKTINFO) <= 0 || (unsigned long)(LCB_DUMP_PKTINFO) != 2UL) {
    char buf[64];
    if ((LCB_DUMP_PKTINFO) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_DUMP_PKTINFO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_DUMP_PKTINFO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_DUMPFLAGS: ", "LCB_DUMP_PKTINFO", buf, "2");
    return -1;
  }
  if ((LCB_DUMP_BUFINFO) <= 0 || (unsigned long)(LCB_DUMP_BUFINFO) != 4UL) {
    char buf[64];
    if ((LCB_DUMP_BUFINFO) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_DUMP_BUFINFO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_DUMP_BUFINFO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_DUMPFLAGS: ", "LCB_DUMP_BUFINFO", buf, "4");
    return -1;
  }
  if ((LCB_DUMP_ALL) <= 0 || (unsigned long)(LCB_DUMP_ALL) != 255UL) {
    char buf[64];
    if ((LCB_DUMP_ALL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_DUMP_ALL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_DUMP_ALL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_DUMPFLAGS: ", "LCB_DUMP_ALL", buf, "255");
    return -1;
  }
  return _cffi_e__lcb_COMPRESSOPTS(lib);
}

static void _cffi_check__lcb_DURABILITYCMDv0(lcb_DURABILITYCMDv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  { void const * *tmp = &p->hashkey; (void)tmp; }
  (void)((p->nhashkey) << 1);
  (void)((p->cas) << 1);
  { lcb_SYNCTOKEN const * *tmp = &p->synctok; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_DURABILITYCMDv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_DURABILITYCMDv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_DURABILITYCMDv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_DURABILITYCMDv0, key),
    sizeof(((lcb_DURABILITYCMDv0 *)0)->key),
    offsetof(lcb_DURABILITYCMDv0, nkey),
    sizeof(((lcb_DURABILITYCMDv0 *)0)->nkey),
    offsetof(lcb_DURABILITYCMDv0, hashkey),
    sizeof(((lcb_DURABILITYCMDv0 *)0)->hashkey),
    offsetof(lcb_DURABILITYCMDv0, nhashkey),
    sizeof(((lcb_DURABILITYCMDv0 *)0)->nhashkey),
    offsetof(lcb_DURABILITYCMDv0, cas),
    sizeof(((lcb_DURABILITYCMDv0 *)0)->cas),
    offsetof(lcb_DURABILITYCMDv0, synctok),
    sizeof(((lcb_DURABILITYCMDv0 *)0)->synctok),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_DURABILITYCMDv0(0);
}

static void _cffi_check__lcb_DURABILITYOPTSv0(lcb_DURABILITYOPTSv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->timeout) << 1);
  (void)((p->interval) << 1);
  (void)((p->persist_to) << 1);
  (void)((p->replicate_to) << 1);
  (void)((p->check_delete) << 1);
  (void)((p->cap_max) << 1);
  (void)((p->pollopts) << 1);
}
static PyObject *
_cffi_layout__lcb_DURABILITYOPTSv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_DURABILITYOPTSv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_DURABILITYOPTSv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_DURABILITYOPTSv0, timeout),
    sizeof(((lcb_DURABILITYOPTSv0 *)0)->timeout),
    offsetof(lcb_DURABILITYOPTSv0, interval),
    sizeof(((lcb_DURABILITYOPTSv0 *)0)->interval),
    offsetof(lcb_DURABILITYOPTSv0, persist_to),
    sizeof(((lcb_DURABILITYOPTSv0 *)0)->persist_to),
    offsetof(lcb_DURABILITYOPTSv0, replicate_to),
    sizeof(((lcb_DURABILITYOPTSv0 *)0)->replicate_to),
    offsetof(lcb_DURABILITYOPTSv0, check_delete),
    sizeof(((lcb_DURABILITYOPTSv0 *)0)->check_delete),
    offsetof(lcb_DURABILITYOPTSv0, cap_max),
    sizeof(((lcb_DURABILITYOPTSv0 *)0)->cap_max),
    offsetof(lcb_DURABILITYOPTSv0, pollopts),
    sizeof(((lcb_DURABILITYOPTSv0 *)0)->pollopts),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_DURABILITYOPTSv0(0);
}

static void _cffi_check__lcb_DURABILITYRESPv0(lcb_DURABILITYRESPv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  { lcb_error_t *tmp = &p->err; (void)tmp; }
  (void)((p->cas) << 1);
  (void)((p->persisted_master) << 1);
  (void)((p->exists_master) << 1);
  (void)((p->npersisted) << 1);
  (void)((p->nreplicated) << 1);
  (void)((p->nresponses) << 1);
}
static PyObject *
_cffi_layout__lcb_DURABILITYRESPv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_DURABILITYRESPv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_DURABILITYRESPv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_DURABILITYRESPv0, key),
    sizeof(((lcb_DURABILITYRESPv0 *)0)->key),
    offsetof(lcb_DURABILITYRESPv0, nkey),
    sizeof(((lcb_DURABILITYRESPv0 *)0)->nkey),
    offsetof(lcb_DURABILITYRESPv0, err),
    sizeof(((lcb_DURABILITYRESPv0 *)0)->err),
    offsetof(lcb_DURABILITYRESPv0, cas),
    sizeof(((lcb_DURABILITYRESPv0 *)0)->cas),
    offsetof(lcb_DURABILITYRESPv0, persisted_master),
    sizeof(((lcb_DURABILITYRESPv0 *)0)->persisted_master),
    offsetof(lcb_DURABILITYRESPv0, exists_master),
    sizeof(((lcb_DURABILITYRESPv0 *)0)->exists_master),
    offsetof(lcb_DURABILITYRESPv0, npersisted),
    sizeof(((lcb_DURABILITYRESPv0 *)0)->npersisted),
    offsetof(lcb_DURABILITYRESPv0, nreplicated),
    sizeof(((lcb_DURABILITYRESPv0 *)0)->nreplicated),
    offsetof(lcb_DURABILITYRESPv0, nresponses),
    sizeof(((lcb_DURABILITYRESPv0 *)0)->nresponses),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_DURABILITYRESPv0(0);
}

static void _cffi_check__lcb_GETCMDv0(lcb_GETCMDv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->exptime) << 1);
  (void)((p->lock) << 1);
  { void const * *tmp = &p->hashkey; (void)tmp; }
  (void)((p->nhashkey) << 1);
}
static PyObject *
_cffi_layout__lcb_GETCMDv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_GETCMDv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_GETCMDv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_GETCMDv0, key),
    sizeof(((lcb_GETCMDv0 *)0)->key),
    offsetof(lcb_GETCMDv0, nkey),
    sizeof(((lcb_GETCMDv0 *)0)->nkey),
    offsetof(lcb_GETCMDv0, exptime),
    sizeof(((lcb_GETCMDv0 *)0)->exptime),
    offsetof(lcb_GETCMDv0, lock),
    sizeof(((lcb_GETCMDv0 *)0)->lock),
    offsetof(lcb_GETCMDv0, hashkey),
    sizeof(((lcb_GETCMDv0 *)0)->hashkey),
    offsetof(lcb_GETCMDv0, nhashkey),
    sizeof(((lcb_GETCMDv0 *)0)->nhashkey),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_GETCMDv0(0);
}

static int _cffi_e__lcb_GETNODETYPE(PyObject *lib)
{
  if ((LCB_NODE_HTCONFIG) <= 0 || (unsigned long)(LCB_NODE_HTCONFIG) != 1UL) {
    char buf[64];
    if ((LCB_NODE_HTCONFIG) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_NODE_HTCONFIG));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_NODE_HTCONFIG));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_GETNODETYPE: ", "LCB_NODE_HTCONFIG", buf, "1");
    return -1;
  }
  if ((LCB_NODE_DATA) <= 0 || (unsigned long)(LCB_NODE_DATA) != 2UL) {
    char buf[64];
    if ((LCB_NODE_DATA) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_NODE_DATA));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_NODE_DATA));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_GETNODETYPE: ", "LCB_NODE_DATA", buf, "2");
    return -1;
  }
  if ((LCB_NODE_VIEWS) <= 0 || (unsigned long)(LCB_NODE_VIEWS) != 4UL) {
    char buf[64];
    if ((LCB_NODE_VIEWS) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_NODE_VIEWS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_NODE_VIEWS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_GETNODETYPE: ", "LCB_NODE_VIEWS", buf, "4");
    return -1;
  }
  if ((LCB_NODE_CONNECTED) <= 0 || (unsigned long)(LCB_NODE_CONNECTED) != 8UL) {
    char buf[64];
    if ((LCB_NODE_CONNECTED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_NODE_CONNECTED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_NODE_CONNECTED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_GETNODETYPE: ", "LCB_NODE_CONNECTED", buf, "8");
    return -1;
  }
  if ((LCB_NODE_NEVERNULL) <= 0 || (unsigned long)(LCB_NODE_NEVERNULL) != 16UL) {
    char buf[64];
    if ((LCB_NODE_NEVERNULL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_NODE_NEVERNULL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_NODE_NEVERNULL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_GETNODETYPE: ", "LCB_NODE_NEVERNULL", buf, "16");
    return -1;
  }
  if ((LCB_NODE_HTCONFIG_CONNECTED) <= 0 || (unsigned long)(LCB_NODE_HTCONFIG_CONNECTED) != 9UL) {
    char buf[64];
    if ((LCB_NODE_HTCONFIG_CONNECTED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_NODE_HTCONFIG_CONNECTED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_NODE_HTCONFIG_CONNECTED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_GETNODETYPE: ", "LCB_NODE_HTCONFIG_CONNECTED", buf, "9");
    return -1;
  }
  if ((LCB_NODE_HTCONFIG_ANY) <= 0 || (unsigned long)(LCB_NODE_HTCONFIG_ANY) != 17UL) {
    char buf[64];
    if ((LCB_NODE_HTCONFIG_ANY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_NODE_HTCONFIG_ANY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_NODE_HTCONFIG_ANY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_GETNODETYPE: ", "LCB_NODE_HTCONFIG_ANY", buf, "17");
    return -1;
  }
  return _cffi_e__lcb_DUMPFLAGS(lib);
}

static void _cffi_check__lcb_GETREPLICACMDv0(lcb_GETREPLICACMDv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  { void const * *tmp = &p->hashkey; (void)tmp; }
  (void)((p->nhashkey) << 1);
}
static PyObject *
_cffi_layout__lcb_GETREPLICACMDv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_GETREPLICACMDv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_GETREPLICACMDv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_GETREPLICACMDv0, key),
    sizeof(((lcb_GETREPLICACMDv0 *)0)->key),
    offsetof(lcb_GETREPLICACMDv0, nkey),
    sizeof(((lcb_GETREPLICACMDv0 *)0)->nkey),
    offsetof(lcb_GETREPLICACMDv0, hashkey),
    sizeof(((lcb_GETREPLICACMDv0 *)0)->hashkey),
    offsetof(lcb_GETREPLICACMDv0, nhashkey),
    sizeof(((lcb_GETREPLICACMDv0 *)0)->nhashkey),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_GETREPLICACMDv0(0);
}

static void _cffi_check__lcb_GETREPLICACMDv1(lcb_GETREPLICACMDv1 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  { void const * *tmp = &p->hashkey; (void)tmp; }
  (void)((p->nhashkey) << 1);
  { lcb_replica_t *tmp = &p->strategy; (void)tmp; }
  (void)((p->index) << 1);
}
static PyObject *
_cffi_layout__lcb_GETREPLICACMDv1(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_GETREPLICACMDv1 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_GETREPLICACMDv1),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_GETREPLICACMDv1, key),
    sizeof(((lcb_GETREPLICACMDv1 *)0)->key),
    offsetof(lcb_GETREPLICACMDv1, nkey),
    sizeof(((lcb_GETREPLICACMDv1 *)0)->nkey),
    offsetof(lcb_GETREPLICACMDv1, hashkey),
    sizeof(((lcb_GETREPLICACMDv1 *)0)->hashkey),
    offsetof(lcb_GETREPLICACMDv1, nhashkey),
    sizeof(((lcb_GETREPLICACMDv1 *)0)->nhashkey),
    offsetof(lcb_GETREPLICACMDv1, strategy),
    sizeof(((lcb_GETREPLICACMDv1 *)0)->strategy),
    offsetof(lcb_GETREPLICACMDv1, index),
    sizeof(((lcb_GETREPLICACMDv1 *)0)->index),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_GETREPLICACMDv1(0);
}

static void _cffi_check__lcb_GETRESPv0(lcb_GETRESPv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  { void const * *tmp = &p->bytes; (void)tmp; }
  (void)((p->nbytes) << 1);
  (void)((p->flags) << 1);
  (void)((p->cas) << 1);
  (void)((p->datatype) << 1);
}
static PyObject *
_cffi_layout__lcb_GETRESPv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_GETRESPv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_GETRESPv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_GETRESPv0, key),
    sizeof(((lcb_GETRESPv0 *)0)->key),
    offsetof(lcb_GETRESPv0, nkey),
    sizeof(((lcb_GETRESPv0 *)0)->nkey),
    offsetof(lcb_GETRESPv0, bytes),
    sizeof(((lcb_GETRESPv0 *)0)->bytes),
    offsetof(lcb_GETRESPv0, nbytes),
    sizeof(((lcb_GETRESPv0 *)0)->nbytes),
    offsetof(lcb_GETRESPv0, flags),
    sizeof(((lcb_GETRESPv0 *)0)->flags),
    offsetof(lcb_GETRESPv0, cas),
    sizeof(((lcb_GETRESPv0 *)0)->cas),
    offsetof(lcb_GETRESPv0, datatype),
    sizeof(((lcb_GETRESPv0 *)0)->datatype),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_GETRESPv0(0);
}

static int _cffi_e__lcb_HTCONFIG_URLTYPE(PyObject *lib)
{
  if ((LCB_HTCONFIG_URLTYPE_25PLUS) <= 0 || (unsigned long)(LCB_HTCONFIG_URLTYPE_25PLUS) != 1UL) {
    char buf[64];
    if ((LCB_HTCONFIG_URLTYPE_25PLUS) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTCONFIG_URLTYPE_25PLUS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTCONFIG_URLTYPE_25PLUS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_HTCONFIG_URLTYPE: ", "LCB_HTCONFIG_URLTYPE_25PLUS", buf, "1");
    return -1;
  }
  if ((LCB_HTCONFIG_URLTYPE_COMPAT) <= 0 || (unsigned long)(LCB_HTCONFIG_URLTYPE_COMPAT) != 2UL) {
    char buf[64];
    if ((LCB_HTCONFIG_URLTYPE_COMPAT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTCONFIG_URLTYPE_COMPAT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTCONFIG_URLTYPE_COMPAT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_HTCONFIG_URLTYPE: ", "LCB_HTCONFIG_URLTYPE_COMPAT", buf, "2");
    return -1;
  }
  if ((LCB_HTCONFIG_URLTYPE_TRYALL) <= 0 || (unsigned long)(LCB_HTCONFIG_URLTYPE_TRYALL) != 3UL) {
    char buf[64];
    if ((LCB_HTCONFIG_URLTYPE_TRYALL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTCONFIG_URLTYPE_TRYALL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTCONFIG_URLTYPE_TRYALL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_HTCONFIG_URLTYPE: ", "LCB_HTCONFIG_URLTYPE_TRYALL", buf, "3");
    return -1;
  }
  return _cffi_e__lcb_GETNODETYPE(lib);
}

static void _cffi_check__lcb_HTTPCMDv0(lcb_HTTPCMDv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { char const * *tmp = &p->path; (void)tmp; }
  (void)((p->npath) << 1);
  { void const * *tmp = &p->body; (void)tmp; }
  (void)((p->nbody) << 1);
  { lcb_http_method_t *tmp = &p->method; (void)tmp; }
  (void)((p->chunked) << 1);
  { char const * *tmp = &p->content_type; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_HTTPCMDv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_HTTPCMDv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_HTTPCMDv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_HTTPCMDv0, path),
    sizeof(((lcb_HTTPCMDv0 *)0)->path),
    offsetof(lcb_HTTPCMDv0, npath),
    sizeof(((lcb_HTTPCMDv0 *)0)->npath),
    offsetof(lcb_HTTPCMDv0, body),
    sizeof(((lcb_HTTPCMDv0 *)0)->body),
    offsetof(lcb_HTTPCMDv0, nbody),
    sizeof(((lcb_HTTPCMDv0 *)0)->nbody),
    offsetof(lcb_HTTPCMDv0, method),
    sizeof(((lcb_HTTPCMDv0 *)0)->method),
    offsetof(lcb_HTTPCMDv0, chunked),
    sizeof(((lcb_HTTPCMDv0 *)0)->chunked),
    offsetof(lcb_HTTPCMDv0, content_type),
    sizeof(((lcb_HTTPCMDv0 *)0)->content_type),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_HTTPCMDv0(0);
}

static void _cffi_check__lcb_HTTPCMDv1(lcb_HTTPCMDv1 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { char const * *tmp = &p->path; (void)tmp; }
  (void)((p->npath) << 1);
  { void const * *tmp = &p->body; (void)tmp; }
  (void)((p->nbody) << 1);
  { lcb_http_method_t *tmp = &p->method; (void)tmp; }
  (void)((p->chunked) << 1);
  { char const * *tmp = &p->content_type; (void)tmp; }
  { char const * *tmp = &p->host; (void)tmp; }
  { char const * *tmp = &p->username; (void)tmp; }
  { char const * *tmp = &p->password; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_HTTPCMDv1(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_HTTPCMDv1 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_HTTPCMDv1),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_HTTPCMDv1, path),
    sizeof(((lcb_HTTPCMDv1 *)0)->path),
    offsetof(lcb_HTTPCMDv1, npath),
    sizeof(((lcb_HTTPCMDv1 *)0)->npath),
    offsetof(lcb_HTTPCMDv1, body),
    sizeof(((lcb_HTTPCMDv1 *)0)->body),
    offsetof(lcb_HTTPCMDv1, nbody),
    sizeof(((lcb_HTTPCMDv1 *)0)->nbody),
    offsetof(lcb_HTTPCMDv1, method),
    sizeof(((lcb_HTTPCMDv1 *)0)->method),
    offsetof(lcb_HTTPCMDv1, chunked),
    sizeof(((lcb_HTTPCMDv1 *)0)->chunked),
    offsetof(lcb_HTTPCMDv1, content_type),
    sizeof(((lcb_HTTPCMDv1 *)0)->content_type),
    offsetof(lcb_HTTPCMDv1, host),
    sizeof(((lcb_HTTPCMDv1 *)0)->host),
    offsetof(lcb_HTTPCMDv1, username),
    sizeof(((lcb_HTTPCMDv1 *)0)->username),
    offsetof(lcb_HTTPCMDv1, password),
    sizeof(((lcb_HTTPCMDv1 *)0)->password),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_HTTPCMDv1(0);
}

static void _cffi_check__lcb_HTTPRESPv0(lcb_HTTPRESPv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { lcb_http_status_t *tmp = &p->status; (void)tmp; }
  { char const * *tmp = &p->path; (void)tmp; }
  (void)((p->npath) << 1);
  { char const * * *tmp = &p->headers; (void)tmp; }
  { void const * *tmp = &p->bytes; (void)tmp; }
  (void)((p->nbytes) << 1);
}
static PyObject *
_cffi_layout__lcb_HTTPRESPv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_HTTPRESPv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_HTTPRESPv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_HTTPRESPv0, status),
    sizeof(((lcb_HTTPRESPv0 *)0)->status),
    offsetof(lcb_HTTPRESPv0, path),
    sizeof(((lcb_HTTPRESPv0 *)0)->path),
    offsetof(lcb_HTTPRESPv0, npath),
    sizeof(((lcb_HTTPRESPv0 *)0)->npath),
    offsetof(lcb_HTTPRESPv0, headers),
    sizeof(((lcb_HTTPRESPv0 *)0)->headers),
    offsetof(lcb_HTTPRESPv0, bytes),
    sizeof(((lcb_HTTPRESPv0 *)0)->bytes),
    offsetof(lcb_HTTPRESPv0, nbytes),
    sizeof(((lcb_HTTPRESPv0 *)0)->nbytes),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_HTTPRESPv0(0);
}

static void _cffi_check__lcb_IOCREATEOPS_FUNCTIONPOINTER(lcb_IOCREATEOPS_FUNCTIONPOINTER *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { lcb_error_t(* *tmp)(int, struct lcb_io_opt_st * *, void *) = &p->create; (void)tmp; }
  { void * *tmp = &p->cookie; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_IOCREATEOPS_FUNCTIONPOINTER(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_IOCREATEOPS_FUNCTIONPOINTER y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_IOCREATEOPS_FUNCTIONPOINTER),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_IOCREATEOPS_FUNCTIONPOINTER, create),
    sizeof(((lcb_IOCREATEOPS_FUNCTIONPOINTER *)0)->create),
    offsetof(lcb_IOCREATEOPS_FUNCTIONPOINTER, cookie),
    sizeof(((lcb_IOCREATEOPS_FUNCTIONPOINTER *)0)->cookie),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_IOCREATEOPS_FUNCTIONPOINTER(0);
}

static void _cffi_check__lcb_IOCREATEOPTS_BUILTIN(lcb_IOCREATEOPTS_BUILTIN *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { lcb_io_ops_type_t *tmp = &p->type; (void)tmp; }
  { void * *tmp = &p->cookie; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_IOCREATEOPTS_BUILTIN(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_IOCREATEOPTS_BUILTIN y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_IOCREATEOPTS_BUILTIN),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_IOCREATEOPTS_BUILTIN, type),
    sizeof(((lcb_IOCREATEOPTS_BUILTIN *)0)->type),
    offsetof(lcb_IOCREATEOPTS_BUILTIN, cookie),
    sizeof(((lcb_IOCREATEOPTS_BUILTIN *)0)->cookie),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_IOCREATEOPTS_BUILTIN(0);
}

static void _cffi_check__lcb_IOCREATEOPTS_DSO(lcb_IOCREATEOPTS_DSO *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { char const * *tmp = &p->sofile; (void)tmp; }
  { char const * *tmp = &p->symbol; (void)tmp; }
  { void * *tmp = &p->cookie; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_IOCREATEOPTS_DSO(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_IOCREATEOPTS_DSO y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_IOCREATEOPTS_DSO),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_IOCREATEOPTS_DSO, sofile),
    sizeof(((lcb_IOCREATEOPTS_DSO *)0)->sofile),
    offsetof(lcb_IOCREATEOPTS_DSO, symbol),
    sizeof(((lcb_IOCREATEOPTS_DSO *)0)->symbol),
    offsetof(lcb_IOCREATEOPTS_DSO, cookie),
    sizeof(((lcb_IOCREATEOPTS_DSO *)0)->cookie),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_IOCREATEOPTS_DSO(0);
}

static int _cffi_e__lcb_KVBUFTYPE(PyObject *lib)
{
  if ((LCB_KV_COPY) > 0 || (long)(LCB_KV_COPY) != 0L) {
    char buf[64];
    if ((LCB_KV_COPY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_KV_COPY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_KV_COPY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_KVBUFTYPE: ", "LCB_KV_COPY", buf, "0");
    return -1;
  }
  if ((LCB_KV_CONTIG) <= 0 || (unsigned long)(LCB_KV_CONTIG) != 1UL) {
    char buf[64];
    if ((LCB_KV_CONTIG) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_KV_CONTIG));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_KV_CONTIG));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_KVBUFTYPE: ", "LCB_KV_CONTIG", buf, "1");
    return -1;
  }
  if ((LCB_KV_IOV) <= 0 || (unsigned long)(LCB_KV_IOV) != 2UL) {
    char buf[64];
    if ((LCB_KV_IOV) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_KV_IOV));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_KV_IOV));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_KVBUFTYPE: ", "LCB_KV_IOV", buf, "2");
    return -1;
  }
  if ((LCB_KV_VBID) <= 0 || (unsigned long)(LCB_KV_VBID) != 3UL) {
    char buf[64];
    if ((LCB_KV_VBID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_KV_VBID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_KV_VBID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_KVBUFTYPE: ", "LCB_KV_VBID", buf, "3");
    return -1;
  }
  return _cffi_e__lcb_HTCONFIG_URLTYPE(lib);
}

static void _cffi_check__lcb_OBSERVECMDv0(lcb_OBSERVECMDv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  { void const * *tmp = &p->hashkey; (void)tmp; }
  (void)((p->nhashkey) << 1);
}
static PyObject *
_cffi_layout__lcb_OBSERVECMDv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_OBSERVECMDv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_OBSERVECMDv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_OBSERVECMDv0, key),
    sizeof(((lcb_OBSERVECMDv0 *)0)->key),
    offsetof(lcb_OBSERVECMDv0, nkey),
    sizeof(((lcb_OBSERVECMDv0 *)0)->nkey),
    offsetof(lcb_OBSERVECMDv0, hashkey),
    sizeof(((lcb_OBSERVECMDv0 *)0)->hashkey),
    offsetof(lcb_OBSERVECMDv0, nhashkey),
    sizeof(((lcb_OBSERVECMDv0 *)0)->nhashkey),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_OBSERVECMDv0(0);
}

static void _cffi_check__lcb_OBSERVECMDv1(lcb_OBSERVECMDv1 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  { void const * *tmp = &p->hashkey; (void)tmp; }
  (void)((p->nhashkey) << 1);
  { lcb_observe_options_t *tmp = &p->options; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_OBSERVECMDv1(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_OBSERVECMDv1 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_OBSERVECMDv1),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_OBSERVECMDv1, key),
    sizeof(((lcb_OBSERVECMDv1 *)0)->key),
    offsetof(lcb_OBSERVECMDv1, nkey),
    sizeof(((lcb_OBSERVECMDv1 *)0)->nkey),
    offsetof(lcb_OBSERVECMDv1, hashkey),
    sizeof(((lcb_OBSERVECMDv1 *)0)->hashkey),
    offsetof(lcb_OBSERVECMDv1, nhashkey),
    sizeof(((lcb_OBSERVECMDv1 *)0)->nhashkey),
    offsetof(lcb_OBSERVECMDv1, options),
    sizeof(((lcb_OBSERVECMDv1 *)0)->options),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_OBSERVECMDv1(0);
}

static void _cffi_check__lcb_OBSERVERESPv0(lcb_OBSERVERESPv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_observe_t *tmp = &p->status; (void)tmp; }
  (void)((p->from_master) << 1);
  (void)((p->ttp) << 1);
  (void)((p->ttr) << 1);
}
static PyObject *
_cffi_layout__lcb_OBSERVERESPv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_OBSERVERESPv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_OBSERVERESPv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_OBSERVERESPv0, key),
    sizeof(((lcb_OBSERVERESPv0 *)0)->key),
    offsetof(lcb_OBSERVERESPv0, nkey),
    sizeof(((lcb_OBSERVERESPv0 *)0)->nkey),
    offsetof(lcb_OBSERVERESPv0, cas),
    sizeof(((lcb_OBSERVERESPv0 *)0)->cas),
    offsetof(lcb_OBSERVERESPv0, status),
    sizeof(((lcb_OBSERVERESPv0 *)0)->status),
    offsetof(lcb_OBSERVERESPv0, from_master),
    sizeof(((lcb_OBSERVERESPv0 *)0)->from_master),
    offsetof(lcb_OBSERVERESPv0, ttp),
    sizeof(((lcb_OBSERVERESPv0 *)0)->ttp),
    offsetof(lcb_OBSERVERESPv0, ttr),
    sizeof(((lcb_OBSERVERESPv0 *)0)->ttr),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_OBSERVERESPv0(0);
}

static void _cffi_check__lcb_REMOVECMDv0(lcb_REMOVECMDv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { void const * *tmp = &p->hashkey; (void)tmp; }
  (void)((p->nhashkey) << 1);
}
static PyObject *
_cffi_layout__lcb_REMOVECMDv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_REMOVECMDv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_REMOVECMDv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_REMOVECMDv0, key),
    sizeof(((lcb_REMOVECMDv0 *)0)->key),
    offsetof(lcb_REMOVECMDv0, nkey),
    sizeof(((lcb_REMOVECMDv0 *)0)->nkey),
    offsetof(lcb_REMOVECMDv0, cas),
    sizeof(((lcb_REMOVECMDv0 *)0)->cas),
    offsetof(lcb_REMOVECMDv0, hashkey),
    sizeof(((lcb_REMOVECMDv0 *)0)->hashkey),
    offsetof(lcb_REMOVECMDv0, nhashkey),
    sizeof(((lcb_REMOVECMDv0 *)0)->nhashkey),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_REMOVECMDv0(0);
}

static void _cffi_check__lcb_REMOVERESPv0(lcb_REMOVERESPv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_SYNCTOKEN const * *tmp = &p->synctoken; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_REMOVERESPv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_REMOVERESPv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_REMOVERESPv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_REMOVERESPv0, key),
    sizeof(((lcb_REMOVERESPv0 *)0)->key),
    offsetof(lcb_REMOVERESPv0, nkey),
    sizeof(((lcb_REMOVERESPv0 *)0)->nkey),
    offsetof(lcb_REMOVERESPv0, cas),
    sizeof(((lcb_REMOVERESPv0 *)0)->cas),
    offsetof(lcb_REMOVERESPv0, synctoken),
    sizeof(((lcb_REMOVERESPv0 *)0)->synctoken),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_REMOVERESPv0(0);
}

static void _cffi_check__lcb_RESPBASE(lcb_RESPBASE *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_error_t *tmp = &p->rc; (void)tmp; }
  (void)((p->version) << 1);
  (void)((p->rflags) << 1);
}
static PyObject *
_cffi_layout__lcb_RESPBASE(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_RESPBASE y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_RESPBASE),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_RESPBASE, cookie),
    sizeof(((lcb_RESPBASE *)0)->cookie),
    offsetof(lcb_RESPBASE, key),
    sizeof(((lcb_RESPBASE *)0)->key),
    offsetof(lcb_RESPBASE, nkey),
    sizeof(((lcb_RESPBASE *)0)->nkey),
    offsetof(lcb_RESPBASE, cas),
    sizeof(((lcb_RESPBASE *)0)->cas),
    offsetof(lcb_RESPBASE, rc),
    sizeof(((lcb_RESPBASE *)0)->rc),
    offsetof(lcb_RESPBASE, version),
    sizeof(((lcb_RESPBASE *)0)->version),
    offsetof(lcb_RESPBASE, rflags),
    sizeof(((lcb_RESPBASE *)0)->rflags),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_RESPBASE(0);
}

static void _cffi_check__lcb_RESPCOUNTER(lcb_RESPCOUNTER *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_error_t *tmp = &p->rc; (void)tmp; }
  (void)((p->version) << 1);
  (void)((p->rflags) << 1);
  (void)((p->value) << 1);
  { lcb_SYNCTOKEN *tmp = &p->synctoken; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_RESPCOUNTER(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_RESPCOUNTER y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_RESPCOUNTER),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_RESPCOUNTER, cookie),
    sizeof(((lcb_RESPCOUNTER *)0)->cookie),
    offsetof(lcb_RESPCOUNTER, key),
    sizeof(((lcb_RESPCOUNTER *)0)->key),
    offsetof(lcb_RESPCOUNTER, nkey),
    sizeof(((lcb_RESPCOUNTER *)0)->nkey),
    offsetof(lcb_RESPCOUNTER, cas),
    sizeof(((lcb_RESPCOUNTER *)0)->cas),
    offsetof(lcb_RESPCOUNTER, rc),
    sizeof(((lcb_RESPCOUNTER *)0)->rc),
    offsetof(lcb_RESPCOUNTER, version),
    sizeof(((lcb_RESPCOUNTER *)0)->version),
    offsetof(lcb_RESPCOUNTER, rflags),
    sizeof(((lcb_RESPCOUNTER *)0)->rflags),
    offsetof(lcb_RESPCOUNTER, value),
    sizeof(((lcb_RESPCOUNTER *)0)->value),
    offsetof(lcb_RESPCOUNTER, synctoken),
    sizeof(((lcb_RESPCOUNTER *)0)->synctoken),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_RESPCOUNTER(0);
}

static void _cffi_check__lcb_RESPENDURE(lcb_RESPENDURE *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_error_t *tmp = &p->rc; (void)tmp; }
  (void)((p->version) << 1);
  (void)((p->rflags) << 1);
  (void)((p->nresponses) << 1);
  (void)((p->exists_master) << 1);
  (void)((p->persisted_master) << 1);
  (void)((p->npersisted) << 1);
  (void)((p->nreplicated) << 1);
}
static PyObject *
_cffi_layout__lcb_RESPENDURE(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_RESPENDURE y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_RESPENDURE),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_RESPENDURE, cookie),
    sizeof(((lcb_RESPENDURE *)0)->cookie),
    offsetof(lcb_RESPENDURE, key),
    sizeof(((lcb_RESPENDURE *)0)->key),
    offsetof(lcb_RESPENDURE, nkey),
    sizeof(((lcb_RESPENDURE *)0)->nkey),
    offsetof(lcb_RESPENDURE, cas),
    sizeof(((lcb_RESPENDURE *)0)->cas),
    offsetof(lcb_RESPENDURE, rc),
    sizeof(((lcb_RESPENDURE *)0)->rc),
    offsetof(lcb_RESPENDURE, version),
    sizeof(((lcb_RESPENDURE *)0)->version),
    offsetof(lcb_RESPENDURE, rflags),
    sizeof(((lcb_RESPENDURE *)0)->rflags),
    offsetof(lcb_RESPENDURE, nresponses),
    sizeof(((lcb_RESPENDURE *)0)->nresponses),
    offsetof(lcb_RESPENDURE, exists_master),
    sizeof(((lcb_RESPENDURE *)0)->exists_master),
    offsetof(lcb_RESPENDURE, persisted_master),
    sizeof(((lcb_RESPENDURE *)0)->persisted_master),
    offsetof(lcb_RESPENDURE, npersisted),
    sizeof(((lcb_RESPENDURE *)0)->npersisted),
    offsetof(lcb_RESPENDURE, nreplicated),
    sizeof(((lcb_RESPENDURE *)0)->nreplicated),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_RESPENDURE(0);
}

static int _cffi_e__lcb_RESPFLAGS(PyObject *lib)
{
  if ((LCB_RESP_F_FINAL) <= 0 || (unsigned long)(LCB_RESP_F_FINAL) != 1UL) {
    char buf[64];
    if ((LCB_RESP_F_FINAL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_RESP_F_FINAL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_RESP_F_FINAL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_RESPFLAGS: ", "LCB_RESP_F_FINAL", buf, "1");
    return -1;
  }
  if ((LCB_RESP_F_CLIENTGEN) <= 0 || (unsigned long)(LCB_RESP_F_CLIENTGEN) != 2UL) {
    char buf[64];
    if ((LCB_RESP_F_CLIENTGEN) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_RESP_F_CLIENTGEN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_RESP_F_CLIENTGEN));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_RESPFLAGS: ", "LCB_RESP_F_CLIENTGEN", buf, "2");
    return -1;
  }
  if ((LCB_RESP_F_NMVGEN) <= 0 || (unsigned long)(LCB_RESP_F_NMVGEN) != 4UL) {
    char buf[64];
    if ((LCB_RESP_F_NMVGEN) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_RESP_F_NMVGEN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_RESP_F_NMVGEN));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_RESPFLAGS: ", "LCB_RESP_F_NMVGEN", buf, "4");
    return -1;
  }
  return _cffi_e__lcb_KVBUFTYPE(lib);
}

static void _cffi_check__lcb_RESPGET(lcb_RESPGET *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_error_t *tmp = &p->rc; (void)tmp; }
  (void)((p->version) << 1);
  (void)((p->rflags) << 1);
  { void const * *tmp = &p->value; (void)tmp; }
  (void)((p->nvalue) << 1);
  { void * *tmp = &p->bufh; (void)tmp; }
  (void)((p->datatype) << 1);
  (void)((p->itmflags) << 1);
}
static PyObject *
_cffi_layout__lcb_RESPGET(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_RESPGET y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_RESPGET),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_RESPGET, cookie),
    sizeof(((lcb_RESPGET *)0)->cookie),
    offsetof(lcb_RESPGET, key),
    sizeof(((lcb_RESPGET *)0)->key),
    offsetof(lcb_RESPGET, nkey),
    sizeof(((lcb_RESPGET *)0)->nkey),
    offsetof(lcb_RESPGET, cas),
    sizeof(((lcb_RESPGET *)0)->cas),
    offsetof(lcb_RESPGET, rc),
    sizeof(((lcb_RESPGET *)0)->rc),
    offsetof(lcb_RESPGET, version),
    sizeof(((lcb_RESPGET *)0)->version),
    offsetof(lcb_RESPGET, rflags),
    sizeof(((lcb_RESPGET *)0)->rflags),
    offsetof(lcb_RESPGET, value),
    sizeof(((lcb_RESPGET *)0)->value),
    offsetof(lcb_RESPGET, nvalue),
    sizeof(((lcb_RESPGET *)0)->nvalue),
    offsetof(lcb_RESPGET, bufh),
    sizeof(((lcb_RESPGET *)0)->bufh),
    offsetof(lcb_RESPGET, datatype),
    sizeof(((lcb_RESPGET *)0)->datatype),
    offsetof(lcb_RESPGET, itmflags),
    sizeof(((lcb_RESPGET *)0)->itmflags),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_RESPGET(0);
}

static void _cffi_check__lcb_RESPHTTP(lcb_RESPHTTP *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_error_t *tmp = &p->rc; (void)tmp; }
  (void)((p->version) << 1);
  (void)((p->rflags) << 1);
  (void)((p->htstatus) << 1);
  { char const * * *tmp = &p->headers; (void)tmp; }
  { void const * *tmp = &p->body; (void)tmp; }
  (void)((p->nbody) << 1);
  { struct lcb_http_request_st * *tmp = &p->_htreq; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_RESPHTTP(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_RESPHTTP y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_RESPHTTP),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_RESPHTTP, cookie),
    sizeof(((lcb_RESPHTTP *)0)->cookie),
    offsetof(lcb_RESPHTTP, key),
    sizeof(((lcb_RESPHTTP *)0)->key),
    offsetof(lcb_RESPHTTP, nkey),
    sizeof(((lcb_RESPHTTP *)0)->nkey),
    offsetof(lcb_RESPHTTP, cas),
    sizeof(((lcb_RESPHTTP *)0)->cas),
    offsetof(lcb_RESPHTTP, rc),
    sizeof(((lcb_RESPHTTP *)0)->rc),
    offsetof(lcb_RESPHTTP, version),
    sizeof(((lcb_RESPHTTP *)0)->version),
    offsetof(lcb_RESPHTTP, rflags),
    sizeof(((lcb_RESPHTTP *)0)->rflags),
    offsetof(lcb_RESPHTTP, htstatus),
    sizeof(((lcb_RESPHTTP *)0)->htstatus),
    offsetof(lcb_RESPHTTP, headers),
    sizeof(((lcb_RESPHTTP *)0)->headers),
    offsetof(lcb_RESPHTTP, body),
    sizeof(((lcb_RESPHTTP *)0)->body),
    offsetof(lcb_RESPHTTP, nbody),
    sizeof(((lcb_RESPHTTP *)0)->nbody),
    offsetof(lcb_RESPHTTP, _htreq),
    sizeof(((lcb_RESPHTTP *)0)->_htreq),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_RESPHTTP(0);
}

static void _cffi_check__lcb_RESPMCVERSION(lcb_RESPMCVERSION *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_error_t *tmp = &p->rc; (void)tmp; }
  (void)((p->version) << 1);
  (void)((p->rflags) << 1);
  { char const * *tmp = &p->server; (void)tmp; }
  { char const * *tmp = &p->mcversion; (void)tmp; }
  (void)((p->nversion) << 1);
}
static PyObject *
_cffi_layout__lcb_RESPMCVERSION(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_RESPMCVERSION y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_RESPMCVERSION),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_RESPMCVERSION, cookie),
    sizeof(((lcb_RESPMCVERSION *)0)->cookie),
    offsetof(lcb_RESPMCVERSION, key),
    sizeof(((lcb_RESPMCVERSION *)0)->key),
    offsetof(lcb_RESPMCVERSION, nkey),
    sizeof(((lcb_RESPMCVERSION *)0)->nkey),
    offsetof(lcb_RESPMCVERSION, cas),
    sizeof(((lcb_RESPMCVERSION *)0)->cas),
    offsetof(lcb_RESPMCVERSION, rc),
    sizeof(((lcb_RESPMCVERSION *)0)->rc),
    offsetof(lcb_RESPMCVERSION, version),
    sizeof(((lcb_RESPMCVERSION *)0)->version),
    offsetof(lcb_RESPMCVERSION, rflags),
    sizeof(((lcb_RESPMCVERSION *)0)->rflags),
    offsetof(lcb_RESPMCVERSION, server),
    sizeof(((lcb_RESPMCVERSION *)0)->server),
    offsetof(lcb_RESPMCVERSION, mcversion),
    sizeof(((lcb_RESPMCVERSION *)0)->mcversion),
    offsetof(lcb_RESPMCVERSION, nversion),
    sizeof(((lcb_RESPMCVERSION *)0)->nversion),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_RESPMCVERSION(0);
}

static void _cffi_check__lcb_RESPOBSEQNO(lcb_RESPOBSEQNO *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_error_t *tmp = &p->rc; (void)tmp; }
  (void)((p->version) << 1);
  (void)((p->rflags) << 1);
  (void)((p->vbid) << 1);
  (void)((p->server_index) << 1);
  (void)((p->cur_uuid) << 1);
  (void)((p->persisted_seqno) << 1);
  (void)((p->mem_seqno) << 1);
  (void)((p->old_uuid) << 1);
  (void)((p->old_seqno) << 1);
}
static PyObject *
_cffi_layout__lcb_RESPOBSEQNO(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_RESPOBSEQNO y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_RESPOBSEQNO),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_RESPOBSEQNO, cookie),
    sizeof(((lcb_RESPOBSEQNO *)0)->cookie),
    offsetof(lcb_RESPOBSEQNO, key),
    sizeof(((lcb_RESPOBSEQNO *)0)->key),
    offsetof(lcb_RESPOBSEQNO, nkey),
    sizeof(((lcb_RESPOBSEQNO *)0)->nkey),
    offsetof(lcb_RESPOBSEQNO, cas),
    sizeof(((lcb_RESPOBSEQNO *)0)->cas),
    offsetof(lcb_RESPOBSEQNO, rc),
    sizeof(((lcb_RESPOBSEQNO *)0)->rc),
    offsetof(lcb_RESPOBSEQNO, version),
    sizeof(((lcb_RESPOBSEQNO *)0)->version),
    offsetof(lcb_RESPOBSEQNO, rflags),
    sizeof(((lcb_RESPOBSEQNO *)0)->rflags),
    offsetof(lcb_RESPOBSEQNO, vbid),
    sizeof(((lcb_RESPOBSEQNO *)0)->vbid),
    offsetof(lcb_RESPOBSEQNO, server_index),
    sizeof(((lcb_RESPOBSEQNO *)0)->server_index),
    offsetof(lcb_RESPOBSEQNO, cur_uuid),
    sizeof(((lcb_RESPOBSEQNO *)0)->cur_uuid),
    offsetof(lcb_RESPOBSEQNO, persisted_seqno),
    sizeof(((lcb_RESPOBSEQNO *)0)->persisted_seqno),
    offsetof(lcb_RESPOBSEQNO, mem_seqno),
    sizeof(((lcb_RESPOBSEQNO *)0)->mem_seqno),
    offsetof(lcb_RESPOBSEQNO, old_uuid),
    sizeof(((lcb_RESPOBSEQNO *)0)->old_uuid),
    offsetof(lcb_RESPOBSEQNO, old_seqno),
    sizeof(((lcb_RESPOBSEQNO *)0)->old_seqno),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_RESPOBSEQNO(0);
}

static void _cffi_check__lcb_RESPOBSERVE(lcb_RESPOBSERVE *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_error_t *tmp = &p->rc; (void)tmp; }
  (void)((p->version) << 1);
  (void)((p->rflags) << 1);
  (void)((p->status) << 1);
  (void)((p->ismaster) << 1);
  (void)((p->ttp) << 1);
  (void)((p->ttr) << 1);
}
static PyObject *
_cffi_layout__lcb_RESPOBSERVE(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_RESPOBSERVE y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_RESPOBSERVE),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_RESPOBSERVE, cookie),
    sizeof(((lcb_RESPOBSERVE *)0)->cookie),
    offsetof(lcb_RESPOBSERVE, key),
    sizeof(((lcb_RESPOBSERVE *)0)->key),
    offsetof(lcb_RESPOBSERVE, nkey),
    sizeof(((lcb_RESPOBSERVE *)0)->nkey),
    offsetof(lcb_RESPOBSERVE, cas),
    sizeof(((lcb_RESPOBSERVE *)0)->cas),
    offsetof(lcb_RESPOBSERVE, rc),
    sizeof(((lcb_RESPOBSERVE *)0)->rc),
    offsetof(lcb_RESPOBSERVE, version),
    sizeof(((lcb_RESPOBSERVE *)0)->version),
    offsetof(lcb_RESPOBSERVE, rflags),
    sizeof(((lcb_RESPOBSERVE *)0)->rflags),
    offsetof(lcb_RESPOBSERVE, status),
    sizeof(((lcb_RESPOBSERVE *)0)->status),
    offsetof(lcb_RESPOBSERVE, ismaster),
    sizeof(((lcb_RESPOBSERVE *)0)->ismaster),
    offsetof(lcb_RESPOBSERVE, ttp),
    sizeof(((lcb_RESPOBSERVE *)0)->ttp),
    offsetof(lcb_RESPOBSERVE, ttr),
    sizeof(((lcb_RESPOBSERVE *)0)->ttr),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_RESPOBSERVE(0);
}

static void _cffi_check__lcb_RESPREMOVE(lcb_RESPREMOVE *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_error_t *tmp = &p->rc; (void)tmp; }
  (void)((p->version) << 1);
  (void)((p->rflags) << 1);
  { lcb_SYNCTOKEN *tmp = &p->synctoken; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_RESPREMOVE(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_RESPREMOVE y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_RESPREMOVE),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_RESPREMOVE, cookie),
    sizeof(((lcb_RESPREMOVE *)0)->cookie),
    offsetof(lcb_RESPREMOVE, key),
    sizeof(((lcb_RESPREMOVE *)0)->key),
    offsetof(lcb_RESPREMOVE, nkey),
    sizeof(((lcb_RESPREMOVE *)0)->nkey),
    offsetof(lcb_RESPREMOVE, cas),
    sizeof(((lcb_RESPREMOVE *)0)->cas),
    offsetof(lcb_RESPREMOVE, rc),
    sizeof(((lcb_RESPREMOVE *)0)->rc),
    offsetof(lcb_RESPREMOVE, version),
    sizeof(((lcb_RESPREMOVE *)0)->version),
    offsetof(lcb_RESPREMOVE, rflags),
    sizeof(((lcb_RESPREMOVE *)0)->rflags),
    offsetof(lcb_RESPREMOVE, synctoken),
    sizeof(((lcb_RESPREMOVE *)0)->synctoken),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_RESPREMOVE(0);
}

static void _cffi_check__lcb_RESPSERVERBASE(lcb_RESPSERVERBASE *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_error_t *tmp = &p->rc; (void)tmp; }
  (void)((p->version) << 1);
  (void)((p->rflags) << 1);
  { char const * *tmp = &p->server; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_RESPSERVERBASE(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_RESPSERVERBASE y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_RESPSERVERBASE),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_RESPSERVERBASE, cookie),
    sizeof(((lcb_RESPSERVERBASE *)0)->cookie),
    offsetof(lcb_RESPSERVERBASE, key),
    sizeof(((lcb_RESPSERVERBASE *)0)->key),
    offsetof(lcb_RESPSERVERBASE, nkey),
    sizeof(((lcb_RESPSERVERBASE *)0)->nkey),
    offsetof(lcb_RESPSERVERBASE, cas),
    sizeof(((lcb_RESPSERVERBASE *)0)->cas),
    offsetof(lcb_RESPSERVERBASE, rc),
    sizeof(((lcb_RESPSERVERBASE *)0)->rc),
    offsetof(lcb_RESPSERVERBASE, version),
    sizeof(((lcb_RESPSERVERBASE *)0)->version),
    offsetof(lcb_RESPSERVERBASE, rflags),
    sizeof(((lcb_RESPSERVERBASE *)0)->rflags),
    offsetof(lcb_RESPSERVERBASE, server),
    sizeof(((lcb_RESPSERVERBASE *)0)->server),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_RESPSERVERBASE(0);
}

static void _cffi_check__lcb_RESPSTATS(lcb_RESPSTATS *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_error_t *tmp = &p->rc; (void)tmp; }
  (void)((p->version) << 1);
  (void)((p->rflags) << 1);
  { char const * *tmp = &p->server; (void)tmp; }
  { char const * *tmp = &p->value; (void)tmp; }
  (void)((p->nvalue) << 1);
}
static PyObject *
_cffi_layout__lcb_RESPSTATS(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_RESPSTATS y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_RESPSTATS),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_RESPSTATS, cookie),
    sizeof(((lcb_RESPSTATS *)0)->cookie),
    offsetof(lcb_RESPSTATS, key),
    sizeof(((lcb_RESPSTATS *)0)->key),
    offsetof(lcb_RESPSTATS, nkey),
    sizeof(((lcb_RESPSTATS *)0)->nkey),
    offsetof(lcb_RESPSTATS, cas),
    sizeof(((lcb_RESPSTATS *)0)->cas),
    offsetof(lcb_RESPSTATS, rc),
    sizeof(((lcb_RESPSTATS *)0)->rc),
    offsetof(lcb_RESPSTATS, version),
    sizeof(((lcb_RESPSTATS *)0)->version),
    offsetof(lcb_RESPSTATS, rflags),
    sizeof(((lcb_RESPSTATS *)0)->rflags),
    offsetof(lcb_RESPSTATS, server),
    sizeof(((lcb_RESPSTATS *)0)->server),
    offsetof(lcb_RESPSTATS, value),
    sizeof(((lcb_RESPSTATS *)0)->value),
    offsetof(lcb_RESPSTATS, nvalue),
    sizeof(((lcb_RESPSTATS *)0)->nvalue),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_RESPSTATS(0);
}

static void _cffi_check__lcb_RESPSTORE(lcb_RESPSTORE *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_error_t *tmp = &p->rc; (void)tmp; }
  (void)((p->version) << 1);
  (void)((p->rflags) << 1);
  { lcb_storage_t *tmp = &p->op; (void)tmp; }
  { lcb_SYNCTOKEN *tmp = &p->synctoken; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_RESPSTORE(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_RESPSTORE y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_RESPSTORE),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_RESPSTORE, cookie),
    sizeof(((lcb_RESPSTORE *)0)->cookie),
    offsetof(lcb_RESPSTORE, key),
    sizeof(((lcb_RESPSTORE *)0)->key),
    offsetof(lcb_RESPSTORE, nkey),
    sizeof(((lcb_RESPSTORE *)0)->nkey),
    offsetof(lcb_RESPSTORE, cas),
    sizeof(((lcb_RESPSTORE *)0)->cas),
    offsetof(lcb_RESPSTORE, rc),
    sizeof(((lcb_RESPSTORE *)0)->rc),
    offsetof(lcb_RESPSTORE, version),
    sizeof(((lcb_RESPSTORE *)0)->version),
    offsetof(lcb_RESPSTORE, rflags),
    sizeof(((lcb_RESPSTORE *)0)->rflags),
    offsetof(lcb_RESPSTORE, op),
    sizeof(((lcb_RESPSTORE *)0)->op),
    offsetof(lcb_RESPSTORE, synctoken),
    sizeof(((lcb_RESPSTORE *)0)->synctoken),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_RESPSTORE(0);
}

static int _cffi_e__lcb_RETRYCMDOPTS(PyObject *lib)
{
  if ((LCB_RETRY_CMDS_NONE) > 0 || (long)(LCB_RETRY_CMDS_NONE) != 0L) {
    char buf[64];
    if ((LCB_RETRY_CMDS_NONE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_RETRY_CMDS_NONE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_RETRY_CMDS_NONE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_RETRYCMDOPTS: ", "LCB_RETRY_CMDS_NONE", buf, "0");
    return -1;
  }
  if ((LCB_RETRY_CMDS_GET) <= 0 || (unsigned long)(LCB_RETRY_CMDS_GET) != 1UL) {
    char buf[64];
    if ((LCB_RETRY_CMDS_GET) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_RETRY_CMDS_GET));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_RETRY_CMDS_GET));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_RETRYCMDOPTS: ", "LCB_RETRY_CMDS_GET", buf, "1");
    return -1;
  }
  if ((LCB_RETRY_CMDS_SAFE) <= 0 || (unsigned long)(LCB_RETRY_CMDS_SAFE) != 3UL) {
    char buf[64];
    if ((LCB_RETRY_CMDS_SAFE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_RETRY_CMDS_SAFE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_RETRY_CMDS_SAFE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_RETRYCMDOPTS: ", "LCB_RETRY_CMDS_SAFE", buf, "3");
    return -1;
  }
  if ((LCB_RETRY_CMDS_ALL) <= 0 || (unsigned long)(LCB_RETRY_CMDS_ALL) != 7UL) {
    char buf[64];
    if ((LCB_RETRY_CMDS_ALL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_RETRY_CMDS_ALL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_RETRY_CMDS_ALL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_RETRYCMDOPTS: ", "LCB_RETRY_CMDS_ALL", buf, "7");
    return -1;
  }
  return _cffi_e__lcb_RESPFLAGS(lib);
}

static int _cffi_e__lcb_RETRYMODEOPTS(PyObject *lib)
{
  if ((LCB_RETRY_ON_TOPOCHANGE) > 0 || (long)(LCB_RETRY_ON_TOPOCHANGE) != 0L) {
    char buf[64];
    if ((LCB_RETRY_ON_TOPOCHANGE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_RETRY_ON_TOPOCHANGE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_RETRY_ON_TOPOCHANGE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_RETRYMODEOPTS: ", "LCB_RETRY_ON_TOPOCHANGE", buf, "0");
    return -1;
  }
  if ((LCB_RETRY_ON_SOCKERR) <= 0 || (unsigned long)(LCB_RETRY_ON_SOCKERR) != 1UL) {
    char buf[64];
    if ((LCB_RETRY_ON_SOCKERR) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_RETRY_ON_SOCKERR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_RETRY_ON_SOCKERR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_RETRYMODEOPTS: ", "LCB_RETRY_ON_SOCKERR", buf, "1");
    return -1;
  }
  if ((LCB_RETRY_ON_VBMAPERR) <= 0 || (unsigned long)(LCB_RETRY_ON_VBMAPERR) != 2UL) {
    char buf[64];
    if ((LCB_RETRY_ON_VBMAPERR) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_RETRY_ON_VBMAPERR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_RETRY_ON_VBMAPERR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_RETRYMODEOPTS: ", "LCB_RETRY_ON_VBMAPERR", buf, "2");
    return -1;
  }
  if ((LCB_RETRY_ON_MISSINGNODE) <= 0 || (unsigned long)(LCB_RETRY_ON_MISSINGNODE) != 3UL) {
    char buf[64];
    if ((LCB_RETRY_ON_MISSINGNODE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_RETRY_ON_MISSINGNODE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_RETRY_ON_MISSINGNODE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_RETRYMODEOPTS: ", "LCB_RETRY_ON_MISSINGNODE", buf, "3");
    return -1;
  }
  if ((LCB_RETRY_ON_MAX) <= 0 || (unsigned long)(LCB_RETRY_ON_MAX) != 4UL) {
    char buf[64];
    if ((LCB_RETRY_ON_MAX) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_RETRY_ON_MAX));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_RETRY_ON_MAX));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_RETRYMODEOPTS: ", "LCB_RETRY_ON_MAX", buf, "4");
    return -1;
  }
  return _cffi_e__lcb_RETRYCMDOPTS(lib);
}

static int _cffi_e__lcb_SSLOPTS(PyObject *lib)
{
  if ((LCB_SSL_ENABLED) <= 0 || (unsigned long)(LCB_SSL_ENABLED) != 1UL) {
    char buf[64];
    if ((LCB_SSL_ENABLED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_SSL_ENABLED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_SSL_ENABLED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_SSLOPTS: ", "LCB_SSL_ENABLED", buf, "1");
    return -1;
  }
  if ((LCB_SSL_NOVERIFY) <= 0 || (unsigned long)(LCB_SSL_NOVERIFY) != 2UL) {
    char buf[64];
    if ((LCB_SSL_NOVERIFY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_SSL_NOVERIFY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_SSL_NOVERIFY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_SSLOPTS: ", "LCB_SSL_NOVERIFY", buf, "2");
    return -1;
  }
  return _cffi_e__lcb_RETRYMODEOPTS(lib);
}

static void _cffi_check__lcb_STATSCMDv0(lcb_STATSCMDv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->name; (void)tmp; }
  (void)((p->nname) << 1);
}
static PyObject *
_cffi_layout__lcb_STATSCMDv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_STATSCMDv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_STATSCMDv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_STATSCMDv0, name),
    sizeof(((lcb_STATSCMDv0 *)0)->name),
    offsetof(lcb_STATSCMDv0, nname),
    sizeof(((lcb_STATSCMDv0 *)0)->nname),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_STATSCMDv0(0);
}

static void _cffi_check__lcb_STATSRESPv0(lcb_STATSRESPv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { char const * *tmp = &p->server_endpoint; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  { void const * *tmp = &p->bytes; (void)tmp; }
  (void)((p->nbytes) << 1);
}
static PyObject *
_cffi_layout__lcb_STATSRESPv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_STATSRESPv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_STATSRESPv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_STATSRESPv0, server_endpoint),
    sizeof(((lcb_STATSRESPv0 *)0)->server_endpoint),
    offsetof(lcb_STATSRESPv0, key),
    sizeof(((lcb_STATSRESPv0 *)0)->key),
    offsetof(lcb_STATSRESPv0, nkey),
    sizeof(((lcb_STATSRESPv0 *)0)->nkey),
    offsetof(lcb_STATSRESPv0, bytes),
    sizeof(((lcb_STATSRESPv0 *)0)->bytes),
    offsetof(lcb_STATSRESPv0, nbytes),
    sizeof(((lcb_STATSRESPv0 *)0)->nbytes),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_STATSRESPv0(0);
}

static void _cffi_check__lcb_STORECMDv0(lcb_STORECMDv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  { void const * *tmp = &p->bytes; (void)tmp; }
  (void)((p->nbytes) << 1);
  (void)((p->flags) << 1);
  (void)((p->cas) << 1);
  (void)((p->datatype) << 1);
  (void)((p->exptime) << 1);
  { lcb_storage_t *tmp = &p->operation; (void)tmp; }
  { void const * *tmp = &p->hashkey; (void)tmp; }
  (void)((p->nhashkey) << 1);
}
static PyObject *
_cffi_layout__lcb_STORECMDv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_STORECMDv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_STORECMDv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_STORECMDv0, key),
    sizeof(((lcb_STORECMDv0 *)0)->key),
    offsetof(lcb_STORECMDv0, nkey),
    sizeof(((lcb_STORECMDv0 *)0)->nkey),
    offsetof(lcb_STORECMDv0, bytes),
    sizeof(((lcb_STORECMDv0 *)0)->bytes),
    offsetof(lcb_STORECMDv0, nbytes),
    sizeof(((lcb_STORECMDv0 *)0)->nbytes),
    offsetof(lcb_STORECMDv0, flags),
    sizeof(((lcb_STORECMDv0 *)0)->flags),
    offsetof(lcb_STORECMDv0, cas),
    sizeof(((lcb_STORECMDv0 *)0)->cas),
    offsetof(lcb_STORECMDv0, datatype),
    sizeof(((lcb_STORECMDv0 *)0)->datatype),
    offsetof(lcb_STORECMDv0, exptime),
    sizeof(((lcb_STORECMDv0 *)0)->exptime),
    offsetof(lcb_STORECMDv0, operation),
    sizeof(((lcb_STORECMDv0 *)0)->operation),
    offsetof(lcb_STORECMDv0, hashkey),
    sizeof(((lcb_STORECMDv0 *)0)->hashkey),
    offsetof(lcb_STORECMDv0, nhashkey),
    sizeof(((lcb_STORECMDv0 *)0)->nhashkey),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_STORECMDv0(0);
}

static void _cffi_check__lcb_STORERESPv0(lcb_STORERESPv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_SYNCTOKEN const * *tmp = &p->synctoken; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_STORERESPv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_STORERESPv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_STORERESPv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_STORERESPv0, key),
    sizeof(((lcb_STORERESPv0 *)0)->key),
    offsetof(lcb_STORERESPv0, nkey),
    sizeof(((lcb_STORERESPv0 *)0)->nkey),
    offsetof(lcb_STORERESPv0, cas),
    sizeof(((lcb_STORERESPv0 *)0)->cas),
    offsetof(lcb_STORERESPv0, synctoken),
    sizeof(((lcb_STORERESPv0 *)0)->synctoken),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_STORERESPv0(0);
}

static void _cffi_check__lcb_SYNCTOKEN(lcb_SYNCTOKEN *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->uuid_) << 1);
  (void)((p->seqno_) << 1);
  (void)((p->vbid_) << 1);
}
static PyObject *
_cffi_layout__lcb_SYNCTOKEN(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_SYNCTOKEN y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_SYNCTOKEN),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_SYNCTOKEN, uuid_),
    sizeof(((lcb_SYNCTOKEN *)0)->uuid_),
    offsetof(lcb_SYNCTOKEN, seqno_),
    sizeof(((lcb_SYNCTOKEN *)0)->seqno_),
    offsetof(lcb_SYNCTOKEN, vbid_),
    sizeof(((lcb_SYNCTOKEN *)0)->vbid_),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_SYNCTOKEN(0);
}

static void _cffi_check__lcb_TOUCHRESPv0(lcb_TOUCHRESPv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
}
static PyObject *
_cffi_layout__lcb_TOUCHRESPv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_TOUCHRESPv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_TOUCHRESPv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_TOUCHRESPv0, key),
    sizeof(((lcb_TOUCHRESPv0 *)0)->key),
    offsetof(lcb_TOUCHRESPv0, nkey),
    sizeof(((lcb_TOUCHRESPv0 *)0)->nkey),
    offsetof(lcb_TOUCHRESPv0, cas),
    sizeof(((lcb_TOUCHRESPv0 *)0)->cas),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_TOUCHRESPv0(0);
}

static void _cffi_check__lcb_UNLOCKCMDv0(lcb_UNLOCKCMDv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { void const * *tmp = &p->hashkey; (void)tmp; }
  (void)((p->nhashkey) << 1);
}
static PyObject *
_cffi_layout__lcb_UNLOCKCMDv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_UNLOCKCMDv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_UNLOCKCMDv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_UNLOCKCMDv0, key),
    sizeof(((lcb_UNLOCKCMDv0 *)0)->key),
    offsetof(lcb_UNLOCKCMDv0, nkey),
    sizeof(((lcb_UNLOCKCMDv0 *)0)->nkey),
    offsetof(lcb_UNLOCKCMDv0, cas),
    sizeof(((lcb_UNLOCKCMDv0 *)0)->cas),
    offsetof(lcb_UNLOCKCMDv0, hashkey),
    sizeof(((lcb_UNLOCKCMDv0 *)0)->hashkey),
    offsetof(lcb_UNLOCKCMDv0, nhashkey),
    sizeof(((lcb_UNLOCKCMDv0 *)0)->nhashkey),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_UNLOCKCMDv0(0);
}

static void _cffi_check__lcb_UNLOCKRESPv0(lcb_UNLOCKRESPv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
}
static PyObject *
_cffi_layout__lcb_UNLOCKRESPv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_UNLOCKRESPv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_UNLOCKRESPv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_UNLOCKRESPv0, key),
    sizeof(((lcb_UNLOCKRESPv0 *)0)->key),
    offsetof(lcb_UNLOCKRESPv0, nkey),
    sizeof(((lcb_UNLOCKRESPv0 *)0)->nkey),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_UNLOCKRESPv0(0);
}

static int _cffi_e__lcb_VALUEFLAGS(PyObject *lib)
{
  if ((LCB_VALUE_RAW) > 0 || (long)(LCB_VALUE_RAW) != 0L) {
    char buf[64];
    if ((LCB_VALUE_RAW) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_VALUE_RAW));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_VALUE_RAW));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_VALUEFLAGS: ", "LCB_VALUE_RAW", buf, "0");
    return -1;
  }
  if ((LCB_VALUE_F_JSON) <= 0 || (unsigned long)(LCB_VALUE_F_JSON) != 1UL) {
    char buf[64];
    if ((LCB_VALUE_F_JSON) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_VALUE_F_JSON));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_VALUE_F_JSON));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_VALUEFLAGS: ", "LCB_VALUE_F_JSON", buf, "1");
    return -1;
  }
  if ((LCB_VALUE_F_SNAPPYCOMP) <= 0 || (unsigned long)(LCB_VALUE_F_SNAPPYCOMP) != 2UL) {
    char buf[64];
    if ((LCB_VALUE_F_SNAPPYCOMP) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_VALUE_F_SNAPPYCOMP));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_VALUE_F_SNAPPYCOMP));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_VALUEFLAGS: ", "LCB_VALUE_F_SNAPPYCOMP", buf, "2");
    return -1;
  }
  return _cffi_e__lcb_SSLOPTS(lib);
}

static void _cffi_check__lcb_VERBOSITYCMDv0(lcb_VERBOSITYCMDv0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { char const * *tmp = &p->server; (void)tmp; }
  { lcb_verbosity_level_t *tmp = &p->level; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_VERBOSITYCMDv0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_VERBOSITYCMDv0 y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_VERBOSITYCMDv0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_VERBOSITYCMDv0, server),
    sizeof(((lcb_VERBOSITYCMDv0 *)0)->server),
    offsetof(lcb_VERBOSITYCMDv0, level),
    sizeof(((lcb_VERBOSITYCMDv0 *)0)->level),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_VERBOSITYCMDv0(0);
}

static int _cffi_e__lcb_WAITFLAGS(PyObject *lib)
{
  if ((LCB_WAIT_DEFAULT) > 0 || (long)(LCB_WAIT_DEFAULT) != 0L) {
    char buf[64];
    if ((LCB_WAIT_DEFAULT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_WAIT_DEFAULT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_WAIT_DEFAULT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_WAITFLAGS: ", "LCB_WAIT_DEFAULT", buf, "0");
    return -1;
  }
  if ((LCB_WAIT_NOCHECK) <= 0 || (unsigned long)(LCB_WAIT_NOCHECK) != 1UL) {
    char buf[64];
    if ((LCB_WAIT_NOCHECK) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_WAIT_NOCHECK));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_WAIT_NOCHECK));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_WAITFLAGS: ", "LCB_WAIT_NOCHECK", buf, "1");
    return -1;
  }
  return _cffi_e__lcb_VALUEFLAGS(lib);
}

static int _cffi_e__lcb__STRUCTSIZES(PyObject *lib)
{
  if ((LCB_C_ST_ID) > 0 || (long)(LCB_C_ST_ID) != 0L) {
    char buf[64];
    if ((LCB_C_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_C_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_C_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_C_ST_ID", buf, "0");
    return -1;
  }
  if ((LCB_C_ST_V) <= 0 || (unsigned long)(LCB_C_ST_V) != 3UL) {
    char buf[64];
    if ((LCB_C_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_C_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_C_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_C_ST_V", buf, "3");
    return -1;
  }
  if ((LCB_C_I_O_ST_ID) <= 0 || (unsigned long)(LCB_C_I_O_ST_ID) != 1UL) {
    char buf[64];
    if ((LCB_C_I_O_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_C_I_O_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_C_I_O_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_C_I_O_ST_ID", buf, "1");
    return -1;
  }
  if ((LCB_C_I_O_ST_V) <= 0 || (unsigned long)(LCB_C_I_O_ST_V) != 1UL) {
    char buf[64];
    if ((LCB_C_I_O_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_C_I_O_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_C_I_O_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_C_I_O_ST_V", buf, "1");
    return -1;
  }
  if ((LCB_G_C_ST_ID) <= 0 || (unsigned long)(LCB_G_C_ST_ID) != 2UL) {
    char buf[64];
    if ((LCB_G_C_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_G_C_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_G_C_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_G_C_ST_ID", buf, "2");
    return -1;
  }
  if ((LCB_G_C_ST_V) > 0 || (long)(LCB_G_C_ST_V) != 0L) {
    char buf[64];
    if ((LCB_G_C_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_G_C_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_G_C_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_G_C_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_G_R_C_ST_ID) <= 0 || (unsigned long)(LCB_G_R_C_ST_ID) != 3UL) {
    char buf[64];
    if ((LCB_G_R_C_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_G_R_C_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_G_R_C_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_G_R_C_ST_ID", buf, "3");
    return -1;
  }
  if ((LCB_G_R_C_ST_V) <= 0 || (unsigned long)(LCB_G_R_C_ST_V) != 1UL) {
    char buf[64];
    if ((LCB_G_R_C_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_G_R_C_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_G_R_C_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_G_R_C_ST_V", buf, "1");
    return -1;
  }
  if ((LCB_U_C_ST_ID) <= 0 || (unsigned long)(LCB_U_C_ST_ID) != 4UL) {
    char buf[64];
    if ((LCB_U_C_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_U_C_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_U_C_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_U_C_ST_ID", buf, "4");
    return -1;
  }
  if ((LCB_U_C_ST_V) > 0 || (long)(LCB_U_C_ST_V) != 0L) {
    char buf[64];
    if ((LCB_U_C_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_U_C_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_U_C_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_U_C_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_T_C_ST_ID) <= 0 || (unsigned long)(LCB_T_C_ST_ID) != 5UL) {
    char buf[64];
    if ((LCB_T_C_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_T_C_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_T_C_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_T_C_ST_ID", buf, "5");
    return -1;
  }
  if ((LCB_T_C_ST_V) > 0 || (long)(LCB_T_C_ST_V) != 0L) {
    char buf[64];
    if ((LCB_T_C_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_T_C_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_T_C_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_T_C_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_S_C_ST_ID) <= 0 || (unsigned long)(LCB_S_C_ST_ID) != 6UL) {
    char buf[64];
    if ((LCB_S_C_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_S_C_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_S_C_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_S_C_ST_ID", buf, "6");
    return -1;
  }
  if ((LCB_S_C_ST_V) > 0 || (long)(LCB_S_C_ST_V) != 0L) {
    char buf[64];
    if ((LCB_S_C_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_S_C_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_S_C_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_S_C_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_A_C_ST_ID) <= 0 || (unsigned long)(LCB_A_C_ST_ID) != 7UL) {
    char buf[64];
    if ((LCB_A_C_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_A_C_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_A_C_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_A_C_ST_ID", buf, "7");
    return -1;
  }
  if ((LCB_A_C_ST_V) > 0 || (long)(LCB_A_C_ST_V) != 0L) {
    char buf[64];
    if ((LCB_A_C_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_A_C_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_A_C_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_A_C_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_O_C_ST_ID) <= 0 || (unsigned long)(LCB_O_C_ST_ID) != 8UL) {
    char buf[64];
    if ((LCB_O_C_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_O_C_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_O_C_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_O_C_ST_ID", buf, "8");
    return -1;
  }
  if ((LCB_O_C_ST_V) > 0 || (long)(LCB_O_C_ST_V) != 0L) {
    char buf[64];
    if ((LCB_O_C_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_O_C_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_O_C_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_O_C_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_R_C_ST_ID) <= 0 || (unsigned long)(LCB_R_C_ST_ID) != 9UL) {
    char buf[64];
    if ((LCB_R_C_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_R_C_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_R_C_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_R_C_ST_ID", buf, "9");
    return -1;
  }
  if ((LCB_R_C_ST_V) > 0 || (long)(LCB_R_C_ST_V) != 0L) {
    char buf[64];
    if ((LCB_R_C_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_R_C_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_R_C_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_R_C_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_H_C_ST_ID) <= 0 || (unsigned long)(LCB_H_C_ST_ID) != 10UL) {
    char buf[64];
    if ((LCB_H_C_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_H_C_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_H_C_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_H_C_ST_ID", buf, "10");
    return -1;
  }
  if ((LCB_H_C_ST_V) <= 0 || (unsigned long)(LCB_H_C_ST_V) != 1UL) {
    char buf[64];
    if ((LCB_H_C_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_H_C_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_H_C_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_H_C_ST_V", buf, "1");
    return -1;
  }
  if ((LCB_S_S_C_ST_ID) <= 0 || (unsigned long)(LCB_S_S_C_ST_ID) != 11UL) {
    char buf[64];
    if ((LCB_S_S_C_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_S_S_C_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_S_S_C_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_S_S_C_ST_ID", buf, "11");
    return -1;
  }
  if ((LCB_S_S_C_ST_V) > 0 || (long)(LCB_S_S_C_ST_V) != 0L) {
    char buf[64];
    if ((LCB_S_S_C_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_S_S_C_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_S_S_C_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_S_S_C_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_S_V_C_ST_ID) <= 0 || (unsigned long)(LCB_S_V_C_ST_ID) != 12UL) {
    char buf[64];
    if ((LCB_S_V_C_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_S_V_C_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_S_V_C_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_S_V_C_ST_ID", buf, "12");
    return -1;
  }
  if ((LCB_S_V_C_ST_V) > 0 || (long)(LCB_S_V_C_ST_V) != 0L) {
    char buf[64];
    if ((LCB_S_V_C_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_S_V_C_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_S_V_C_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_S_V_C_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_V_C_ST_ID) <= 0 || (unsigned long)(LCB_V_C_ST_ID) != 13UL) {
    char buf[64];
    if ((LCB_V_C_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_V_C_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_V_C_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_V_C_ST_ID", buf, "13");
    return -1;
  }
  if ((LCB_V_C_ST_V) > 0 || (long)(LCB_V_C_ST_V) != 0L) {
    char buf[64];
    if ((LCB_V_C_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_V_C_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_V_C_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_V_C_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_F_C_ST_ID) <= 0 || (unsigned long)(LCB_F_C_ST_ID) != 14UL) {
    char buf[64];
    if ((LCB_F_C_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_F_C_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_F_C_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_F_C_ST_ID", buf, "14");
    return -1;
  }
  if ((LCB_F_C_ST_V) > 0 || (long)(LCB_F_C_ST_V) != 0L) {
    char buf[64];
    if ((LCB_F_C_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_F_C_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_F_C_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_F_C_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_G_R_ST_ID) <= 0 || (unsigned long)(LCB_G_R_ST_ID) != 15UL) {
    char buf[64];
    if ((LCB_G_R_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_G_R_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_G_R_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_G_R_ST_ID", buf, "15");
    return -1;
  }
  if ((LCB_G_R_ST_V) > 0 || (long)(LCB_G_R_ST_V) != 0L) {
    char buf[64];
    if ((LCB_G_R_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_G_R_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_G_R_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_G_R_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_S_R_ST_ID) <= 0 || (unsigned long)(LCB_S_R_ST_ID) != 16UL) {
    char buf[64];
    if ((LCB_S_R_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_S_R_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_S_R_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_S_R_ST_ID", buf, "16");
    return -1;
  }
  if ((LCB_S_R_ST_V) > 0 || (long)(LCB_S_R_ST_V) != 0L) {
    char buf[64];
    if ((LCB_S_R_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_S_R_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_S_R_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_S_R_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_R_R_ST_ID) <= 0 || (unsigned long)(LCB_R_R_ST_ID) != 17UL) {
    char buf[64];
    if ((LCB_R_R_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_R_R_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_R_R_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_R_R_ST_ID", buf, "17");
    return -1;
  }
  if ((LCB_R_R_ST_V) > 0 || (long)(LCB_R_R_ST_V) != 0L) {
    char buf[64];
    if ((LCB_R_R_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_R_R_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_R_R_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_R_R_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_T_R_ST_ID) <= 0 || (unsigned long)(LCB_T_R_ST_ID) != 18UL) {
    char buf[64];
    if ((LCB_T_R_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_T_R_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_T_R_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_T_R_ST_ID", buf, "18");
    return -1;
  }
  if ((LCB_T_R_ST_V) > 0 || (long)(LCB_T_R_ST_V) != 0L) {
    char buf[64];
    if ((LCB_T_R_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_T_R_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_T_R_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_T_R_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_U_R_ST_ID) <= 0 || (unsigned long)(LCB_U_R_ST_ID) != 19UL) {
    char buf[64];
    if ((LCB_U_R_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_U_R_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_U_R_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_U_R_ST_ID", buf, "19");
    return -1;
  }
  if ((LCB_U_R_ST_V) > 0 || (long)(LCB_U_R_ST_V) != 0L) {
    char buf[64];
    if ((LCB_U_R_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_U_R_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_U_R_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_U_R_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_A_R_ST_ID) <= 0 || (unsigned long)(LCB_A_R_ST_ID) != 20UL) {
    char buf[64];
    if ((LCB_A_R_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_A_R_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_A_R_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_A_R_ST_ID", buf, "20");
    return -1;
  }
  if ((LCB_A_R_ST_V) > 0 || (long)(LCB_A_R_ST_V) != 0L) {
    char buf[64];
    if ((LCB_A_R_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_A_R_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_A_R_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_A_R_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_O_R_ST_ID) <= 0 || (unsigned long)(LCB_O_R_ST_ID) != 21UL) {
    char buf[64];
    if ((LCB_O_R_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_O_R_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_O_R_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_O_R_ST_ID", buf, "21");
    return -1;
  }
  if ((LCB_O_R_ST_V) > 0 || (long)(LCB_O_R_ST_V) != 0L) {
    char buf[64];
    if ((LCB_O_R_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_O_R_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_O_R_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_O_R_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_H_R_ST_ID) <= 0 || (unsigned long)(LCB_H_R_ST_ID) != 22UL) {
    char buf[64];
    if ((LCB_H_R_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_H_R_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_H_R_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_H_R_ST_ID", buf, "22");
    return -1;
  }
  if ((LCB_H_R_ST_V) > 0 || (long)(LCB_H_R_ST_V) != 0L) {
    char buf[64];
    if ((LCB_H_R_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_H_R_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_H_R_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_H_R_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_S_S_R_ST_ID) <= 0 || (unsigned long)(LCB_S_S_R_ST_ID) != 23UL) {
    char buf[64];
    if ((LCB_S_S_R_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_S_S_R_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_S_S_R_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_S_S_R_ST_ID", buf, "23");
    return -1;
  }
  if ((LCB_S_S_R_ST_V) > 0 || (long)(LCB_S_S_R_ST_V) != 0L) {
    char buf[64];
    if ((LCB_S_S_R_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_S_S_R_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_S_S_R_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_S_S_R_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_S_V_R_ST_ID) <= 0 || (unsigned long)(LCB_S_V_R_ST_ID) != 24UL) {
    char buf[64];
    if ((LCB_S_V_R_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_S_V_R_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_S_V_R_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_S_V_R_ST_ID", buf, "24");
    return -1;
  }
  if ((LCB_S_V_R_ST_V) > 0 || (long)(LCB_S_V_R_ST_V) != 0L) {
    char buf[64];
    if ((LCB_S_V_R_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_S_V_R_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_S_V_R_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_S_V_R_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_V_R_ST_ID) <= 0 || (unsigned long)(LCB_V_R_ST_ID) != 25UL) {
    char buf[64];
    if ((LCB_V_R_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_V_R_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_V_R_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_V_R_ST_ID", buf, "25");
    return -1;
  }
  if ((LCB_V_R_ST_V) > 0 || (long)(LCB_V_R_ST_V) != 0L) {
    char buf[64];
    if ((LCB_V_R_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_V_R_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_V_R_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_V_R_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_F_R_ST_ID) <= 0 || (unsigned long)(LCB_F_R_ST_ID) != 26UL) {
    char buf[64];
    if ((LCB_F_R_ST_ID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_F_R_ST_ID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_F_R_ST_ID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_F_R_ST_ID", buf, "26");
    return -1;
  }
  if ((LCB_F_R_ST_V) > 0 || (long)(LCB_F_R_ST_V) != 0L) {
    char buf[64];
    if ((LCB_F_R_ST_V) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_F_R_ST_V));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_F_R_ST_V));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_F_R_ST_V", buf, "0");
    return -1;
  }
  if ((LCB_ST_M) <= 0 || (unsigned long)(LCB_ST_M) != 26UL) {
    char buf[64];
    if ((LCB_ST_M) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ST_M));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ST_M));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb__STRUCTSIZES: ", "LCB_ST_M", buf, "26");
    return -1;
  }
  return _cffi_e__lcb_WAITFLAGS(lib);
}

static void _cffi_check__lcb_arithmetic_resp_t(lcb_arithmetic_resp_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $14' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout__lcb_arithmetic_resp_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_arithmetic_resp_t y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_arithmetic_resp_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_arithmetic_resp_t, version),
    sizeof(((lcb_arithmetic_resp_t *)0)->version),
    offsetof(lcb_arithmetic_resp_t, v),
    sizeof(((lcb_arithmetic_resp_t *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_arithmetic_resp_t(0);
}

static void _cffi_check__lcb_completion_procs(lcb_completion_procs *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { lcb_sockdata_t *(* *tmp)(struct lcb_io_opt_st *, int, int, int) = &p->socket; (void)tmp; }
  { unsigned int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *) = &p->close; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, void(*)(lcb_sockdata_t *, int)) = &p->read; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, struct sockaddr const *, unsigned int, void(*)(lcb_sockdata_t *, int)) = &p->connect; (void)tmp; }
  { lcb_io_writebuf_t *(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *) = &p->wballoc; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, lcb_io_writebuf_t *) = &p->wbfree; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, lcb_io_writebuf_t *, void(*)(lcb_sockdata_t *, lcb_io_writebuf_t *, int)) = &p->write; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, lcb_IOV *, unsigned long, void *, void(*)(lcb_sockdata_t *, int, void *)) = &p->write2; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, lcb_IOV *, unsigned long, void *, void(*)(lcb_sockdata_t *, int, void *)) = &p->read2; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, struct sockaddr const *, void(*)(lcb_sockdata_t *, lcb_sockdata_t *, int)) = &p->serve; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, struct lcb_nameinfo_st *) = &p->nameinfo; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, int) = &p->is_closed; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, int, int, void *) = &p->cntl; (void)tmp; }
}
static PyObject *
_cffi_layout__lcb_completion_procs(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_completion_procs y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_completion_procs),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_completion_procs, socket),
    sizeof(((lcb_completion_procs *)0)->socket),
    offsetof(lcb_completion_procs, close),
    sizeof(((lcb_completion_procs *)0)->close),
    offsetof(lcb_completion_procs, read),
    sizeof(((lcb_completion_procs *)0)->read),
    offsetof(lcb_completion_procs, connect),
    sizeof(((lcb_completion_procs *)0)->connect),
    offsetof(lcb_completion_procs, wballoc),
    sizeof(((lcb_completion_procs *)0)->wballoc),
    offsetof(lcb_completion_procs, wbfree),
    sizeof(((lcb_completion_procs *)0)->wbfree),
    offsetof(lcb_completion_procs, write),
    sizeof(((lcb_completion_procs *)0)->write),
    offsetof(lcb_completion_procs, write2),
    sizeof(((lcb_completion_procs *)0)->write2),
    offsetof(lcb_completion_procs, read2),
    sizeof(((lcb_completion_procs *)0)->read2),
    offsetof(lcb_completion_procs, serve),
    sizeof(((lcb_completion_procs *)0)->serve),
    offsetof(lcb_completion_procs, nameinfo),
    sizeof(((lcb_completion_procs *)0)->nameinfo),
    offsetof(lcb_completion_procs, is_closed),
    sizeof(((lcb_completion_procs *)0)->is_closed),
    offsetof(lcb_completion_procs, cntl),
    sizeof(((lcb_completion_procs *)0)->cntl),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_completion_procs(0);
}

static int _cffi_e__lcb_config_transport_t(PyObject *lib)
{
  if ((LCB_CONFIG_TRANSPORT_LIST_END) > 0 || (long)(LCB_CONFIG_TRANSPORT_LIST_END) != 0L) {
    char buf[64];
    if ((LCB_CONFIG_TRANSPORT_LIST_END) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CONFIG_TRANSPORT_LIST_END));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CONFIG_TRANSPORT_LIST_END));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_config_transport_t: ", "LCB_CONFIG_TRANSPORT_LIST_END", buf, "0");
    return -1;
  }
  if ((LCB_CONFIG_TRANSPORT_HTTP) <= 0 || (unsigned long)(LCB_CONFIG_TRANSPORT_HTTP) != 1UL) {
    char buf[64];
    if ((LCB_CONFIG_TRANSPORT_HTTP) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CONFIG_TRANSPORT_HTTP));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CONFIG_TRANSPORT_HTTP));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_config_transport_t: ", "LCB_CONFIG_TRANSPORT_HTTP", buf, "1");
    return -1;
  }
  if ((LCB_CONFIG_TRANSPORT_CCCP) <= 0 || (unsigned long)(LCB_CONFIG_TRANSPORT_CCCP) != 2UL) {
    char buf[64];
    if ((LCB_CONFIG_TRANSPORT_CCCP) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CONFIG_TRANSPORT_CCCP));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CONFIG_TRANSPORT_CCCP));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_config_transport_t: ", "LCB_CONFIG_TRANSPORT_CCCP", buf, "2");
    return -1;
  }
  if ((LCB_CONFIG_TRANSPORT_MAX) <= 0 || (unsigned long)(LCB_CONFIG_TRANSPORT_MAX) != 3UL) {
    char buf[64];
    if ((LCB_CONFIG_TRANSPORT_MAX) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CONFIG_TRANSPORT_MAX));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CONFIG_TRANSPORT_MAX));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_config_transport_t: ", "LCB_CONFIG_TRANSPORT_MAX", buf, "3");
    return -1;
  }
  return _cffi_e__lcb__STRUCTSIZES(lib);
}

static int _cffi_e__lcb_configuration_t(PyObject *lib)
{
  if ((LCB_CONFIGURATION_NEW) > 0 || (long)(LCB_CONFIGURATION_NEW) != 0L) {
    char buf[64];
    if ((LCB_CONFIGURATION_NEW) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CONFIGURATION_NEW));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CONFIGURATION_NEW));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_configuration_t: ", "LCB_CONFIGURATION_NEW", buf, "0");
    return -1;
  }
  if ((LCB_CONFIGURATION_CHANGED) <= 0 || (unsigned long)(LCB_CONFIGURATION_CHANGED) != 1UL) {
    char buf[64];
    if ((LCB_CONFIGURATION_CHANGED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CONFIGURATION_CHANGED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CONFIGURATION_CHANGED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_configuration_t: ", "LCB_CONFIGURATION_CHANGED", buf, "1");
    return -1;
  }
  if ((LCB_CONFIGURATION_UNCHANGED) <= 0 || (unsigned long)(LCB_CONFIGURATION_UNCHANGED) != 2UL) {
    char buf[64];
    if ((LCB_CONFIGURATION_UNCHANGED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CONFIGURATION_UNCHANGED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CONFIGURATION_UNCHANGED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_configuration_t: ", "LCB_CONFIGURATION_UNCHANGED", buf, "2");
    return -1;
  }
  return _cffi_e__lcb_config_transport_t(lib);
}

static int _cffi_e__lcb_errflags_t(PyObject *lib)
{
  if ((LCB_ERRTYPE_INPUT) <= 0 || (unsigned long)(LCB_ERRTYPE_INPUT) != 1UL) {
    char buf[64];
    if ((LCB_ERRTYPE_INPUT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ERRTYPE_INPUT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ERRTYPE_INPUT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_errflags_t: ", "LCB_ERRTYPE_INPUT", buf, "1");
    return -1;
  }
  if ((LCB_ERRTYPE_NETWORK) <= 0 || (unsigned long)(LCB_ERRTYPE_NETWORK) != 2UL) {
    char buf[64];
    if ((LCB_ERRTYPE_NETWORK) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ERRTYPE_NETWORK));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ERRTYPE_NETWORK));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_errflags_t: ", "LCB_ERRTYPE_NETWORK", buf, "2");
    return -1;
  }
  if ((LCB_ERRTYPE_FATAL) <= 0 || (unsigned long)(LCB_ERRTYPE_FATAL) != 4UL) {
    char buf[64];
    if ((LCB_ERRTYPE_FATAL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ERRTYPE_FATAL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ERRTYPE_FATAL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_errflags_t: ", "LCB_ERRTYPE_FATAL", buf, "4");
    return -1;
  }
  if ((LCB_ERRTYPE_TRANSIENT) <= 0 || (unsigned long)(LCB_ERRTYPE_TRANSIENT) != 8UL) {
    char buf[64];
    if ((LCB_ERRTYPE_TRANSIENT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ERRTYPE_TRANSIENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ERRTYPE_TRANSIENT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_errflags_t: ", "LCB_ERRTYPE_TRANSIENT", buf, "8");
    return -1;
  }
  if ((LCB_ERRTYPE_DATAOP) <= 0 || (unsigned long)(LCB_ERRTYPE_DATAOP) != 16UL) {
    char buf[64];
    if ((LCB_ERRTYPE_DATAOP) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ERRTYPE_DATAOP));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ERRTYPE_DATAOP));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_errflags_t: ", "LCB_ERRTYPE_DATAOP", buf, "16");
    return -1;
  }
  if ((LCB_ERRTYPE_INTERNAL) <= 0 || (unsigned long)(LCB_ERRTYPE_INTERNAL) != 32UL) {
    char buf[64];
    if ((LCB_ERRTYPE_INTERNAL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ERRTYPE_INTERNAL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ERRTYPE_INTERNAL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_errflags_t: ", "LCB_ERRTYPE_INTERNAL", buf, "32");
    return -1;
  }
  if ((LCB_ERRTYPE_PLUGIN) <= 0 || (unsigned long)(LCB_ERRTYPE_PLUGIN) != 64UL) {
    char buf[64];
    if ((LCB_ERRTYPE_PLUGIN) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ERRTYPE_PLUGIN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ERRTYPE_PLUGIN));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_errflags_t: ", "LCB_ERRTYPE_PLUGIN", buf, "64");
    return -1;
  }
  if ((LCB_ERRTYPE_SRVLOAD) <= 0 || (unsigned long)(LCB_ERRTYPE_SRVLOAD) != 128UL) {
    char buf[64];
    if ((LCB_ERRTYPE_SRVLOAD) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ERRTYPE_SRVLOAD));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ERRTYPE_SRVLOAD));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_errflags_t: ", "LCB_ERRTYPE_SRVLOAD", buf, "128");
    return -1;
  }
  if ((LCB_ERRTYPE_SRVGEN) <= 0 || (unsigned long)(LCB_ERRTYPE_SRVGEN) != 256UL) {
    char buf[64];
    if ((LCB_ERRTYPE_SRVGEN) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ERRTYPE_SRVGEN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ERRTYPE_SRVGEN));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_errflags_t: ", "LCB_ERRTYPE_SRVGEN", buf, "256");
    return -1;
  }
  return _cffi_e__lcb_configuration_t(lib);
}

static int _cffi_e__lcb_error_t(PyObject *lib)
{
  if ((LCB_SUCCESS) > 0 || (long)(LCB_SUCCESS) != 0L) {
    char buf[64];
    if ((LCB_SUCCESS) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_SUCCESS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_SUCCESS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_SUCCESS", buf, "0");
    return -1;
  }
  if ((LCB_AUTH_CONTINUE) <= 0 || (unsigned long)(LCB_AUTH_CONTINUE) != 1UL) {
    char buf[64];
    if ((LCB_AUTH_CONTINUE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_AUTH_CONTINUE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_AUTH_CONTINUE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_AUTH_CONTINUE", buf, "1");
    return -1;
  }
  if ((LCB_AUTH_ERROR) <= 0 || (unsigned long)(LCB_AUTH_ERROR) != 2UL) {
    char buf[64];
    if ((LCB_AUTH_ERROR) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_AUTH_ERROR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_AUTH_ERROR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_AUTH_ERROR", buf, "2");
    return -1;
  }
  if ((LCB_DELTA_BADVAL) <= 0 || (unsigned long)(LCB_DELTA_BADVAL) != 3UL) {
    char buf[64];
    if ((LCB_DELTA_BADVAL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_DELTA_BADVAL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_DELTA_BADVAL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_DELTA_BADVAL", buf, "3");
    return -1;
  }
  if ((LCB_E2BIG) <= 0 || (unsigned long)(LCB_E2BIG) != 4UL) {
    char buf[64];
    if ((LCB_E2BIG) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_E2BIG));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_E2BIG));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_E2BIG", buf, "4");
    return -1;
  }
  if ((LCB_EBUSY) <= 0 || (unsigned long)(LCB_EBUSY) != 5UL) {
    char buf[64];
    if ((LCB_EBUSY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_EBUSY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_EBUSY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_EBUSY", buf, "5");
    return -1;
  }
  if ((LCB_EINTERNAL) <= 0 || (unsigned long)(LCB_EINTERNAL) != 6UL) {
    char buf[64];
    if ((LCB_EINTERNAL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_EINTERNAL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_EINTERNAL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_EINTERNAL", buf, "6");
    return -1;
  }
  if ((LCB_EINVAL) <= 0 || (unsigned long)(LCB_EINVAL) != 7UL) {
    char buf[64];
    if ((LCB_EINVAL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_EINVAL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_EINVAL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_EINVAL", buf, "7");
    return -1;
  }
  if ((LCB_ENOMEM) <= 0 || (unsigned long)(LCB_ENOMEM) != 8UL) {
    char buf[64];
    if ((LCB_ENOMEM) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ENOMEM));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ENOMEM));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_ENOMEM", buf, "8");
    return -1;
  }
  if ((LCB_ERANGE) <= 0 || (unsigned long)(LCB_ERANGE) != 9UL) {
    char buf[64];
    if ((LCB_ERANGE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ERANGE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ERANGE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_ERANGE", buf, "9");
    return -1;
  }
  if ((LCB_ERROR) <= 0 || (unsigned long)(LCB_ERROR) != 10UL) {
    char buf[64];
    if ((LCB_ERROR) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ERROR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ERROR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_ERROR", buf, "10");
    return -1;
  }
  if ((LCB_ETMPFAIL) <= 0 || (unsigned long)(LCB_ETMPFAIL) != 11UL) {
    char buf[64];
    if ((LCB_ETMPFAIL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ETMPFAIL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ETMPFAIL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_ETMPFAIL", buf, "11");
    return -1;
  }
  if ((LCB_KEY_EEXISTS) <= 0 || (unsigned long)(LCB_KEY_EEXISTS) != 12UL) {
    char buf[64];
    if ((LCB_KEY_EEXISTS) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_KEY_EEXISTS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_KEY_EEXISTS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_KEY_EEXISTS", buf, "12");
    return -1;
  }
  if ((LCB_KEY_ENOENT) <= 0 || (unsigned long)(LCB_KEY_ENOENT) != 13UL) {
    char buf[64];
    if ((LCB_KEY_ENOENT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_KEY_ENOENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_KEY_ENOENT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_KEY_ENOENT", buf, "13");
    return -1;
  }
  if ((LCB_DLOPEN_FAILED) <= 0 || (unsigned long)(LCB_DLOPEN_FAILED) != 14UL) {
    char buf[64];
    if ((LCB_DLOPEN_FAILED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_DLOPEN_FAILED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_DLOPEN_FAILED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_DLOPEN_FAILED", buf, "14");
    return -1;
  }
  if ((LCB_DLSYM_FAILED) <= 0 || (unsigned long)(LCB_DLSYM_FAILED) != 15UL) {
    char buf[64];
    if ((LCB_DLSYM_FAILED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_DLSYM_FAILED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_DLSYM_FAILED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_DLSYM_FAILED", buf, "15");
    return -1;
  }
  if ((LCB_NETWORK_ERROR) <= 0 || (unsigned long)(LCB_NETWORK_ERROR) != 16UL) {
    char buf[64];
    if ((LCB_NETWORK_ERROR) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_NETWORK_ERROR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_NETWORK_ERROR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_NETWORK_ERROR", buf, "16");
    return -1;
  }
  if ((LCB_NOT_MY_VBUCKET) <= 0 || (unsigned long)(LCB_NOT_MY_VBUCKET) != 17UL) {
    char buf[64];
    if ((LCB_NOT_MY_VBUCKET) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_NOT_MY_VBUCKET));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_NOT_MY_VBUCKET));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_NOT_MY_VBUCKET", buf, "17");
    return -1;
  }
  if ((LCB_NOT_STORED) <= 0 || (unsigned long)(LCB_NOT_STORED) != 18UL) {
    char buf[64];
    if ((LCB_NOT_STORED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_NOT_STORED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_NOT_STORED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_NOT_STORED", buf, "18");
    return -1;
  }
  if ((LCB_NOT_SUPPORTED) <= 0 || (unsigned long)(LCB_NOT_SUPPORTED) != 19UL) {
    char buf[64];
    if ((LCB_NOT_SUPPORTED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_NOT_SUPPORTED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_NOT_SUPPORTED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_NOT_SUPPORTED", buf, "19");
    return -1;
  }
  if ((LCB_UNKNOWN_COMMAND) <= 0 || (unsigned long)(LCB_UNKNOWN_COMMAND) != 20UL) {
    char buf[64];
    if ((LCB_UNKNOWN_COMMAND) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_UNKNOWN_COMMAND));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_UNKNOWN_COMMAND));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_UNKNOWN_COMMAND", buf, "20");
    return -1;
  }
  if ((LCB_UNKNOWN_HOST) <= 0 || (unsigned long)(LCB_UNKNOWN_HOST) != 21UL) {
    char buf[64];
    if ((LCB_UNKNOWN_HOST) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_UNKNOWN_HOST));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_UNKNOWN_HOST));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_UNKNOWN_HOST", buf, "21");
    return -1;
  }
  if ((LCB_PROTOCOL_ERROR) <= 0 || (unsigned long)(LCB_PROTOCOL_ERROR) != 22UL) {
    char buf[64];
    if ((LCB_PROTOCOL_ERROR) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_PROTOCOL_ERROR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_PROTOCOL_ERROR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_PROTOCOL_ERROR", buf, "22");
    return -1;
  }
  if ((LCB_ETIMEDOUT) <= 0 || (unsigned long)(LCB_ETIMEDOUT) != 23UL) {
    char buf[64];
    if ((LCB_ETIMEDOUT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ETIMEDOUT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ETIMEDOUT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_ETIMEDOUT", buf, "23");
    return -1;
  }
  if ((LCB_CONNECT_ERROR) <= 0 || (unsigned long)(LCB_CONNECT_ERROR) != 24UL) {
    char buf[64];
    if ((LCB_CONNECT_ERROR) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CONNECT_ERROR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CONNECT_ERROR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_CONNECT_ERROR", buf, "24");
    return -1;
  }
  if ((LCB_BUCKET_ENOENT) <= 0 || (unsigned long)(LCB_BUCKET_ENOENT) != 25UL) {
    char buf[64];
    if ((LCB_BUCKET_ENOENT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_BUCKET_ENOENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_BUCKET_ENOENT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_BUCKET_ENOENT", buf, "25");
    return -1;
  }
  if ((LCB_CLIENT_ENOMEM) <= 0 || (unsigned long)(LCB_CLIENT_ENOMEM) != 26UL) {
    char buf[64];
    if ((LCB_CLIENT_ENOMEM) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CLIENT_ENOMEM));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CLIENT_ENOMEM));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_CLIENT_ENOMEM", buf, "26");
    return -1;
  }
  if ((LCB_CLIENT_ETMPFAIL) <= 0 || (unsigned long)(LCB_CLIENT_ETMPFAIL) != 27UL) {
    char buf[64];
    if ((LCB_CLIENT_ETMPFAIL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CLIENT_ETMPFAIL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CLIENT_ETMPFAIL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_CLIENT_ETMPFAIL", buf, "27");
    return -1;
  }
  if ((LCB_EBADHANDLE) <= 0 || (unsigned long)(LCB_EBADHANDLE) != 28UL) {
    char buf[64];
    if ((LCB_EBADHANDLE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_EBADHANDLE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_EBADHANDLE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_EBADHANDLE", buf, "28");
    return -1;
  }
  if ((LCB_SERVER_BUG) <= 0 || (unsigned long)(LCB_SERVER_BUG) != 29UL) {
    char buf[64];
    if ((LCB_SERVER_BUG) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_SERVER_BUG));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_SERVER_BUG));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_SERVER_BUG", buf, "29");
    return -1;
  }
  if ((LCB_PLUGIN_VERSION_MISMATCH) <= 0 || (unsigned long)(LCB_PLUGIN_VERSION_MISMATCH) != 30UL) {
    char buf[64];
    if ((LCB_PLUGIN_VERSION_MISMATCH) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_PLUGIN_VERSION_MISMATCH));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_PLUGIN_VERSION_MISMATCH));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_PLUGIN_VERSION_MISMATCH", buf, "30");
    return -1;
  }
  if ((LCB_INVALID_HOST_FORMAT) <= 0 || (unsigned long)(LCB_INVALID_HOST_FORMAT) != 31UL) {
    char buf[64];
    if ((LCB_INVALID_HOST_FORMAT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_INVALID_HOST_FORMAT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_INVALID_HOST_FORMAT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_INVALID_HOST_FORMAT", buf, "31");
    return -1;
  }
  if ((LCB_INVALID_CHAR) <= 0 || (unsigned long)(LCB_INVALID_CHAR) != 32UL) {
    char buf[64];
    if ((LCB_INVALID_CHAR) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_INVALID_CHAR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_INVALID_CHAR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_INVALID_CHAR", buf, "32");
    return -1;
  }
  if ((LCB_DURABILITY_ETOOMANY) <= 0 || (unsigned long)(LCB_DURABILITY_ETOOMANY) != 33UL) {
    char buf[64];
    if ((LCB_DURABILITY_ETOOMANY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_DURABILITY_ETOOMANY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_DURABILITY_ETOOMANY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_DURABILITY_ETOOMANY", buf, "33");
    return -1;
  }
  if ((LCB_DUPLICATE_COMMANDS) <= 0 || (unsigned long)(LCB_DUPLICATE_COMMANDS) != 34UL) {
    char buf[64];
    if ((LCB_DUPLICATE_COMMANDS) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_DUPLICATE_COMMANDS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_DUPLICATE_COMMANDS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_DUPLICATE_COMMANDS", buf, "34");
    return -1;
  }
  if ((LCB_NO_MATCHING_SERVER) <= 0 || (unsigned long)(LCB_NO_MATCHING_SERVER) != 35UL) {
    char buf[64];
    if ((LCB_NO_MATCHING_SERVER) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_NO_MATCHING_SERVER));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_NO_MATCHING_SERVER));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_NO_MATCHING_SERVER", buf, "35");
    return -1;
  }
  if ((LCB_BAD_ENVIRONMENT) <= 0 || (unsigned long)(LCB_BAD_ENVIRONMENT) != 36UL) {
    char buf[64];
    if ((LCB_BAD_ENVIRONMENT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_BAD_ENVIRONMENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_BAD_ENVIRONMENT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_BAD_ENVIRONMENT", buf, "36");
    return -1;
  }
  if ((LCB_BUSY) <= 0 || (unsigned long)(LCB_BUSY) != 37UL) {
    char buf[64];
    if ((LCB_BUSY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_BUSY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_BUSY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_BUSY", buf, "37");
    return -1;
  }
  if ((LCB_INVALID_USERNAME) <= 0 || (unsigned long)(LCB_INVALID_USERNAME) != 38UL) {
    char buf[64];
    if ((LCB_INVALID_USERNAME) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_INVALID_USERNAME));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_INVALID_USERNAME));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_INVALID_USERNAME", buf, "38");
    return -1;
  }
  if ((LCB_CONFIG_CACHE_INVALID) <= 0 || (unsigned long)(LCB_CONFIG_CACHE_INVALID) != 39UL) {
    char buf[64];
    if ((LCB_CONFIG_CACHE_INVALID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CONFIG_CACHE_INVALID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CONFIG_CACHE_INVALID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_CONFIG_CACHE_INVALID", buf, "39");
    return -1;
  }
  if ((LCB_SASLMECH_UNAVAILABLE) <= 0 || (unsigned long)(LCB_SASLMECH_UNAVAILABLE) != 40UL) {
    char buf[64];
    if ((LCB_SASLMECH_UNAVAILABLE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_SASLMECH_UNAVAILABLE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_SASLMECH_UNAVAILABLE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_SASLMECH_UNAVAILABLE", buf, "40");
    return -1;
  }
  if ((LCB_TOO_MANY_REDIRECTS) <= 0 || (unsigned long)(LCB_TOO_MANY_REDIRECTS) != 41UL) {
    char buf[64];
    if ((LCB_TOO_MANY_REDIRECTS) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_TOO_MANY_REDIRECTS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_TOO_MANY_REDIRECTS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_TOO_MANY_REDIRECTS", buf, "41");
    return -1;
  }
  if ((LCB_MAP_CHANGED) <= 0 || (unsigned long)(LCB_MAP_CHANGED) != 42UL) {
    char buf[64];
    if ((LCB_MAP_CHANGED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_MAP_CHANGED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_MAP_CHANGED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_MAP_CHANGED", buf, "42");
    return -1;
  }
  if ((LCB_INCOMPLETE_PACKET) <= 0 || (unsigned long)(LCB_INCOMPLETE_PACKET) != 43UL) {
    char buf[64];
    if ((LCB_INCOMPLETE_PACKET) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_INCOMPLETE_PACKET));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_INCOMPLETE_PACKET));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_INCOMPLETE_PACKET", buf, "43");
    return -1;
  }
  if ((LCB_ECONNREFUSED) <= 0 || (unsigned long)(LCB_ECONNREFUSED) != 44UL) {
    char buf[64];
    if ((LCB_ECONNREFUSED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ECONNREFUSED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ECONNREFUSED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_ECONNREFUSED", buf, "44");
    return -1;
  }
  if ((LCB_ESOCKSHUTDOWN) <= 0 || (unsigned long)(LCB_ESOCKSHUTDOWN) != 45UL) {
    char buf[64];
    if ((LCB_ESOCKSHUTDOWN) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ESOCKSHUTDOWN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ESOCKSHUTDOWN));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_ESOCKSHUTDOWN", buf, "45");
    return -1;
  }
  if ((LCB_ECONNRESET) <= 0 || (unsigned long)(LCB_ECONNRESET) != 46UL) {
    char buf[64];
    if ((LCB_ECONNRESET) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ECONNRESET));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ECONNRESET));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_ECONNRESET", buf, "46");
    return -1;
  }
  if ((LCB_ECANTGETPORT) <= 0 || (unsigned long)(LCB_ECANTGETPORT) != 47UL) {
    char buf[64];
    if ((LCB_ECANTGETPORT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ECANTGETPORT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ECANTGETPORT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_ECANTGETPORT", buf, "47");
    return -1;
  }
  if ((LCB_EFDLIMITREACHED) <= 0 || (unsigned long)(LCB_EFDLIMITREACHED) != 48UL) {
    char buf[64];
    if ((LCB_EFDLIMITREACHED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_EFDLIMITREACHED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_EFDLIMITREACHED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_EFDLIMITREACHED", buf, "48");
    return -1;
  }
  if ((LCB_ENETUNREACH) <= 0 || (unsigned long)(LCB_ENETUNREACH) != 49UL) {
    char buf[64];
    if ((LCB_ENETUNREACH) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ENETUNREACH));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ENETUNREACH));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_ENETUNREACH", buf, "49");
    return -1;
  }
  if ((LCB_ECTL_UNKNOWN) <= 0 || (unsigned long)(LCB_ECTL_UNKNOWN) != 50UL) {
    char buf[64];
    if ((LCB_ECTL_UNKNOWN) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ECTL_UNKNOWN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ECTL_UNKNOWN));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_ECTL_UNKNOWN", buf, "50");
    return -1;
  }
  if ((LCB_ECTL_UNSUPPMODE) <= 0 || (unsigned long)(LCB_ECTL_UNSUPPMODE) != 51UL) {
    char buf[64];
    if ((LCB_ECTL_UNSUPPMODE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ECTL_UNSUPPMODE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ECTL_UNSUPPMODE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_ECTL_UNSUPPMODE", buf, "51");
    return -1;
  }
  if ((LCB_ECTL_BADARG) <= 0 || (unsigned long)(LCB_ECTL_BADARG) != 52UL) {
    char buf[64];
    if ((LCB_ECTL_BADARG) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ECTL_BADARG));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ECTL_BADARG));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_ECTL_BADARG", buf, "52");
    return -1;
  }
  if ((LCB_EMPTY_KEY) <= 0 || (unsigned long)(LCB_EMPTY_KEY) != 53UL) {
    char buf[64];
    if ((LCB_EMPTY_KEY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_EMPTY_KEY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_EMPTY_KEY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_EMPTY_KEY", buf, "53");
    return -1;
  }
  if ((LCB_SSL_ERROR) <= 0 || (unsigned long)(LCB_SSL_ERROR) != 54UL) {
    char buf[64];
    if ((LCB_SSL_ERROR) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_SSL_ERROR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_SSL_ERROR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_SSL_ERROR", buf, "54");
    return -1;
  }
  if ((LCB_SSL_CANTVERIFY) <= 0 || (unsigned long)(LCB_SSL_CANTVERIFY) != 55UL) {
    char buf[64];
    if ((LCB_SSL_CANTVERIFY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_SSL_CANTVERIFY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_SSL_CANTVERIFY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_SSL_CANTVERIFY", buf, "55");
    return -1;
  }
  if ((LCB_SCHEDFAIL_INTERNAL) <= 0 || (unsigned long)(LCB_SCHEDFAIL_INTERNAL) != 56UL) {
    char buf[64];
    if ((LCB_SCHEDFAIL_INTERNAL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_SCHEDFAIL_INTERNAL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_SCHEDFAIL_INTERNAL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_SCHEDFAIL_INTERNAL", buf, "56");
    return -1;
  }
  if ((LCB_CLIENT_FEATURE_UNAVAILABLE) <= 0 || (unsigned long)(LCB_CLIENT_FEATURE_UNAVAILABLE) != 57UL) {
    char buf[64];
    if ((LCB_CLIENT_FEATURE_UNAVAILABLE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CLIENT_FEATURE_UNAVAILABLE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CLIENT_FEATURE_UNAVAILABLE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_CLIENT_FEATURE_UNAVAILABLE", buf, "57");
    return -1;
  }
  if ((LCB_OPTIONS_CONFLICT) <= 0 || (unsigned long)(LCB_OPTIONS_CONFLICT) != 58UL) {
    char buf[64];
    if ((LCB_OPTIONS_CONFLICT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_OPTIONS_CONFLICT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_OPTIONS_CONFLICT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_OPTIONS_CONFLICT", buf, "58");
    return -1;
  }
  if ((LCB_HTTP_ERROR) <= 0 || (unsigned long)(LCB_HTTP_ERROR) != 59UL) {
    char buf[64];
    if ((LCB_HTTP_ERROR) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_ERROR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_ERROR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_HTTP_ERROR", buf, "59");
    return -1;
  }
  if ((LCB_DURABILITY_NO_SYNCTOKEN) <= 0 || (unsigned long)(LCB_DURABILITY_NO_SYNCTOKEN) != 60UL) {
    char buf[64];
    if ((LCB_DURABILITY_NO_SYNCTOKEN) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_DURABILITY_NO_SYNCTOKEN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_DURABILITY_NO_SYNCTOKEN));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_DURABILITY_NO_SYNCTOKEN", buf, "60");
    return -1;
  }
  if ((LCB_UNKNOWN_MEMCACHED_ERROR) <= 0 || (unsigned long)(LCB_UNKNOWN_MEMCACHED_ERROR) != 61UL) {
    char buf[64];
    if ((LCB_UNKNOWN_MEMCACHED_ERROR) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_UNKNOWN_MEMCACHED_ERROR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_UNKNOWN_MEMCACHED_ERROR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_UNKNOWN_MEMCACHED_ERROR", buf, "61");
    return -1;
  }
  if ((LCB_MUTATION_LOST) <= 0 || (unsigned long)(LCB_MUTATION_LOST) != 62UL) {
    char buf[64];
    if ((LCB_MUTATION_LOST) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_MUTATION_LOST));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_MUTATION_LOST));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_MUTATION_LOST", buf, "62");
    return -1;
  }
  if ((LCB_MAX_ERROR) <= 0 || (unsigned long)(LCB_MAX_ERROR) != 4096UL) {
    char buf[64];
    if ((LCB_MAX_ERROR) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_MAX_ERROR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_MAX_ERROR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_error_t: ", "LCB_MAX_ERROR", buf, "4096");
    return -1;
  }
  return _cffi_e__lcb_errflags_t(lib);
}

static void _cffi_check__lcb_get_resp_t(lcb_get_resp_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $7' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout__lcb_get_resp_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_get_resp_t y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_get_resp_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_get_resp_t, version),
    sizeof(((lcb_get_resp_t *)0)->version),
    offsetof(lcb_get_resp_t, v),
    sizeof(((lcb_get_resp_t *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_get_resp_t(0);
}

static int _cffi_e__lcb_http_method_t(PyObject *lib)
{
  if ((LCB_HTTP_METHOD_GET) > 0 || (long)(LCB_HTTP_METHOD_GET) != 0L) {
    char buf[64];
    if ((LCB_HTTP_METHOD_GET) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_METHOD_GET));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_METHOD_GET));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_method_t: ", "LCB_HTTP_METHOD_GET", buf, "0");
    return -1;
  }
  if ((LCB_HTTP_METHOD_POST) <= 0 || (unsigned long)(LCB_HTTP_METHOD_POST) != 1UL) {
    char buf[64];
    if ((LCB_HTTP_METHOD_POST) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_METHOD_POST));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_METHOD_POST));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_method_t: ", "LCB_HTTP_METHOD_POST", buf, "1");
    return -1;
  }
  if ((LCB_HTTP_METHOD_PUT) <= 0 || (unsigned long)(LCB_HTTP_METHOD_PUT) != 2UL) {
    char buf[64];
    if ((LCB_HTTP_METHOD_PUT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_METHOD_PUT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_METHOD_PUT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_method_t: ", "LCB_HTTP_METHOD_PUT", buf, "2");
    return -1;
  }
  if ((LCB_HTTP_METHOD_DELETE) <= 0 || (unsigned long)(LCB_HTTP_METHOD_DELETE) != 3UL) {
    char buf[64];
    if ((LCB_HTTP_METHOD_DELETE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_METHOD_DELETE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_METHOD_DELETE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_method_t: ", "LCB_HTTP_METHOD_DELETE", buf, "3");
    return -1;
  }
  if ((LCB_HTTP_METHOD_MAX) <= 0 || (unsigned long)(LCB_HTTP_METHOD_MAX) != 4UL) {
    char buf[64];
    if ((LCB_HTTP_METHOD_MAX) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_METHOD_MAX));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_METHOD_MAX));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_method_t: ", "LCB_HTTP_METHOD_MAX", buf, "4");
    return -1;
  }
  return _cffi_e__lcb_error_t(lib);
}

static void _cffi_check__lcb_http_resp_t(lcb_http_resp_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $37' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout__lcb_http_resp_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_http_resp_t y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_http_resp_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_http_resp_t, version),
    sizeof(((lcb_http_resp_t *)0)->version),
    offsetof(lcb_http_resp_t, v),
    sizeof(((lcb_http_resp_t *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_http_resp_t(0);
}

static int _cffi_e__lcb_http_status_t(PyObject *lib)
{
  if ((LCB_HTTP_STATUS_CONTINUE) <= 0 || (unsigned long)(LCB_HTTP_STATUS_CONTINUE) != 100UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_CONTINUE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_CONTINUE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_CONTINUE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_CONTINUE", buf, "100");
    return -1;
  }
  if ((LCB_HTTP_STATUS_SWITCHING_PROTOCOLS) <= 0 || (unsigned long)(LCB_HTTP_STATUS_SWITCHING_PROTOCOLS) != 101UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_SWITCHING_PROTOCOLS) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_SWITCHING_PROTOCOLS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_SWITCHING_PROTOCOLS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_SWITCHING_PROTOCOLS", buf, "101");
    return -1;
  }
  if ((LCB_HTTP_STATUS_PROCESSING) <= 0 || (unsigned long)(LCB_HTTP_STATUS_PROCESSING) != 102UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_PROCESSING) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_PROCESSING));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_PROCESSING));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_PROCESSING", buf, "102");
    return -1;
  }
  if ((LCB_HTTP_STATUS_OK) <= 0 || (unsigned long)(LCB_HTTP_STATUS_OK) != 200UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_OK) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_OK));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_OK));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_OK", buf, "200");
    return -1;
  }
  if ((LCB_HTTP_STATUS_CREATED) <= 0 || (unsigned long)(LCB_HTTP_STATUS_CREATED) != 201UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_CREATED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_CREATED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_CREATED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_CREATED", buf, "201");
    return -1;
  }
  if ((LCB_HTTP_STATUS_ACCEPTED) <= 0 || (unsigned long)(LCB_HTTP_STATUS_ACCEPTED) != 202UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_ACCEPTED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_ACCEPTED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_ACCEPTED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_ACCEPTED", buf, "202");
    return -1;
  }
  if ((LCB_HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION) <= 0 || (unsigned long)(LCB_HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION) != 203UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION", buf, "203");
    return -1;
  }
  if ((LCB_HTTP_STATUS_NO_CONTENT) <= 0 || (unsigned long)(LCB_HTTP_STATUS_NO_CONTENT) != 204UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_NO_CONTENT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_NO_CONTENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_NO_CONTENT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_NO_CONTENT", buf, "204");
    return -1;
  }
  if ((LCB_HTTP_STATUS_RESET_CONTENT) <= 0 || (unsigned long)(LCB_HTTP_STATUS_RESET_CONTENT) != 205UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_RESET_CONTENT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_RESET_CONTENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_RESET_CONTENT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_RESET_CONTENT", buf, "205");
    return -1;
  }
  if ((LCB_HTTP_STATUS_PARTIAL_CONTENT) <= 0 || (unsigned long)(LCB_HTTP_STATUS_PARTIAL_CONTENT) != 206UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_PARTIAL_CONTENT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_PARTIAL_CONTENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_PARTIAL_CONTENT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_PARTIAL_CONTENT", buf, "206");
    return -1;
  }
  if ((LCB_HTTP_STATUS_MULTI_STATUS) <= 0 || (unsigned long)(LCB_HTTP_STATUS_MULTI_STATUS) != 207UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_MULTI_STATUS) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_MULTI_STATUS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_MULTI_STATUS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_MULTI_STATUS", buf, "207");
    return -1;
  }
  if ((LCB_HTTP_STATUS_MULTIPLE_CHOICES) <= 0 || (unsigned long)(LCB_HTTP_STATUS_MULTIPLE_CHOICES) != 300UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_MULTIPLE_CHOICES) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_MULTIPLE_CHOICES));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_MULTIPLE_CHOICES));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_MULTIPLE_CHOICES", buf, "300");
    return -1;
  }
  if ((LCB_HTTP_STATUS_MOVED_PERMANENTLY) <= 0 || (unsigned long)(LCB_HTTP_STATUS_MOVED_PERMANENTLY) != 301UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_MOVED_PERMANENTLY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_MOVED_PERMANENTLY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_MOVED_PERMANENTLY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_MOVED_PERMANENTLY", buf, "301");
    return -1;
  }
  if ((LCB_HTTP_STATUS_FOUND) <= 0 || (unsigned long)(LCB_HTTP_STATUS_FOUND) != 302UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_FOUND) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_FOUND));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_FOUND));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_FOUND", buf, "302");
    return -1;
  }
  if ((LCB_HTTP_STATUS_SEE_OTHER) <= 0 || (unsigned long)(LCB_HTTP_STATUS_SEE_OTHER) != 303UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_SEE_OTHER) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_SEE_OTHER));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_SEE_OTHER));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_SEE_OTHER", buf, "303");
    return -1;
  }
  if ((LCB_HTTP_STATUS_NOT_MODIFIED) <= 0 || (unsigned long)(LCB_HTTP_STATUS_NOT_MODIFIED) != 304UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_NOT_MODIFIED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_NOT_MODIFIED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_NOT_MODIFIED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_NOT_MODIFIED", buf, "304");
    return -1;
  }
  if ((LCB_HTTP_STATUS_USE_PROXY) <= 0 || (unsigned long)(LCB_HTTP_STATUS_USE_PROXY) != 305UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_USE_PROXY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_USE_PROXY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_USE_PROXY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_USE_PROXY", buf, "305");
    return -1;
  }
  if ((LCB_HTTP_STATUS_UNUSED) <= 0 || (unsigned long)(LCB_HTTP_STATUS_UNUSED) != 306UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_UNUSED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_UNUSED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_UNUSED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_UNUSED", buf, "306");
    return -1;
  }
  if ((LCB_HTTP_STATUS_TEMPORARY_REDIRECT) <= 0 || (unsigned long)(LCB_HTTP_STATUS_TEMPORARY_REDIRECT) != 307UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_TEMPORARY_REDIRECT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_TEMPORARY_REDIRECT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_TEMPORARY_REDIRECT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_TEMPORARY_REDIRECT", buf, "307");
    return -1;
  }
  if ((LCB_HTTP_STATUS_BAD_REQUEST) <= 0 || (unsigned long)(LCB_HTTP_STATUS_BAD_REQUEST) != 400UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_BAD_REQUEST) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_BAD_REQUEST));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_BAD_REQUEST));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_BAD_REQUEST", buf, "400");
    return -1;
  }
  if ((LCB_HTTP_STATUS_UNAUTHORIZED) <= 0 || (unsigned long)(LCB_HTTP_STATUS_UNAUTHORIZED) != 401UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_UNAUTHORIZED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_UNAUTHORIZED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_UNAUTHORIZED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_UNAUTHORIZED", buf, "401");
    return -1;
  }
  if ((LCB_HTTP_STATUS_PAYMENT_REQUIRED) <= 0 || (unsigned long)(LCB_HTTP_STATUS_PAYMENT_REQUIRED) != 402UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_PAYMENT_REQUIRED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_PAYMENT_REQUIRED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_PAYMENT_REQUIRED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_PAYMENT_REQUIRED", buf, "402");
    return -1;
  }
  if ((LCB_HTTP_STATUS_FORBIDDEN) <= 0 || (unsigned long)(LCB_HTTP_STATUS_FORBIDDEN) != 403UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_FORBIDDEN) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_FORBIDDEN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_FORBIDDEN));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_FORBIDDEN", buf, "403");
    return -1;
  }
  if ((LCB_HTTP_STATUS_NOT_FOUND) <= 0 || (unsigned long)(LCB_HTTP_STATUS_NOT_FOUND) != 404UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_NOT_FOUND) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_NOT_FOUND));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_NOT_FOUND));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_NOT_FOUND", buf, "404");
    return -1;
  }
  if ((LCB_HTTP_STATUS_METHOD_NOT_ALLOWED) <= 0 || (unsigned long)(LCB_HTTP_STATUS_METHOD_NOT_ALLOWED) != 405UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_METHOD_NOT_ALLOWED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_METHOD_NOT_ALLOWED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_METHOD_NOT_ALLOWED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_METHOD_NOT_ALLOWED", buf, "405");
    return -1;
  }
  if ((LCB_HTTP_STATUS_NOT_ACCEPTABLE) <= 0 || (unsigned long)(LCB_HTTP_STATUS_NOT_ACCEPTABLE) != 406UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_NOT_ACCEPTABLE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_NOT_ACCEPTABLE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_NOT_ACCEPTABLE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_NOT_ACCEPTABLE", buf, "406");
    return -1;
  }
  if ((LCB_HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED) <= 0 || (unsigned long)(LCB_HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED) != 407UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED", buf, "407");
    return -1;
  }
  if ((LCB_HTTP_STATUS_REQUEST_TIMEOUT) <= 0 || (unsigned long)(LCB_HTTP_STATUS_REQUEST_TIMEOUT) != 408UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_REQUEST_TIMEOUT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_REQUEST_TIMEOUT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_REQUEST_TIMEOUT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_REQUEST_TIMEOUT", buf, "408");
    return -1;
  }
  if ((LCB_HTTP_STATUS_CONFLICT) <= 0 || (unsigned long)(LCB_HTTP_STATUS_CONFLICT) != 409UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_CONFLICT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_CONFLICT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_CONFLICT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_CONFLICT", buf, "409");
    return -1;
  }
  if ((LCB_HTTP_STATUS_GONE) <= 0 || (unsigned long)(LCB_HTTP_STATUS_GONE) != 410UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_GONE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_GONE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_GONE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_GONE", buf, "410");
    return -1;
  }
  if ((LCB_HTTP_STATUS_LENGTH_REQUIRED) <= 0 || (unsigned long)(LCB_HTTP_STATUS_LENGTH_REQUIRED) != 411UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_LENGTH_REQUIRED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_LENGTH_REQUIRED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_LENGTH_REQUIRED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_LENGTH_REQUIRED", buf, "411");
    return -1;
  }
  if ((LCB_HTTP_STATUS_PRECONDITION_FAILED) <= 0 || (unsigned long)(LCB_HTTP_STATUS_PRECONDITION_FAILED) != 412UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_PRECONDITION_FAILED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_PRECONDITION_FAILED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_PRECONDITION_FAILED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_PRECONDITION_FAILED", buf, "412");
    return -1;
  }
  if ((LCB_HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE) <= 0 || (unsigned long)(LCB_HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE) != 413UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE", buf, "413");
    return -1;
  }
  if ((LCB_HTTP_STATUS_REQUEST_URI_TOO_LONG) <= 0 || (unsigned long)(LCB_HTTP_STATUS_REQUEST_URI_TOO_LONG) != 414UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_REQUEST_URI_TOO_LONG) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_REQUEST_URI_TOO_LONG));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_REQUEST_URI_TOO_LONG));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_REQUEST_URI_TOO_LONG", buf, "414");
    return -1;
  }
  if ((LCB_HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE) <= 0 || (unsigned long)(LCB_HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE) != 415UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE", buf, "415");
    return -1;
  }
  if ((LCB_HTTP_STATUS_REQUESTED_RANGE_NOT_SATISFIABLE) <= 0 || (unsigned long)(LCB_HTTP_STATUS_REQUESTED_RANGE_NOT_SATISFIABLE) != 416UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_REQUESTED_RANGE_NOT_SATISFIABLE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_REQUESTED_RANGE_NOT_SATISFIABLE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_REQUESTED_RANGE_NOT_SATISFIABLE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_REQUESTED_RANGE_NOT_SATISFIABLE", buf, "416");
    return -1;
  }
  if ((LCB_HTTP_STATUS_EXPECTATION_FAILED) <= 0 || (unsigned long)(LCB_HTTP_STATUS_EXPECTATION_FAILED) != 417UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_EXPECTATION_FAILED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_EXPECTATION_FAILED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_EXPECTATION_FAILED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_EXPECTATION_FAILED", buf, "417");
    return -1;
  }
  if ((LCB_HTTP_STATUS_UNPROCESSABLE_ENTITY) <= 0 || (unsigned long)(LCB_HTTP_STATUS_UNPROCESSABLE_ENTITY) != 422UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_UNPROCESSABLE_ENTITY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_UNPROCESSABLE_ENTITY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_UNPROCESSABLE_ENTITY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_UNPROCESSABLE_ENTITY", buf, "422");
    return -1;
  }
  if ((LCB_HTTP_STATUS_LOCKED) <= 0 || (unsigned long)(LCB_HTTP_STATUS_LOCKED) != 423UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_LOCKED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_LOCKED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_LOCKED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_LOCKED", buf, "423");
    return -1;
  }
  if ((LCB_HTTP_STATUS_FAILED_DEPENDENCY) <= 0 || (unsigned long)(LCB_HTTP_STATUS_FAILED_DEPENDENCY) != 424UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_FAILED_DEPENDENCY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_FAILED_DEPENDENCY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_FAILED_DEPENDENCY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_FAILED_DEPENDENCY", buf, "424");
    return -1;
  }
  if ((LCB_HTTP_STATUS_INTERNAL_SERVER_ERROR) <= 0 || (unsigned long)(LCB_HTTP_STATUS_INTERNAL_SERVER_ERROR) != 500UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_INTERNAL_SERVER_ERROR) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_INTERNAL_SERVER_ERROR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_INTERNAL_SERVER_ERROR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_INTERNAL_SERVER_ERROR", buf, "500");
    return -1;
  }
  if ((LCB_HTTP_STATUS_NOT_IMPLEMENTED) <= 0 || (unsigned long)(LCB_HTTP_STATUS_NOT_IMPLEMENTED) != 501UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_NOT_IMPLEMENTED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_NOT_IMPLEMENTED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_NOT_IMPLEMENTED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_NOT_IMPLEMENTED", buf, "501");
    return -1;
  }
  if ((LCB_HTTP_STATUS_BAD_GATEWAY) <= 0 || (unsigned long)(LCB_HTTP_STATUS_BAD_GATEWAY) != 502UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_BAD_GATEWAY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_BAD_GATEWAY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_BAD_GATEWAY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_BAD_GATEWAY", buf, "502");
    return -1;
  }
  if ((LCB_HTTP_STATUS_SERVICE_UNAVAILABLE) <= 0 || (unsigned long)(LCB_HTTP_STATUS_SERVICE_UNAVAILABLE) != 503UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_SERVICE_UNAVAILABLE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_SERVICE_UNAVAILABLE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_SERVICE_UNAVAILABLE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_SERVICE_UNAVAILABLE", buf, "503");
    return -1;
  }
  if ((LCB_HTTP_STATUS_GATEWAY_TIMEOUT) <= 0 || (unsigned long)(LCB_HTTP_STATUS_GATEWAY_TIMEOUT) != 504UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_GATEWAY_TIMEOUT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_GATEWAY_TIMEOUT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_GATEWAY_TIMEOUT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_GATEWAY_TIMEOUT", buf, "504");
    return -1;
  }
  if ((LCB_HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED) <= 0 || (unsigned long)(LCB_HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED) != 505UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED", buf, "505");
    return -1;
  }
  if ((LCB_HTTP_STATUS_INSUFFICIENT_STORAGE) <= 0 || (unsigned long)(LCB_HTTP_STATUS_INSUFFICIENT_STORAGE) != 507UL) {
    char buf[64];
    if ((LCB_HTTP_STATUS_INSUFFICIENT_STORAGE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_STATUS_INSUFFICIENT_STORAGE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_STATUS_INSUFFICIENT_STORAGE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_status_t: ", "LCB_HTTP_STATUS_INSUFFICIENT_STORAGE", buf, "507");
    return -1;
  }
  return _cffi_e__lcb_http_method_t(lib);
}

static int _cffi_e__lcb_http_type_t(PyObject *lib)
{
  if ((LCB_HTTP_TYPE_VIEW) > 0 || (long)(LCB_HTTP_TYPE_VIEW) != 0L) {
    char buf[64];
    if ((LCB_HTTP_TYPE_VIEW) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_TYPE_VIEW));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_TYPE_VIEW));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_type_t: ", "LCB_HTTP_TYPE_VIEW", buf, "0");
    return -1;
  }
  if ((LCB_HTTP_TYPE_MANAGEMENT) <= 0 || (unsigned long)(LCB_HTTP_TYPE_MANAGEMENT) != 1UL) {
    char buf[64];
    if ((LCB_HTTP_TYPE_MANAGEMENT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_TYPE_MANAGEMENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_TYPE_MANAGEMENT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_type_t: ", "LCB_HTTP_TYPE_MANAGEMENT", buf, "1");
    return -1;
  }
  if ((LCB_HTTP_TYPE_RAW) <= 0 || (unsigned long)(LCB_HTTP_TYPE_RAW) != 2UL) {
    char buf[64];
    if ((LCB_HTTP_TYPE_RAW) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_TYPE_RAW));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_TYPE_RAW));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_type_t: ", "LCB_HTTP_TYPE_RAW", buf, "2");
    return -1;
  }
  if ((LCB_HTTP_TYPE_N1QL) <= 0 || (unsigned long)(LCB_HTTP_TYPE_N1QL) != 3UL) {
    char buf[64];
    if ((LCB_HTTP_TYPE_N1QL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_TYPE_N1QL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_TYPE_N1QL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_type_t: ", "LCB_HTTP_TYPE_N1QL", buf, "3");
    return -1;
  }
  if ((LCB_HTTP_TYPE_MAX) <= 0 || (unsigned long)(LCB_HTTP_TYPE_MAX) != 4UL) {
    char buf[64];
    if ((LCB_HTTP_TYPE_MAX) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_HTTP_TYPE_MAX));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_HTTP_TYPE_MAX));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_http_type_t: ", "LCB_HTTP_TYPE_MAX", buf, "4");
    return -1;
  }
  return _cffi_e__lcb_http_status_t(lib);
}

static int _cffi_e__lcb_io_ops_type_t(PyObject *lib)
{
  if ((LCB_IO_OPS_INVALID) > 0 || (long)(LCB_IO_OPS_INVALID) != 0L) {
    char buf[64];
    if ((LCB_IO_OPS_INVALID) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_IO_OPS_INVALID));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_IO_OPS_INVALID));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_io_ops_type_t: ", "LCB_IO_OPS_INVALID", buf, "0");
    return -1;
  }
  if ((LCB_IO_OPS_DEFAULT) <= 0 || (unsigned long)(LCB_IO_OPS_DEFAULT) != 1UL) {
    char buf[64];
    if ((LCB_IO_OPS_DEFAULT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_IO_OPS_DEFAULT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_IO_OPS_DEFAULT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_io_ops_type_t: ", "LCB_IO_OPS_DEFAULT", buf, "1");
    return -1;
  }
  if ((LCB_IO_OPS_LIBEVENT) <= 0 || (unsigned long)(LCB_IO_OPS_LIBEVENT) != 2UL) {
    char buf[64];
    if ((LCB_IO_OPS_LIBEVENT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_IO_OPS_LIBEVENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_IO_OPS_LIBEVENT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_io_ops_type_t: ", "LCB_IO_OPS_LIBEVENT", buf, "2");
    return -1;
  }
  if ((LCB_IO_OPS_WINSOCK) <= 0 || (unsigned long)(LCB_IO_OPS_WINSOCK) != 3UL) {
    char buf[64];
    if ((LCB_IO_OPS_WINSOCK) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_IO_OPS_WINSOCK));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_IO_OPS_WINSOCK));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_io_ops_type_t: ", "LCB_IO_OPS_WINSOCK", buf, "3");
    return -1;
  }
  if ((LCB_IO_OPS_LIBEV) <= 0 || (unsigned long)(LCB_IO_OPS_LIBEV) != 4UL) {
    char buf[64];
    if ((LCB_IO_OPS_LIBEV) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_IO_OPS_LIBEV));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_IO_OPS_LIBEV));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_io_ops_type_t: ", "LCB_IO_OPS_LIBEV", buf, "4");
    return -1;
  }
  if ((LCB_IO_OPS_SELECT) <= 0 || (unsigned long)(LCB_IO_OPS_SELECT) != 5UL) {
    char buf[64];
    if ((LCB_IO_OPS_SELECT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_IO_OPS_SELECT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_IO_OPS_SELECT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_io_ops_type_t: ", "LCB_IO_OPS_SELECT", buf, "5");
    return -1;
  }
  if ((LCB_IO_OPS_WINIOCP) <= 0 || (unsigned long)(LCB_IO_OPS_WINIOCP) != 6UL) {
    char buf[64];
    if ((LCB_IO_OPS_WINIOCP) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_IO_OPS_WINIOCP));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_IO_OPS_WINIOCP));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_io_ops_type_t: ", "LCB_IO_OPS_WINIOCP", buf, "6");
    return -1;
  }
  if ((LCB_IO_OPS_LIBUV) <= 0 || (unsigned long)(LCB_IO_OPS_LIBUV) != 7UL) {
    char buf[64];
    if ((LCB_IO_OPS_LIBUV) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_IO_OPS_LIBUV));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_IO_OPS_LIBUV));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_io_ops_type_t: ", "LCB_IO_OPS_LIBUV", buf, "7");
    return -1;
  }
  return _cffi_e__lcb_http_type_t(lib);
}

static int _cffi_e__lcb_iomodel_t(PyObject *lib)
{
  if ((LCB_IOMODEL_EVENT) > 0 || (long)(LCB_IOMODEL_EVENT) != 0L) {
    char buf[64];
    if ((LCB_IOMODEL_EVENT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_IOMODEL_EVENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_IOMODEL_EVENT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_iomodel_t: ", "LCB_IOMODEL_EVENT", buf, "0");
    return -1;
  }
  if ((LCB_IOMODEL_COMPLETION) <= 0 || (unsigned long)(LCB_IOMODEL_COMPLETION) != 1UL) {
    char buf[64];
    if ((LCB_IOMODEL_COMPLETION) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_IOMODEL_COMPLETION));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_IOMODEL_COMPLETION));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_iomodel_t: ", "LCB_IOMODEL_COMPLETION", buf, "1");
    return -1;
  }
  return _cffi_e__lcb_io_ops_type_t(lib);
}

static int _cffi_e__lcb_ipv6_t(PyObject *lib)
{
  if ((LCB_IPV6_DISABLED) > 0 || (long)(LCB_IPV6_DISABLED) != 0L) {
    char buf[64];
    if ((LCB_IPV6_DISABLED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_IPV6_DISABLED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_IPV6_DISABLED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_ipv6_t: ", "LCB_IPV6_DISABLED", buf, "0");
    return -1;
  }
  if ((LCB_IPV6_ONLY) <= 0 || (unsigned long)(LCB_IPV6_ONLY) != 1UL) {
    char buf[64];
    if ((LCB_IPV6_ONLY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_IPV6_ONLY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_IPV6_ONLY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_ipv6_t: ", "LCB_IPV6_ONLY", buf, "1");
    return -1;
  }
  if ((LCB_IPV6_ALLOW) <= 0 || (unsigned long)(LCB_IPV6_ALLOW) != 2UL) {
    char buf[64];
    if ((LCB_IPV6_ALLOW) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_IPV6_ALLOW));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_IPV6_ALLOW));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_ipv6_t: ", "LCB_IPV6_ALLOW", buf, "2");
    return -1;
  }
  return _cffi_e__lcb_iomodel_t(lib);
}

static int _cffi_e__lcb_log_severity_t(PyObject *lib)
{
  if ((LCB_LOG_TRACE) > 0 || (long)(LCB_LOG_TRACE) != 0L) {
    char buf[64];
    if ((LCB_LOG_TRACE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_LOG_TRACE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_LOG_TRACE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_log_severity_t: ", "LCB_LOG_TRACE", buf, "0");
    return -1;
  }
  if ((LCB_LOG_DEBUG) <= 0 || (unsigned long)(LCB_LOG_DEBUG) != 1UL) {
    char buf[64];
    if ((LCB_LOG_DEBUG) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_LOG_DEBUG));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_LOG_DEBUG));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_log_severity_t: ", "LCB_LOG_DEBUG", buf, "1");
    return -1;
  }
  if ((LCB_LOG_INFO) <= 0 || (unsigned long)(LCB_LOG_INFO) != 2UL) {
    char buf[64];
    if ((LCB_LOG_INFO) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_LOG_INFO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_LOG_INFO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_log_severity_t: ", "LCB_LOG_INFO", buf, "2");
    return -1;
  }
  if ((LCB_LOG_WARN) <= 0 || (unsigned long)(LCB_LOG_WARN) != 3UL) {
    char buf[64];
    if ((LCB_LOG_WARN) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_LOG_WARN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_LOG_WARN));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_log_severity_t: ", "LCB_LOG_WARN", buf, "3");
    return -1;
  }
  if ((LCB_LOG_ERROR) <= 0 || (unsigned long)(LCB_LOG_ERROR) != 4UL) {
    char buf[64];
    if ((LCB_LOG_ERROR) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_LOG_ERROR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_LOG_ERROR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_log_severity_t: ", "LCB_LOG_ERROR", buf, "4");
    return -1;
  }
  if ((LCB_LOG_FATAL) <= 0 || (unsigned long)(LCB_LOG_FATAL) != 5UL) {
    char buf[64];
    if ((LCB_LOG_FATAL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_LOG_FATAL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_LOG_FATAL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_log_severity_t: ", "LCB_LOG_FATAL", buf, "5");
    return -1;
  }
  if ((LCB_LOG_MAX) <= 0 || (unsigned long)(LCB_LOG_MAX) != 6UL) {
    char buf[64];
    if ((LCB_LOG_MAX) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_LOG_MAX));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_LOG_MAX));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_log_severity_t: ", "LCB_LOG_MAX", buf, "6");
    return -1;
  }
  return _cffi_e__lcb_ipv6_t(lib);
}

static int _cffi_e__lcb_observe_options_t(PyObject *lib)
{
  if ((LCB_OBSERVE_MASTER_ONLY) <= 0 || (unsigned long)(LCB_OBSERVE_MASTER_ONLY) != 1UL) {
    char buf[64];
    if ((LCB_OBSERVE_MASTER_ONLY) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_OBSERVE_MASTER_ONLY));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_OBSERVE_MASTER_ONLY));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_observe_options_t: ", "LCB_OBSERVE_MASTER_ONLY", buf, "1");
    return -1;
  }
  return _cffi_e__lcb_log_severity_t(lib);
}

static void _cffi_check__lcb_observe_resp_t(lcb_observe_resp_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $16' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout__lcb_observe_resp_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_observe_resp_t y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_observe_resp_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_observe_resp_t, version),
    sizeof(((lcb_observe_resp_t *)0)->version),
    offsetof(lcb_observe_resp_t, v),
    sizeof(((lcb_observe_resp_t *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_observe_resp_t(0);
}

static int _cffi_e__lcb_observe_t(PyObject *lib)
{
  if ((LCB_OBSERVE_FOUND) > 0 || (long)(LCB_OBSERVE_FOUND) != 0L) {
    char buf[64];
    if ((LCB_OBSERVE_FOUND) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_OBSERVE_FOUND));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_OBSERVE_FOUND));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_observe_t: ", "LCB_OBSERVE_FOUND", buf, "0");
    return -1;
  }
  if ((LCB_OBSERVE_PERSISTED) <= 0 || (unsigned long)(LCB_OBSERVE_PERSISTED) != 1UL) {
    char buf[64];
    if ((LCB_OBSERVE_PERSISTED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_OBSERVE_PERSISTED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_OBSERVE_PERSISTED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_observe_t: ", "LCB_OBSERVE_PERSISTED", buf, "1");
    return -1;
  }
  if ((LCB_OBSERVE_NOT_FOUND) <= 0 || (unsigned long)(LCB_OBSERVE_NOT_FOUND) != 128UL) {
    char buf[64];
    if ((LCB_OBSERVE_NOT_FOUND) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_OBSERVE_NOT_FOUND));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_OBSERVE_NOT_FOUND));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_observe_t: ", "LCB_OBSERVE_NOT_FOUND", buf, "128");
    return -1;
  }
  if ((LCB_OBSERVE_LOGICALLY_DELETED) <= 0 || (unsigned long)(LCB_OBSERVE_LOGICALLY_DELETED) != 129UL) {
    char buf[64];
    if ((LCB_OBSERVE_LOGICALLY_DELETED) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_OBSERVE_LOGICALLY_DELETED));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_OBSERVE_LOGICALLY_DELETED));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_observe_t: ", "LCB_OBSERVE_LOGICALLY_DELETED", buf, "129");
    return -1;
  }
  if ((LCB_OBSERVE_MAX) <= 0 || (unsigned long)(LCB_OBSERVE_MAX) != 130UL) {
    char buf[64];
    if ((LCB_OBSERVE_MAX) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_OBSERVE_MAX));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_OBSERVE_MAX));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_observe_t: ", "LCB_OBSERVE_MAX", buf, "130");
    return -1;
  }
  return _cffi_e__lcb_observe_options_t(lib);
}

static void _cffi_check__lcb_remove_resp_t(lcb_remove_resp_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $18' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout__lcb_remove_resp_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_remove_resp_t y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_remove_resp_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_remove_resp_t, version),
    sizeof(((lcb_remove_resp_t *)0)->version),
    offsetof(lcb_remove_resp_t, v),
    sizeof(((lcb_remove_resp_t *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_remove_resp_t(0);
}

static int _cffi_e__lcb_replica_t(PyObject *lib)
{
  if ((LCB_REPLICA_FIRST) > 0 || (long)(LCB_REPLICA_FIRST) != 0L) {
    char buf[64];
    if ((LCB_REPLICA_FIRST) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_REPLICA_FIRST));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_REPLICA_FIRST));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_replica_t: ", "LCB_REPLICA_FIRST", buf, "0");
    return -1;
  }
  if ((LCB_REPLICA_ALL) <= 0 || (unsigned long)(LCB_REPLICA_ALL) != 1UL) {
    char buf[64];
    if ((LCB_REPLICA_ALL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_REPLICA_ALL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_REPLICA_ALL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_replica_t: ", "LCB_REPLICA_ALL", buf, "1");
    return -1;
  }
  if ((LCB_REPLICA_SELECT) <= 0 || (unsigned long)(LCB_REPLICA_SELECT) != 2UL) {
    char buf[64];
    if ((LCB_REPLICA_SELECT) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_REPLICA_SELECT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_REPLICA_SELECT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_replica_t: ", "LCB_REPLICA_SELECT", buf, "2");
    return -1;
  }
  return _cffi_e__lcb_observe_t(lib);
}

static int _cffi_e__lcb_storage_t(PyObject *lib)
{
  if ((LCB_ADD) <= 0 || (unsigned long)(LCB_ADD) != 1UL) {
    char buf[64];
    if ((LCB_ADD) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ADD));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ADD));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_storage_t: ", "LCB_ADD", buf, "1");
    return -1;
  }
  if ((LCB_REPLACE) <= 0 || (unsigned long)(LCB_REPLACE) != 2UL) {
    char buf[64];
    if ((LCB_REPLACE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_REPLACE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_REPLACE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_storage_t: ", "LCB_REPLACE", buf, "2");
    return -1;
  }
  if ((LCB_SET) <= 0 || (unsigned long)(LCB_SET) != 3UL) {
    char buf[64];
    if ((LCB_SET) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_SET));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_SET));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_storage_t: ", "LCB_SET", buf, "3");
    return -1;
  }
  if ((LCB_APPEND) <= 0 || (unsigned long)(LCB_APPEND) != 4UL) {
    char buf[64];
    if ((LCB_APPEND) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_APPEND));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_APPEND));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_storage_t: ", "LCB_APPEND", buf, "4");
    return -1;
  }
  if ((LCB_PREPEND) <= 0 || (unsigned long)(LCB_PREPEND) != 5UL) {
    char buf[64];
    if ((LCB_PREPEND) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_PREPEND));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_PREPEND));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_storage_t: ", "LCB_PREPEND", buf, "5");
    return -1;
  }
  return _cffi_e__lcb_replica_t(lib);
}

static void _cffi_check__lcb_store_resp_t(lcb_store_resp_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $12' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout__lcb_store_resp_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_store_resp_t y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_store_resp_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_store_resp_t, version),
    sizeof(((lcb_store_resp_t *)0)->version),
    offsetof(lcb_store_resp_t, v),
    sizeof(((lcb_store_resp_t *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_store_resp_t(0);
}

static int _cffi_e__lcb_syncmode_t(PyObject *lib)
{
  if ((LCB_ASYNCHRONOUS) > 0 || (long)(LCB_ASYNCHRONOUS) != 0L) {
    char buf[64];
    if ((LCB_ASYNCHRONOUS) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_ASYNCHRONOUS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_ASYNCHRONOUS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_syncmode_t: ", "LCB_ASYNCHRONOUS", buf, "0");
    return -1;
  }
  if ((LCB_SYNCHRONOUS) <= 0 || (unsigned long)(LCB_SYNCHRONOUS) != 255UL) {
    char buf[64];
    if ((LCB_SYNCHRONOUS) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_SYNCHRONOUS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_SYNCHRONOUS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_syncmode_t: ", "LCB_SYNCHRONOUS", buf, "255");
    return -1;
  }
  return _cffi_e__lcb_storage_t(lib);
}

static void _cffi_check__lcb_touch_resp_t(lcb_touch_resp_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $19' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout__lcb_touch_resp_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_touch_resp_t y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_touch_resp_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_touch_resp_t, version),
    sizeof(((lcb_touch_resp_t *)0)->version),
    offsetof(lcb_touch_resp_t, v),
    sizeof(((lcb_touch_resp_t *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_touch_resp_t(0);
}

static int _cffi_e__lcb_type_t(PyObject *lib)
{
  if ((LCB_TYPE_BUCKET) > 0 || (long)(LCB_TYPE_BUCKET) != 0L) {
    char buf[64];
    if ((LCB_TYPE_BUCKET) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_TYPE_BUCKET));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_TYPE_BUCKET));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_type_t: ", "LCB_TYPE_BUCKET", buf, "0");
    return -1;
  }
  if ((LCB_TYPE_CLUSTER) <= 0 || (unsigned long)(LCB_TYPE_CLUSTER) != 1UL) {
    char buf[64];
    if ((LCB_TYPE_CLUSTER) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_TYPE_CLUSTER));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_TYPE_CLUSTER));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_type_t: ", "LCB_TYPE_CLUSTER", buf, "1");
    return -1;
  }
  return _cffi_e__lcb_syncmode_t(lib);
}

static void _cffi_check__lcb_unlock_resp_t(lcb_unlock_resp_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $10' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout__lcb_unlock_resp_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; lcb_unlock_resp_t y; };
  static Py_ssize_t nums[] = {
    sizeof(lcb_unlock_resp_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(lcb_unlock_resp_t, version),
    sizeof(((lcb_unlock_resp_t *)0)->version),
    offsetof(lcb_unlock_resp_t, v),
    sizeof(((lcb_unlock_resp_t *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__lcb_unlock_resp_t(0);
}

static int _cffi_e__lcb_vbucket_state_t(PyObject *lib)
{
  if ((LCB_VBUCKET_STATE_ACTIVE) <= 0 || (unsigned long)(LCB_VBUCKET_STATE_ACTIVE) != 1UL) {
    char buf[64];
    if ((LCB_VBUCKET_STATE_ACTIVE) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_VBUCKET_STATE_ACTIVE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_VBUCKET_STATE_ACTIVE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_vbucket_state_t: ", "LCB_VBUCKET_STATE_ACTIVE", buf, "1");
    return -1;
  }
  if ((LCB_VBUCKET_STATE_REPLICA) <= 0 || (unsigned long)(LCB_VBUCKET_STATE_REPLICA) != 2UL) {
    char buf[64];
    if ((LCB_VBUCKET_STATE_REPLICA) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_VBUCKET_STATE_REPLICA));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_VBUCKET_STATE_REPLICA));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_vbucket_state_t: ", "LCB_VBUCKET_STATE_REPLICA", buf, "2");
    return -1;
  }
  if ((LCB_VBUCKET_STATE_PENDING) <= 0 || (unsigned long)(LCB_VBUCKET_STATE_PENDING) != 3UL) {
    char buf[64];
    if ((LCB_VBUCKET_STATE_PENDING) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_VBUCKET_STATE_PENDING));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_VBUCKET_STATE_PENDING));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_vbucket_state_t: ", "LCB_VBUCKET_STATE_PENDING", buf, "3");
    return -1;
  }
  if ((LCB_VBUCKET_STATE_DEAD) <= 0 || (unsigned long)(LCB_VBUCKET_STATE_DEAD) != 4UL) {
    char buf[64];
    if ((LCB_VBUCKET_STATE_DEAD) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_VBUCKET_STATE_DEAD));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_VBUCKET_STATE_DEAD));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_vbucket_state_t: ", "LCB_VBUCKET_STATE_DEAD", buf, "4");
    return -1;
  }
  return _cffi_e__lcb_type_t(lib);
}

static int _cffi_e__lcb_verbosity_level_t(PyObject *lib)
{
  if ((LCB_VERBOSITY_DETAIL) > 0 || (long)(LCB_VERBOSITY_DETAIL) != 0L) {
    char buf[64];
    if ((LCB_VERBOSITY_DETAIL) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_VERBOSITY_DETAIL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_VERBOSITY_DETAIL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_verbosity_level_t: ", "LCB_VERBOSITY_DETAIL", buf, "0");
    return -1;
  }
  if ((LCB_VERBOSITY_DEBUG) <= 0 || (unsigned long)(LCB_VERBOSITY_DEBUG) != 1UL) {
    char buf[64];
    if ((LCB_VERBOSITY_DEBUG) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_VERBOSITY_DEBUG));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_VERBOSITY_DEBUG));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_verbosity_level_t: ", "LCB_VERBOSITY_DEBUG", buf, "1");
    return -1;
  }
  if ((LCB_VERBOSITY_INFO) <= 0 || (unsigned long)(LCB_VERBOSITY_INFO) != 2UL) {
    char buf[64];
    if ((LCB_VERBOSITY_INFO) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_VERBOSITY_INFO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_VERBOSITY_INFO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_verbosity_level_t: ", "LCB_VERBOSITY_INFO", buf, "2");
    return -1;
  }
  if ((LCB_VERBOSITY_WARNING) <= 0 || (unsigned long)(LCB_VERBOSITY_WARNING) != 3UL) {
    char buf[64];
    if ((LCB_VERBOSITY_WARNING) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_VERBOSITY_WARNING));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_VERBOSITY_WARNING));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_verbosity_level_t: ", "LCB_VERBOSITY_WARNING", buf, "3");
    return -1;
  }
  return _cffi_e__lcb_vbucket_state_t(lib);
}

static int _cffi_const_lcb_version_g(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(lcb_version_g);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "lcb_version_g", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_e__lcb_verbosity_level_t(lib);
}

static int _cffi_e_enum_lcb_compat_t(PyObject *lib)
{
  if ((LCB_MEMCACHED_CLUSTER) > 0 || (long)(LCB_MEMCACHED_CLUSTER) != 0L) {
    char buf[64];
    if ((LCB_MEMCACHED_CLUSTER) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_MEMCACHED_CLUSTER));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_MEMCACHED_CLUSTER));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_compat_t: ", "LCB_MEMCACHED_CLUSTER", buf, "0");
    return -1;
  }
  if ((LCB_CACHED_CONFIG) <= 0 || (unsigned long)(LCB_CACHED_CONFIG) != 1UL) {
    char buf[64];
    if ((LCB_CACHED_CONFIG) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_CACHED_CONFIG));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_CACHED_CONFIG));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_compat_t: ", "LCB_CACHED_CONFIG", buf, "1");
    return -1;
  }
  return _cffi_const_lcb_version_g(lib);
}

static int _cffi_e_enum_lcb_timeunit_t(PyObject *lib)
{
  if ((LCB_TIMEUNIT_NSEC) > 0 || (long)(LCB_TIMEUNIT_NSEC) != 0L) {
    char buf[64];
    if ((LCB_TIMEUNIT_NSEC) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_TIMEUNIT_NSEC));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_TIMEUNIT_NSEC));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_timeunit_t: ", "LCB_TIMEUNIT_NSEC", buf, "0");
    return -1;
  }
  if ((LCB_TIMEUNIT_USEC) <= 0 || (unsigned long)(LCB_TIMEUNIT_USEC) != 1UL) {
    char buf[64];
    if ((LCB_TIMEUNIT_USEC) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_TIMEUNIT_USEC));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_TIMEUNIT_USEC));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_timeunit_t: ", "LCB_TIMEUNIT_USEC", buf, "1");
    return -1;
  }
  if ((LCB_TIMEUNIT_MSEC) <= 0 || (unsigned long)(LCB_TIMEUNIT_MSEC) != 2UL) {
    char buf[64];
    if ((LCB_TIMEUNIT_MSEC) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_TIMEUNIT_MSEC));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_TIMEUNIT_MSEC));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_timeunit_t: ", "LCB_TIMEUNIT_MSEC", buf, "2");
    return -1;
  }
  if ((LCB_TIMEUNIT_SEC) <= 0 || (unsigned long)(LCB_TIMEUNIT_SEC) != 3UL) {
    char buf[64];
    if ((LCB_TIMEUNIT_SEC) <= 0)
        snprintf(buf, 63, "%ld", (long)(LCB_TIMEUNIT_SEC));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(LCB_TIMEUNIT_SEC));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum lcb_timeunit_t: ", "LCB_TIMEUNIT_SEC", buf, "3");
    return -1;
  }
  return _cffi_e_enum_lcb_compat_t(lib);
}

static PyObject *
_cffi_f__Cb_do_callback(PyObject *self, PyObject *args)
{
  int x0;
  short x1;
  void(* x2)(int, short, void *);
  void * x3;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:_Cb_do_callback", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_int(arg1, short);
  if (x1 == (short)-1 && PyErr_Occurred())
    return NULL;

  x2 = (void(*)(int, short, void *))_cffi_to_c_pointer(arg2, _cffi_type(0));
  if (x2 == (void(*)(int, short, void *))NULL && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(1), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { _Cb_do_callback(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f__Cb_set_key(PyObject *self, PyObject *args)
{
  void * x0;
  void const * x1;
  unsigned long x2;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:_Cb_set_key", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(1), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { _Cb_set_key(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f__Cb_set_val(PyObject *self, PyObject *args)
{
  void * x0;
  void const * x1;
  unsigned long x2;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:_Cb_set_val", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(1), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { _Cb_set_val(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb__create_compat_230(PyObject *self, PyObject *args)
{
  lcb_compat_t x0;
  void const * x1;
  struct lcb_st * * x2;
  struct lcb_io_opt_st * x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb__create_compat_230", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  if (_cffi_to_c((char *)&x0, _cffi_type(4), arg0) < 0)
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(5), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(5), arg2) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(6), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb__create_compat_230(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_arithmetic(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  unsigned long x2;
  lcb_arithmetic_cmd_t const * * x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb_arithmetic", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(9), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_arithmetic(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_behavior_get_config_errors_threshold(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  unsigned long result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_behavior_get_config_errors_threshold(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, unsigned long);
}

static PyObject *
_cffi_f_lcb_behavior_get_ipv6(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  lcb_ipv6_t result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_behavior_get_ipv6(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(10));
}

static PyObject *
_cffi_f_lcb_behavior_get_syncmode(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  lcb_syncmode_t result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_behavior_get_syncmode(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(11));
}

static PyObject *
_cffi_f_lcb_behavior_set_config_errors_threshold(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  unsigned long x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_behavior_set_config_errors_threshold", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned long);
  if (x1 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_behavior_set_config_errors_threshold(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_behavior_set_ipv6(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  lcb_ipv6_t x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_behavior_set_ipv6", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x1, _cffi_type(10), arg1) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_behavior_set_ipv6(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_behavior_set_syncmode(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  lcb_syncmode_t x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_behavior_set_syncmode", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x1, _cffi_type(11), arg1) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_behavior_set_syncmode(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_breakout(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_breakout(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_cancel_http_request(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  struct lcb_http_request_st * x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_cancel_http_request", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(12), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(12), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_cancel_http_request(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_cbflush3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDBASE const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_cbflush3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(13), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(13), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_cbflush3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_cntl(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  int x1;
  int x2;
  void * x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb_cntl", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(1), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_cntl(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_cntl_exists(PyObject *self, PyObject *arg0)
{
  int x0;
  int result;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_cntl_exists(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_lcb_cntl_getu32(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  int x1;
  Py_ssize_t datasize;
  unsigned int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_cntl_getu32", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_cntl_getu32(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, unsigned int);
}

static PyObject *
_cffi_f_lcb_cntl_setu32(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  int x1;
  unsigned int x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_cntl_setu32", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, unsigned int);
  if (x2 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_cntl_setu32(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_cntl_string(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  char const * x1;
  char const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_cntl_string", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(14), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(14), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(14), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(14), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_cntl_string(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_connect(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  lcb_error_t result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_connect(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_counter3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDCOUNTER const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_counter3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(15), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(15), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_counter3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_create(PyObject *self, PyObject *args)
{
  struct lcb_st * * x0;
  struct lcb_create_st const * x1;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_create", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(5), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(5), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(16), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_create(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_create_io_ops(PyObject *self, PyObject *args)
{
  struct lcb_io_opt_st * * x0;
  struct lcb_create_io_ops_st const * x1;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_create_io_ops", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(17), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(17), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(18), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(18), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_create_io_ops(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_destroy(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_destroy(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_destroy_async(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_destroy_async", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_destroy_async(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_destroy_io_ops(PyObject *self, PyObject *arg0)
{
  struct lcb_io_opt_st * x0;
  Py_ssize_t datasize;
  lcb_error_t result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(6), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_destroy_io_ops(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_disable_timings(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  lcb_error_t result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_disable_timings(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_dump(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  int * x1;
  unsigned int x2;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_dump", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(19), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(19), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned int);
  if (x2 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_dump(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_durability_poll(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_durability_opts_t const * x2;
  unsigned long x3;
  lcb_durability_cmd_t const * * x4;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:lcb_durability_poll", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(20), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(20), arg2) < 0)
      return NULL;
  }

  x3 = _cffi_to_c_int(arg3, unsigned long);
  if (x3 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(21), arg4, (char **)&x4);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x4 = alloca((size_t)datasize);
    memset((void *)x4, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x4, _cffi_type(21), arg4) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_durability_poll(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_enable_timings(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  lcb_error_t result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_enable_timings(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_endure3_ctxnew(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  lcb_durability_opts_t const * x1;
  lcb_error_t * x2;
  Py_ssize_t datasize;
  lcb_MULTICMD_CTX * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_endure3_ctxnew", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(20), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(20), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(22), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(22), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_endure3_ctxnew(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(23));
}

static PyObject *
_cffi_f_lcb_errmap_default(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  unsigned short x1;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_errmap_default", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned short);
  if (x1 == (unsigned short)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_errmap_default(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_flush(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  unsigned long x2;
  lcb_flush_cmd_t const * * x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb_flush", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(24), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(24), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_flush(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_flush3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDBASE const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_flush3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(13), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(13), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_flush3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_flush_buffers(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_flush_buffers", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_flush_buffers(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_get(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  unsigned long x2;
  lcb_get_cmd_t const * * x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb_get", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(25), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(25), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_get3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDGET const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_get3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(26), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(26), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_get_bootstrap_status(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  lcb_error_t result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_bootstrap_status(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_get_callback3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  int x1;
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, int, lcb_RESPBASE const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_get_callback3", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_callback3(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(27));
}

static PyObject *
_cffi_f_lcb_get_cookie(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  void const * result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_cookie(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(3));
}

static PyObject *
_cffi_f_lcb_get_errtype(PyObject *self, PyObject *arg0)
{
  lcb_error_t x0;
  int result;

  if (_cffi_to_c((char *)&x0, _cffi_type(7), arg0) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_errtype(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_lcb_get_host(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  char const * result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_host(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(14));
}

static PyObject *
_cffi_f_lcb_get_last_error(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  lcb_error_t result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_last_error(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_get_node(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  lcb_GETNODETYPE x1;
  unsigned int x2;
  Py_ssize_t datasize;
  char const * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_get_node", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x1, _cffi_type(28), arg1) < 0)
    return NULL;

  x2 = _cffi_to_c_int(arg2, unsigned int);
  if (x2 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_node(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(14));
}

static PyObject *
_cffi_f_lcb_get_num_nodes(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_num_nodes(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_lcb_get_num_replicas(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_num_replicas(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_lcb_get_port(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  char const * result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_port(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(14));
}

static PyObject *
_cffi_f_lcb_get_replica(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  unsigned long x2;
  lcb_get_replica_cmd_t const * * x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb_get_replica", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(29), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(29), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_replica(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_get_server_list(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  char const * * result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_server_list(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(30));
}

static PyObject *
_cffi_f_lcb_get_synctoken(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  lcb_KEYBUF const * x1;
  lcb_error_t * x2;
  Py_ssize_t datasize;
  lcb_SYNCTOKEN const * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_get_synctoken", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(31), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(31), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(22), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(22), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_synctoken(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(32));
}

static PyObject *
_cffi_f_lcb_get_timeout(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  unsigned int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_timeout(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, unsigned int);
}

static PyObject *
_cffi_f_lcb_get_timings(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  void(* x2)(struct lcb_st *, void const *, enum lcb_timeunit_t, unsigned int, unsigned int, unsigned int, unsigned int);
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_get_timings", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = (void(*)(struct lcb_st *, void const *, enum lcb_timeunit_t, unsigned int, unsigned int, unsigned int, unsigned int))_cffi_to_c_pointer(arg2, _cffi_type(33));
  if (x2 == (void(*)(struct lcb_st *, void const *, enum lcb_timeunit_t, unsigned int, unsigned int, unsigned int, unsigned int))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_timings(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_get_version(PyObject *self, PyObject *arg0)
{
  unsigned int * x0;
  Py_ssize_t datasize;
  char const * result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(34), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(34), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_version(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(14));
}

static PyObject *
_cffi_f_lcb_get_view_timeout(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  unsigned int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_get_view_timeout(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, unsigned int);
}

static PyObject *
_cffi_f_lcb_http3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDHTTP const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_http3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(35), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(35), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_http3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_install_callback3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  int x1;
  void(* x2)(struct lcb_st *, int, lcb_RESPBASE const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, int, lcb_RESPBASE const *);
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_install_callback3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  x2 = (void(*)(struct lcb_st *, int, lcb_RESPBASE const *))_cffi_to_c_pointer(arg2, _cffi_type(27));
  if (x2 == (void(*)(struct lcb_st *, int, lcb_RESPBASE const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_install_callback3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(27));
}

static PyObject *
_cffi_f_lcb_iops_wire_bsd_impl2(PyObject *self, PyObject *args)
{
  lcb_bsd_procs * x0;
  int x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_iops_wire_bsd_impl2", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(36), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(36), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_iops_wire_bsd_impl2(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_is_waiting(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_is_waiting(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_lcb_make_http_request(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_http_type_t x2;
  lcb_http_cmd_t const * x3;
  struct lcb_http_request_st * * x4;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:lcb_make_http_request", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x2, _cffi_type(37), arg2) < 0)
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(38), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(38), arg3) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(39), arg4, (char **)&x4);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x4 = alloca((size_t)datasize);
    memset((void *)x4, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x4, _cffi_type(39), arg4) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_make_http_request(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_mem_alloc(PyObject *self, PyObject *arg0)
{
  unsigned long x0;
  void * result;

  x0 = _cffi_to_c_int(arg0, unsigned long);
  if (x0 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_mem_alloc(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(1));
}

static PyObject *
_cffi_f_lcb_mem_free(PyObject *self, PyObject *arg0)
{
  void * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(1), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_mem_free(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_n1p_encode(PyObject *self, PyObject *args)
{
  lcb_N1QLPARAMS * x0;
  lcb_error_t * x1;
  Py_ssize_t datasize;
  char const * result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_n1p_encode", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(40), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(40), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(22), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(22), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_n1p_encode(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(14));
}

static PyObject *
_cffi_f_lcb_n1p_free(PyObject *self, PyObject *arg0)
{
  lcb_N1QLPARAMS * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(40), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(40), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_n1p_free(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_n1p_mkcmd(PyObject *self, PyObject *args)
{
  lcb_N1QLPARAMS * x0;
  lcb_CMDN1QL * x1;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_n1p_mkcmd", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(40), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(40), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(41), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(41), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_n1p_mkcmd(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_n1p_namedparam(PyObject *self, PyObject *args)
{
  lcb_N1QLPARAMS * x0;
  char const * x1;
  unsigned long x2;
  char const * x3;
  unsigned long x4;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:lcb_n1p_namedparam", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(40), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(40), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(14), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(14), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(14), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(14), arg3) < 0)
      return NULL;
  }

  x4 = _cffi_to_c_int(arg4, unsigned long);
  if (x4 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_n1p_namedparam(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_n1p_new(PyObject *self, PyObject *noarg)
{
  lcb_N1QLPARAMS * result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_n1p_new(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(40));
}

static PyObject *
_cffi_f_lcb_n1p_posparam(PyObject *self, PyObject *args)
{
  lcb_N1QLPARAMS * x0;
  char const * x1;
  unsigned long x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_n1p_posparam", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(40), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(40), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(14), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(14), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_n1p_posparam(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_n1p_reset(PyObject *self, PyObject *arg0)
{
  lcb_N1QLPARAMS * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(40), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(40), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_n1p_reset(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_n1p_setconsistency(PyObject *self, PyObject *args)
{
  lcb_N1QLPARAMS * x0;
  int x1;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_n1p_setconsistency", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(40), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(40), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_n1p_setconsistency(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_n1p_setopt(PyObject *self, PyObject *args)
{
  lcb_N1QLPARAMS * x0;
  char const * x1;
  unsigned long x2;
  char const * x3;
  unsigned long x4;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:lcb_n1p_setopt", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(40), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(40), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(14), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(14), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(14), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(14), arg3) < 0)
      return NULL;
  }

  x4 = _cffi_to_c_int(arg4, unsigned long);
  if (x4 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_n1p_setopt(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_n1p_setquery(PyObject *self, PyObject *args)
{
  lcb_N1QLPARAMS * x0;
  char const * x1;
  unsigned long x2;
  int x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb_n1p_setquery", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(40), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(40), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(14), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(14), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, int);
  if (x3 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_n1p_setquery(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_n1p_synctoken(PyObject *self, PyObject *args)
{
  lcb_N1QLPARAMS * x0;
  lcb_SYNCTOKEN const * x1;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_n1p_synctoken", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(40), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(40), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(32), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(32), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_n1p_synctoken(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_n1ql_query(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDN1QL const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_n1ql_query", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(42), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(42), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_n1ql_query(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_nstime(PyObject *self, PyObject *noarg)
{
  unsigned long long result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_nstime(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_from_c_int(result, unsigned long long);
}

static PyObject *
_cffi_f_lcb_observe(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  unsigned long x2;
  lcb_observe_cmd_t const * * x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb_observe", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(43), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(43), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_observe(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_observe3_ctxnew(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  lcb_MULTICMD_CTX * result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_observe3_ctxnew(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(23));
}

static PyObject *
_cffi_f_lcb_observe_seqno3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDOBSEQNO const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_observe_seqno3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(44), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(44), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_observe_seqno3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_refresh_config(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_refresh_config(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_remove(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  unsigned long x2;
  lcb_remove_cmd_t const * * x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb_remove", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(45), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(45), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_remove(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_remove3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDBASE const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_remove3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(13), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(13), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_remove3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_resp_get_synctoken(PyObject *self, PyObject *args)
{
  int x0;
  lcb_RESPBASE const * x1;
  Py_ssize_t datasize;
  lcb_SYNCTOKEN const * result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_resp_get_synctoken", &arg0, &arg1))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(46), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(46), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_resp_get_synctoken(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(32));
}

static PyObject *
_cffi_f_lcb_rget3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDGETREPLICA const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_rget3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(47), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(47), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_rget3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_run_loop(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_run_loop(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_sched_enter(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_sched_enter(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_sched_fail(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_sched_fail(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_sched_flush(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_sched_flush(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_sched_leave(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_sched_leave(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_server_stats(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  unsigned long x2;
  lcb_server_stats_cmd_t const * * x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb_server_stats", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(48), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(48), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_server_stats(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_server_verbosity3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDVERBOSITY const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_server_verbosity3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(49), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(49), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_server_verbosity3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_server_versions(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  unsigned long x2;
  lcb_server_version_cmd_t const * * x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb_server_versions", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(50), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(50), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_server_versions(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_server_versions3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDBASE const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_server_versions3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(13), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(13), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_server_versions3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_set_arithmetic_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, void const *, lcb_error_t, lcb_arithmetic_resp_t const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, void const *, lcb_error_t, lcb_arithmetic_resp_t const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_arithmetic_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_arithmetic_resp_t const *))_cffi_to_c_pointer(arg1, _cffi_type(51));
  if (x1 == (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_arithmetic_resp_t const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_arithmetic_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(51));
}

static PyObject *
_cffi_f_lcb_set_bootstrap_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, lcb_error_t);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, lcb_error_t);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_bootstrap_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, lcb_error_t))_cffi_to_c_pointer(arg1, _cffi_type(52));
  if (x1 == (void(*)(struct lcb_st *, lcb_error_t))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_bootstrap_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(52));
}

static PyObject *
_cffi_f_lcb_set_configuration_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, lcb_configuration_t);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, lcb_configuration_t);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_configuration_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, lcb_configuration_t))_cffi_to_c_pointer(arg1, _cffi_type(53));
  if (x1 == (void(*)(struct lcb_st *, lcb_configuration_t))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_configuration_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(53));
}

static PyObject *
_cffi_f_lcb_set_cookie(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_cookie", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_set_cookie(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_set_destroy_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(void const *);
  Py_ssize_t datasize;
  void(* result)(void const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_destroy_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(void const *))_cffi_to_c_pointer(arg1, _cffi_type(54));
  if (x1 == (void(*)(void const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_destroy_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(54));
}

static PyObject *
_cffi_f_lcb_set_durability_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, void const *, lcb_error_t, lcb_durability_resp_t const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, void const *, lcb_error_t, lcb_durability_resp_t const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_durability_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_durability_resp_t const *))_cffi_to_c_pointer(arg1, _cffi_type(55));
  if (x1 == (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_durability_resp_t const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_durability_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(55));
}

static PyObject *
_cffi_f_lcb_set_errmap_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  lcb_error_t(* x1)(struct lcb_st *, unsigned short);
  Py_ssize_t datasize;
  lcb_error_t(* result)(struct lcb_st *, unsigned short);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_errmap_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (lcb_error_t(*)(struct lcb_st *, unsigned short))_cffi_to_c_pointer(arg1, _cffi_type(56));
  if (x1 == (lcb_error_t(*)(struct lcb_st *, unsigned short))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_errmap_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(56));
}

static PyObject *
_cffi_f_lcb_set_error_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, lcb_error_t, char const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, lcb_error_t, char const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_error_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, lcb_error_t, char const *))_cffi_to_c_pointer(arg1, _cffi_type(57));
  if (x1 == (void(*)(struct lcb_st *, lcb_error_t, char const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_error_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(57));
}

static PyObject *
_cffi_f_lcb_set_flush_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, void const *, lcb_error_t, lcb_flush_resp_t const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, void const *, lcb_error_t, lcb_flush_resp_t const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_flush_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_flush_resp_t const *))_cffi_to_c_pointer(arg1, _cffi_type(58));
  if (x1 == (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_flush_resp_t const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_flush_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(58));
}

static PyObject *
_cffi_f_lcb_set_get_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, void const *, lcb_error_t, lcb_get_resp_t const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, void const *, lcb_error_t, lcb_get_resp_t const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_get_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_get_resp_t const *))_cffi_to_c_pointer(arg1, _cffi_type(59));
  if (x1 == (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_get_resp_t const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_get_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(59));
}

static PyObject *
_cffi_f_lcb_set_http_complete_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_http_request_st *, struct lcb_st *, void const *, lcb_error_t, lcb_http_resp_t const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_http_request_st *, struct lcb_st *, void const *, lcb_error_t, lcb_http_resp_t const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_http_complete_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_http_request_st *, struct lcb_st *, void const *, lcb_error_t, lcb_http_resp_t const *))_cffi_to_c_pointer(arg1, _cffi_type(60));
  if (x1 == (void(*)(struct lcb_http_request_st *, struct lcb_st *, void const *, lcb_error_t, lcb_http_resp_t const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_http_complete_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(60));
}

static PyObject *
_cffi_f_lcb_set_http_data_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_http_request_st *, struct lcb_st *, void const *, lcb_error_t, lcb_http_resp_t const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_http_request_st *, struct lcb_st *, void const *, lcb_error_t, lcb_http_resp_t const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_http_data_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_http_request_st *, struct lcb_st *, void const *, lcb_error_t, lcb_http_resp_t const *))_cffi_to_c_pointer(arg1, _cffi_type(60));
  if (x1 == (void(*)(struct lcb_http_request_st *, struct lcb_st *, void const *, lcb_error_t, lcb_http_resp_t const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_http_data_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(60));
}

static PyObject *
_cffi_f_lcb_set_observe_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, void const *, lcb_error_t, lcb_observe_resp_t const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, void const *, lcb_error_t, lcb_observe_resp_t const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_observe_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_observe_resp_t const *))_cffi_to_c_pointer(arg1, _cffi_type(61));
  if (x1 == (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_observe_resp_t const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_observe_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(61));
}

static PyObject *
_cffi_f_lcb_set_remove_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, void const *, lcb_error_t, lcb_remove_resp_t const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, void const *, lcb_error_t, lcb_remove_resp_t const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_remove_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_remove_resp_t const *))_cffi_to_c_pointer(arg1, _cffi_type(62));
  if (x1 == (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_remove_resp_t const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_remove_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(62));
}

static PyObject *
_cffi_f_lcb_set_stat_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, void const *, lcb_error_t, lcb_server_stat_resp_t const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, void const *, lcb_error_t, lcb_server_stat_resp_t const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_stat_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_server_stat_resp_t const *))_cffi_to_c_pointer(arg1, _cffi_type(63));
  if (x1 == (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_server_stat_resp_t const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_stat_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(63));
}

static PyObject *
_cffi_f_lcb_set_store_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, void const *, lcb_storage_t, lcb_error_t, lcb_store_resp_t const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, void const *, lcb_storage_t, lcb_error_t, lcb_store_resp_t const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_store_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, void const *, lcb_storage_t, lcb_error_t, lcb_store_resp_t const *))_cffi_to_c_pointer(arg1, _cffi_type(64));
  if (x1 == (void(*)(struct lcb_st *, void const *, lcb_storage_t, lcb_error_t, lcb_store_resp_t const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_store_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(64));
}

static PyObject *
_cffi_f_lcb_set_timeout(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  unsigned int x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_timeout", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_set_timeout(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_set_touch_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, void const *, lcb_error_t, lcb_touch_resp_t const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, void const *, lcb_error_t, lcb_touch_resp_t const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_touch_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_touch_resp_t const *))_cffi_to_c_pointer(arg1, _cffi_type(65));
  if (x1 == (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_touch_resp_t const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_touch_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(65));
}

static PyObject *
_cffi_f_lcb_set_unlock_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, void const *, lcb_error_t, lcb_unlock_resp_t const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, void const *, lcb_error_t, lcb_unlock_resp_t const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_unlock_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_unlock_resp_t const *))_cffi_to_c_pointer(arg1, _cffi_type(66));
  if (x1 == (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_unlock_resp_t const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_unlock_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(66));
}

static PyObject *
_cffi_f_lcb_set_verbosity(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  unsigned long x2;
  lcb_verbosity_cmd_t const * * x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb_set_verbosity", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(67), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(67), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_verbosity(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_set_verbosity_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, void const *, lcb_error_t, lcb_verbosity_resp_t const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, void const *, lcb_error_t, lcb_verbosity_resp_t const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_verbosity_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_verbosity_resp_t const *))_cffi_to_c_pointer(arg1, _cffi_type(68));
  if (x1 == (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_verbosity_resp_t const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_verbosity_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(68));
}

static PyObject *
_cffi_f_lcb_set_version_callback(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void(* x1)(struct lcb_st *, void const *, lcb_error_t, lcb_server_version_resp_t const *);
  Py_ssize_t datasize;
  void(* result)(struct lcb_st *, void const *, lcb_error_t, lcb_server_version_resp_t const *);
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_version_callback", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_server_version_resp_t const *))_cffi_to_c_pointer(arg1, _cffi_type(69));
  if (x1 == (void(*)(struct lcb_st *, void const *, lcb_error_t, lcb_server_version_resp_t const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_set_version_callback(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(69));
}

static PyObject *
_cffi_f_lcb_set_view_timeout(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  unsigned int x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_set_view_timeout", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_set_view_timeout(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_stats3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDBASE const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_stats3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(13), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(13), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_stats3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_stop_loop(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_stop_loop(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_store(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  unsigned long x2;
  lcb_store_cmd_t const * * x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb_store", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(70), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(70), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_store(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_store3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDSTORE const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_store3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(71), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(71), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_store3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_strerror(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  lcb_error_t x1;
  Py_ssize_t datasize;
  char const * result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_strerror", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x1, _cffi_type(7), arg1) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_strerror(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(14));
}

static PyObject *
_cffi_f_lcb_supports_feature(PyObject *self, PyObject *arg0)
{
  int x0;
  int result;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_supports_feature(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_lcb_tick_nowait(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  lcb_error_t result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_tick_nowait(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_timer_create(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  unsigned int x2;
  int x3;
  void(* x4)(struct lcb_timer_st *, struct lcb_st *, void const *);
  lcb_error_t * x5;
  Py_ssize_t datasize;
  struct lcb_timer_st * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;
  PyObject *arg5;

  if (!PyArg_ParseTuple(args, "OOOOOO:lcb_timer_create", &arg0, &arg1, &arg2, &arg3, &arg4, &arg5))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned int);
  if (x2 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, int);
  if (x3 == (int)-1 && PyErr_Occurred())
    return NULL;

  x4 = (void(*)(struct lcb_timer_st *, struct lcb_st *, void const *))_cffi_to_c_pointer(arg4, _cffi_type(72));
  if (x4 == (void(*)(struct lcb_timer_st *, struct lcb_st *, void const *))NULL && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(22), arg5, (char **)&x5);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x5 = alloca((size_t)datasize);
    memset((void *)x5, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x5, _cffi_type(22), arg5) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_timer_create(x0, x1, x2, x3, x4, x5); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(73));
}

static PyObject *
_cffi_f_lcb_timer_destroy(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  struct lcb_timer_st * x1;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_timer_destroy", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(73), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(73), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_timer_destroy(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_touch(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  unsigned long x2;
  lcb_get_cmd_t const * * x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb_touch", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(25), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(25), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_touch(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_touch3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDBASE const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_touch3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(13), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(13), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_touch3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_unlock(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  unsigned long x2;
  lcb_unlock_cmd_t const * * x3;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:lcb_unlock", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(74), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(74), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_unlock(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_unlock3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDBASE const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_unlock3", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(13), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(13), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_unlock3(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_verify_struct_size(PyObject *self, PyObject *args)
{
  unsigned int x0;
  unsigned int x1;
  unsigned long x2;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_verify_struct_size", &arg0, &arg1, &arg2))
    return NULL;

  x0 = _cffi_to_c_int(arg0, unsigned int);
  if (x0 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, unsigned long);
  if (x2 == (unsigned long)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_verify_struct_size(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_view_cancel(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  struct lcbview_REQUEST_st * x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_view_cancel", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(75), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(75), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_view_cancel(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_view_query(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  void const * x1;
  lcb_CMDVIEWQUERY const * x2;
  Py_ssize_t datasize;
  lcb_error_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:lcb_view_query", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(76), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(76), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_view_query(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_view_query_initcmd(PyObject *self, PyObject *args)
{
  lcb_CMDVIEWQUERY * x0;
  char const * x1;
  char const * x2;
  char const * x3;
  void(* x4)(struct lcb_st *, int, lcb_RESPVIEWQUERY const *);
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:lcb_view_query_initcmd", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(77), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(77), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(14), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(14), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(14), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(14), arg2) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(14), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(14), arg3) < 0)
      return NULL;
  }

  x4 = (void(*)(struct lcb_st *, int, lcb_RESPVIEWQUERY const *))_cffi_to_c_pointer(arg4, _cffi_type(78));
  if (x4 == (void(*)(struct lcb_st *, int, lcb_RESPVIEWQUERY const *))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_view_query_initcmd(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_lcb_wait(PyObject *self, PyObject *arg0)
{
  struct lcb_st * x0;
  Py_ssize_t datasize;
  lcb_error_t result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = lcb_wait(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_deref((char *)&result, _cffi_type(7));
}

static PyObject *
_cffi_f_lcb_wait3(PyObject *self, PyObject *args)
{
  struct lcb_st * x0;
  lcb_WAITFLAGS x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:lcb_wait3", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x1, _cffi_type(79), arg1) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { lcb_wait3(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_memset(PyObject *self, PyObject *args)
{
  void * x0;
  int x1;
  int x2;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:memset", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(1), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { memset(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static int _cffi_const_LCB_CMDOBSERVE_F_MASTER_ONLY(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(LCB_CMDOBSERVE_F_MASTER_ONLY);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "LCB_CMDOBSERVE_F_MASTER_ONLY", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_e_enum_lcb_timeunit_t(lib);
}

static int _cffi_const_LCB_CMDVIEWQUERY_F_INCLUDE_DOCS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(LCB_CMDVIEWQUERY_F_INCLUDE_DOCS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "LCB_CMDVIEWQUERY_F_INCLUDE_DOCS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_LCB_CMDOBSERVE_F_MASTER_ONLY(lib);
}

static int _cffi_const_LCB_CNTL_BUCKETNAME(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(LCB_CNTL_BUCKETNAME);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "LCB_CNTL_BUCKETNAME", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_LCB_CMDVIEWQUERY_F_INCLUDE_DOCS(lib);
}

static int _cffi_const_LCB_CNTL_GET(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(LCB_CNTL_GET);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "LCB_CNTL_GET", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_LCB_CNTL_BUCKETNAME(lib);
}

static int _cffi_const_LCB_CNTL_SET(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(LCB_CNTL_SET);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "LCB_CNTL_SET", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_LCB_CNTL_GET(lib);
}

static int _cffi_const_LCB_CNTL_VBMAP(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(LCB_CNTL_VBMAP);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "LCB_CNTL_VBMAP", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_LCB_CNTL_SET(lib);
}

static int _cffi_const_LCB_N1P_QUERY_STATEMENT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(LCB_N1P_QUERY_STATEMENT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "LCB_N1P_QUERY_STATEMENT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_LCB_CNTL_VBMAP(lib);
}

static int _cffi_const_LCB_RESP_F_FINAL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(LCB_RESP_F_FINAL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "LCB_RESP_F_FINAL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_LCB_N1P_QUERY_STATEMENT(lib);
}

static void _cffi_check_struct_lcb_CMDBASE(struct lcb_CMDBASE *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->cmdflags) << 1);
  (void)((p->exptime) << 1);
  (void)((p->cas) << 1);
  { lcb_KEYBUF *tmp = &p->key; (void)tmp; }
  { lcb_KEYBUF *tmp = &p->_hashkey; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_CMDBASE(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_CMDBASE y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_CMDBASE),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_CMDBASE, cmdflags),
    sizeof(((struct lcb_CMDBASE *)0)->cmdflags),
    offsetof(struct lcb_CMDBASE, exptime),
    sizeof(((struct lcb_CMDBASE *)0)->exptime),
    offsetof(struct lcb_CMDBASE, cas),
    sizeof(((struct lcb_CMDBASE *)0)->cas),
    offsetof(struct lcb_CMDBASE, key),
    sizeof(((struct lcb_CMDBASE *)0)->key),
    offsetof(struct lcb_CMDBASE, _hashkey),
    sizeof(((struct lcb_CMDBASE *)0)->_hashkey),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_CMDBASE(0);
}

static void _cffi_check_struct_lcb_CMDN1QL(struct lcb_CMDN1QL *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->cmdflags) << 1);
  { char const * *tmp = &p->query; (void)tmp; }
  (void)((p->nquery) << 1);
  { char const * *tmp = &p->host; (void)tmp; }
  { char const * *tmp = &p->content_type; (void)tmp; }
  { void(* *tmp)(struct lcb_st *, int, lcb_RESPN1QL const *) = &p->callback; (void)tmp; }
  { struct lcb_N1QLREQ * * *tmp = &p->handle; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_CMDN1QL(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_CMDN1QL y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_CMDN1QL),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_CMDN1QL, cmdflags),
    sizeof(((struct lcb_CMDN1QL *)0)->cmdflags),
    offsetof(struct lcb_CMDN1QL, query),
    sizeof(((struct lcb_CMDN1QL *)0)->query),
    offsetof(struct lcb_CMDN1QL, nquery),
    sizeof(((struct lcb_CMDN1QL *)0)->nquery),
    offsetof(struct lcb_CMDN1QL, host),
    sizeof(((struct lcb_CMDN1QL *)0)->host),
    offsetof(struct lcb_CMDN1QL, content_type),
    sizeof(((struct lcb_CMDN1QL *)0)->content_type),
    offsetof(struct lcb_CMDN1QL, callback),
    sizeof(((struct lcb_CMDN1QL *)0)->callback),
    offsetof(struct lcb_CMDN1QL, handle),
    sizeof(((struct lcb_CMDN1QL *)0)->handle),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_CMDN1QL(0);
}

static void _cffi_check_struct_lcb_CONTIGBUF(struct lcb_CONTIGBUF *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void const * *tmp = &p->bytes; (void)tmp; }
  (void)((p->nbytes) << 1);
}
static PyObject *
_cffi_layout_struct_lcb_CONTIGBUF(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_CONTIGBUF y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_CONTIGBUF),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_CONTIGBUF, bytes),
    sizeof(((struct lcb_CONTIGBUF *)0)->bytes),
    offsetof(struct lcb_CONTIGBUF, nbytes),
    sizeof(((struct lcb_CONTIGBUF *)0)->nbytes),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_CONTIGBUF(0);
}

static void _cffi_check_struct_lcb_FRAGBUF(struct lcb_FRAGBUF *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { lcb_IOV * *tmp = &p->iov; (void)tmp; }
  (void)((p->niov) << 1);
  (void)((p->total_length) << 1);
}
static PyObject *
_cffi_layout_struct_lcb_FRAGBUF(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_FRAGBUF y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_FRAGBUF),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_FRAGBUF, iov),
    sizeof(((struct lcb_FRAGBUF *)0)->iov),
    offsetof(struct lcb_FRAGBUF, niov),
    sizeof(((struct lcb_FRAGBUF *)0)->niov),
    offsetof(struct lcb_FRAGBUF, total_length),
    sizeof(((struct lcb_FRAGBUF *)0)->total_length),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_FRAGBUF(0);
}

static void _cffi_check_struct_lcb_KEYBUF(struct lcb_KEYBUF *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { lcb_KVBUFTYPE *tmp = &p->type; (void)tmp; }
  { lcb_CONTIGBUF *tmp = &p->contig; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_KEYBUF(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_KEYBUF y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_KEYBUF),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_KEYBUF, type),
    sizeof(((struct lcb_KEYBUF *)0)->type),
    offsetof(struct lcb_KEYBUF, contig),
    sizeof(((struct lcb_KEYBUF *)0)->contig),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_KEYBUF(0);
}

static void _cffi_check_struct_lcb_MULTICMD_CTX_st(struct lcb_MULTICMD_CTX_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { lcb_error_t(* *tmp)(lcb_MULTICMD_CTX *, lcb_CMDBASE const *) = &p->addcmd; (void)tmp; }
  { lcb_error_t(* *tmp)(lcb_MULTICMD_CTX *, void const *) = &p->done; (void)tmp; }
  { void(* *tmp)(lcb_MULTICMD_CTX *) = &p->fail; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_MULTICMD_CTX_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_MULTICMD_CTX_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_MULTICMD_CTX_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_MULTICMD_CTX_st, addcmd),
    sizeof(((struct lcb_MULTICMD_CTX_st *)0)->addcmd),
    offsetof(struct lcb_MULTICMD_CTX_st, done),
    sizeof(((struct lcb_MULTICMD_CTX_st *)0)->done),
    offsetof(struct lcb_MULTICMD_CTX_st, fail),
    sizeof(((struct lcb_MULTICMD_CTX_st *)0)->fail),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_MULTICMD_CTX_st(0);
}

static void _cffi_check_struct_lcb_RESPN1QL(struct lcb_RESPN1QL *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_error_t *tmp = &p->rc; (void)tmp; }
  (void)((p->version) << 1);
  (void)((p->rflags) << 1);
  { char const * *tmp = &p->row; (void)tmp; }
  (void)((p->nrow) << 1);
  { lcb_RESPHTTP const * *tmp = &p->htresp; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_RESPN1QL(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_RESPN1QL y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_RESPN1QL),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_RESPN1QL, cookie),
    sizeof(((struct lcb_RESPN1QL *)0)->cookie),
    offsetof(struct lcb_RESPN1QL, key),
    sizeof(((struct lcb_RESPN1QL *)0)->key),
    offsetof(struct lcb_RESPN1QL, nkey),
    sizeof(((struct lcb_RESPN1QL *)0)->nkey),
    offsetof(struct lcb_RESPN1QL, cas),
    sizeof(((struct lcb_RESPN1QL *)0)->cas),
    offsetof(struct lcb_RESPN1QL, rc),
    sizeof(((struct lcb_RESPN1QL *)0)->rc),
    offsetof(struct lcb_RESPN1QL, version),
    sizeof(((struct lcb_RESPN1QL *)0)->version),
    offsetof(struct lcb_RESPN1QL, rflags),
    sizeof(((struct lcb_RESPN1QL *)0)->rflags),
    offsetof(struct lcb_RESPN1QL, row),
    sizeof(((struct lcb_RESPN1QL *)0)->row),
    offsetof(struct lcb_RESPN1QL, nrow),
    sizeof(((struct lcb_RESPN1QL *)0)->nrow),
    offsetof(struct lcb_RESPN1QL, htresp),
    sizeof(((struct lcb_RESPN1QL *)0)->htresp),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_RESPN1QL(0);
}

static void _cffi_check_struct_lcb_RESPVIEW_st(struct lcb_RESPVIEW_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  { void const * *tmp = &p->key; (void)tmp; }
  (void)((p->nkey) << 1);
  (void)((p->cas) << 1);
  { lcb_error_t *tmp = &p->rc; (void)tmp; }
  (void)((p->version) << 1);
  (void)((p->rflags) << 1);
  { char const * *tmp = &p->docid; (void)tmp; }
  (void)((p->ndocid) << 1);
  { char const * *tmp = &p->value; (void)tmp; }
  (void)((p->nvalue) << 1);
  { char const * *tmp = &p->geometry; (void)tmp; }
  (void)((p->ngeometry) << 1);
  { lcb_RESPHTTP const * *tmp = &p->htresp; (void)tmp; }
  { lcb_RESPGET const * *tmp = &p->docresp; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_RESPVIEW_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_RESPVIEW_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_RESPVIEW_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_RESPVIEW_st, cookie),
    sizeof(((struct lcb_RESPVIEW_st *)0)->cookie),
    offsetof(struct lcb_RESPVIEW_st, key),
    sizeof(((struct lcb_RESPVIEW_st *)0)->key),
    offsetof(struct lcb_RESPVIEW_st, nkey),
    sizeof(((struct lcb_RESPVIEW_st *)0)->nkey),
    offsetof(struct lcb_RESPVIEW_st, cas),
    sizeof(((struct lcb_RESPVIEW_st *)0)->cas),
    offsetof(struct lcb_RESPVIEW_st, rc),
    sizeof(((struct lcb_RESPVIEW_st *)0)->rc),
    offsetof(struct lcb_RESPVIEW_st, version),
    sizeof(((struct lcb_RESPVIEW_st *)0)->version),
    offsetof(struct lcb_RESPVIEW_st, rflags),
    sizeof(((struct lcb_RESPVIEW_st *)0)->rflags),
    offsetof(struct lcb_RESPVIEW_st, docid),
    sizeof(((struct lcb_RESPVIEW_st *)0)->docid),
    offsetof(struct lcb_RESPVIEW_st, ndocid),
    sizeof(((struct lcb_RESPVIEW_st *)0)->ndocid),
    offsetof(struct lcb_RESPVIEW_st, value),
    sizeof(((struct lcb_RESPVIEW_st *)0)->value),
    offsetof(struct lcb_RESPVIEW_st, nvalue),
    sizeof(((struct lcb_RESPVIEW_st *)0)->nvalue),
    offsetof(struct lcb_RESPVIEW_st, geometry),
    sizeof(((struct lcb_RESPVIEW_st *)0)->geometry),
    offsetof(struct lcb_RESPVIEW_st, ngeometry),
    sizeof(((struct lcb_RESPVIEW_st *)0)->ngeometry),
    offsetof(struct lcb_RESPVIEW_st, htresp),
    sizeof(((struct lcb_RESPVIEW_st *)0)->htresp),
    offsetof(struct lcb_RESPVIEW_st, docresp),
    sizeof(((struct lcb_RESPVIEW_st *)0)->docresp),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_RESPVIEW_st(0);
}

static void _cffi_check_struct_lcb_VALBUF(struct lcb_VALBUF *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { lcb_KVBUFTYPE *tmp = &p->vtype; (void)tmp; }
  /* cannot generate 'union $49' in field 'u_buf': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_VALBUF(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_VALBUF y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_VALBUF),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_VALBUF, vtype),
    sizeof(((struct lcb_VALBUF *)0)->vtype),
    offsetof(struct lcb_VALBUF, u_buf),
    sizeof(((struct lcb_VALBUF *)0)->u_buf),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_VALBUF(0);
}

static void _cffi_check_struct_lcb_arithmetic_cmd_st(struct lcb_arithmetic_cmd_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $13' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_arithmetic_cmd_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_arithmetic_cmd_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_arithmetic_cmd_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_arithmetic_cmd_st, version),
    sizeof(((struct lcb_arithmetic_cmd_st *)0)->version),
    offsetof(struct lcb_arithmetic_cmd_st, v),
    sizeof(((struct lcb_arithmetic_cmd_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_arithmetic_cmd_st(0);
}

static void _cffi_check_struct_lcb_bsdprocs_st(struct lcb_bsdprocs_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { int(* *tmp)(struct lcb_io_opt_st *, int, int, int) = &p->socket0; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, struct sockaddr const *, unsigned int) = &p->connect0; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, void *, unsigned long, int) = &p->recv; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, lcb_IOV *, unsigned long) = &p->recvv; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, void const *, unsigned long, int) = &p->send; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, lcb_IOV *, unsigned long) = &p->sendv; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, int) = &p->close; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, struct sockaddr const *, unsigned int) = &p->bind; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, unsigned int) = &p->listen; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int) = &p->accept; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, int) = &p->is_closed; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, int, int, void *) = &p->cntl; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_bsdprocs_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_bsdprocs_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_bsdprocs_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_bsdprocs_st, socket0),
    sizeof(((struct lcb_bsdprocs_st *)0)->socket0),
    offsetof(struct lcb_bsdprocs_st, connect0),
    sizeof(((struct lcb_bsdprocs_st *)0)->connect0),
    offsetof(struct lcb_bsdprocs_st, recv),
    sizeof(((struct lcb_bsdprocs_st *)0)->recv),
    offsetof(struct lcb_bsdprocs_st, recvv),
    sizeof(((struct lcb_bsdprocs_st *)0)->recvv),
    offsetof(struct lcb_bsdprocs_st, send),
    sizeof(((struct lcb_bsdprocs_st *)0)->send),
    offsetof(struct lcb_bsdprocs_st, sendv),
    sizeof(((struct lcb_bsdprocs_st *)0)->sendv),
    offsetof(struct lcb_bsdprocs_st, close),
    sizeof(((struct lcb_bsdprocs_st *)0)->close),
    offsetof(struct lcb_bsdprocs_st, bind),
    sizeof(((struct lcb_bsdprocs_st *)0)->bind),
    offsetof(struct lcb_bsdprocs_st, listen),
    sizeof(((struct lcb_bsdprocs_st *)0)->listen),
    offsetof(struct lcb_bsdprocs_st, accept),
    sizeof(((struct lcb_bsdprocs_st *)0)->accept),
    offsetof(struct lcb_bsdprocs_st, is_closed),
    sizeof(((struct lcb_bsdprocs_st *)0)->is_closed),
    offsetof(struct lcb_bsdprocs_st, cntl),
    sizeof(((struct lcb_bsdprocs_st *)0)->cntl),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_bsdprocs_st(0);
}

static void _cffi_check_struct_lcb_buf_info(struct lcb_buf_info *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { char * *tmp = &p->root; (void)tmp; }
  (void)((p->size) << 1);
  { struct ringbuffer_st * *tmp = &p->ringbuffer; (void)tmp; }
  { lcb_IOV(*tmp)[2] = &p->iov; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_buf_info(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_buf_info y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_buf_info),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_buf_info, root),
    sizeof(((struct lcb_buf_info *)0)->root),
    offsetof(struct lcb_buf_info, size),
    sizeof(((struct lcb_buf_info *)0)->size),
    offsetof(struct lcb_buf_info, ringbuffer),
    sizeof(((struct lcb_buf_info *)0)->ringbuffer),
    offsetof(struct lcb_buf_info, iov),
    sizeof(((struct lcb_buf_info *)0)->iov),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_buf_info(0);
}

static void _cffi_check_struct_lcb_cached_config_st(struct lcb_cached_config_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { char const * *tmp = &p->cachefile; (void)tmp; }
  { struct lcb_create_st *tmp = &p->createopt; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_cached_config_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_cached_config_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_cached_config_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_cached_config_st, cachefile),
    sizeof(((struct lcb_cached_config_st *)0)->cachefile),
    offsetof(struct lcb_cached_config_st, createopt),
    sizeof(((struct lcb_cached_config_st *)0)->createopt),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_cached_config_st(0);
}

static void _cffi_check_struct_lcb_cntl_iops_info_st(struct lcb_cntl_iops_info_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $47' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_cntl_iops_info_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_cntl_iops_info_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_cntl_iops_info_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_cntl_iops_info_st, version),
    sizeof(((struct lcb_cntl_iops_info_st *)0)->version),
    offsetof(struct lcb_cntl_iops_info_st, v),
    sizeof(((struct lcb_cntl_iops_info_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_cntl_iops_info_st(0);
}

static void _cffi_check_struct_lcb_cntl_rdballocfactory(struct lcb_cntl_rdballocfactory *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { struct rdb_ALLOCATOR *(* *tmp)(void) = &p->factory; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_cntl_rdballocfactory(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_cntl_rdballocfactory y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_cntl_rdballocfactory),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_cntl_rdballocfactory, factory),
    sizeof(((struct lcb_cntl_rdballocfactory *)0)->factory),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_cntl_rdballocfactory(0);
}

static void _cffi_check_struct_lcb_cntl_server_st(struct lcb_cntl_server_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $42' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_cntl_server_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_cntl_server_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_cntl_server_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_cntl_server_st, version),
    sizeof(((struct lcb_cntl_server_st *)0)->version),
    offsetof(struct lcb_cntl_server_st, v),
    sizeof(((struct lcb_cntl_server_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_cntl_server_st(0);
}

static void _cffi_check_struct_lcb_cntl_vbinfo_st(struct lcb_cntl_vbinfo_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $38' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_cntl_vbinfo_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_cntl_vbinfo_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_cntl_vbinfo_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_cntl_vbinfo_st, version),
    sizeof(((struct lcb_cntl_vbinfo_st *)0)->version),
    offsetof(struct lcb_cntl_vbinfo_st, v),
    sizeof(((struct lcb_cntl_vbinfo_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_cntl_vbinfo_st(0);
}

static void _cffi_check_struct_lcb_create_io_ops_st(struct lcb_create_io_ops_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $5' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_create_io_ops_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_create_io_ops_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_create_io_ops_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_create_io_ops_st, version),
    sizeof(((struct lcb_create_io_ops_st *)0)->version),
    offsetof(struct lcb_create_io_ops_st, v),
    sizeof(((struct lcb_create_io_ops_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_create_io_ops_st(0);
}

static void _cffi_check_struct_lcb_create_st(struct lcb_create_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  { union lcb_CRST_u *tmp = &p->v; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_create_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_create_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_create_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_create_st, version),
    sizeof(((struct lcb_create_st *)0)->version),
    offsetof(struct lcb_create_st, v),
    sizeof(((struct lcb_create_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_create_st(0);
}

static void _cffi_check_struct_lcb_create_st0(struct lcb_create_st0 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { char const * *tmp = &p->host; (void)tmp; }
  { char const * *tmp = &p->user; (void)tmp; }
  { char const * *tmp = &p->passwd; (void)tmp; }
  { char const * *tmp = &p->bucket; (void)tmp; }
  { struct lcb_io_opt_st * *tmp = &p->io; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_create_st0(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_create_st0 y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_create_st0),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_create_st0, host),
    sizeof(((struct lcb_create_st0 *)0)->host),
    offsetof(struct lcb_create_st0, user),
    sizeof(((struct lcb_create_st0 *)0)->user),
    offsetof(struct lcb_create_st0, passwd),
    sizeof(((struct lcb_create_st0 *)0)->passwd),
    offsetof(struct lcb_create_st0, bucket),
    sizeof(((struct lcb_create_st0 *)0)->bucket),
    offsetof(struct lcb_create_st0, io),
    sizeof(((struct lcb_create_st0 *)0)->io),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_create_st0(0);
}

static void _cffi_check_struct_lcb_create_st1(struct lcb_create_st1 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { char const * *tmp = &p->host; (void)tmp; }
  { char const * *tmp = &p->user; (void)tmp; }
  { char const * *tmp = &p->passwd; (void)tmp; }
  { char const * *tmp = &p->bucket; (void)tmp; }
  { struct lcb_io_opt_st * *tmp = &p->io; (void)tmp; }
  { lcb_type_t *tmp = &p->type; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_create_st1(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_create_st1 y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_create_st1),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_create_st1, host),
    sizeof(((struct lcb_create_st1 *)0)->host),
    offsetof(struct lcb_create_st1, user),
    sizeof(((struct lcb_create_st1 *)0)->user),
    offsetof(struct lcb_create_st1, passwd),
    sizeof(((struct lcb_create_st1 *)0)->passwd),
    offsetof(struct lcb_create_st1, bucket),
    sizeof(((struct lcb_create_st1 *)0)->bucket),
    offsetof(struct lcb_create_st1, io),
    sizeof(((struct lcb_create_st1 *)0)->io),
    offsetof(struct lcb_create_st1, type),
    sizeof(((struct lcb_create_st1 *)0)->type),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_create_st1(0);
}

static void _cffi_check_struct_lcb_create_st2(struct lcb_create_st2 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { char const * *tmp = &p->host; (void)tmp; }
  { char const * *tmp = &p->user; (void)tmp; }
  { char const * *tmp = &p->passwd; (void)tmp; }
  { char const * *tmp = &p->bucket; (void)tmp; }
  { struct lcb_io_opt_st * *tmp = &p->io; (void)tmp; }
  { lcb_type_t *tmp = &p->type; (void)tmp; }
  { char const * *tmp = &p->mchosts; (void)tmp; }
  { lcb_config_transport_t const * *tmp = &p->transports; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_create_st2(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_create_st2 y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_create_st2),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_create_st2, host),
    sizeof(((struct lcb_create_st2 *)0)->host),
    offsetof(struct lcb_create_st2, user),
    sizeof(((struct lcb_create_st2 *)0)->user),
    offsetof(struct lcb_create_st2, passwd),
    sizeof(((struct lcb_create_st2 *)0)->passwd),
    offsetof(struct lcb_create_st2, bucket),
    sizeof(((struct lcb_create_st2 *)0)->bucket),
    offsetof(struct lcb_create_st2, io),
    sizeof(((struct lcb_create_st2 *)0)->io),
    offsetof(struct lcb_create_st2, type),
    sizeof(((struct lcb_create_st2 *)0)->type),
    offsetof(struct lcb_create_st2, mchosts),
    sizeof(((struct lcb_create_st2 *)0)->mchosts),
    offsetof(struct lcb_create_st2, transports),
    sizeof(((struct lcb_create_st2 *)0)->transports),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_create_st2(0);
}

static void _cffi_check_struct_lcb_create_st3(struct lcb_create_st3 *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { char const * *tmp = &p->connstr; (void)tmp; }
  { char const * *tmp = &p->username; (void)tmp; }
  { char const * *tmp = &p->passwd; (void)tmp; }
  { void * *tmp = &p->_pad_bucket; (void)tmp; }
  { struct lcb_io_opt_st * *tmp = &p->io; (void)tmp; }
  { lcb_type_t *tmp = &p->type; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_create_st3(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_create_st3 y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_create_st3),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_create_st3, connstr),
    sizeof(((struct lcb_create_st3 *)0)->connstr),
    offsetof(struct lcb_create_st3, username),
    sizeof(((struct lcb_create_st3 *)0)->username),
    offsetof(struct lcb_create_st3, passwd),
    sizeof(((struct lcb_create_st3 *)0)->passwd),
    offsetof(struct lcb_create_st3, _pad_bucket),
    sizeof(((struct lcb_create_st3 *)0)->_pad_bucket),
    offsetof(struct lcb_create_st3, io),
    sizeof(((struct lcb_create_st3 *)0)->io),
    offsetof(struct lcb_create_st3, type),
    sizeof(((struct lcb_create_st3 *)0)->type),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_create_st3(0);
}

static void _cffi_check_struct_lcb_durability_cmd_st(struct lcb_durability_cmd_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $20' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_durability_cmd_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_durability_cmd_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_durability_cmd_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_durability_cmd_st, version),
    sizeof(((struct lcb_durability_cmd_st *)0)->version),
    offsetof(struct lcb_durability_cmd_st, v),
    sizeof(((struct lcb_durability_cmd_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_durability_cmd_st(0);
}

static void _cffi_check_struct_lcb_durability_opts_st(struct lcb_durability_opts_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $21' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_durability_opts_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_durability_opts_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_durability_opts_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_durability_opts_st, version),
    sizeof(((struct lcb_durability_opts_st *)0)->version),
    offsetof(struct lcb_durability_opts_st, v),
    sizeof(((struct lcb_durability_opts_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_durability_opts_st(0);
}

static void _cffi_check_struct_lcb_durability_resp_st(struct lcb_durability_resp_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $22' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_durability_resp_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_durability_resp_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_durability_resp_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_durability_resp_st, version),
    sizeof(((struct lcb_durability_resp_st *)0)->version),
    offsetof(struct lcb_durability_resp_st, v),
    sizeof(((struct lcb_durability_resp_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_durability_resp_st(0);
}

static void _cffi_check_struct_lcb_evprocs_st(struct lcb_evprocs_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void *(* *tmp)(struct lcb_io_opt_st *) = &p->create; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, void *) = &p->destroy; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, int, void *) = &p->cancel; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, void *, short, void *, void(*)(int, short, void *)) = &p->watch; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_evprocs_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_evprocs_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_evprocs_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_evprocs_st, create),
    sizeof(((struct lcb_evprocs_st *)0)->create),
    offsetof(struct lcb_evprocs_st, destroy),
    sizeof(((struct lcb_evprocs_st *)0)->destroy),
    offsetof(struct lcb_evprocs_st, cancel),
    sizeof(((struct lcb_evprocs_st *)0)->cancel),
    offsetof(struct lcb_evprocs_st, watch),
    sizeof(((struct lcb_evprocs_st *)0)->watch),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_evprocs_st(0);
}

static void _cffi_check_struct_lcb_flush_cmd_st(struct lcb_flush_cmd_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $32' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_flush_cmd_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_flush_cmd_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_flush_cmd_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_flush_cmd_st, version),
    sizeof(((struct lcb_flush_cmd_st *)0)->version),
    offsetof(struct lcb_flush_cmd_st, v),
    sizeof(((struct lcb_flush_cmd_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_flush_cmd_st(0);
}

static void _cffi_check_struct_lcb_flush_resp_st(struct lcb_flush_resp_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $34' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_flush_resp_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_flush_resp_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_flush_resp_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_flush_resp_st, version),
    sizeof(((struct lcb_flush_resp_st *)0)->version),
    offsetof(struct lcb_flush_resp_st, v),
    sizeof(((struct lcb_flush_resp_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_flush_resp_st(0);
}

static void _cffi_check_struct_lcb_get_cmd_st(struct lcb_get_cmd_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $6' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_get_cmd_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_get_cmd_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_get_cmd_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_get_cmd_st, version),
    sizeof(((struct lcb_get_cmd_st *)0)->version),
    offsetof(struct lcb_get_cmd_st, v),
    sizeof(((struct lcb_get_cmd_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_get_cmd_st(0);
}

static void _cffi_check_struct_lcb_get_replica_cmd_st(struct lcb_get_replica_cmd_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $8' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_get_replica_cmd_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_get_replica_cmd_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_get_replica_cmd_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_get_replica_cmd_st, version),
    sizeof(((struct lcb_get_replica_cmd_st *)0)->version),
    offsetof(struct lcb_get_replica_cmd_st, v),
    sizeof(((struct lcb_get_replica_cmd_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_get_replica_cmd_st(0);
}

static void _cffi_check_struct_lcb_http_cmd_st(struct lcb_http_cmd_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $36' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_http_cmd_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_http_cmd_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_http_cmd_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_http_cmd_st, version),
    sizeof(((struct lcb_http_cmd_st *)0)->version),
    offsetof(struct lcb_http_cmd_st, v),
    sizeof(((struct lcb_http_cmd_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_http_cmd_st(0);
}

static void _cffi_check_struct_lcb_io_opt_st(struct lcb_io_opt_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  { void * *tmp = &p->dlhandle; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *) = &p->destructor; (void)tmp; }
  /* cannot generate 'union $3' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_io_opt_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_io_opt_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_io_opt_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_io_opt_st, version),
    sizeof(((struct lcb_io_opt_st *)0)->version),
    offsetof(struct lcb_io_opt_st, dlhandle),
    sizeof(((struct lcb_io_opt_st *)0)->dlhandle),
    offsetof(struct lcb_io_opt_st, destructor),
    sizeof(((struct lcb_io_opt_st *)0)->destructor),
    offsetof(struct lcb_io_opt_st, v),
    sizeof(((struct lcb_io_opt_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_io_opt_st(0);
}

static void _cffi_check_struct_lcb_io_writebuf_st(struct lcb_io_writebuf_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { struct lcb_io_opt_st * *tmp = &p->parent; (void)tmp; }
  { struct lcb_buf_info *tmp = &p->buffer; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_io_writebuf_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_io_writebuf_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_io_writebuf_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_io_writebuf_st, parent),
    sizeof(((struct lcb_io_writebuf_st *)0)->parent),
    offsetof(struct lcb_io_writebuf_st, buffer),
    sizeof(((struct lcb_io_writebuf_st *)0)->buffer),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_io_writebuf_st(0);
}

static void _cffi_check_struct_lcb_iops2_st(struct lcb_iops2_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  (void)((p->error) << 1);
  (void)((p->need_cleanup) << 1);
  { void(* *tmp)(int, lcb_loop_procs *, lcb_timer_procs *, lcb_bsd_procs *, lcb_ev_procs *, lcb_completion_procs *, lcb_iomodel_t *) = &p->get_procs; (void)tmp; }
  { struct lcbio_TABLE * *tmp = &p->iot; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_iops2_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_iops2_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_iops2_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_iops2_st, cookie),
    sizeof(((struct lcb_iops2_st *)0)->cookie),
    offsetof(struct lcb_iops2_st, error),
    sizeof(((struct lcb_iops2_st *)0)->error),
    offsetof(struct lcb_iops2_st, need_cleanup),
    sizeof(((struct lcb_iops2_st *)0)->need_cleanup),
    offsetof(struct lcb_iops2_st, get_procs),
    sizeof(((struct lcb_iops2_st *)0)->get_procs),
    offsetof(struct lcb_iops2_st, iot),
    sizeof(((struct lcb_iops2_st *)0)->iot),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_iops2_st(0);
}

static void _cffi_check_struct_lcb_iops3_st(struct lcb_iops3_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  (void)((p->error) << 1);
  (void)((p->need_cleanup) << 1);
  { void(*(*tmp)[17])(void) = &p->pads; (void)tmp; }
  { void(* *tmp)(int, lcb_loop_procs *, lcb_timer_procs *, lcb_bsd_procs *, lcb_ev_procs *, lcb_completion_procs *, lcb_iomodel_t *) = &p->get_procs; (void)tmp; }
  { struct lcbio_TABLE * *tmp = &p->iot; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_iops3_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_iops3_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_iops3_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_iops3_st, cookie),
    sizeof(((struct lcb_iops3_st *)0)->cookie),
    offsetof(struct lcb_iops3_st, error),
    sizeof(((struct lcb_iops3_st *)0)->error),
    offsetof(struct lcb_iops3_st, need_cleanup),
    sizeof(((struct lcb_iops3_st *)0)->need_cleanup),
    offsetof(struct lcb_iops3_st, pads),
    sizeof(((struct lcb_iops3_st *)0)->pads),
    offsetof(struct lcb_iops3_st, get_procs),
    sizeof(((struct lcb_iops3_st *)0)->get_procs),
    offsetof(struct lcb_iops3_st, iot),
    sizeof(((struct lcb_iops3_st *)0)->iot),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_iops3_st(0);
}

static void _cffi_check_struct_lcb_iops_completion_st(struct lcb_iops_completion_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  (void)((p->error) << 1);
  (void)((p->need_cleanup) << 1);
  { lcb_sockdata_t *(* *tmp)(struct lcb_io_opt_st *, int, int, int) = &p->create_socket; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, struct sockaddr const *, unsigned int, void(*)(lcb_sockdata_t *, int)) = &p->start_connect; (void)tmp; }
  { lcb_io_writebuf_t *(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *) = &p->create_writebuf; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, lcb_io_writebuf_t *) = &p->release_writebuf; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, lcb_io_writebuf_t *, void(*)(lcb_sockdata_t *, lcb_io_writebuf_t *, int)) = &p->start_write; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, void(*)(lcb_sockdata_t *, int)) = &p->start_read; (void)tmp; }
  { unsigned int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *) = &p->close_socket; (void)tmp; }
  { void *(* *tmp)(struct lcb_io_opt_st *) = &p->create_timer; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, void *) = &p->destroy_timer; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, void *) = &p->delete_timer; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, void *, unsigned int, void *, void(*)(int, short, void *)) = &p->update_timer; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, struct lcb_nameinfo_st *) = &p->get_nameinfo; (void)tmp; }
  { void(* *tmp)(void) = &p->pad1; (void)tmp; }
  { void(* *tmp)(void) = &p->pad2; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, lcb_sockdata_t *, void(*)(lcb_sockdata_t *)) = &p->send_error; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *) = &p->stop_event_loop; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *) = &p->run_event_loop; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_iops_completion_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_iops_completion_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_iops_completion_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_iops_completion_st, cookie),
    sizeof(((struct lcb_iops_completion_st *)0)->cookie),
    offsetof(struct lcb_iops_completion_st, error),
    sizeof(((struct lcb_iops_completion_st *)0)->error),
    offsetof(struct lcb_iops_completion_st, need_cleanup),
    sizeof(((struct lcb_iops_completion_st *)0)->need_cleanup),
    offsetof(struct lcb_iops_completion_st, create_socket),
    sizeof(((struct lcb_iops_completion_st *)0)->create_socket),
    offsetof(struct lcb_iops_completion_st, start_connect),
    sizeof(((struct lcb_iops_completion_st *)0)->start_connect),
    offsetof(struct lcb_iops_completion_st, create_writebuf),
    sizeof(((struct lcb_iops_completion_st *)0)->create_writebuf),
    offsetof(struct lcb_iops_completion_st, release_writebuf),
    sizeof(((struct lcb_iops_completion_st *)0)->release_writebuf),
    offsetof(struct lcb_iops_completion_st, start_write),
    sizeof(((struct lcb_iops_completion_st *)0)->start_write),
    offsetof(struct lcb_iops_completion_st, start_read),
    sizeof(((struct lcb_iops_completion_st *)0)->start_read),
    offsetof(struct lcb_iops_completion_st, close_socket),
    sizeof(((struct lcb_iops_completion_st *)0)->close_socket),
    offsetof(struct lcb_iops_completion_st, create_timer),
    sizeof(((struct lcb_iops_completion_st *)0)->create_timer),
    offsetof(struct lcb_iops_completion_st, destroy_timer),
    sizeof(((struct lcb_iops_completion_st *)0)->destroy_timer),
    offsetof(struct lcb_iops_completion_st, delete_timer),
    sizeof(((struct lcb_iops_completion_st *)0)->delete_timer),
    offsetof(struct lcb_iops_completion_st, update_timer),
    sizeof(((struct lcb_iops_completion_st *)0)->update_timer),
    offsetof(struct lcb_iops_completion_st, get_nameinfo),
    sizeof(((struct lcb_iops_completion_st *)0)->get_nameinfo),
    offsetof(struct lcb_iops_completion_st, pad1),
    sizeof(((struct lcb_iops_completion_st *)0)->pad1),
    offsetof(struct lcb_iops_completion_st, pad2),
    sizeof(((struct lcb_iops_completion_st *)0)->pad2),
    offsetof(struct lcb_iops_completion_st, send_error),
    sizeof(((struct lcb_iops_completion_st *)0)->send_error),
    offsetof(struct lcb_iops_completion_st, stop_event_loop),
    sizeof(((struct lcb_iops_completion_st *)0)->stop_event_loop),
    offsetof(struct lcb_iops_completion_st, run_event_loop),
    sizeof(((struct lcb_iops_completion_st *)0)->run_event_loop),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_iops_completion_st(0);
}

static void _cffi_check_struct_lcb_iops_evented_st(struct lcb_iops_evented_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->cookie; (void)tmp; }
  (void)((p->error) << 1);
  (void)((p->need_cleanup) << 1);
  { int(* *tmp)(struct lcb_io_opt_st *, int, int, int) = &p->socket; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, struct sockaddr const *, unsigned int) = &p->connect; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, void *, unsigned long, int) = &p->recv; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, void const *, unsigned long, int) = &p->send; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, lcb_IOV *, unsigned long) = &p->recvv; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, lcb_IOV *, unsigned long) = &p->sendv; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, int) = &p->close; (void)tmp; }
  { void *(* *tmp)(struct lcb_io_opt_st *) = &p->create_timer; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, void *) = &p->destroy_timer; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, void *) = &p->delete_timer; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, void *, unsigned int, void *, void(*)(int, short, void *)) = &p->update_timer; (void)tmp; }
  { void *(* *tmp)(struct lcb_io_opt_st *) = &p->create_event; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, void *) = &p->destroy_event; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, int, void *, short, void *, void(*)(int, short, void *)) = &p->update_event; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, int, void *) = &p->delete_event; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *) = &p->stop_event_loop; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *) = &p->run_event_loop; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_iops_evented_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_iops_evented_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_iops_evented_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_iops_evented_st, cookie),
    sizeof(((struct lcb_iops_evented_st *)0)->cookie),
    offsetof(struct lcb_iops_evented_st, error),
    sizeof(((struct lcb_iops_evented_st *)0)->error),
    offsetof(struct lcb_iops_evented_st, need_cleanup),
    sizeof(((struct lcb_iops_evented_st *)0)->need_cleanup),
    offsetof(struct lcb_iops_evented_st, socket),
    sizeof(((struct lcb_iops_evented_st *)0)->socket),
    offsetof(struct lcb_iops_evented_st, connect),
    sizeof(((struct lcb_iops_evented_st *)0)->connect),
    offsetof(struct lcb_iops_evented_st, recv),
    sizeof(((struct lcb_iops_evented_st *)0)->recv),
    offsetof(struct lcb_iops_evented_st, send),
    sizeof(((struct lcb_iops_evented_st *)0)->send),
    offsetof(struct lcb_iops_evented_st, recvv),
    sizeof(((struct lcb_iops_evented_st *)0)->recvv),
    offsetof(struct lcb_iops_evented_st, sendv),
    sizeof(((struct lcb_iops_evented_st *)0)->sendv),
    offsetof(struct lcb_iops_evented_st, close),
    sizeof(((struct lcb_iops_evented_st *)0)->close),
    offsetof(struct lcb_iops_evented_st, create_timer),
    sizeof(((struct lcb_iops_evented_st *)0)->create_timer),
    offsetof(struct lcb_iops_evented_st, destroy_timer),
    sizeof(((struct lcb_iops_evented_st *)0)->destroy_timer),
    offsetof(struct lcb_iops_evented_st, delete_timer),
    sizeof(((struct lcb_iops_evented_st *)0)->delete_timer),
    offsetof(struct lcb_iops_evented_st, update_timer),
    sizeof(((struct lcb_iops_evented_st *)0)->update_timer),
    offsetof(struct lcb_iops_evented_st, create_event),
    sizeof(((struct lcb_iops_evented_st *)0)->create_event),
    offsetof(struct lcb_iops_evented_st, destroy_event),
    sizeof(((struct lcb_iops_evented_st *)0)->destroy_event),
    offsetof(struct lcb_iops_evented_st, update_event),
    sizeof(((struct lcb_iops_evented_st *)0)->update_event),
    offsetof(struct lcb_iops_evented_st, delete_event),
    sizeof(((struct lcb_iops_evented_st *)0)->delete_event),
    offsetof(struct lcb_iops_evented_st, stop_event_loop),
    sizeof(((struct lcb_iops_evented_st *)0)->stop_event_loop),
    offsetof(struct lcb_iops_evented_st, run_event_loop),
    sizeof(((struct lcb_iops_evented_st *)0)->run_event_loop),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_iops_evented_st(0);
}

static void _cffi_check_struct_lcb_iovec_st(struct lcb_iovec_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->iov_base; (void)tmp; }
  (void)((p->iov_len) << 1);
}
static PyObject *
_cffi_layout_struct_lcb_iovec_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_iovec_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_iovec_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_iovec_st, iov_base),
    sizeof(((struct lcb_iovec_st *)0)->iov_base),
    offsetof(struct lcb_iovec_st, iov_len),
    sizeof(((struct lcb_iovec_st *)0)->iov_len),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_iovec_st(0);
}

static void _cffi_check_struct_lcb_logprocs_st(struct lcb_logprocs_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $40' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_logprocs_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_logprocs_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_logprocs_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_logprocs_st, version),
    sizeof(((struct lcb_logprocs_st *)0)->version),
    offsetof(struct lcb_logprocs_st, v),
    sizeof(((struct lcb_logprocs_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_logprocs_st(0);
}

static void _cffi_check_struct_lcb_loopprocs_st(struct lcb_loopprocs_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void(* *tmp)(struct lcb_io_opt_st *) = &p->start; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *) = &p->stop; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *) = &p->tick; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_loopprocs_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_loopprocs_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_loopprocs_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_loopprocs_st, start),
    sizeof(((struct lcb_loopprocs_st *)0)->start),
    offsetof(struct lcb_loopprocs_st, stop),
    sizeof(((struct lcb_loopprocs_st *)0)->stop),
    offsetof(struct lcb_loopprocs_st, tick),
    sizeof(((struct lcb_loopprocs_st *)0)->tick),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_loopprocs_st(0);
}

static void _cffi_check_struct_lcb_memcached_st(struct lcb_memcached_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { char const * *tmp = &p->serverlist; (void)tmp; }
  { char const * *tmp = &p->username; (void)tmp; }
  { char const * *tmp = &p->password; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_memcached_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_memcached_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_memcached_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_memcached_st, serverlist),
    sizeof(((struct lcb_memcached_st *)0)->serverlist),
    offsetof(struct lcb_memcached_st, username),
    sizeof(((struct lcb_memcached_st *)0)->username),
    offsetof(struct lcb_memcached_st, password),
    sizeof(((struct lcb_memcached_st *)0)->password),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_memcached_st(0);
}

static void _cffi_check_struct_lcb_nameinfo_st(struct lcb_nameinfo_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  /* cannot generate 'struct $1' in field 'local': unknown type name */
  /* cannot generate 'struct $2' in field 'remote': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_nameinfo_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_nameinfo_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_nameinfo_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_nameinfo_st, local),
    sizeof(((struct lcb_nameinfo_st *)0)->local),
    offsetof(struct lcb_nameinfo_st, remote),
    sizeof(((struct lcb_nameinfo_st *)0)->remote),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_nameinfo_st(0);
}

static void _cffi_check_struct_lcb_observe_cmd_st(struct lcb_observe_cmd_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $15' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_observe_cmd_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_observe_cmd_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_observe_cmd_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_observe_cmd_st, version),
    sizeof(((struct lcb_observe_cmd_st *)0)->version),
    offsetof(struct lcb_observe_cmd_st, v),
    sizeof(((struct lcb_observe_cmd_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_observe_cmd_st(0);
}

static void _cffi_check_struct_lcb_remove_cmd_st(struct lcb_remove_cmd_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $17' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_remove_cmd_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_remove_cmd_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_remove_cmd_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_remove_cmd_st, version),
    sizeof(((struct lcb_remove_cmd_st *)0)->version),
    offsetof(struct lcb_remove_cmd_st, v),
    sizeof(((struct lcb_remove_cmd_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_remove_cmd_st(0);
}

static void _cffi_check_struct_lcb_server_stat_resp_st(struct lcb_server_stat_resp_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $24' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_server_stat_resp_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_server_stat_resp_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_server_stat_resp_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_server_stat_resp_st, version),
    sizeof(((struct lcb_server_stat_resp_st *)0)->version),
    offsetof(struct lcb_server_stat_resp_st, v),
    sizeof(((struct lcb_server_stat_resp_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_server_stat_resp_st(0);
}

static void _cffi_check_struct_lcb_server_stats_cmd_st(struct lcb_server_stats_cmd_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $23' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_server_stats_cmd_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_server_stats_cmd_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_server_stats_cmd_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_server_stats_cmd_st, version),
    sizeof(((struct lcb_server_stats_cmd_st *)0)->version),
    offsetof(struct lcb_server_stats_cmd_st, v),
    sizeof(((struct lcb_server_stats_cmd_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_server_stats_cmd_st(0);
}

static void _cffi_check_struct_lcb_server_version_cmd_st(struct lcb_server_version_cmd_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $25' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_server_version_cmd_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_server_version_cmd_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_server_version_cmd_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_server_version_cmd_st, version),
    sizeof(((struct lcb_server_version_cmd_st *)0)->version),
    offsetof(struct lcb_server_version_cmd_st, v),
    sizeof(((struct lcb_server_version_cmd_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_server_version_cmd_st(0);
}

static void _cffi_check_struct_lcb_server_version_resp_st(struct lcb_server_version_resp_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $27' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_server_version_resp_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_server_version_resp_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_server_version_resp_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_server_version_resp_st, version),
    sizeof(((struct lcb_server_version_resp_st *)0)->version),
    offsetof(struct lcb_server_version_resp_st, v),
    sizeof(((struct lcb_server_version_resp_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_server_version_resp_st(0);
}

static void _cffi_check_struct_lcb_sockdata_st(struct lcb_sockdata_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->socket) << 1);
  { struct lcb_io_opt_st * *tmp = &p->parent; (void)tmp; }
  { struct lcbio_SOCKET * *tmp = &p->lcbconn; (void)tmp; }
  (void)((p->closed) << 1);
  (void)((p->is_reading) << 1);
  { struct lcb_buf_info *tmp = &p->read_buffer; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_sockdata_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_sockdata_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_sockdata_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_sockdata_st, socket),
    sizeof(((struct lcb_sockdata_st *)0)->socket),
    offsetof(struct lcb_sockdata_st, parent),
    sizeof(((struct lcb_sockdata_st *)0)->parent),
    offsetof(struct lcb_sockdata_st, lcbconn),
    sizeof(((struct lcb_sockdata_st *)0)->lcbconn),
    offsetof(struct lcb_sockdata_st, closed),
    sizeof(((struct lcb_sockdata_st *)0)->closed),
    offsetof(struct lcb_sockdata_st, is_reading),
    sizeof(((struct lcb_sockdata_st *)0)->is_reading),
    offsetof(struct lcb_sockdata_st, read_buffer),
    sizeof(((struct lcb_sockdata_st *)0)->read_buffer),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_sockdata_st(0);
}

static void _cffi_check_struct_lcb_store_cmd_st(struct lcb_store_cmd_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $11' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_store_cmd_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_store_cmd_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_store_cmd_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_store_cmd_st, version),
    sizeof(((struct lcb_store_cmd_st *)0)->version),
    offsetof(struct lcb_store_cmd_st, v),
    sizeof(((struct lcb_store_cmd_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_store_cmd_st(0);
}

static void _cffi_check_struct_lcb_timerprocs_st(struct lcb_timerprocs_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void *(* *tmp)(struct lcb_io_opt_st *) = &p->create; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, void *) = &p->destroy; (void)tmp; }
  { void(* *tmp)(struct lcb_io_opt_st *, void *) = &p->cancel; (void)tmp; }
  { int(* *tmp)(struct lcb_io_opt_st *, void *, unsigned int, void *, void(*)(int, short, void *)) = &p->schedule; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_lcb_timerprocs_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_timerprocs_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_timerprocs_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_timerprocs_st, create),
    sizeof(((struct lcb_timerprocs_st *)0)->create),
    offsetof(struct lcb_timerprocs_st, destroy),
    sizeof(((struct lcb_timerprocs_st *)0)->destroy),
    offsetof(struct lcb_timerprocs_st, cancel),
    sizeof(((struct lcb_timerprocs_st *)0)->cancel),
    offsetof(struct lcb_timerprocs_st, schedule),
    sizeof(((struct lcb_timerprocs_st *)0)->schedule),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_timerprocs_st(0);
}

static void _cffi_check_struct_lcb_unlock_cmd_st(struct lcb_unlock_cmd_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $9' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_unlock_cmd_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_unlock_cmd_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_unlock_cmd_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_unlock_cmd_st, version),
    sizeof(((struct lcb_unlock_cmd_st *)0)->version),
    offsetof(struct lcb_unlock_cmd_st, v),
    sizeof(((struct lcb_unlock_cmd_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_unlock_cmd_st(0);
}

static void _cffi_check_struct_lcb_verbosity_cmd_st(struct lcb_verbosity_cmd_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $29' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_verbosity_cmd_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_verbosity_cmd_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_verbosity_cmd_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_verbosity_cmd_st, version),
    sizeof(((struct lcb_verbosity_cmd_st *)0)->version),
    offsetof(struct lcb_verbosity_cmd_st, v),
    sizeof(((struct lcb_verbosity_cmd_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_verbosity_cmd_st(0);
}

static void _cffi_check_struct_lcb_verbosity_resp_st(struct lcb_verbosity_resp_st *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->version) << 1);
  /* cannot generate 'union $30' in field 'v': unknown type name */
}
static PyObject *
_cffi_layout_struct_lcb_verbosity_resp_st(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct lcb_verbosity_resp_st y; };
  static Py_ssize_t nums[] = {
    sizeof(struct lcb_verbosity_resp_st),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct lcb_verbosity_resp_st, version),
    sizeof(((struct lcb_verbosity_resp_st *)0)->version),
    offsetof(struct lcb_verbosity_resp_st, v),
    sizeof(((struct lcb_verbosity_resp_st *)0)->v),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_lcb_verbosity_resp_st(0);
}

static void _cffi_check_union_lcb_CRST_u(union lcb_CRST_u *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { struct lcb_create_st0 *tmp = &p->v0; (void)tmp; }
  { struct lcb_create_st1 *tmp = &p->v1; (void)tmp; }
  { struct lcb_create_st2 *tmp = &p->v2; (void)tmp; }
  { struct lcb_create_st3 *tmp = &p->v3; (void)tmp; }
}
static PyObject *
_cffi_layout_union_lcb_CRST_u(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; union lcb_CRST_u y; };
  static Py_ssize_t nums[] = {
    sizeof(union lcb_CRST_u),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(union lcb_CRST_u, v0),
    sizeof(((union lcb_CRST_u *)0)->v0),
    offsetof(union lcb_CRST_u, v1),
    sizeof(((union lcb_CRST_u *)0)->v1),
    offsetof(union lcb_CRST_u, v2),
    sizeof(((union lcb_CRST_u *)0)->v2),
    offsetof(union lcb_CRST_u, v3),
    sizeof(((union lcb_CRST_u *)0)->v3),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_union_lcb_CRST_u(0);
}

static int _cffi_setup_custom(PyObject *lib)
{
  return _cffi_const_LCB_RESP_F_FINAL(lib);
}

static PyMethodDef _cffi_methods[] = {
  {"_cffi_layout__lcb_ARITHCMDv0", _cffi_layout__lcb_ARITHCMDv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_ARITHRESPv0", _cffi_layout__lcb_ARITHRESPv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_CMDCOUNTER", _cffi_layout__lcb_CMDCOUNTER, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_CMDENDURE", _cffi_layout__lcb_CMDENDURE, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_CMDGET", _cffi_layout__lcb_CMDGET, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_CMDGETREPLICA", _cffi_layout__lcb_CMDGETREPLICA, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_CMDHTTP", _cffi_layout__lcb_CMDHTTP, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_CMDOBSEQNO", _cffi_layout__lcb_CMDOBSEQNO, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_CMDOBSERVE", _cffi_layout__lcb_CMDOBSERVE, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_CMDSTORE", _cffi_layout__lcb_CMDSTORE, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_CMDVERBOSITY", _cffi_layout__lcb_CMDVERBOSITY, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_CMDVIEWQUERY", _cffi_layout__lcb_CMDVIEWQUERY, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_DURABILITYCMDv0", _cffi_layout__lcb_DURABILITYCMDv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_DURABILITYOPTSv0", _cffi_layout__lcb_DURABILITYOPTSv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_DURABILITYRESPv0", _cffi_layout__lcb_DURABILITYRESPv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_GETCMDv0", _cffi_layout__lcb_GETCMDv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_GETREPLICACMDv0", _cffi_layout__lcb_GETREPLICACMDv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_GETREPLICACMDv1", _cffi_layout__lcb_GETREPLICACMDv1, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_GETRESPv0", _cffi_layout__lcb_GETRESPv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_HTTPCMDv0", _cffi_layout__lcb_HTTPCMDv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_HTTPCMDv1", _cffi_layout__lcb_HTTPCMDv1, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_HTTPRESPv0", _cffi_layout__lcb_HTTPRESPv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_IOCREATEOPS_FUNCTIONPOINTER", _cffi_layout__lcb_IOCREATEOPS_FUNCTIONPOINTER, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_IOCREATEOPTS_BUILTIN", _cffi_layout__lcb_IOCREATEOPTS_BUILTIN, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_IOCREATEOPTS_DSO", _cffi_layout__lcb_IOCREATEOPTS_DSO, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_OBSERVECMDv0", _cffi_layout__lcb_OBSERVECMDv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_OBSERVECMDv1", _cffi_layout__lcb_OBSERVECMDv1, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_OBSERVERESPv0", _cffi_layout__lcb_OBSERVERESPv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_REMOVECMDv0", _cffi_layout__lcb_REMOVECMDv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_REMOVERESPv0", _cffi_layout__lcb_REMOVERESPv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_RESPBASE", _cffi_layout__lcb_RESPBASE, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_RESPCOUNTER", _cffi_layout__lcb_RESPCOUNTER, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_RESPENDURE", _cffi_layout__lcb_RESPENDURE, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_RESPGET", _cffi_layout__lcb_RESPGET, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_RESPHTTP", _cffi_layout__lcb_RESPHTTP, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_RESPMCVERSION", _cffi_layout__lcb_RESPMCVERSION, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_RESPOBSEQNO", _cffi_layout__lcb_RESPOBSEQNO, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_RESPOBSERVE", _cffi_layout__lcb_RESPOBSERVE, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_RESPREMOVE", _cffi_layout__lcb_RESPREMOVE, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_RESPSERVERBASE", _cffi_layout__lcb_RESPSERVERBASE, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_RESPSTATS", _cffi_layout__lcb_RESPSTATS, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_RESPSTORE", _cffi_layout__lcb_RESPSTORE, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_STATSCMDv0", _cffi_layout__lcb_STATSCMDv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_STATSRESPv0", _cffi_layout__lcb_STATSRESPv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_STORECMDv0", _cffi_layout__lcb_STORECMDv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_STORERESPv0", _cffi_layout__lcb_STORERESPv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_SYNCTOKEN", _cffi_layout__lcb_SYNCTOKEN, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_TOUCHRESPv0", _cffi_layout__lcb_TOUCHRESPv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_UNLOCKCMDv0", _cffi_layout__lcb_UNLOCKCMDv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_UNLOCKRESPv0", _cffi_layout__lcb_UNLOCKRESPv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_VERBOSITYCMDv0", _cffi_layout__lcb_VERBOSITYCMDv0, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_arithmetic_resp_t", _cffi_layout__lcb_arithmetic_resp_t, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_completion_procs", _cffi_layout__lcb_completion_procs, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_get_resp_t", _cffi_layout__lcb_get_resp_t, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_http_resp_t", _cffi_layout__lcb_http_resp_t, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_observe_resp_t", _cffi_layout__lcb_observe_resp_t, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_remove_resp_t", _cffi_layout__lcb_remove_resp_t, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_store_resp_t", _cffi_layout__lcb_store_resp_t, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_touch_resp_t", _cffi_layout__lcb_touch_resp_t, METH_NOARGS, NULL},
  {"_cffi_layout__lcb_unlock_resp_t", _cffi_layout__lcb_unlock_resp_t, METH_NOARGS, NULL},
  {"_Cb_do_callback", _cffi_f__Cb_do_callback, METH_VARARGS, NULL},
  {"_Cb_set_key", _cffi_f__Cb_set_key, METH_VARARGS, NULL},
  {"_Cb_set_val", _cffi_f__Cb_set_val, METH_VARARGS, NULL},
  {"lcb__create_compat_230", _cffi_f_lcb__create_compat_230, METH_VARARGS, NULL},
  {"lcb_arithmetic", _cffi_f_lcb_arithmetic, METH_VARARGS, NULL},
  {"lcb_behavior_get_config_errors_threshold", _cffi_f_lcb_behavior_get_config_errors_threshold, METH_O, NULL},
  {"lcb_behavior_get_ipv6", _cffi_f_lcb_behavior_get_ipv6, METH_O, NULL},
  {"lcb_behavior_get_syncmode", _cffi_f_lcb_behavior_get_syncmode, METH_O, NULL},
  {"lcb_behavior_set_config_errors_threshold", _cffi_f_lcb_behavior_set_config_errors_threshold, METH_VARARGS, NULL},
  {"lcb_behavior_set_ipv6", _cffi_f_lcb_behavior_set_ipv6, METH_VARARGS, NULL},
  {"lcb_behavior_set_syncmode", _cffi_f_lcb_behavior_set_syncmode, METH_VARARGS, NULL},
  {"lcb_breakout", _cffi_f_lcb_breakout, METH_O, NULL},
  {"lcb_cancel_http_request", _cffi_f_lcb_cancel_http_request, METH_VARARGS, NULL},
  {"lcb_cbflush3", _cffi_f_lcb_cbflush3, METH_VARARGS, NULL},
  {"lcb_cntl", _cffi_f_lcb_cntl, METH_VARARGS, NULL},
  {"lcb_cntl_exists", _cffi_f_lcb_cntl_exists, METH_O, NULL},
  {"lcb_cntl_getu32", _cffi_f_lcb_cntl_getu32, METH_VARARGS, NULL},
  {"lcb_cntl_setu32", _cffi_f_lcb_cntl_setu32, METH_VARARGS, NULL},
  {"lcb_cntl_string", _cffi_f_lcb_cntl_string, METH_VARARGS, NULL},
  {"lcb_connect", _cffi_f_lcb_connect, METH_O, NULL},
  {"lcb_counter3", _cffi_f_lcb_counter3, METH_VARARGS, NULL},
  {"lcb_create", _cffi_f_lcb_create, METH_VARARGS, NULL},
  {"lcb_create_io_ops", _cffi_f_lcb_create_io_ops, METH_VARARGS, NULL},
  {"lcb_destroy", _cffi_f_lcb_destroy, METH_O, NULL},
  {"lcb_destroy_async", _cffi_f_lcb_destroy_async, METH_VARARGS, NULL},
  {"lcb_destroy_io_ops", _cffi_f_lcb_destroy_io_ops, METH_O, NULL},
  {"lcb_disable_timings", _cffi_f_lcb_disable_timings, METH_O, NULL},
  {"lcb_dump", _cffi_f_lcb_dump, METH_VARARGS, NULL},
  {"lcb_durability_poll", _cffi_f_lcb_durability_poll, METH_VARARGS, NULL},
  {"lcb_enable_timings", _cffi_f_lcb_enable_timings, METH_O, NULL},
  {"lcb_endure3_ctxnew", _cffi_f_lcb_endure3_ctxnew, METH_VARARGS, NULL},
  {"lcb_errmap_default", _cffi_f_lcb_errmap_default, METH_VARARGS, NULL},
  {"lcb_flush", _cffi_f_lcb_flush, METH_VARARGS, NULL},
  {"lcb_flush3", _cffi_f_lcb_flush3, METH_VARARGS, NULL},
  {"lcb_flush_buffers", _cffi_f_lcb_flush_buffers, METH_VARARGS, NULL},
  {"lcb_get", _cffi_f_lcb_get, METH_VARARGS, NULL},
  {"lcb_get3", _cffi_f_lcb_get3, METH_VARARGS, NULL},
  {"lcb_get_bootstrap_status", _cffi_f_lcb_get_bootstrap_status, METH_O, NULL},
  {"lcb_get_callback3", _cffi_f_lcb_get_callback3, METH_VARARGS, NULL},
  {"lcb_get_cookie", _cffi_f_lcb_get_cookie, METH_O, NULL},
  {"lcb_get_errtype", _cffi_f_lcb_get_errtype, METH_O, NULL},
  {"lcb_get_host", _cffi_f_lcb_get_host, METH_O, NULL},
  {"lcb_get_last_error", _cffi_f_lcb_get_last_error, METH_O, NULL},
  {"lcb_get_node", _cffi_f_lcb_get_node, METH_VARARGS, NULL},
  {"lcb_get_num_nodes", _cffi_f_lcb_get_num_nodes, METH_O, NULL},
  {"lcb_get_num_replicas", _cffi_f_lcb_get_num_replicas, METH_O, NULL},
  {"lcb_get_port", _cffi_f_lcb_get_port, METH_O, NULL},
  {"lcb_get_replica", _cffi_f_lcb_get_replica, METH_VARARGS, NULL},
  {"lcb_get_server_list", _cffi_f_lcb_get_server_list, METH_O, NULL},
  {"lcb_get_synctoken", _cffi_f_lcb_get_synctoken, METH_VARARGS, NULL},
  {"lcb_get_timeout", _cffi_f_lcb_get_timeout, METH_O, NULL},
  {"lcb_get_timings", _cffi_f_lcb_get_timings, METH_VARARGS, NULL},
  {"lcb_get_version", _cffi_f_lcb_get_version, METH_O, NULL},
  {"lcb_get_view_timeout", _cffi_f_lcb_get_view_timeout, METH_O, NULL},
  {"lcb_http3", _cffi_f_lcb_http3, METH_VARARGS, NULL},
  {"lcb_install_callback3", _cffi_f_lcb_install_callback3, METH_VARARGS, NULL},
  {"lcb_iops_wire_bsd_impl2", _cffi_f_lcb_iops_wire_bsd_impl2, METH_VARARGS, NULL},
  {"lcb_is_waiting", _cffi_f_lcb_is_waiting, METH_O, NULL},
  {"lcb_make_http_request", _cffi_f_lcb_make_http_request, METH_VARARGS, NULL},
  {"lcb_mem_alloc", _cffi_f_lcb_mem_alloc, METH_O, NULL},
  {"lcb_mem_free", _cffi_f_lcb_mem_free, METH_O, NULL},
  {"lcb_n1p_encode", _cffi_f_lcb_n1p_encode, METH_VARARGS, NULL},
  {"lcb_n1p_free", _cffi_f_lcb_n1p_free, METH_O, NULL},
  {"lcb_n1p_mkcmd", _cffi_f_lcb_n1p_mkcmd, METH_VARARGS, NULL},
  {"lcb_n1p_namedparam", _cffi_f_lcb_n1p_namedparam, METH_VARARGS, NULL},
  {"lcb_n1p_new", _cffi_f_lcb_n1p_new, METH_NOARGS, NULL},
  {"lcb_n1p_posparam", _cffi_f_lcb_n1p_posparam, METH_VARARGS, NULL},
  {"lcb_n1p_reset", _cffi_f_lcb_n1p_reset, METH_O, NULL},
  {"lcb_n1p_setconsistency", _cffi_f_lcb_n1p_setconsistency, METH_VARARGS, NULL},
  {"lcb_n1p_setopt", _cffi_f_lcb_n1p_setopt, METH_VARARGS, NULL},
  {"lcb_n1p_setquery", _cffi_f_lcb_n1p_setquery, METH_VARARGS, NULL},
  {"lcb_n1p_synctoken", _cffi_f_lcb_n1p_synctoken, METH_VARARGS, NULL},
  {"lcb_n1ql_query", _cffi_f_lcb_n1ql_query, METH_VARARGS, NULL},
  {"lcb_nstime", _cffi_f_lcb_nstime, METH_NOARGS, NULL},
  {"lcb_observe", _cffi_f_lcb_observe, METH_VARARGS, NULL},
  {"lcb_observe3_ctxnew", _cffi_f_lcb_observe3_ctxnew, METH_O, NULL},
  {"lcb_observe_seqno3", _cffi_f_lcb_observe_seqno3, METH_VARARGS, NULL},
  {"lcb_refresh_config", _cffi_f_lcb_refresh_config, METH_O, NULL},
  {"lcb_remove", _cffi_f_lcb_remove, METH_VARARGS, NULL},
  {"lcb_remove3", _cffi_f_lcb_remove3, METH_VARARGS, NULL},
  {"lcb_resp_get_synctoken", _cffi_f_lcb_resp_get_synctoken, METH_VARARGS, NULL},
  {"lcb_rget3", _cffi_f_lcb_rget3, METH_VARARGS, NULL},
  {"lcb_run_loop", _cffi_f_lcb_run_loop, METH_O, NULL},
  {"lcb_sched_enter", _cffi_f_lcb_sched_enter, METH_O, NULL},
  {"lcb_sched_fail", _cffi_f_lcb_sched_fail, METH_O, NULL},
  {"lcb_sched_flush", _cffi_f_lcb_sched_flush, METH_O, NULL},
  {"lcb_sched_leave", _cffi_f_lcb_sched_leave, METH_O, NULL},
  {"lcb_server_stats", _cffi_f_lcb_server_stats, METH_VARARGS, NULL},
  {"lcb_server_verbosity3", _cffi_f_lcb_server_verbosity3, METH_VARARGS, NULL},
  {"lcb_server_versions", _cffi_f_lcb_server_versions, METH_VARARGS, NULL},
  {"lcb_server_versions3", _cffi_f_lcb_server_versions3, METH_VARARGS, NULL},
  {"lcb_set_arithmetic_callback", _cffi_f_lcb_set_arithmetic_callback, METH_VARARGS, NULL},
  {"lcb_set_bootstrap_callback", _cffi_f_lcb_set_bootstrap_callback, METH_VARARGS, NULL},
  {"lcb_set_configuration_callback", _cffi_f_lcb_set_configuration_callback, METH_VARARGS, NULL},
  {"lcb_set_cookie", _cffi_f_lcb_set_cookie, METH_VARARGS, NULL},
  {"lcb_set_destroy_callback", _cffi_f_lcb_set_destroy_callback, METH_VARARGS, NULL},
  {"lcb_set_durability_callback", _cffi_f_lcb_set_durability_callback, METH_VARARGS, NULL},
  {"lcb_set_errmap_callback", _cffi_f_lcb_set_errmap_callback, METH_VARARGS, NULL},
  {"lcb_set_error_callback", _cffi_f_lcb_set_error_callback, METH_VARARGS, NULL},
  {"lcb_set_flush_callback", _cffi_f_lcb_set_flush_callback, METH_VARARGS, NULL},
  {"lcb_set_get_callback", _cffi_f_lcb_set_get_callback, METH_VARARGS, NULL},
  {"lcb_set_http_complete_callback", _cffi_f_lcb_set_http_complete_callback, METH_VARARGS, NULL},
  {"lcb_set_http_data_callback", _cffi_f_lcb_set_http_data_callback, METH_VARARGS, NULL},
  {"lcb_set_observe_callback", _cffi_f_lcb_set_observe_callback, METH_VARARGS, NULL},
  {"lcb_set_remove_callback", _cffi_f_lcb_set_remove_callback, METH_VARARGS, NULL},
  {"lcb_set_stat_callback", _cffi_f_lcb_set_stat_callback, METH_VARARGS, NULL},
  {"lcb_set_store_callback", _cffi_f_lcb_set_store_callback, METH_VARARGS, NULL},
  {"lcb_set_timeout", _cffi_f_lcb_set_timeout, METH_VARARGS, NULL},
  {"lcb_set_touch_callback", _cffi_f_lcb_set_touch_callback, METH_VARARGS, NULL},
  {"lcb_set_unlock_callback", _cffi_f_lcb_set_unlock_callback, METH_VARARGS, NULL},
  {"lcb_set_verbosity", _cffi_f_lcb_set_verbosity, METH_VARARGS, NULL},
  {"lcb_set_verbosity_callback", _cffi_f_lcb_set_verbosity_callback, METH_VARARGS, NULL},
  {"lcb_set_version_callback", _cffi_f_lcb_set_version_callback, METH_VARARGS, NULL},
  {"lcb_set_view_timeout", _cffi_f_lcb_set_view_timeout, METH_VARARGS, NULL},
  {"lcb_stats3", _cffi_f_lcb_stats3, METH_VARARGS, NULL},
  {"lcb_stop_loop", _cffi_f_lcb_stop_loop, METH_O, NULL},
  {"lcb_store", _cffi_f_lcb_store, METH_VARARGS, NULL},
  {"lcb_store3", _cffi_f_lcb_store3, METH_VARARGS, NULL},
  {"lcb_strerror", _cffi_f_lcb_strerror, METH_VARARGS, NULL},
  {"lcb_supports_feature", _cffi_f_lcb_supports_feature, METH_O, NULL},
  {"lcb_tick_nowait", _cffi_f_lcb_tick_nowait, METH_O, NULL},
  {"lcb_timer_create", _cffi_f_lcb_timer_create, METH_VARARGS, NULL},
  {"lcb_timer_destroy", _cffi_f_lcb_timer_destroy, METH_VARARGS, NULL},
  {"lcb_touch", _cffi_f_lcb_touch, METH_VARARGS, NULL},
  {"lcb_touch3", _cffi_f_lcb_touch3, METH_VARARGS, NULL},
  {"lcb_unlock", _cffi_f_lcb_unlock, METH_VARARGS, NULL},
  {"lcb_unlock3", _cffi_f_lcb_unlock3, METH_VARARGS, NULL},
  {"lcb_verify_struct_size", _cffi_f_lcb_verify_struct_size, METH_VARARGS, NULL},
  {"lcb_view_cancel", _cffi_f_lcb_view_cancel, METH_VARARGS, NULL},
  {"lcb_view_query", _cffi_f_lcb_view_query, METH_VARARGS, NULL},
  {"lcb_view_query_initcmd", _cffi_f_lcb_view_query_initcmd, METH_VARARGS, NULL},
  {"lcb_wait", _cffi_f_lcb_wait, METH_O, NULL},
  {"lcb_wait3", _cffi_f_lcb_wait3, METH_VARARGS, NULL},
  {"memset", _cffi_f_memset, METH_VARARGS, NULL},
  {"_cffi_layout_struct_lcb_CMDBASE", _cffi_layout_struct_lcb_CMDBASE, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_CMDN1QL", _cffi_layout_struct_lcb_CMDN1QL, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_CONTIGBUF", _cffi_layout_struct_lcb_CONTIGBUF, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_FRAGBUF", _cffi_layout_struct_lcb_FRAGBUF, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_KEYBUF", _cffi_layout_struct_lcb_KEYBUF, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_MULTICMD_CTX_st", _cffi_layout_struct_lcb_MULTICMD_CTX_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_RESPN1QL", _cffi_layout_struct_lcb_RESPN1QL, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_RESPVIEW_st", _cffi_layout_struct_lcb_RESPVIEW_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_VALBUF", _cffi_layout_struct_lcb_VALBUF, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_arithmetic_cmd_st", _cffi_layout_struct_lcb_arithmetic_cmd_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_bsdprocs_st", _cffi_layout_struct_lcb_bsdprocs_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_buf_info", _cffi_layout_struct_lcb_buf_info, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_cached_config_st", _cffi_layout_struct_lcb_cached_config_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_cntl_iops_info_st", _cffi_layout_struct_lcb_cntl_iops_info_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_cntl_rdballocfactory", _cffi_layout_struct_lcb_cntl_rdballocfactory, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_cntl_server_st", _cffi_layout_struct_lcb_cntl_server_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_cntl_vbinfo_st", _cffi_layout_struct_lcb_cntl_vbinfo_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_create_io_ops_st", _cffi_layout_struct_lcb_create_io_ops_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_create_st", _cffi_layout_struct_lcb_create_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_create_st0", _cffi_layout_struct_lcb_create_st0, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_create_st1", _cffi_layout_struct_lcb_create_st1, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_create_st2", _cffi_layout_struct_lcb_create_st2, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_create_st3", _cffi_layout_struct_lcb_create_st3, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_durability_cmd_st", _cffi_layout_struct_lcb_durability_cmd_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_durability_opts_st", _cffi_layout_struct_lcb_durability_opts_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_durability_resp_st", _cffi_layout_struct_lcb_durability_resp_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_evprocs_st", _cffi_layout_struct_lcb_evprocs_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_flush_cmd_st", _cffi_layout_struct_lcb_flush_cmd_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_flush_resp_st", _cffi_layout_struct_lcb_flush_resp_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_get_cmd_st", _cffi_layout_struct_lcb_get_cmd_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_get_replica_cmd_st", _cffi_layout_struct_lcb_get_replica_cmd_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_http_cmd_st", _cffi_layout_struct_lcb_http_cmd_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_io_opt_st", _cffi_layout_struct_lcb_io_opt_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_io_writebuf_st", _cffi_layout_struct_lcb_io_writebuf_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_iops2_st", _cffi_layout_struct_lcb_iops2_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_iops3_st", _cffi_layout_struct_lcb_iops3_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_iops_completion_st", _cffi_layout_struct_lcb_iops_completion_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_iops_evented_st", _cffi_layout_struct_lcb_iops_evented_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_iovec_st", _cffi_layout_struct_lcb_iovec_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_logprocs_st", _cffi_layout_struct_lcb_logprocs_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_loopprocs_st", _cffi_layout_struct_lcb_loopprocs_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_memcached_st", _cffi_layout_struct_lcb_memcached_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_nameinfo_st", _cffi_layout_struct_lcb_nameinfo_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_observe_cmd_st", _cffi_layout_struct_lcb_observe_cmd_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_remove_cmd_st", _cffi_layout_struct_lcb_remove_cmd_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_server_stat_resp_st", _cffi_layout_struct_lcb_server_stat_resp_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_server_stats_cmd_st", _cffi_layout_struct_lcb_server_stats_cmd_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_server_version_cmd_st", _cffi_layout_struct_lcb_server_version_cmd_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_server_version_resp_st", _cffi_layout_struct_lcb_server_version_resp_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_sockdata_st", _cffi_layout_struct_lcb_sockdata_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_store_cmd_st", _cffi_layout_struct_lcb_store_cmd_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_timerprocs_st", _cffi_layout_struct_lcb_timerprocs_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_unlock_cmd_st", _cffi_layout_struct_lcb_unlock_cmd_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_verbosity_cmd_st", _cffi_layout_struct_lcb_verbosity_cmd_st, METH_NOARGS, NULL},
  {"_cffi_layout_struct_lcb_verbosity_resp_st", _cffi_layout_struct_lcb_verbosity_resp_st, METH_NOARGS, NULL},
  {"_cffi_layout_union_lcb_CRST_u", _cffi_layout_union_lcb_CRST_u, METH_NOARGS, NULL},
  {"_cffi_setup", _cffi_setup, METH_VARARGS, NULL},
  {NULL, NULL, 0, NULL}    /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef _cffi_module_def = {
  PyModuleDef_HEAD_INIT,
  "_cffi__xcb10e4exeff87a51",
  NULL,
  -1,
  _cffi_methods,
  NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit__cffi__xcb10e4exeff87a51(void)
{
  PyObject *lib;
  lib = PyModule_Create(&_cffi_module_def);
  if (lib == NULL)
    return NULL;
  if (((void)lib,0) < 0 || _cffi_init() < 0) {
    Py_DECREF(lib);
    return NULL;
  }
  return lib;
}

#else

PyMODINIT_FUNC
init_cffi__xcb10e4exeff87a51(void)
{
  PyObject *lib;
  lib = Py_InitModule("_cffi__xcb10e4exeff87a51", _cffi_methods);
  if (lib == NULL)
    return;
  if (((void)lib,0) < 0 || _cffi_init() < 0)
    return;
  return;
}

#endif
