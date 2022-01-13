#ifndef HASH_TABLE_COLLECTION_H
#define HASH_TABLE_COLLECTION_H

#include <vector>
#include <algorithm>
#include <functional>
# include <iostream>
#include "collection.h"

template <typename K, typename V>
class HashTableCollection : public Collection<K, V>
{
public:
  //  Create an empty linked list
  HashTableCollection();

  //  Copy a linked list
  HashTableCollection(const HashTableCollection<K, V>& rhs);

  //  Assign a linked list
  HashTableCollection<K, V>& operator=(const HashTableCollection<K, V>& rhs);

  //  Delete a linked list
  ~HashTableCollection();

  //  Insert a key-value pair into the Collection
  void insert(const K& key, const V& val);

  //  Remove a key-value pair into the collection
  void remove(const K& key);

  //  Find a value associated with the key
  bool find(const K& key, V& val) const;

  //  Find the keys associated with the range
  void find(const K& k1, const K& k2, std::vector<K>& keys) const;

  //  Return all keys in the Collection
  void keys(std::vector<K>& keys) const;

  //  Return collection keys in sorted order
  void sort(std::vector<K>& keys) const;

  //  Return the number of keys in the Collection
  int size() const;

  //  prints the list
  void print() const;

private:
  //  helper to empty entire hash HashTableCollection
  void make_empty();

  //  resize and rehash the hash table
  void resize_and_rehash();

  //  linked list node structure
  struct Node
  {
    K key;
    V value;
    Node* next;
  };

  //  Int number of k-v pairs in the collection
  int collection_size;
  //  Number of hash table buckets (default: 16)
  int table_capacity;
  //  Hash table array load factor (set at 75% for resizing)
  const double load_factor_threshold = .75;
  //  Hash table array
  Node** hash_table;
};

template < typename K , typename V >
HashTableCollection <K ,V >:: HashTableCollection():
collection_size(0), table_capacity(16), load_factor_threshold(0.75)
{
// dynamically allocate the hash table array
 hash_table = new Node*[ table_capacity ];
// initialize the hash table chains
for(int i=0;i<table_capacity;++i)
 hash_table[i] = nullptr;
}

template < typename K , typename V >
void HashTableCollection <K ,V >:: make_empty()
{
 if(hash_table != nullptr)
 {
  for(int i=0;i<table_capacity;++i)
  {
   Node* cur=hash_table[i];
   Node* temp;
   while(cur != nullptr)
   {
    temp=cur;
    cur=cur->next;
    delete temp;
    collection_size--;
   }
  }
 delete hash_table;
}

template < typename K , typename V >
HashTableCollection <K ,V >::~ HashTableCollection()
{
 make_empty();
}

template < typename K , typename V >
HashTableCollection <K ,V >:: HashTableCollection(const HashTableCollection <K ,V >& rhs): load_factor_threshold(rhs.load_factor_threshold), collection_size(rhs.collection_size), table_capacity(rhs.table_capacity)
{
 *this = rhs;
}

template < typename K , typename V >
HashTableCollection <K ,V >&
HashTableCollection <K ,V >:: operator=(const HashTableCollection <K ,V >& rhs )
{

 if(this == & rhs)
  return *this;
 make_empty();
 hash_table=new Node*[rhs.table_capacity];
 for(int i=0;i<rhs.table_capacity;++i){
  hash_table[i]=nullptr;
 }

 for(int i=0;i <= rhs.table_capacity;++i)
  {
   if(rhs.hash_table[i] != nullptr)
   {
    Node* temp=rhs.hash_table[i];
    while(temp != nullptr)
    {
     insert(temp->key, temp->value);
     temp=temp->next;
    }
   }
  }/
 return *this;
}

template < typename K , typename V >
void HashTableCollection <K ,V >:: resize_and_rehash()
{
 // setup new table
 int new_capacity = table_capacity * 2;
 // ... similarly with collection size 
 int new_collection_size = 0;
 // dynamically allocate the new table
 Node** new_table = new Node*[new_capacity];
 // initialize new table
 for(int i=0;i<new_capacity;++i) {
    new_table[i] = nullptr;
 }
 // insert key values
 std :: vector <K > ks;
 keys(ks);

 for(K key : ks) {
 // hash the key
  std::hash<K> hash_key;
  size_t h=hash_key(key);
  size_t i=h % new_capacity;
  Node* temp=new_table[i];
  V val;
  find(key, val);
  if(temp == nullptr)
  {
   temp=new Node;
   temp->key=key;
   temp->value=val;
   temp->next=nullptr;
   new_table[i]=temp;
  }
  else
  {
   temp=new Node;
   temp->key=key;
   temp->value=val;
   temp->next=new_table[i];
  }
  new_collection_size++;
 }
 make_empty();
 hash_table=new_table;
 collection_size=new_collection_size;
 table_capacity=new_capacity;
}

template < typename K , typename V >
void HashTableCollection <K ,V >:: insert(const K& key , const V& val )
{
 // check current load factor versus load factor threshold ,
 // and resize and copy if necessary by calling resize_and_rehash ()
 double lfactor=collection_size / table_capacity;
 if(lfactor == 0){
  lfactor=-1;
 }
 if(lfactor > load_factor_threshold){
   resize_and_rehash();
 }

 std::hash<K> hash_key;
 size_t h=hash_key(key);
 size_t i=h % table_capacity;

 if(hash_table[i] == nullptr)
 {
  hash_table[i]=new Node;
  hash_table[i]->key=key;
  hash_table[i]->value=val;
  hash_table[i]->next=nullptr;
  collection_size++;
 }
 else
 {
  Node* cur=hash_table[index];
  while(cur->next != nullptr){
   cur=cur->next;
  }
  Node* temp=new Node;
  temp=cur->next
  temp->key=key;
  temp->value=val;
  temp->next=nullptr;
  collection_size++;
 }
}

template <typename K, typename V>
void HashTableCollection<K, V>::remove(const K& key)
{
 std::hash<K> hash_key;
 size_t h=hash_key(key);
 size_t i=h % table_capacity;
 if(hash_table[index] == nullptr){
  return;
 }

 if(hash_table[i]->key == key)
 {
  Node* temp=hash_table[i];
  hash_table[i]=hash_table[i]->next;
  delete temp;
  collection_size--;
  return;
 }
 else
 {
  Node* prev=hash_table[i];
  Node* cur=hash_table[i]->next;
  while(cur != nullptr)
  {
   if(cur->key == key)
   {
    prev->next=cur->next;
    delete cur;
    cur=nullptr;
    collection_size--;
    break;
   }
   cur=cur->next;
   prev=prev->next;
  }
 }
}

template <typename K, typename V>
bool HashTableCollection<K, V>::find(const K& key, V& val) const
{
 std::hash<K> hash_key;
 size_t h=hash_key(key);
 size_t i=h % table_capacity;
 if(hash_table[i] == nullptr)
   return false;

 Node* temp=hash_table[i];
 while(temp != nullptr)
 {
  if(temp->key == key)
  {
   val=temp->value;
   return true;
  }
  cur=cur->next;
 }
 return false;
}

template <typename K, typename V>
void HashTableCollection<K, V>::
find(const K& k1, const K& k2, std::vector<K>& keys) const
{
 Node* cur;
 for(int i=0;i<table_capacity;++i)
 {
  if(hash_table[i]->key >= k1 && hash_table[i]->key <= k2)
  {
   keys.push_back(hash_table[i]->key);
   cur=hash_table[i]->next;
   while(ptr != nullptr)
   {
    if(cur->key >= k1 && cur->key <= k2){
     keys.push_back(cur->key);
    }
    cur=cur->next;
   }
  }
 }
}

template <typename K, typename V>
void HashTableCollection<K, V>::keys(std::vector<K>& keys) const
{
 Node* cur;
 for(int i=0;i<table_capacity;++i)
 {
  keys.push_back(hash_table[i]->key);
  cur=hash_table[i]->next;
  while(cur != nullptr)
  {
   keys.push_back(cur->key);
   cur=cur->next;
  }
 }
}

template < typename K , typename V >
void HashTableCollection <K ,V >:: sort(std :: vector <K >& ks) const
{
 keys(ks);
 std :: sort(ks.begin() , ks.end());
}

template < typename K , typename V >
int HashTableCollection <K ,V >:: size() const
{
 return collection_size;
}

template <typename K, typename V>
void HashTableCollection<K, V>::print() const
{
  for(int i=0;i < table_capacity;++i)
  {
    Node* cur=hash_table[i];
    if(cur == nullptr)
      continue;

    while(cur != nullptr)
    {
      std::cout<<cur->key<<" ";
      cur=cur->next;
    }
  }

  std::cout<<std::endl;
}

# endif