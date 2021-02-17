#include "stdafx.h"
#include "Node.h"
//-----------------------------------------------------------------------------
void Node::AddChild(Node* Child)
{
	m_children.push_back(Child);
}
//-----------------------------------------------------------------------------
void Node::AddChild(const std::list<Node*>& Children)
{
	for (auto it = Children.begin(); it != Children.end(); ++it)
		m_children.push_back(*it);
}
//-----------------------------------------------------------------------------
bool Node::RemoveChild(Node* Child)
{
	auto it = std::find(m_children.begin(), m_children.end(), Child);
	if (it != m_children.end())
	{
		m_children.erase(it);
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------------
bool Node::RemoveChild()
{
	if (!m_children.empty())
	{
		m_children.erase(m_children.begin());
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------
uint32_t Node::RemoveChildren(const std::list<Node*>& Children)
{
	uint32_t RemovedChildren = 0;

	for (auto it = Children.begin(); it != Children.end(); ++it)
	{
		if (RemoveChild(*it))
			++RemovedChildren;
	}

	return RemovedChildren;
}
//-----------------------------------------------------------------------------
void Node::RemoveChildren()
{
	m_children.clear();
}
//-----------------------------------------------------------------------------