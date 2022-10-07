#include "ls.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include "bv/bitvector.h"
#include "ls/bv/bitvector_node.h"
#include "rng/rng.h"

namespace bzla {
namespace ls {

/* -------------------------------------------------------------------------- */

class OstreamVoider
{
 public:
  OstreamVoider() = default;
  void operator&(std::ostream& ostream) { (void) ostream; }
};

#define BZLALSLOG_ENABLED(level) (d_log_level >= (level))
#define BZLALSLOGSTREAM(level) \
  !(BZLALSLOG_ENABLED(level)) ? (void) 0 : OstreamVoider() & std::cout
#define BZLALSLOG(level) BZLALSLOGSTREAM(level) << "[bzla-ls]"

/* -------------------------------------------------------------------------- */

std::ostream&
operator<<(std::ostream& out, const OperatorKind& kind)
{
  switch (kind)
  {
    case OperatorKind::AND: out << "and"; break;
    case OperatorKind::EQ: out << "eq"; break;
    case OperatorKind::ITE: out << "ite"; break;
    case OperatorKind::NOT: out << "not"; break;
    case OperatorKind::XOR: out << "xor"; break;
    case OperatorKind::BV_ADD: out << "bvadd"; break;
    case OperatorKind::BV_AND: out << "bvand"; break;
    case OperatorKind::BV_ASHR: out << "bvashr"; break;
    case OperatorKind::BV_CONCAT: out << "bvconcat"; break;
    case OperatorKind::BV_EXTRACT: out << "bvextract"; break;
    case OperatorKind::BV_MUL: out << "bvmul"; break;
    case OperatorKind::BV_NOT: out << "bvnot"; break;
    case OperatorKind::BV_SEXT: out << "bvsext"; break;
    case OperatorKind::BV_SHL: out << "bvshl"; break;
    case OperatorKind::BV_SHR: out << "bvshr"; break;
    case OperatorKind::BV_SLT: out << "bvslt"; break;
    case OperatorKind::BV_UDIV: out << "bvudiv"; break;
    case OperatorKind::BV_ULT: out << "bvult"; break;
    case OperatorKind::BV_UREM: out << "bvurem"; break;
    case OperatorKind::BV_XOR: out << "bvxor"; break;
    default: assert(false);
  }
  return out;
}

/* -------------------------------------------------------------------------- */

template <class VALUE, class NODE>
struct LocalSearchMove
{
  LocalSearchMove() : d_nprops(0), d_nupdates(0), d_input(nullptr) {}

  LocalSearchMove(uint64_t nprops,
                  uint64_t nupdates,
                  NODE* input,
                  VALUE assignment)
      : d_nprops(nprops),
        d_nupdates(nupdates),
        d_input(input),
        d_assignment(assignment)
  {
  }

  uint64_t d_nprops;
  uint64_t d_nupdates;
  NODE* d_input;
  VALUE d_assignment;
};

/* -------------------------------------------------------------------------- */

template <class BOOL, class VALUE, class NODE>
LocalSearch<BOOL, VALUE, NODE>::LocalSearch(uint64_t max_nprops,
                                            uint64_t max_nupdates,
                                            uint32_t seed)
    : d_max_nprops(max_nprops), d_max_nupdates(max_nupdates), d_seed(seed)

{
  d_rng.reset(new RNG(d_seed));
}

template <class BOOL, class VALUE, class NODE>
LocalSearch<BOOL, VALUE, NODE>::~LocalSearch()
{
}

template <class BOOL, class VALUE, class NODE>
void
LocalSearch<BOOL, VALUE, NODE>::init()
{
  NODE::s_path_sel_essential  = d_options.use_path_sel_essential;
  NODE::s_prob_pick_ess_input = d_options.prob_pick_ess_input;
}

template <class BOOL, class VALUE, class NODE>
const VALUE&
LocalSearch<BOOL, VALUE, NODE>::get_assignment(uint64_t id) const
{
  assert(id < d_nodes.size());  // API check
  return get_node(id)->assignment();
}

template <class BOOL, class VALUE, class NODE>
void
LocalSearch<BOOL, VALUE, NODE>::set_assignment(uint64_t id,
                                               const VALUE& assignment)
{
  assert(id < d_nodes.size());  // API check
  get_node(id)->set_assignment(assignment);
}

template <class BOOL, class VALUE, class NODE>
void
LocalSearch<BOOL, VALUE, NODE>::register_root(uint64_t id)
{
  assert(id < d_nodes.size());  // API check
  NODE* root = get_node(id);
  d_roots.push_back(root);
  if (root->is_inequality())
  {
    d_roots_ineq.insert({root, true});
  }
  if (root->is_not() && (*root)[0]->is_inequality())
  {
    d_roots_ineq.insert({(*root)[0], false});
  }
  update_unsat_roots(root);
}

template <class BOOL, class VALUE, class NODE>
uint32_t
LocalSearch<BOOL, VALUE, NODE>::get_arity(uint64_t id) const
{
  assert(id < d_nodes.size());  // API check
  return get_node(id)->arity();
}

template <class BOOL, class VALUE, class NODE>
uint64_t
LocalSearch<BOOL, VALUE, NODE>::get_child(uint64_t id, uint32_t idx) const
{
  assert(id < d_nodes.size());  // API check
  assert(idx < get_arity(id));  // API check
  return (*get_node(id))[idx]->id();
}

/* -------------------------------------------------------------------------- */

template <class BOOL, class VALUE, class NODE>
NODE*
LocalSearch<BOOL, VALUE, NODE>::get_node(uint64_t id) const
{
  assert(id < d_nodes.size());
  assert(d_nodes[id]->id() == id);
  return d_nodes[id].get();
}

template <class BOOL, class VALUE, class NODE>
bool
LocalSearch<BOOL, VALUE, NODE>::is_leaf_node(const NODE* node) const
{
  assert(node);
  return node->arity() == 0;
}

template <class BOOL, class VALUE, class NODE>
bool
LocalSearch<BOOL, VALUE, NODE>::is_root_node(const NODE* node) const
{
  assert(node);
  assert(d_parents.find(node->id()) != d_parents.end());
  return d_parents.at(node->id()).empty();
}

template <class BOOL, class VALUE, class NODE>
bool
LocalSearch<BOOL, VALUE, NODE>::is_ineq_root(const NODE* node) const
{
  return d_roots_ineq.find(node) != d_roots_ineq.end();
}

template <class BOOL, class VALUE, class NODE>
LocalSearchMove<VALUE, NODE>
LocalSearch<BOOL, VALUE, NODE>::select_move(NODE* root, const VALUE& t_root)
{
  assert(root);

  uint64_t nprops = 0, nupdates = 0;
  NODE* cur = root;
  VALUE t   = t_root;

  for (;;)
  {
    uint32_t arity = cur->arity();

    BZLALSLOG(1) << std::endl;
    BZLALSLOG(1) << "  propagate:" << std::endl;
    BZLALSLOG(1) << "    node: " << *cur << (is_root_node(cur) ? " (root)" : "")
                 << std::endl;

    if (arity == 0)
    {
      BZLALSLOG(1) << "    target value: " << t << std::endl;
      return LocalSearchMove(nprops, nupdates, cur, t);
    }
    if (cur->is_const() || cur->all_const())
    {
      BZLALSLOG(1) << "    target value: " << t << std::endl;
      break;
    }
    else
    {
      assert(!cur->domain().is_fixed());

      /* Compute min/max bounds of current node wrt. current assignment. */
      if (d_options.use_ineq_bounds)
      {
        compute_bounds(cur);
      }

      if (BZLALSLOG_ENABLED(1))
      {
        for (uint32_t i = 0, n = cur->arity(); i < n; ++i)
        {
          BZLALSLOG(1) << "      |- node[" << i << "]: " << *(*cur)[i]
                       << std::endl;
          if ((*cur)[i]->min_u())
          {
            BZLALSLOG(1) << "           + min_u: " << *(*cur)[i]->min_u()
                         << std::endl;
          }
          if ((*cur)[i]->max_u())
          {
            BZLALSLOG(1) << "           + max_u: " << *(*cur)[i]->max_u()
                         << std::endl;
          }
          if ((*cur)[i]->min_s())
          {
            BZLALSLOG(1) << "           + min_s: " << *(*cur)[i]->min_s()
                         << std::endl;
          }
          if ((*cur)[i]->max_s())
          {
            BZLALSLOG(1) << "           + max_s: " << *(*cur)[i]->max_s()
                         << std::endl;
          }
        }
      }
      BZLALSLOG(1) << "    target value: " << t << std::endl;

      /* Select path */
      uint32_t pos_x = cur->select_path(t);
      assert(pos_x < arity);

      BZLALSLOG(1) << "      select path: node[" << pos_x << "]" << std::endl;
      if (BZLALSLOG_ENABLED(1))
      {
        for (uint32_t i = 0, n = cur->arity(); i < n; ++i)
        {
          BZLALSLOG(1) << "        |- is_essential[" << i
                       << "]: " << (cur->is_essential(t, i) ? "true" : "false")
                       << std::endl;
        }
      }

      /** Select value
       *
       * 1) randomly choose to compute inverse over consistent value
       *    with probability BZLALS_PROB_USE_INV_VALUE
       * 2) if inverse value selected and no inverse value exists,
       *    fall back to consistent value
       * 3) if consistent value selected and no consistent value exists,
       *    conflict
       */

      if (d_rng->pick_with_prob(d_options.prob_pick_inv_value)
          && cur->is_invertible(t, pos_x))
      {
        t = cur->inverse_value(t, pos_x);
        BZLALSLOG(1) << "      inverse value: " << t << std::endl;
        d_statistics.d_nprops_inv += 1;
#ifndef NDEBUG
        switch (cur->get_kind())
        {
          case BitVectorNode::Kind::ADD:
            d_statistics.d_ninv[OperatorKind::BV_ADD] += 1;
            break;
          case BitVectorNode::Kind::AND:
            d_statistics.d_ninv[OperatorKind::BV_AND] += 1;
            break;
          case BitVectorNode::Kind::ASHR:
            d_statistics.d_ninv[OperatorKind::BV_ASHR] += 1;
            break;
          case BitVectorNode::Kind::CONCAT:
            d_statistics.d_ninv[OperatorKind::BV_CONCAT] += 1;
            break;
          case BitVectorNode::Kind::EXTRACT:
            d_statistics.d_ninv[OperatorKind::BV_EXTRACT] += 1;
            break;
          case BitVectorNode::Kind::EQ:
            d_statistics.d_ninv[OperatorKind::EQ] += 1;
            break;
          case BitVectorNode::Kind::ITE:
            d_statistics.d_ninv[OperatorKind::ITE] += 1;
            break;
          case BitVectorNode::Kind::MUL:
            d_statistics.d_ninv[OperatorKind::BV_MUL] += 1;
            break;
          case BitVectorNode::Kind::NOT:
            d_statistics.d_ninv[OperatorKind::BV_NOT] += 1;
            break;
          case BitVectorNode::Kind::SEXT:
            d_statistics.d_ninv[OperatorKind::BV_SEXT] += 1;
            break;
          case BitVectorNode::Kind::SHL:
            d_statistics.d_ninv[OperatorKind::BV_SHL] += 1;
            break;
          case BitVectorNode::Kind::SHR:
            d_statistics.d_ninv[OperatorKind::BV_SHR] += 1;
            break;
          case BitVectorNode::Kind::SLT:
            d_statistics.d_ninv[OperatorKind::BV_SLT] += 1;
            break;
          case BitVectorNode::Kind::UDIV:
            d_statistics.d_ninv[OperatorKind::BV_UDIV] += 1;
            break;
          case BitVectorNode::Kind::ULT:
            d_statistics.d_ninv[OperatorKind::BV_ULT] += 1;
            break;
          case BitVectorNode::Kind::UREM:
            d_statistics.d_ninv[OperatorKind::BV_UREM] += 1;
            break;
          case BitVectorNode::Kind::XOR:
            d_statistics.d_ninv[OperatorKind::BV_XOR] += 1;
            break;
          default: assert(false);
        };
#endif
      }
      else if (cur->is_consistent(t, pos_x))
      {
        t = cur->consistent_value(t, pos_x);
        BZLALSLOG(1) << "      consistent value: " << t << std::endl;
        d_statistics.d_nprops_cons += 1;
#ifndef NDEBUG
        switch (cur->get_kind())
        {
          case BitVectorNode::Kind::ADD:
            d_statistics.d_ncons[OperatorKind::BV_ADD] += 1;
            break;
          case BitVectorNode::Kind::AND:
            d_statistics.d_ncons[OperatorKind::BV_AND] += 1;
            break;
          case BitVectorNode::Kind::ASHR:
            d_statistics.d_ncons[OperatorKind::BV_ASHR] += 1;
            break;
          case BitVectorNode::Kind::CONCAT:
            d_statistics.d_ncons[OperatorKind::BV_CONCAT] += 1;
            break;
          case BitVectorNode::Kind::EXTRACT:
            d_statistics.d_ncons[OperatorKind::BV_EXTRACT] += 1;
            break;
          case BitVectorNode::Kind::EQ:
            d_statistics.d_ncons[OperatorKind::EQ] += 1;
            break;
          case BitVectorNode::Kind::ITE:
            d_statistics.d_ncons[OperatorKind::ITE] += 1;
            break;
          case BitVectorNode::Kind::MUL:
            d_statistics.d_ncons[OperatorKind::BV_MUL] += 1;
            break;
          case BitVectorNode::Kind::NOT:
            d_statistics.d_ncons[OperatorKind::BV_NOT] += 1;
            break;
          case BitVectorNode::Kind::SEXT:
            d_statistics.d_ncons[OperatorKind::BV_SEXT] += 1;
            break;
          case BitVectorNode::Kind::SHL:
            d_statistics.d_ncons[OperatorKind::BV_SHL] += 1;
            break;
          case BitVectorNode::Kind::SHR:
            d_statistics.d_ncons[OperatorKind::BV_SHR] += 1;
            break;
          case BitVectorNode::Kind::SLT:
            d_statistics.d_ncons[OperatorKind::BV_SLT] += 1;
            break;
          case BitVectorNode::Kind::UDIV:
            d_statistics.d_ncons[OperatorKind::BV_UDIV] += 1;
            break;
          case BitVectorNode::Kind::ULT:
            d_statistics.d_ncons[OperatorKind::BV_ULT] += 1;
            break;
          case BitVectorNode::Kind::UREM:
            d_statistics.d_ncons[OperatorKind::BV_UREM] += 1;
            break;
          case BitVectorNode::Kind::XOR:
            d_statistics.d_ncons[OperatorKind::BV_XOR] += 1;
            break;
          default: assert(false);
        };
#endif
      }
      else
      {
        d_statistics.d_nconf += 1;
        break;
      }

      // TODO skip when no progress?

      /* Propagate down */
      cur = (*cur)[pos_x];
      nprops += 1;
    }
  }

  BZLALSLOG(1) << "*** conflict" << std::endl;

  /* Conflict case */
  return LocalSearchMove<VALUE, NODE>(nprops, nupdates, nullptr, VALUE());
}

template <class BOOL, class VALUE, class NODE>
void
LocalSearch<BOOL, VALUE, NODE>::update_unsat_roots(NODE* root)
{
  assert(is_root_node(root));

  uint64_t id = root->id();
  auto it     = d_roots_unsat.find(id);
  if (it != d_roots_unsat.end())
  {
    if (root->assignment().is_true())
    {
      /* remove from unsatisfied roots list */
      d_roots_unsat.erase(it);
    }
  }
  else if (root->assignment().is_false())
  {
    /* add to unsatisfied roots list */
    d_roots_unsat.insert(id);
  }
}

template <class BOOL, class VALUE, class NODE>
uint64_t
LocalSearch<BOOL, VALUE, NODE>::update_cone(NODE* node, const VALUE& assignment)
{
  assert(node);
  assert(is_leaf_node(node));

  BZLALSLOG(1) << "*** update cone: " << *node << " with: " << assignment
               << std::endl;
  BZLALSLOG(1) << std::endl;
#ifndef NDEBUG
  for (uint64_t id : d_roots_unsat)
  {
    assert(get_node(id)->assignment().is_false());
  }
#endif

  /* nothing to do if node already has given assignment */
  if (node->assignment().compare(assignment) == 0) return 0;

  /* update assignment of given node */
  node->set_assignment(assignment);
  uint64_t nupdates = 1;

  std::vector<uint64_t> cone;
  std::vector<NODE*> to_visit;
  std::unordered_set<uint64_t> visited;

  /* reset cone */
  const std::unordered_set<uint64_t>& parents = d_parents.at(node->id());
  for (uint64_t p : parents)
  {
    to_visit.push_back(get_node(p));
  }

  while (!to_visit.empty())
  {
    NODE* cur = to_visit.back();
    to_visit.pop_back();

    if (visited.find(cur->id()) != visited.end()) continue;
    visited.insert(cur->id());
    cone.push_back(cur->id());

    const std::unordered_set<uint64_t>& parents = d_parents.at(cur->id());
    for (uint64_t p : parents)
    {
      to_visit.push_back(get_node(p));
    }
  }

  /* update assignments of cone */
  if (is_root_node(node))
  {
    update_unsat_roots(node);
  }

  std::sort(cone.begin(), cone.end());

  for (uint64_t id : cone)
  {
    NODE* cur = get_node(id);
    BZLALSLOG(2) << "  node: " << *cur << " -> ";
    cur->evaluate();
    nupdates += 1;
    BZLALSLOGSTREAM(2) << cur->assignment() << std::endl;
    if (BZLALSLOG_ENABLED(2))
    {
      for (uint32_t i = 0, n = cur->arity(); i < n; ++i)
      {
        BZLALSLOG(2) << "    |- node[" << i << "]: " << *(*cur)[i] << std::endl;
      }
      BZLALSLOG(2) << std::endl;
    }

    if (is_root_node(cur))
    {
      update_unsat_roots(cur);
    }
  }
#ifndef NDEBUG
  for (uint64_t id : d_roots_unsat)
  {
    assert(get_node(id)->assignment().is_false());
  }
#endif
  return nupdates;
}

template <class BOOL, class VALUE, class NODE>
Result
LocalSearch<BOOL, VALUE, NODE>::move()
{
  BZLALSLOG(1) << "*** move: " << d_statistics.d_nmoves + 1 << std::endl;
  if (BZLALSLOG_ENABLED(1))
  {
    BZLALSLOG(1) << "  unsatisfied roots:" << std::endl;
    for (uint64_t id : d_roots_unsat)
    {
      BZLALSLOG(1) << "    - " << *(get_node(id)) << std::endl;
    }
    BZLALSLOG(1) << "  satisfied roots:" << std::endl;
    for (const BitVectorNode* r : d_roots)
    {
      if (d_roots_unsat.find(r->id()) != d_roots_unsat.end()) continue;
      BZLALSLOG(1) << "    - " << *r << std::endl;
    }
  }

  if (d_roots_unsat.empty()) return Result::SAT;

  LocalSearchMove<VALUE, NODE> m;
  do
  {
    if (d_max_nprops > 0 && d_statistics.d_nprops >= d_max_nprops)
      return Result::UNKNOWN;
    if (d_max_nupdates > 0 && d_statistics.d_nupdates >= d_max_nupdates)
      return Result::UNKNOWN;

    NODE* root =
        get_node(d_rng->pick_from_set<std::unordered_set<uint64_t>, uint64_t>(
            d_roots_unsat));

    if (root->is_const() && root->domain().lo().is_false())
    {
      return Result::UNSAT;
    }

    BZLALSLOG(1) << std::endl;
    BZLALSLOG(1) << "  select constraint: " << *root << std::endl;

    m = select_move(root, *d_true);
    d_statistics.d_nprops += m.d_nprops;
    d_statistics.d_nupdates += m.d_nupdates;
  } while (m.d_input == nullptr);

  assert(!m.d_assignment.is_null());

  BZLALSLOG(1) << std::endl;
  BZLALSLOG(1) << "  move" << std::endl;
  BZLALSLOG(1) << "  input: " << *m.d_input << std::endl;
  BZLALSLOG(1) << "  prev. assignment: " << m.d_input->assignment()
               << std::endl;
  BZLALSLOG(1) << "  new   assignment: " << m.d_assignment << std::endl;
  BZLALSLOG(1) << std::endl;

  d_statistics.d_nmoves += 1;
  d_statistics.d_nupdates += update_cone(m.d_input, m.d_assignment);

  BZLALSLOG(1) << "*** number of propagations: " << d_statistics.d_nprops
               << std::endl;
  BZLALSLOG(1) << std::endl;
  BZLALSLOG(1) << "*** number of updates: " << d_statistics.d_nupdates
               << std::endl;
  BZLALSLOG(1) << std::endl;

  if (d_roots_unsat.empty()) return Result::SAT;
  return Result::UNKNOWN;
}

template class LocalSearch<BitVector, BitVector, BitVectorNode>;

/* -------------------------------------------------------------------------- */

}  // namespace ls
}  // namespace bzla
