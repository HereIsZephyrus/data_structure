//
//  tree.hpp
//  data_structure
//
//  Created by ChanningTong on 10/21/24.
//

#ifndef tree_h
#define tree_h
#include <iostream>
#include <fstream>
#include <strstream>
#include <exception>

namespace tcb {
using std::ostream;
enum class TraversalType{
    preorder,
    inorder,
    postorder
};
template<class Object>
struct TreeNode{
    Object element;
    TreeNode *firstChild;
    TreeNode * nextSibling;
    TreeNode(const Object& x, TreeNode* fchild = nullptr, TreeNode* sibling = nullptr):
    element(x),firstChild(fchild),nextSibling(sibling){}
};
template<class Object>
struct BinaryTreeNode{
    Object element;
    BinaryTreeNode *left;
    BinaryTreeNode * right;
    BinaryTreeNode(const Object& x, BinaryTreeNode* lt = nullptr, BinaryTreeNode* rt = nullptr):
    element(x),left(lt),right(rt){}
};
template<class Object>
struct QuadTreeNode{
    Object element;
    QuadTreeNode* northeast;
    QuadTreeNode* northwest;
    QuadTreeNode* southeast;
    QuadTreeNode* southwest;
    QuadTreeNode(const Object& x, QuadTreeNode* ne = nullptr, QuadTreeNode* nw = nullptr,QuadTreeNode* se = nullptr, QuadTreeNode* sw = nullptr):
    element(x),northeast(ne),northwest(nw),southeast(se),southwest(sw){}
};

template <class Object,class NodeStructure>
class Tree {
protected:
    size_t size;
    NodeStructure* root;
public:
    Tree(){}
    ~Tree(){}
    bool isEmpty(){
        if (root == nullptr)
            return true;
        return false;
    }
    virtual void clear() = 0;
    virtual bool contains(const Object& x) const = 0;
    virtual void insert(const Object& x, NodeStructure* & node) const = 0;
    virtual void remove(const Object& x, NodeStructure* & node) const = 0;
};

template <class Object>
class BinarySearchTree : protected Tree<Object, BinaryTreeNode<Object>>{
    using node = BinaryTreeNode<Object>;
    using BST = BinarySearchTree;
    static TraversalType outputFlag;
public:
    BinarySearchTree(){this->root = nullptr;}
    ~BinarySearchTree(){clear();}
    const BST & operator = (const BST & rhs){
        if (this != &rhs){
            clear();
            this->root = clone(rhs.root);
        }
        return *this;
    }
    friend ostream& operator<<(ostream& os, const BST& tree);
    bool contains(const Object& x) const{return contains(x, this->root);}
    void insert(const Object& x){insert(x, this->root);}
    void remove(const Object& x){remove(x, this->root);}
    void clear(){destroy(this->root);}
    const Object& findMin() const{
        if (this -> root == nullptr)
            throw std::logic_error("the tree is empty");
        return findMin(this->root)->element;
    }
    const Object& findMax() const{
        if (this -> root == nullptr)
            throw std::logic_error("the tree is empty");
        return findMax(this->root)->element;
    }
    static ostream& preorder(ostream& os){
        outputFlag = TraversalType::preorder;
        return os;
    }
    static ostream& inorder(ostream& os){
        outputFlag = TraversalType::inorder;
        return os;
    }
    static ostream& postorder(ostream& os){
        outputFlag = TraversalType::postorder;
        return os;
    }
protected:
    static ostream& print(ostream& os, node* p){
        if (p == nullptr)
            return os;
        switch (outputFlag) {
            case TraversalType::inorder:
                os<<print(os, p->left)<<p->element<<print(os,p->right);
                break;
            case TraversalType::preorder:
                os<<p->element<<print(os, p->left)<<print(os,p->right);
                break;
            case TraversalType::postorder:
                os<<print(os, p->left)<<print(os,p->right)<<p->element;
                break;
        }
    }
    node* clone(node* rhst) const{
        if (rhst == nullptr)
            return nullptr;
        return new node(rhst->element,clone(rhst->left),clone(rhst->right));
    }
    bool contains(const Object& x, node* p) const{
        if (p == nullptr)
            return false;
        else if (x == p->element)
            return true;
        else if (x < p->element)
            return  contains(x,p->left);
        else
            return contains(x,p->right);
        return true;
    }
    node* findMin(node* p) const{
        if (p->left == nullptr)
            return p;
        return findMin(p->left);
    }
    node* findMax(node * p) const{
        if (p->right == nullptr)
            return p;
        return findMax(p->right);
    }
private:
    void insert(const Object& x, node* & p) const{
        if ( p == nullptr)
            p  = new node(x, nullptr, nullptr);
        if (p->element == x)
            return;
        else if (x < p->element)
            insert(x, p->left);
        else
            insert(x, p->right);
    }
    void remove(const Object& x, node* & p) const{
        if (p == nullptr)
            return;
        if (x < p->element)
            remove(x, p->left);
        else if (x > p->element)
            remove(x, p->right);
        else{// find it
            if (p->left != nullptr && p->right != nullptr){
                p->element = findMin(p->right)->element;
                remove(p->element, p->right);
            }
            else{
                node* ret_p = p;
                if (p->left == nullptr)
                    p = p->right;
                else
                    p = p->left;
                delete ret_p;
            }
        }
    }
    void destroy(node* p){
        if (p != nullptr){
            destroy(p->left);
            destroy(p->right);
            delete p;
        }
    }
};
template <class Object>
TraversalType BinarySearchTree<Object>::outputFlag = TraversalType::inorder;
template <class Object>
ostream& operator<<(ostream& os,const BinarySearchTree<Object>& tree){return BinarySearchTree<Object>::print(os,tree.root);}

template <class Object>
class QuadTree : protected Tree<Object, QuadTreeNode<Object>>{
    using node = QuadTreeNode<Object>;
public:
    QuadTree(){this->root = nullptr;}
    ~QuadTree(){clear();}
    void clear(){destroy(this->root);}
private:
    void destroy(node* p){
        if (p != nullptr){
            destroy(p->left);
            destroy(p->right);
            delete p;
        }
    }
};
}
#endif /* tree_h */
