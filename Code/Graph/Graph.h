#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <string>
#include "NodeBase.h"

namespace Aurora
{
	class  Graph
	{
	public:
		Graph();
		~Graph();

		bool LoadFromFile(const std::string& strPath);

		void Process();

		void Test();

		void SetStartNode(NodeBase * pNode){m_startNode = pNode;}
		NodeBase * GetStartNode()const{return m_startNode;}
	private:
		NodeBase * m_startNode;	//每张Graph仅允许有一个开始节点;
	};
}
#endif //_GRAPH_H_