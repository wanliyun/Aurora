
#include "stdafx.h"
#include "Graph.h"
#include <FileSystem.h>
#include <tinyXML/tinyxml.h>
#include "Node_Start.h"
#include "Node_Math.h"
namespace Aurora
{
	Graph::Graph():m_startNode(0)
	{

	}

	Graph::~Graph()
	{

	}

	bool Graph::LoadFromFile( const std::string& strPath )
	{
		std::string relPath = FileSystem::get_mutable_instance().FindFilePath(strPath);
		TiXmlDocument doc;
		if(!doc.LoadFile(relPath.c_str()))
			return false;
		return true;
	}

	void Graph::Process()
	{
		assert(m_startNode);
		if(!m_startNode)
			return;
		m_startNode->Process();
	}

	void Graph::Test()
	{
		boost::shared_ptr<NodeAdd> pNodeAdd(new NodeAdd);
		//NodeAdd	* pNodeAdd = new NodeAdd;
		//pNodeAdd->m_input_a.SetVal(boost::any(int(10)));
		//pNodeAdd->m_input_b.SetVal(boost::any(int(12)));

		pNodeAdd->m_input_a.SetVal(boost::any(std::string("10")));
		pNodeAdd->m_input_b.SetVal(boost::any(std::string("12")));

		boost::shared_ptr<NodeStart> pNodeStart(new NodeStart);
		pNodeStart->m_output.m_inputs.push_back( & (pNodeAdd->GetActiveInput()));

		SetStartNode(pNodeStart.get());
		Process();
		SetStartNode(0);

	}

}
