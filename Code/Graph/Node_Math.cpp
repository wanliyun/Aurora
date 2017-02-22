
#include "stdafx.h"
#include "Node_Math.h"
#include <iostream>
#include <boost/any.hpp>

namespace Aurora
{
	NodeAdd::NodeAdd()
	{
		m_input_a.SetOwner(this);
		m_input_b.SetOwner(this);
	}

	NodeAdd::~NodeAdd()
	{

	}

	bool NodeAdd::Process()
	{
		if(m_input_a.GetVal().type() != m_input_b.GetVal().type())
		{
			LogF("invalid type\n");
			return false;
		}
		if(m_input_a.GetVal().type() == typeid(int))
		{
			
			int a = boost::any_cast<int>(m_input_a.GetVal());
			int b = boost::any_cast<int>(m_input_b.GetVal());
			LogF("NodeAdd::Process %d+%d=%d\n",a,b,a+b);
		}
		else if(m_input_a.GetVal().type() == typeid(std::string))
		{
			std::string a = boost::any_cast<std::string>(m_input_a.GetVal());
			std::string b = boost::any_cast<std::string>(m_input_b.GetVal());
			LogF("NodeAdd::Process [%s]+[%s]=[%s]\n",a.c_str(),b.c_str(),(a+b).c_str());
		}
		else if(m_input_a.GetVal().type() == typeid(float))
		{
			float a = boost::any_cast<float>(m_input_a.GetVal());
			float b = boost::any_cast<float>(m_input_b.GetVal());
			LogF("NodeAdd::Process [%f]+[%f]=[%f]\n",a,b,a+b);
		}
		else
		{
			LogF("NodeAdd::Process Bad Type = %s\n",m_input_a.GetVal().type().name());
		}
		return true;
	}

}
