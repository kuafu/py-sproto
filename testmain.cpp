#include <stdio.h>
#include <python.h>

#include <iostream>
using namespace std;


//extern "C" __declspec(dllexport) PyMODINIT_FUNC initpysproto(void);
// extern PyMODINIT_FUNC initpysproto(void);

void unreal_engine_py_log_error() {
	PyObject *type = NULL;
	PyObject *value = NULL;
	PyObject *traceback = NULL;

	PyErr_Fetch(&type, &value, &traceback);
	PyErr_NormalizeException(&type, &value, &traceback);

	if(!value)
	{
		PyErr_Clear();
		return;
	}

	char *msg = NULL;
#if PY_MAJOR_VERSION >= 3
	PyObject *zero = PyUnicode_AsUTF8String(PyObject_Str(value));
	if(zero)
	{
		msg = PyBytes_AsString(zero);
	}
#else
	msg = PyString_AsString(PyObject_Str(value));
#endif
	if(!msg)
	{
		PyErr_Clear();
		return;
	}

	cout << msg << endl;
	//UE_LOG(LogPython, Error, TEXT("%s"), UTF8_TO_TCHAR(msg));

	// taken from uWSGI ;)
	if(!traceback)
	{
		PyErr_Clear();
		return;
	}

	PyObject *traceback_module = PyImport_ImportModule("traceback");
	if(!traceback_module)
	{
		PyErr_Clear();
		return;
	}

	PyObject *traceback_dict = PyModule_GetDict(traceback_module);
	PyObject *format_exception = PyDict_GetItemString(traceback_dict, "format_exception");

	if(format_exception)
	{
		PyObject *ret = PyObject_CallFunctionObjArgs(format_exception, type, value, traceback, NULL);
		if(!ret)
		{
			PyErr_Clear();
			return;
		}
		if(PyList_Check(ret))
		{
			for(int i = 0; i < PyList_Size(ret); i++)
			{
				PyObject *item = PyList_GetItem(ret, i);
				if(item)
				{
					cout << PyObject_Str(item) << endl;
					//UE_LOG(LogPython, Error, TEXT("%s"), UTF8_TO_TCHAR(PyUnicode_AsUTF8(PyObject_Str(item))));
				}
			}
		}
		else
		{
			cout << PyObject_Str(ret) << endl;
			//UE_LOG(LogPython, Error, TEXT("%s"), UTF8_TO_TCHAR(PyUnicode_AsUTF8(PyObject_Str(ret))));
		}
	}

	PyErr_Clear();
}

// static PyObject *init_unreal_engine(void) {
// 	return PyModule_Create(&unreal_engine_module);
// }

extern "C" void unreal_engine_init_py_module();


int wmain(int argc, wchar_t **argv)
{
	Py_Initialize();
//#if PY_MAJOR_VERSION >= 3
//	wchar_t *argv[] = { "UnrealEngine", NULL };
//#else
//	char *argv[] = { (char *)"UnrealEngine", NULL };
//#endif
	PySys_SetArgv(1, argv);

	PyEval_InitThreads();

	//////////////////////////////////////////////////////////////////////////
	unreal_engine_init_py_module();
	//initpysproto();
	//static PyObject * module = 
	//	initpysproto();
	//PyImport_AppendInittab("pysproto", module);
	//PyObject *new_unreal_engine_module = PyImport_AddModule("pysproto");


	//////////////////////////////////////////////////////////////////////////

	PyObject *py_sys = PyImport_ImportModule("sys");
	PyObject *py_sys_dict = PyModule_GetDict(py_sys);

	PyObject *py_path = PyDict_GetItemString(py_sys_dict, "path");

	//char *zip_path = TCHAR_TO_UTF8(*FPaths::Combine(*FPaths::GameContentDir(), UTF8_TO_TCHAR("python35.zip")));
	//PyObject *py_zip_path = PyUnicode_FromString(zip_path);
	//PyList_Insert(py_path, 0, py_zip_path);

	char *scripts_path = "Scripts";
	PyObject *py_scripts_path = PyUnicode_FromString(scripts_path);
	PyList_Insert(py_path, 0, py_scripts_path);

	cout << "--->" << Py_GetVersion() << endl;
	cout << "script path:" << scripts_path << endl;

	PyObject *main_module = PyImport_AddModule("__main__");

	void *main_dict;
	void *local_dict;

	main_dict = PyModule_GetDict(main_module);
	local_dict = main_dict;// PyDict_New();

	if(PyImport_ImportModule("ue_site"))
	{
		cout << "ue_site Python module successfully imported" << endl;
	}
	else
	{
		// TODO gracefully manage the error
		unreal_engine_py_log_error();
	}



	char c;
	cin >> c;
	return c;
	//return Py_Main(argc, argv);
}