
#include "stdafx.h"
#include "Node_Start.h"

namespace Aurora
{
	NodeStart::NodeStart()
	{
		boost::any val(bool(false));
		m_output.SetOwner(this);
		m_output.SetVal(val);
	}

	NodeStart::~NodeStart()
	{

	}

	bool NodeStart::Process()
	{
		std::set<NodeBase *> affectedNode;
		m_output.AppendRelativeNodes(affectedNode);
		m_output.OnActived();
		ProcessRelativeNode(affectedNode);
		return true;
	}

}
