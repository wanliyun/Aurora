#ifndef _NODE_BASE_H_
#define _NODE_BASE_H_

#include <Types.h>
#include <map>
#include <string>
#include <vector>
#include <set>
#include "boost/any.hpp"

namespace Aurora
{
#define NODE_INPUT_DECALRE(NAME)\
	NodeParamInput	NAME

#define NODE_OUTPUT_DECLARE(NAME)\
	NodeParamOutput	NAME

	class NodeBase;


	class NodeParam
	{
	public:
		NodeParam(NodeBase * pOwner = 0):m_pOwner(pOwner){}
		virtual ~NodeParam(){}

		virtual void OnActived()=0;

		void SetOwner(NodeBase * pOwner){m_pOwner = pOwner;}
		NodeBase * GetOwner()const{return m_pOwner;}

		void SetDesc(const std::string & desc){m_desc = desc;}
		const std::string GetDesc()const{return m_desc;}

		void SetVal(const boost::any  & val){m_val = val;}
		const boost::any & GetVal()const{return m_val;}
	protected:
		boost::any  m_val;
		NodeBase *	m_pOwner;
		std::string m_desc;
	};

	class NodeParamInput : public NodeParam
	{
	public:
		virtual void OnActived();

		void SetActive(bool bActive){m_bActived = bActive;}
		bool GetActive()const{m_bActived;}

	private:
		bool		m_bActived;
	};

	class NodeParamOutput : public NodeParam
	{
	public:
		void AppendRelativeNodes(std::set<NodeBase *> & nodeSet);
		virtual void OnActived();

		std::vector<NodeParamInput *> m_inputs;
	};

	class  NodeBase
	{
	public:
		NodeBase();
		~NodeBase();

		virtual bool Process();

		NodeParamInput & GetActiveInput(){return m_inputActive;}

		void SetActive(bool bActive){m_bActived = bActive;}
		bool GetActive()const{m_bActived;}

		void ProcessRelativeNode(const std::set<NodeBase *> & nodeSet);
	protected:
		bool m_bActived;
		NodeParamInput m_inputActive;
	};

}
#endif
