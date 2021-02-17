#pragma once

class Node
{
public:
	virtual ~Node() = default;

	void AddChild(Node* Child);
	void AddChild(const std::list<Node*>& Children);

	bool RemoveChild(Node* Child);
	bool RemoveChild();

	uint32_t RemoveChildren(const std::list<Node*>& Children);
	void RemoveChildren();


	void SetParent(Node* Parent)
	{
		m_parent = Parent;
	}

	Node* GetParent() const
	{
		return m_parent;
	}

	const std::list<Node*>& GetChildren() const
	{
		return m_children;
	}

	void SetVisible(bool isVisible)
	{
		m_isVisible = isVisible;
	}

	bool GetVisible() const
	{
		return m_isVisible;
	}

protected:
	Node* m_parent = nullptr;
	std::list<Node*> m_children;

	bool m_isVisible = true;
};