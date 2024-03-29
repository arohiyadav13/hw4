#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n); 
		void removeFix(AVLNode<Key, Value>* n, int diff); 
		void rrotate(AVLNode<Key, Value>* c);
		void lrotate(AVLNode<Key, Value>* c); 

};


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

//good
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    const Key& key = new_item.first;
    const Value& value = new_item.second;

    AVLNode<Key,Value>* parent = NULL; 
    AVLNode<Key,Value>* current = static_cast<AVLNode<Key,Value>*>(this->root_);

    while (current) 
    {
        parent = current;
        if (key < current->getKey()) 
        {
            current = current->getLeft();
        } 
        else if (key > current->getKey()) 
        {
            current = current->getRight();
        } 
        else 
        {
            current->setValue(value);
            return;
        }
    }

    AVLNode<Key,Value>* temp = new AVLNode<Key,Value>(key, value, parent);
    if (!parent) 
    {
        this->root_ = temp;
    } 
    else if (key < parent->getKey()) 
    {
        parent->setLeft(temp); 
    } 
    else 
    {
        parent->setRight(temp);   
    }

    if (parent && parent->getBalance() == 0) 
    {
        if (temp == parent->getLeft()) 
        {
            parent->setBalance(-1);
        } 
        else 
        {
            parent->setBalance(1);
        }
        insertFix(parent, temp);
    } 
    else if (parent) 
    {
        parent->setBalance(0); 
    }
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
 //good
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
  AVLNode<Key, Value>* removing = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
  if (!removing) 
  {
    return; 
  }
  AVLNode<Key, Value>* parent = removing->getParent();
  int diff = 0;

  if (removing->getLeft() && removing->getRight()) 
  {
    AVLNode<Key, Value>* predecessor = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(removing));
    nodeSwap(predecessor, removing);
    parent = removing->getParent();
  }

   if (parent) 
   {
    if (parent->getLeft() == removing) 
    {
      diff = 1;
    } 
    else 
    {
      diff = -1;
    }
  }

  AVLNode<Key, Value>* child = nullptr;
    if (removing->getLeft()) 
    {
      child = removing->getLeft();
    } 
    else 
    {
      child = removing->getRight();
    }

  if (!parent) 
  {
    this->root_ = child;
  } 
  else if (parent->getLeft() == removing) 
  {
    parent->setLeft(child);
  } 
  else 
  {
    parent->setRight(child);
  }

  if (child) 
  {
    child->setParent(parent);
  }

  delete removing;
  removeFix(parent, diff);
}


//good
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n) 
{
  if(p==NULL)
  {
    return;
  }
  if(p->getParent() == nullptr)
  {
    return;
  }
  AVLNode<Key, Value> *temp = p->getParent();
  

if(temp->getLeft() == p)
	{
		temp->setBalance(temp->getBalance() - 1);
		if(temp->getBalance() == -1)
		{
			insertFix(temp, p); 
		}
		else if(temp->getBalance() == -2)
		{
			if(n == n->getParent()->getParent()->getLeft()->getLeft())
			{
				rrotate(temp);
				p->setBalance(0);
				temp->setBalance(0);
			}
		  else
			{
				lrotate(p);
				rrotate(temp);
				if(n->getBalance() == -1)
				{
					p->setBalance(0);
					temp->setBalance(1); 
          n->setBalance(0);
				}
				else if(!n->getBalance())
				{
					p->setBalance(0);
					temp->setBalance(0); 
          n->setBalance(0);
				}
				else
				{
					p->setBalance(-1);
					temp->setBalance(0); 
          n->setBalance(0);
				}
			}
		}
	}
	else
	{
		temp->setBalance(temp->getBalance() + 1);
		if(temp->getBalance() == 1)
		{
			insertFix(temp, p);
		}
		else if(temp->getBalance() == 2)
		{
			if(temp->getRight()->getRight() == n)
			{
				lrotate(temp);
				p->setBalance(0);
				temp->setBalance(0);
			}
		  else
			{
				rrotate(p);
				lrotate(temp);
				if(n->getBalance() == 1)
				{
					p->setBalance(0);
					temp->setBalance(-1);
          n->setBalance(0);
				}
				else if(!n->getBalance())
				{
					p->setBalance(0);
					temp->setBalance(0);
          n->setBalance(0);
				}
				else
				{
					p->setBalance(1);
					temp->setBalance(0);
          n->setBalance(0);
				}
			}
		}
	}
}






//kinda good
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int diff)
{
  // TODO
	if(n == NULL)
	{ 
		return;
	}
	AVLNode<Key, Value>* p = n->getParent();; 
	AVLNode<Key, Value>* child = n->getLeft();; 
	int d = 0; 

	if(p)
	{
		if(n == p->getLeft())
		{
			d = 1; 
		}
		else
		{
			d = -1; 
		}
	}

  if(diff == 1)
  {
    if(n->getBalance() + diff == 2)
    {
    AVLNode<Key, Value>* child = n->getRight();
    if(child->getBalance() == 1)
    {
      lrotate(n);
      n->setBalance(0);
      child->setBalance(0);
      removeFix(p, d);
    }
    else if(child->getBalance() == 0)
    {
      lrotate(n);
      n->setBalance(1);
      child->setBalance(-1);
    }
    else if(child->getBalance() == -1)
    {
      AVLNode<Key, Value>* grandchild = child->getLeft();
      rrotate(child);
      lrotate(n);

      if(grandchild->getBalance() == -1)
      {
        n->setBalance(0);
        child->setBalance(1);
      }
      else if(grandchild->getBalance() == 0)
      {
        n->setBalance(0);
        child->setBalance(0);
      }
      else if(grandchild->getBalance() == 1)
      {
        n->setBalance(-1);
        child->setBalance(0);
      }
      grandchild->setBalance(0);
      removeFix(p, d);
    }
    }
    else if(n->getBalance() + diff == 1)
    {
      n->setBalance(1);
    }
    else if(n->getBalance() + diff == 0)
    {
      n->setBalance(0);
      removeFix(p, d);
      }
    }


	if(diff == -1)
  {
    if(n->getBalance() + diff == -2)
    {
      if(child->getBalance() == -1)
      {
        rrotate(n);
        n->setBalance(0);
        child->setBalance(0);
        removeFix(p, d);
      }
      else if(child->getBalance() == 0)
      {
        rrotate(n);
        n->setBalance(-1);
        child->setBalance(1);
      }
      else if(child->getBalance() == 1)
      {
        AVLNode<Key, Value>* grandchild = child->getRight();
        lrotate(child);
        rrotate(n);

        if(grandchild->getBalance() == 1)
        {
          child->setBalance(-1);
          n->setBalance(0);
        }
        else if(grandchild->getBalance() == 0)
        {
          child->setBalance(0);
          n->setBalance(0);
        }
        else if(grandchild->getBalance() == -1)
        {
          child->setBalance(0);
          n->setBalance(1);
        }
        grandchild->setBalance(0);
        removeFix(p, d);
      }
    }
    else if(n->getBalance() + diff == -1)
    {
      n->setBalance(-1);
    }
    else
    {
      n->setBalance(0);
      removeFix(p, d);
    }
  }
}


//good
template<class Key, class Value>
void AVLTree<Key, Value>::rrotate(AVLNode<Key, Value>* c)
{
    
    if (!c || !c->getLeft()) {
        return;
    }

    AVLNode<Key, Value>* p = c->getLeft();
    AVLNode<Key, Value>* k = p->getRight();

    
    p->setParent(c->getParent());
    if (c->getParent()) {
        if (c == c->getParent()->getLeft()) {
            c->getParent()->setLeft(p);
        } else {
            c->getParent()->setRight(p);
        }
    } else {
        this->root_ = p;
    }

    if (k) {
        k->setParent(c);
    }
    c->setLeft(k);
    p->setRight(c);
    c->setParent(p);
}

template<class Key, class Value>
void AVLTree<Key, Value>::lrotate(AVLNode<Key, Value>* c)
{
    if (!c || !c->getRight()) {
        return;
    }

    AVLNode<Key, Value>* p = c->getRight();
    AVLNode<Key, Value>* k = p->getLeft();

    p->setParent(c->getParent());
    if (c->getParent()) {
        if (c == c->getParent()->getLeft()) {
            c->getParent()->setLeft(p);
        } else {
            c->getParent()->setRight(p);
        }
    } else {
        this->root_ = p;
    }

    if (k) {
        k->setParent(c);
    }
    c->setRight(k);
    p->setLeft(c);
    c->setParent(p);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif


