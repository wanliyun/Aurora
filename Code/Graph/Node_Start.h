#ifndef _NODE_START_H_
#define _NODE_START_H_

#include "NodeBase.h"

namespace Aurora
{
	class  NodeStart : public NodeBase
	{
	public:
		NodeStart();
		~NodeStart();

	private:
		virtual bool Process();

	public:
		NodeParamOutput m_output;
	};
}
#endif //_NODE_START_H_