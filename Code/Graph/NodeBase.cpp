
#include "stdafx.h"
#include "NodeBase.h"

namespace Aurora
{

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//NodeParamInput
	void NodeParamInput::OnActived()
	{
		assert(GetOwner());
		GetOwner()->SetActive(true);
	}


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//NodeParamOutput
	void NodeParamOutput::OnActived()
	{
		for(std::vector<NodeParamInput *>::const_iterator it = m_inputs.begin();it!=m_inputs.end();++it)
		{
			NodeParamInput * in = *it;
			assert(in);
			if(!in)
				continue;
			in->SetVal(m_val);
			in->OnActived();
		}
	}

	void NodeParamOutput::AppendRelativeNodes( std::set<NodeBase *> & nodeSet )
	{
		for(std::vector<NodeParamInput *>::const_iterator it = m_inputs.begin();it!=m_inputs.end();++it)
		{
			NodeParamInput * in = *it;
			assert(in && in->GetOwner());
			if(!in || !in->GetOwner())
				continue;
			nodeSet.insert(in->GetOwner());
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//NodeBase
	NodeBase::NodeBase():m_bActived(false)
	{
		m_inputActive.SetOwner(this);
		m_inputActive.SetVal(boost::any(bool(false)));

		
	}

	NodeBase::~NodeBase()
	{

	}

	bool NodeBase::Process()
	{
		if(!m_bActived)
			return true;

		SetActive(false);
		return false;
	}

	void NodeBase::ProcessRelativeNode( const std::set<NodeBase *> & nodeSet )
	{
		for(std::set<NodeBase *>::const_iterator it = nodeSet.begin();it!=nodeSet.end();++it)
		{
			(*it)->Process();
		}
	}

}

