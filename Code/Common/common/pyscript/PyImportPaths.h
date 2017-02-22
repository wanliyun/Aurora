

#ifndef PY_IMPORT_PATHS_H
#define PY_IMPORT_PATHS_H

#include <boost/python.hpp>

namespace Aurora
{
	class PyImportPaths
	{
	public:
		static const char DEFAULT_DELIMITER =
#ifndef _WIN32
			';';
#else
			':';
#endif


		PyImportPaths( char delimiter = DEFAULT_DELIMITER );

		const std::string pathsAsString() const;
		PyObject * pathAsObject() const;

		bool empty() const 
		{ return paths_.empty(); }

		void append( const PyImportPaths & other );

		void addResPath( const std::string & path );
		void addNonResPath( const std::string & path );

		void setDelimiter( char delimiter )
		{ delimiter_ = delimiter; }

	private:
		typedef std::vector< std::string > OrderedPaths;
		OrderedPaths paths_;
		char delimiter_;
	};

}
#endif // PY_IMPORT_PATHS_HPP
