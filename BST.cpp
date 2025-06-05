#include "BST.h"

#include <algorithm>
#include <iostream>
#include <stack>
#include <stdexcept>

BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right): keyValuePair(std::make_pair(key, value)),
    parent(parent), 
    left(left), 
    right(right)
{}

BinarySearchTree::Node::Node(const Node &other)
    : keyValuePair(other.keyValuePair),
      parent(nullptr),
      left(nullptr),
      right(nullptr) {
    if (other.left) {
        left = new Node(*other.left);
        left->parent = this;
    }
    if (other.right) {
        right = new Node(*other.right);
        right->parent = this;
    }
}

bool BinarySearchTree::Node::operator==(const Node &other) const{
    if (keyValuePair == other.keyValuePair &&
    parent == other.parent && left == other.left && right == other.right){
        return true;
    }
    else{
        return false;
    }
}

void BinarySearchTree::Node::output_node_tree() const {
    if (left){
        left->output_node_tree();
    }

    std::cout << "[" << keyValuePair.first << ": " << keyValuePair.second << "] ";
    if (right){
        right->output_node_tree();
    }
}

void BinarySearchTree::Node::insert(const Key &key, const Value &value) {
    if (key < keyValuePair.first) {
        if (left){
            left->insert(key, value);
        }
        else{
            left = new Node(key, value, this);
        }
    }
    else{
        if (right){
            right->insert(key, value);
        }
        else{
            right = new Node(key, value, this);
        }
    }
}
//--------------
void BinarySearchTree::Node::erase(const Key &key){
    if (this){
        if (left){
            left->erase(key);
        }
        if (right){
            right->erase(key);
        }
        if (keyValuePair.first == key){
            if (left && right){
                if (parent->keyValuePair.first > keyValuePair.first){
                    parent->left = new Node(*left);
                }
                else{
                    parent->right = new Node(*left);
                }
                delete left;
            }
            else if(left){
                if (parent->keyValuePair.first > keyValuePair.first){
                    parent->left = new Node(*left);
                }
                else{
                    parent->right = new Node(*left);
                }
                delete left;
            }
            else if (right){
                if (parent->keyValuePair.first > keyValuePair.first){
                    parent->left = new Node(*right);
                }
                else{
                    parent->right = new Node(*right);
                }
                delete right;
            }
            else{
                delete this;
            }
        }
    }
}
//--------------------------------------------------------------------

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other){
    _size=other._size;
    if (other._root)
    {
        _root=new Node(*other._root);
    }
}

BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &other){
    if (this != &other) 
    {
        _root=nullptr;
        _size = other._size; 
        if (other._root != nullptr) {
            _root = new Node(*other._root);
        } 
        else {
            _root = nullptr;
        }
    }
    return *this;
}

//-------------------------------------------------->

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept {
    std::swap(_root, other._root);
    std::swap(_size, other._size);
}

BinarySearchTree &BinarySearchTree::operator=(BinarySearchTree &&other) noexcept{
    if (this != &other) 
    {
        _root == nullptr;
        _size = other._size;
        other._root->clear();
    }
    return *this;
}

//------------------------------------------------------>

BinarySearchTree::~BinarySearchTree() {
    std::stack<Node *> nodes;
    if (_root){
        nodes.push(_root);
    }
    while (!nodes.empty()) {
        Node *current = nodes.top();
        nodes.pop();
        if (current->left){
             nodes.push(current->left);
        }
        if (current->right){
             nodes.push(current->right);
        }
        delete current;
    }  
    _size.~size_t();
}

BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {}

//---------------------------------------------------->

std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() {
    return _node->keyValuePair;
}

const std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() const {
    return _node->keyValuePair;
}

std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() {
    return &_node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    if (!_node) return *this;

    if (_node->right) {
        _node = _node->right;
        while (_node->left){
             _node = _node->left;
        }
    } 
    else {
        Node *parent = _node->parent;
        while (parent && _node == parent->right) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}
//------------------------------->
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator t = *this;
    ++(*this);
    return t;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    if (_node->left) {
        _node = _node->left;
        while (_node->right) _node = _node->right;
    } else {
        Node *parent = _node->parent;
        while (parent && _node == parent->left) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }

    return *this;
}

//--------------------------------------------------->

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    if (!this){
        return Iterator(nullptr);
    }
    Iterator t = *this;
    --(*this);
    return t;
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const
{
    return _node != other._node;
}
//---------------------------------------------------------------------------->
BinarySearchTree::ConstIterator::ConstIterator(const Node *node): _node(node) 
{}

const std::pair<Key, Value> &BinarySearchTree::ConstIterator::operator*() const 
{
    return _node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::ConstIterator::operator->() const 
{
    return &_node->keyValuePair;
}
//----------------------------------------------------------------------------------->
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    if (!_node) return *this;

    if (_node->right) {
        _node = _node->right;
        while (_node->left) _node = _node->left;
    } else {
        const Node *parent = _node->parent;
        while (parent && _node == parent->right) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) 
{
    ConstIterator t = *this;
    ++(*this);
    return t;
}
//------------------------------------------------------------------->
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    if (!_node){
        return *this;
    }

    if (_node->left){
        _node = _node->left;
        while (_node->right) _node = _node->right;
    } 
    else{
        const Node *parent = _node->parent;
        while (parent && _node == parent->left) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) 
{
    if (!this){
        return ConstIterator(nullptr);
    }
    ConstIterator t = *this;
    --(*this);
    return t;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const 
{
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const 
{
    return !(*this == other);
}

//------------------------------------------------------------->

void BinarySearchTree::insert(const Key &key, const Value &value)
{
    if (_root == nullptr)
    {
        _root = new Node(key, value, nullptr);
    }
    Node* node = _root;
    if (node->keyValuePair.first > key)
    {
        if (node->left != nullptr)
        {
            node->left->insert(key, value);
        }
        else
        {
            node->left = new Node(key, value, node);
        }
    }
    else if (node->keyValuePair.first <= key)
    {
        if (node->right != nullptr)
        {
            node->right->insert(key, value);
        }
        else
        {
            node->right = new Node(key, value, node, nullptr, nullptr);
        }
    }
    ++_size;
}
//------------------------------------------->
void BinarySearchTree::erase(const Key &key)
{
    _root->erase(key);
    --_size;
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
    Node *current = _root;
    while (current){
        if (key < current->keyValuePair.first){
            current = current->left;
        }
        else if (key > current->keyValuePair.first){
            current = current->right;
        }
        else{
            return ConstIterator(current);
        }
    }
    return cend();
}
//---------------------------------->
BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
    Node* node = _root;
    while (node != nullptr) 
    if (key < node->keyValuePair.first) {
            node = node->left;
    } 
    else if (key > node->keyValuePair.first){
        node = node->right;
    }
    else{
            return Iterator(node);
    }
    return Iterator(nullptr);
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key &key) {
    Iterator lower = find(key);
    Iterator upper = lower;

    if (lower != end()) {
        while (upper != end() && upper->first == key) {
            ++upper;
        }
    }
    return {lower, upper};
}
std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key &key) const {
    ConstIterator lower = find(key);
    ConstIterator upper = lower;

    if (lower != cend()) {
        while (upper != cend() && upper->first == key) {
            ++upper;
        }
    }

    return {lower, upper};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
    if (!_root) return ConstIterator(nullptr);
    const Node *current = _root;
    while (current->left) current = current->left;
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
    if (!_root) return ConstIterator(nullptr);
    const Node *current = _root;
    while (current->right) current = current->right;
    return ConstIterator(current->parent);
}
BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
    ConstIterator it = find(key);
    ConstIterator itMin = find(key);
    it++;
    while (it->first == key) {
        itMin = (itMin->second < it->second ? itMin : it);
        it++;
    }
    return itMin;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
    ConstIterator it = find(key);
    ConstIterator itMin = find(key);
    it++;
    while (it->first == key) {
        itMin = (itMin->second > it->second ? itMin : it);
        it++;
    }
    return itMin;
}

BinarySearchTree::Iterator BinarySearchTree::begin() {
    if (!_root) return Iterator(nullptr);
    Node *current = _root;
    while (current->left) current = current->left;
    return Iterator(current);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
    if (!_root) return Iterator(nullptr);
    Node *current = _root;
    while (current->right) current = current->right;
    return Iterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    if (!_root) return cend();
    const Node *current = _root;
    while (current->left) current = current->left;
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    if (!_root) return cend();
    const Node *current = _root;
    while (current->right) current = current->right;
    return ConstIterator(current);
}

size_t BinarySearchTree::size() const { return _size; }

void BinarySearchTree::output_tree() {
    if (_root) {
        _root->output_node_tree();
        std::cout << std::endl;
    }
}
size_t BinarySearchTree::max_height() const {
    if (!_root) return 0;

    auto max_height_helper = [](const Node *node, auto &&self) {
        if (!node) return 0;
        return 1 + std::max(self(node->left, self), self(node->right, self));
    };

    return max_height_helper(_root, max_height_helper);
}
//---------------------------