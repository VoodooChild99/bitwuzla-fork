#include "solver/fp/fp_solver.h"

#include "node/node_kind.h"
#include "node/node_manager.h"
#include "node/node_ref_vector.h"
#include "node/unordered_node_ref_map.h"
#include "solver/fp/floating_point.h"
#include "solver/fp/rounding_mode.h"
#include "solver/solver_engine.h"

namespace bzla::fp {

bool
FpSolver::is_leaf(const Node& term)
{
  node::Kind k = term.kind();
  return k == node::Kind::FP_IS_INF || k == node::Kind::FP_IS_NAN
         || k == node::Kind::FP_IS_NEG || k == node::Kind::FP_IS_NORM
         || k == node::Kind::FP_IS_POS || k == node::Kind::FP_IS_SUBNORM
         || k == node::Kind::FP_IS_ZERO || k == node::Kind::FP_EQUAL
         || k == node::Kind::FP_LE || k == node::Kind::FP_LT
         || k == node::Kind::FP_TO_SBV || k == node::Kind::FP_TO_UBV
         || (k == node::Kind::EQUAL
             && (term[0].type().is_fp() || term[0].type().is_rm()));
}

Node
FpSolver::default_value(const Type& type)
{
  NodeManager& nm = NodeManager::get();
  if (type.is_fp())
  {
    return nm.mk_value(FloatingPoint::fpzero(type, false));
  }
  assert(type.is_rm());
  return nm.mk_value(RoundingMode::RNE);
}

FpSolver::FpSolver(SolverEngine& solver_engine)
    : Solver(solver_engine), d_word_blaster(solver_engine)
{
}

FpSolver::~FpSolver() {}

void
FpSolver::check()
{
  NodeManager& nm = NodeManager::get();
  for (const Node& node : d_word_blast_queue)
  {
    d_solver_engine.lemma(
        nm.mk_node(node::Kind::EQUAL, {node, d_word_blaster.word_blast(node)}));
  }
  d_word_blast_queue.clear();
}

Node
FpSolver::value(const Node& term)
{
  assert(term.type().is_fp() || term.type().is_rm());

  NodeManager& nm = NodeManager::get();
  node::node_ref_vector visit{term};
  node::unordered_node_ref_map<bool> visited;

  do
  {
    const Node& cur = visit.back();
    assert(cur.type().is_fp() || cur.type().is_rm());

    if (!get_cached_value(cur).is_null())
    {
      visit.pop_back();
      continue;
    }

    auto it = visited.find(cur);
    if (it == visited.end())
    {
      visited.emplace(cur, false);
      if (!is_leaf(cur))
      {
        visit.insert(visit.end(), cur.begin(), cur.end());
      }
      continue;
    }
    else if (!it->second)
    {
      it->second = true;

      Node wb =
          d_solver_engine.rewriter().rewrite(d_word_blaster.word_blast(cur));
      Node value = d_solver_engine.value(wb);
      assert(value.type().is_bv());
      const BitVector& bv = value.value<BitVector>();
      if (cur.type().is_rm())
      {
        uint64_t rm = bv.to_uint64();
        value       = nm.mk_value(static_cast<RoundingMode>(rm));
      }
      else
      {
        assert(cur.type().is_fp());
        value = nm.mk_value(FloatingPoint(cur.type(), bv));
      }
      cache_value(cur, value);
    }
    visit.pop_back();
  } while (!visit.empty());

  return get_cached_value(term);
}

void
FpSolver::register_term(const Node& term)
{
  d_word_blast_queue.push_back(term);
}

}  // namespace bzla::fp