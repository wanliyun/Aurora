#ifndef _MAIN_MACRO_H_
#define _MAIN_MACRO_H_

#ifdef WIN32

#define MY_MAIN__(APPCLASS,TICKTIME)\
	static void my_exit__()\
{\
	printf("before exit()!\n");\
}\
	static int main__()\
{\
	atexit(my_exit__);\
	Aurora::NetworkInit::get_mutable_instance();\
	try\
	{\
		APPCLASS::get_mutable_instance().Init(TICKTIME);\
		APPCLASS::get_mutable_instance().Run();\
		APPCLASS::get_mutable_instance().Fini();\
	}\
	catch(bpy::error_already_set e)\
	{\
		PyErr_Print();\
	}\
	catch(...)\
	{\
		printf("main__ except..\n");\
	}\
	return 0;\
}

#define MY_MAIN(APPCLASS,TICKTIME)\
	MY_MAIN__(APPCLASS,TICKTIME)\
	int wmain(int argc, wchar_t **argv)\
{\
	return main__();\
}

#define PY_MY_MAIN(APPCLASS,TICKTIME)\
	MY_MAIN__(APPCLASS,TICKTIME)\
	int wmain(int argc, wchar_t **argv)\
{\
	return Py_Main(argc, argv);\
}\
namespace Aurora\
{\
	using namespace boost::python;\
	BOOST_PYTHON_MODULE(AuroraStart)\
	{\
	def("run",main__);\
	};\
	AUTO_REGISTER_PYTHON_MODULE(AuroraStart);\
}


#else//	WIN32
#include <signal.h>

#define MY_MAIN__(APPCLASS,TICKTIME)\
static void SignHandler__(int iSignNo)\
{\
	exit(0);\
}\
static void my_exit__()\
{\
	printf("before exit()!\n");\
}\
static int main__()\
{\
	atexit(my_exit__);\
	signal(SIGINT,SignHandler__);\
	signal(SIGILL,SignHandler__);\
	signal(SIGSEGV,SignHandler__);\
	signal(SIGTERM,SignHandler__);\
	signal(SIGABRT,SignHandler__);\
	Aurora::NetworkInit::get_mutable_instance();\
	APPCLASS::get_mutable_instance().Init(TICKTIME);\
	APPCLASS::get_mutable_instance().Run();\
	APPCLASS::get_mutable_instance().Fini();\
	return 0;\
}

#define MY_MAIN(APPCLASS,TICKTIME)\
	MY_MAIN__(APPCLASS,TICKTIME)\
	int main(int argc, char **argv)\
	{\
		return main__();\
	}\

#define PY_MY_MAIN(APPCLASS,TICKTIME)\
	MY_MAIN__(APPCLASS,TICKTIME)\
int main(int argc, char **argv)\
{\
	wchar_t **argv_copy;\
	wchar_t **argv_copy2;\
	int i, res;\
	char *oldloc;\
	argv_copy = (wchar_t **)PyMem_Malloc(sizeof(wchar_t*)*(argc+1));\
	argv_copy2 = (wchar_t **)PyMem_Malloc(sizeof(wchar_t*)*(argc+1));\
	if (!argv_copy || !argv_copy2) {\
	fprintf(stderr, "out of memory\n");\
	return 1;\
	}\
	oldloc = strdup(setlocale(LC_ALL, NULL));\
	setlocale(LC_ALL, "");\
	for (i = 0; i < argc; i++) {\
	argv_copy[i] = _Py_char2wchar(argv[i], NULL);\
	if (!argv_copy[i]) {\
	free(oldloc);\
	fprintf(stderr, "Fatal Python error: "\
	"unable to decode the command line argument #%i\n",\
	i + 1);\
	return 1;\
	}\
	argv_copy2[i] = argv_copy[i];\
	}\
	argv_copy2[argc] = argv_copy[argc] = NULL;\
	setlocale(LC_ALL, oldloc);\
	free(oldloc);\
	res = Py_Main(argc, argv_copy);\
	for (i = 0; i < argc; i++) {\
	PyMem_Free(argv_copy2[i]);\
	}\
	PyMem_Free(argv_copy);\
	PyMem_Free(argv_copy2);\
	return res;\
}\
namespace Aurora\
{\
	using namespace boost::python;\
	BOOST_PYTHON_MODULE(AuroraStart)\
	{\
		def("run",main__);\
	};\
	AUTO_REGISTER_PYTHON_MODULE(AuroraStart);\
}
#endif



#endif //_MAIN_MACRO_H_
