#ifndef _EXPORTRS_H_
#define _EXPORTRS_H_

#if 0
	#ifdef GRAPH_EXPORTS
		#define GRAPH_API __declspec(dllexport)
	#else
		#define GRAPH_API __declspec(dllimport)
	#endif
#else
	#define GRAPH_API
#endif

#endif
