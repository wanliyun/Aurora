#include "pch.h"
#include "FileSystem.h"
#include "PyImportPaths.h"
#include "FileSystem.h"


namespace Aurora
{

	PyImportPaths::PyImportPaths( char delimiter ) :
		paths_(),
		delimiter_( delimiter )
	{}

	PyObject * PyImportPaths::pathAsObject() const
	{
		PyObject *pResult, *pPath;
		size_t len = paths_.size();
		pResult = PyList_New( len );

		size_t i = 0;
		for (OrderedPaths::const_iterator iPath = paths_.begin();
			iPath != paths_.end(); 
			++iPath)
		{
			pPath = PyUnicode_FromString((*iPath).c_str());

			if (pPath != NULL) 
			{
				PyList_SetItem(pResult, i, pPath);
				++i;
			}
		}
		return pResult;
	}

	const std::string PyImportPaths::pathsAsString() const
	{
		std::ostringstream oss;

		bool first = true;
		OrderedPaths::const_iterator iPath = paths_.begin();
		while (iPath != paths_.end())
		{
			if (!first)
			{
				oss << delimiter_;
			}

			oss << *iPath;

			first = false;
			++iPath;
		}

		return oss.str();
	}


	void PyImportPaths::addResPath( const std::string & path )
	{
		size_t numPaths = FileSystem::get_mutable_instance().GetNumPaths();

		for( size_t i = 0; i < numPaths; i++)
		{
			this->addNonResPath( FileSystem::get_mutable_instance().GetPath( i ) + path );
		}
	}

	void PyImportPaths::addNonResPath( const std::string & path )
	{
		if (std::find( paths_.begin(), paths_.end(), path) == paths_.end())
		{
			paths_.push_back( path );
		}
	}

	void PyImportPaths::append( const PyImportPaths & other )
	{
		OrderedPaths::const_iterator iOtherPath = other.paths_.begin();

		while (iOtherPath != other.paths_.end())
		{
			this->addNonResPath( *iOtherPath );
			++iOtherPath;
		}
	}

}

