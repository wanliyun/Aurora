#ifndef _NODE_MATH_H
#define _NODE_MATH_H

#include "NodeBase.h"

namespace Aurora
{
	class  NodeAdd : public NodeBase
	{
	public:
		NodeAdd();
		~NodeAdd();

	private:
		virtual bool Process();

	public:
		NodeParamInput m_input_a;
		NodeParamInput m_input_b;
	};
}
#endif //_NODE_MATH_H