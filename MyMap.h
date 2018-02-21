//
//  MyMap.h
//  Project 4
//
//  Created by Michelle Su on 3/9/17.
//  Copyright © 2017 Michelle Su. All rights reserved.
//

#ifndef MyMap_h
#define MyMap_h

template <typename KeyType,	typename ValueType>

class MyMap
{
public:
    MyMap();//	constructor
    ~MyMap(); //	destructor;	deletes	all	of	the	tree's	nodes
    void clear();		 //	deletes	all	of	the	trees	nodes	producing an	empty	tree
    int size() const; //	return	the	number	of	associations	in	the	map
    
    //	The	associate	method	associates	one	item	(key)	with	another	(value).
    //	If	no	association	currently	exists	with	that	key,	this	method	inserts
    //	a	new	association	into	the	tree	with	that	key/value	pair.		If	there	is
    //	already	an	association	with	that	key	in	the	tree,	then	the	item
    //	associated	with	that	key	is	replaced	by	the	second	parameter	(value).
    //	Thus,	the	tree	contains	no	duplicate	keys.
    void associate(const KeyType&	key,	const ValueType&	value);
    
    //	If	no	association	exists	with	the	given	key,	return	nullptr;	otherwise,
    //	return	a	pointer	to	the	value	associated	with	that	key.		This	pointer	can	be
    //	used	to	examine	that	value,	and	if	the	map	is	allowed	to	be	modified,	to
    //	modify	that	value	directly	within	the	map (the	second	overload	enables
    //	this).		Using	a	little	C++	magic,	we	have	implemented	it	in	terms	of	the
    //	first	overload,	which	you	must	implement.
    const	ValueType*	find(const KeyType&	key)	const;
    ValueType*	find(const KeyType&	key){
        return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyMap(const MyMap&) = delete;
    MyMap& operator=(const MyMap&) = delete;
private:
    struct Node {
        KeyType key;
        ValueType value;
        Node* left;
        Node* right;
    };
    Node* head;
    int m_size;
    
    //recursive helper function for clear
    void clear(Node* head);
    
    //recursive helper function for associate
    void associate(Node* head, const KeyType&	key, const ValueType&	value);
    
    //recursive helper function for find
    const ValueType* find(Node* head, const KeyType& key) const;
    
};

//Implementations

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap(){
    head = nullptr;
    m_size = 0;
}

template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const {return m_size;}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap(){
    clear();
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear(){
    clear(head);
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear(typename MyMap<KeyType, ValueType>::Node* head){
    Node* deleteMe = head;
    if (head->left == nullptr && head->right == nullptr){
        delete deleteMe;
        return;
    }
    if (head->right != nullptr)
        clear(head->right);
    if (head->left != nullptr)
        clear(head->left);
    delete deleteMe;
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType&	key) const {return find(head, key);}


//recursive helper function for find
template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(Node* head, const KeyType& key) const{
    if (head == nullptr)
        return nullptr;
    if (head->key == key)
        return &(head->value);
    find(head->left, key);
    find(head->right, key);
    return nullptr; //return nullptr if key isn't found
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType&	key,	const ValueType&	value){associate(head, key, value);}

//recursive helper function for associate
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(Node* head, const KeyType& key,	const ValueType& value){
    if (head == nullptr){
        head = new Node;
        head->key = key;
        head->value = value;
        m_size++;
        return;
    }
    if (head->key == key){
        head->value = value;
        return;
    }
    if (key < head->key){
        associate(head->left, key, value);
    }
    else{
        associate(head->right, key, value);
    }
}







#endif /* MyMap_h */
