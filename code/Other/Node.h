#pragma once

#include "BaseObject.h"

class Node : public BaseObject
{

public:

    Node();
    virtual ~Node();

    /* === Functions === */

    //! Enables or disables the node.
    virtual void setVisible(bool isVisible);

    //! Adds the specified child.
    virtual void addChild(Node* Child);

    //! Adds the specified children.
    virtual void addChildren(const std::list<Node*>& Children);

    //! Removes the specified child and returns true if the child could be removed.
    virtual bool removeChild(Node* Child);

    //! Removes the first child and returns true if a child could be removed.
    virtual bool removeChild();

    //! Removes the specified children and returns the count of removed children.
    virtual u32 removeChildren(const std::list<Node*>& Children);

    //! Removes all children.
    virtual void removeChildren();

    /* === Inline functions === */

    //! Returns the children list.
    inline const std::list<Node*>& getChildren() const
    {
        return Children_;
    }

    //! Returns true if this node is visible (or rather enabled).
    inline bool getVisible() const
    {
        return isVisible_;
    }

    //! Sets the parent node.
    inline void setParent(Node* Parent)
    {
        Parent_ = Parent;
    }
    //! Returns the parent node.
    inline Node* getParent() const
    {
        return Parent_;
    }

protected:

    /* === Members === */

    bool isVisible_;

    Node* Parent_;
    std::list<Node*> Children_;

};