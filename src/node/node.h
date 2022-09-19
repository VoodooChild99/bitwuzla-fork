#ifndef BZLA_NODE_NODE_H_INCLUDED
#define BZLA_NODE_NODE_H_INCLUDED

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <optional>

#include "node/node_kind.h"
#include "type/type.h"

namespace bzla {

class BitVector;
class NodeManager;

enum class RoundingMode;
class FloatingPoint;

namespace node {
class NodeData;
}  // namespace node

/* --- Node ---------------------------------------------------------------- */

class Node
{
  friend NodeManager;
  friend bool operator==(const Node& a, const Node& b);
  friend bool operator!=(const Node& a, const Node& b);

 public:
  using iterator = const Node*;

  Node() = default;
  ~Node();
  Node(const Node& other);
  Node& operator=(const Node& other);
  Node(Node&& other);
  Node& operator=(Node&& other);

  /**
   * @return The id of this node.
   */
  uint64_t get_id() const;

  /**
   * @return The kind of this node.
   */
  node::Kind get_kind() const;

  /**
   * @return The type of this node.
   */
  const Type& get_type() const;

  /**
   * @return True if this node is null.
   */
  bool is_null() const;

  /**
   * @return True if this node is a value.
   */
  bool is_value() const;

  /**
   * @return The number of children.
   */
  size_t get_num_children() const;

  /**
   * Return child at position `index`.
   *
   * @note Only valid to call if get_num_children() > 0.
   *
   * @param index The position of the child.
   * @return The child node at position `index`.
   */
  const Node& operator[](size_t index) const;

  /**
   * @return The number of indices of this node.
   */
  size_t get_num_indices() const;

  /**
   * Return index at position `index`.
   *
   * @note: Only valid to call if get_num_indices() > 0.
   *
   * @param index The position of the index.
   * @return The index.
   */
  uint64_t get_index(size_t index) const;

  /**
   * Get the value represented by this node.
   *
   * @return The value of type T.
   */
  template <class T>
  const T& get_value() const;

  /**
   * @return Symbol of this node or empty string if node does not have a symbol.
   */
  std::optional<std::reference_wrapper<const std::string>> get_symbol() const;

  /**
   * @return An iterator to the first child of this node.
   */
  iterator begin() const;

  /**
   * @return An iterator to the end of the children list of this node.
   */
  iterator end() const;

 private:
  Node(node::NodeData* data);

  /** Node payload. */
  node::NodeData* d_data = nullptr;
};

/** Syntactical equality over two nodes. */
bool operator==(const Node& a, const Node& b);
/** Syntactical disequality over two nodes. */
bool operator!=(const Node& a, const Node& b);

// Node::get_value() instantiations for different value types
template <>
const bool& Node::get_value() const;
template <>
const BitVector& Node::get_value() const;
template <>
const RoundingMode& Node::get_value() const;
template <>
const FloatingPoint& Node::get_value() const;

}  // namespace bzla

namespace std {

template <>
struct hash<bzla::Node>
{
  size_t operator()(const bzla::Node& node) const;
};

template <>
struct hash<bzla::Node*>
{
  size_t operator()(const bzla::Node* node) const;
};

}  // namespace std
#endif
