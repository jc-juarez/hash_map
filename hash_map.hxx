// *************************************
// syp hash_map implementation.
// Current implementation for numerics.
// Author: Juan Carlos Juarez.
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef HASH_MAP_
#define HASH_MAP_

#include <limits>
#include <memory>
#include <optional>

namespace syp
{

template <typename T, typename V>
struct hash_map_node
{

  hash_map_node(
    T p_key,
    V p_value)
    : key(p_key),
      value(p_value)
  {}
  
  T key;
  V value;
  std::shared_ptr<hash_map_node<T, V>> next;

};

template <typename T, typename V>
class hash_map
{

public:
  
  //
  // Constructor.
  // Allocates head nodes on the heap.
  //
  hash_map()
    : m_size(0),
      m_memory_bucket(new std::shared_ptr<hash_map_node<T, V>>[c_max_hash_map_head_nodes])
  {}

  //
  // Inserts a key-value pair node into the hash map.
  //
  void insert(
    T p_key,
    V p_value)
  {
    std::shared_ptr<hash_map_node<T, V>>& head_node = get_head_node(p_key);
    std::shared_ptr<hash_map_node<T, V>> new_node = std::make_shared<hash_map_node<T, V>>(p_key, p_value);

    //
    // Head node is not yet allocated.
    //
    if (head_node == nullptr)
    {
      head_node = new_node;
      ++m_size;
      return;
    }

    //
    // Search for a tail node for the new allocation.
    //
    std::shared_ptr<hash_map_node<T, V>> current_node = head_node;

    while (current_node->next != nullptr) current_node = current_node->next;

    current_node->next = new_node;
    ++m_size;
  }

  //
  // Gets a value from the hash map if the key exists.
  //
  std::optional<V> get(
    T p_key)
  {
    std::shared_ptr<hash_map_node<T, V>>& head_node = get_head_node(p_key);
    std::shared_ptr<hash_map_node<T, V>> current_node = head_node;

    //
    // Loop through the linked list until an invalid node is found.
    //
    while (current_node != nullptr)
    {
      if (current_node->key == p_key) return std::optional<V>(current_node->value);
      current_node = current_node->next;
    }

    return std::nullopt;
  }

  void remove(
    T p_key)
  {
    std::shared_ptr<hash_map_node<T, V>>& head_node = get_head_node(p_key);

    if (head_node == nullptr) return;

    if (head_node->key == p_key)
    {
        head_node = head_node->next;
        --m_size;
        return;
    }

    std::shared_ptr<hash_map_node<T, V>> current_node = head_node;

    //
    // Loop through the linked list to search for a node to remove.
    //
    while (current_node->next != nullptr)
    {
      if (current_node->next->key == p_key)
      {
        current_node->next = current_node->next->next;
        --m_size;
        return;
      }
      current_node = current_node->next;
    }
  }

  void clear()
  {
    for (unsigned short bucket_index = 0; bucket_index < c_max_hash_map_head_nodes; ++bucket_index)
    {
      m_memory_bucket.get()[bucket_index].reset();
    }
    m_size = 0;
  }

  size_t size() const
  {
    return m_size;
  }

private:
  
  //
  // Total number of key-value pair nodes.
  //
  size_t m_size;

  //
  // Contiguous memory array of key-value pair head nodes.
  // Memory for head nodes is allocated on the heap for reducing stack footprint.
  //
  std::unique_ptr<std::shared_ptr<hash_map_node<T, V>>[]> m_memory_bucket;

  //
  // Returns a reference to a head node.
  //
  std::shared_ptr<hash_map_node<T, V>>& get_head_node(
    T p_key)
  {
    unsigned short bucket_index = hash(p_key);
    return m_memory_bucket.get()[bucket_index];
  }


private:

  //
  // Hashing function.
  //
  static unsigned short hash(
    T p_key)
  {
    return p_key % c_max_hash_map_head_nodes;
  }
  
  //
  // Max number of key-value pair head nodes per object instance.
  //
  static constexpr unsigned short c_max_hash_map_head_nodes = std::numeric_limits<unsigned short>::max();

};

} // namespace syp

#endif