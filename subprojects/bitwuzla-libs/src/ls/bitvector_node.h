#ifndef BZLALS__BITVECTOR_OP_H
#define BZLALS__BITVECTOR_OP_H

#include <vector>

#include "bitvector.h"
#include "bitvector_domain.h"

namespace bzla {

class RNG;

namespace ls {

/* -------------------------------------------------------------------------- */

class BitVectorNode
{
 public:
  /**
   * Path selection mode.
   * True if path is to be selected based on essential inputs, false if it is
   * to be selected randomly.
   */
  static constexpr bool s_sel_path_essential = true;

  enum Kind
  {
    ADD,
    AND,
    ASHR,
    CONCAT,
    CONST,
    EXTRACT,
    EQ,
    ITE,
    MUL,
    NOT,
    SEXT,
    SHL,
    SHR,
    SLT,
    UDIV,
    ULT,
    UREM,
    XOR,
  };

  /** Constructor. */
  BitVectorNode(RNG* rng, uint32_t size);
  BitVectorNode(RNG* rng,
                const BitVector& assignment,
                const BitVectorDomain& domain);
  /** Destructor. */
  virtual ~BitVectorNode() {}

  /**
   * Get the domain of this node.
   * @return A reference to the domain of this node.
   */
  const BitVectorDomain& get_domain() { return d_domain; }

  /**
   * Get the kind of the node.
   * @return The kind of this node.
   */
  virtual Kind get_kind() const { return CONST; }

  /**
   * Get the bit-vector size of the node.
   * @return The size of this node.
   */
  uint32_t size() { return d_assignment.size(); }

  /** Update assignment based on the assignment of its children. */
  virtual void evaluate() {}

  /**
   * Check if operand at index `pos_x` is essential with respect to constant
   * bits and target value `t`.
   *
   * @note
   * For is_essential() checks, we have to disable the consideration of
   * bounds derived from top-level, currently satisfied inequalities
   * since else this may lead us into a cycle.
   * For example, assume we have 3 roots:
   *   y_[64] <= z_[64]
   *   z_[64] <= sign_extend((1844674407_[32] + x_[32]), 32)
   *   (844674407_[32] + x_[32]) <= 0_[32]
   * Now, assume that the first root and one of the other two are satisfied
   * with the initial assignment where all inputs are assigned to zero.
   * Now, due to the inequality bounds derived from root 1 and 2/3 (depending
   * on which one is satisfied), either the sign extension or the addition
   * are essential, but z never is. Thus, we never propagate down to z and
   * the first root (and thus the bounds of these two terms) remain unchanged.
   * This traps us cycling between root 2 and 3 but never reaching a
   * satisfiable assignment, which would require us to change the assignments
   * of y or z.
   *
   * @param t The target value.
   * @param pos_x The index of `x`.
   * @return True if operand at index `pos_x` is essential.
   */
  virtual bool is_essential(const BitVector& t, uint32_t pos_x)
  {
    return !is_invertible(t, 1 - pos_x, false, true);
  }

  /**
   * Check invertibility condition for `x` at index `pos_x with` respect to
   * constant bits and target value `t`.
   * @param t The target value.
   * @param pos_x The index of `x`.
   * @param is_essential_check True if called to determine is_essential(). For
   *                           is_essential() checks, we don't consider bounds
   *                           derived from top-level inequalities since this
   * may trap us in a cycle (see is_essential()).
   * @return True if there exists an inverse value for `x`.
   */
  virtual bool is_invertible(const BitVector& t,
                             uint32_t pos_x,
                             bool find_inverse       = true,
                             bool is_essential_check = false)
  {
    (void) t;
    (void) pos_x;
    (void) find_inverse;
    (void) is_essential_check;
    return true;
  }

  /**
   * Check consistency condition for `x` at index `pos_x` with respect to
   * constant bits and target value `t`.
   * @param t The target value.
   * @param pos_x The index of `x`.
   * @return True if there exists a consistent value for `x`.
   */
  virtual bool is_consistent(const BitVector& t, uint32_t pos_x)
  {
    (void) t;
    (void) pos_x;
    return true;
  }

  /**
   * Get an inverse value for `x` at index `pos_x` with respect to constant bits
   * and target value `t`.
   * @param t The target value.
   * @param pos_x The index of `x`.
   * @return The inverse value for `x`.
   */
  virtual const BitVector& inverse_value(const BitVector& t, uint32_t pos_x)
  {
    (void) t;
    (void) pos_x;
    return *d_inverse;
  }
  /**
   * Get an consistent value for `x` at index `pos_x` with respect to constant
   * bits and target value `t`.
   * @param t The target value.
   * @param pos_x The index of `x`.
   * @return The consistent value for `x`.
   */
  virtual const BitVector& consistent_value(const BitVector& t, uint32_t pos_x)
  {
    (void) t;
    (void) pos_x;
    return *d_consistent;
  }

  /**
   * Select the next step in the propagation path based on target value `t` and
   * the current assignment of this node's children.
   * @param t The target value of this node.
   * @return The index of child to propagate the target value down to.
   */
  virtual uint32_t select_path(const BitVector& t);

  /**
   * Get child at given index.
   * @param pos The index of the child to get.
   * @return The child at the given index.
   */
  BitVectorNode* operator[](uint32_t pos) const;

  /**
   * Get the arity of this node.
   * @return The arity of this node.
   */
  uint32_t arity() const { return d_arity; }
  /**
   * Set the assignment of this node.
   * @param assignment The assignment to set.
   */
  void set_assignment(const BitVector& assignment);
  /**
   * Get the assignment of this node.
   * @return The assignment of this node.
   */
  const BitVector& assignment() const { return d_assignment; }
  /**
   * Get the domain of this node.
   * @return The domain of this node.
   */
  const BitVectorDomain& domain() const { return d_domain; }
  /**
   * Determine if the underlying domain is fixed.
   * @return True if the underlying domain is fixed.
   */
  bool is_const() const { return d_is_const; }

  /**
   * Determine if all children are const.
   * @return True if all children are const.
   */
  bool all_const() const { return d_all_const; }

  /**
   * Set id of this node.
   * @param id The id to set.
   */
  void set_id(uint32_t id) { d_id = id; }
  /**
   * Get id of this node.
   * @return The id of this node.
   */
  uint32_t id() const { return d_id; }

  /**
   * Get the unsigned upper bound (incl) for inverse value computation.
   * @return The upper unsigned bound.
   */
  BitVector* max_u() { return d_max_u.get(); }
  /**
   * Get the unsigned lower bound (incl) for inverse value computation.
   * @return The lower unsigned bound.
   */
  BitVector* min_u() { return d_min_u.get(); }
  /**
   * Get the signed upper bound (incl) for inverse value computation.
   * @return The upper signed bound.
   */
  BitVector* max_s() { return d_max_s.get(); }
  /**
   * Get the signed lower bound (incl) for inverse value computation.
   * @return The lower signed bound.
   */
  BitVector* min_s() { return d_min_s.get(); }

  /**
   * Fix domain bit at index `idx` to `value`.
   * @param idx The index of the bit to fix to the given value.
   * @param value  The value to fix the bit to.
   */
  void fix_bit(uint32_t idx, bool value);

  /**
   * Update bounds.
   * @param min The minimum value.
   * @param max The maximum value.
   * @param min_is_exclusive True if the given min bound is exclusive, in which
   * case `min + 1` will be used.
   * @param max_is_exclusive True if the given max bound is exclusive, in which
   * case `min - 1` will be used.
   * @param is_signed True if the given bounds are signed bounds.
   */
  void update_bounds(const BitVector& min,
                     const BitVector& max,
                     bool min_is_exclusive,
                     bool max_is_exclusive,
                     bool is_signed);
  /**
   * Reset signed and unsigned bounds.
   */
  void reset_bounds();
  /**
   * Normalize signed and unsigned bounds into a lower (from min_signed and
   * ones) and upper (from zero to max_signed) ranges. If signed and unsigned
   * ranges of this node don't have any intersection, all return parameters
   * will be null nodes.
   * @param min_lo The minimum value of the resulting lower range, null if
   *               no values in the lower range are covered.
   * @param max_lo The maximum value of the resulting lower range, null if
   *               no values in the lower range are covered.
   * @param min_hi The minimum value of the resulting upper range, null if
   *               no values in the upper range are covered.
   * @param max_hi The maximum value of the resulting upper range, null if
   *               no values in the upper range are covered.
   */
  void normalize_bounds(BitVector& res_min_lo,
                        BitVector& res_max_lo,
                        BitVector& res_min_hi,
                        BitVector& res_max_hi);

  /** Get the string representation of this node. */
  virtual std::string to_string() const;

 protected:
  BitVectorNode(RNG* rng, uint32_t size, BitVectorNode* child0);
  BitVectorNode(RNG* rng,
                uint32_t size,
                BitVectorNode* child0,
                BitVectorNode* child1);
  BitVectorNode(RNG* rng,
                uint32_t size,
                BitVectorNode* child0,
                BitVectorNode* child1,
                BitVectorNode* child2);
  BitVectorNode(RNG* rng, const BitVectorDomain& domain, BitVectorNode* child0);
  BitVectorNode(RNG* rng,
                const BitVectorDomain& domain,
                BitVectorNode* child0,
                BitVectorNode* child1);
  BitVectorNode(RNG* rng,
                const BitVectorDomain& domain,
                BitVectorNode* child0,
                BitVectorNode* child1,
                BitVectorNode* child2);

  virtual int32_t select_path_non_const(std::vector<uint32_t>& inputs) const;

  uint32_t d_id = 0;
  std::unique_ptr<BitVectorNode*[]> d_children;
  RNG* d_rng;
  uint32_t d_arity;
  BitVector d_assignment;
  BitVectorDomain d_domain;
  bool d_is_const;
  bool d_all_const;

  /** Unsigned upper bound (incl) for inverse value computation. */
  std::unique_ptr<BitVector> d_max_u;
  /** Unsigned lower bound (incl) for inverse value computation. */
  std::unique_ptr<BitVector> d_min_u;
  /** Signed upper bound (incl) for inverse value computation. */
  std::unique_ptr<BitVector> d_max_s;
  /** Signed lower bound (incl) for inverse value computation. */
  std::unique_ptr<BitVector> d_min_s;

  /** Cached inverse value result. */
  std::unique_ptr<BitVector> d_inverse;
  /** Cached consistent value result. */
  std::unique_ptr<BitVector> d_consistent;
};

std::ostream& operator<<(std::ostream& out, const BitVectorNode& node);

/* -------------------------------------------------------------------------- */

class BitVectorAdd : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorAdd(RNG* rng,
               uint32_t size,
               BitVectorNode* child0,
               BitVectorNode* child1);
  BitVectorAdd(RNG* rng,
               const BitVectorDomain& domain,
               BitVectorNode* child0,
               BitVectorNode* child1);

  Kind get_kind() const override { return ADD; }

  void evaluate() override;

  /**
   * IC:
   *   w/o  const bits: true
   *   with const bits: mfb(x, t - s)
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *   with const bits: true
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();
};

std::ostream& operator<<(std::ostream& out, const BitVectorAdd& node);

/* -------------------------------------------------------------------------- */

class BitVectorAnd : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorAnd(RNG* rng,
               uint32_t size,
               BitVectorNode* child0,
               BitVectorNode* child1);
  BitVectorAnd(RNG* rng,
               const BitVectorDomain& domain,
               BitVectorNode* child0,
               BitVectorNode* child1);

  Kind get_kind() const override { return AND; }

  void evaluate() override;

  /**
   * IC:
   *   w/o const bits (IC_wo): (t & s) = t
   *   with const bits       : IC_wo && ((s & x_hi) & m) = (t & m)
   *                         with m = ~(x_lo ^ x_hi)
   *                              ... mask out all non-const bits
   * Intuition:
   * 1) x & s = t on all const bits of x
   * 2) s & t = t on all non-const bits of x
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *   with const bits: t & x_hi = t
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();
};

std::ostream& operator<<(std::ostream& out, const BitVectorAnd& node);

/* -------------------------------------------------------------------------- */

class BitVectorConcat : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorConcat(RNG* rng,
                  uint32_t size,
                  BitVectorNode* child0,
                  BitVectorNode* child1);
  BitVectorConcat(RNG* rng,
                  const BitVectorDomain& domain,
                  BitVectorNode* child0,
                  BitVectorNode* child1);

  Kind get_kind() const override { return CONCAT; }

  void evaluate() override;

  /**
   * x o s = tx o ts
   * s o x = ts o tx
   *
   * IC:
   *   w/o  const bits: s = ts
   *     pos_x = 0: ts = t[bw(s) - 1 : 0]
   *     pos_x = 1: ts = t[bw(t) - 1 : bw(t) - bw(s)]
   *
   * with const bits: mfb(x, tx) && s = ts
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *
   *   with const bits: mfb(x, tx)
   *     pos_x = 0: tx = t[bw(t) - 1 : bw(t) - bw(x)]
   *     pos_x = 1: tx = t[bw(x) - 1 : 0]
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();
};

std::ostream& operator<<(std::ostream& out, const BitVectorConcat& node);

/* -------------------------------------------------------------------------- */

class BitVectorEq : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorEq(RNG* rng,
              uint32_t size,
              BitVectorNode* child0,
              BitVectorNode* child1);
  BitVectorEq(RNG* rng,
              const BitVectorDomain& domain,
              BitVectorNode* child0,
              BitVectorNode* child1);

  Kind get_kind() const override { return EQ; }

  void evaluate() override;

  /**
   * IC:
   *   w/o  const bits: true
   *   with const bits:
   *    t = 0: (x_hi != x_lo) || (x_hi != s)
   *    t = 1: mfb(x, s)
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *   with const bits: true
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();
};

std::ostream& operator<<(std::ostream& out, const BitVectorEq& node);

/* -------------------------------------------------------------------------- */

class BitVectorMul : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorMul(RNG* rng,
               uint32_t size,
               BitVectorNode* child0,
               BitVectorNode* child1);
  BitVectorMul(RNG* rng,
               const BitVectorDomain& domain,
               BitVectorNode* child0,
               BitVectorNode* child1);

  Kind get_kind() const override { return MUL; }

  void evaluate() override;

  /**
   * IC:
   *   w/o const bits (IC_wo): ((-s | s) & t) = t
   *   with const bits       : IC_wo &&
   *                           (s = 0 ||
   *                            ((odd(s) => mfb(x, t * s^-1)) &&
   *                             (!odd(s) => mfb (x << c, y << c))))
   *                    with c = ctz(s) and y = (t >> c) * (s >> c)^-1
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *   with const bits: (t != 0 => x_hi != 0) &&
   *                    (odd(t) => x_hi[lsb] != 0) &&
   *                    (!odd(t) => \exists y. (mfb(x, y) && ctz(t) >= ctz(y))
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();

  /** Cached inverse values result. */
  std::unique_ptr<BitVectorDomain> d_inverse_domain;
};

std::ostream& operator<<(std::ostream& out, const BitVectorMul& node);

/* -------------------------------------------------------------------------- */

class BitVectorShl : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorShl(RNG* rng,
               uint32_t size,
               BitVectorNode* child0,
               BitVectorNode* child1);
  BitVectorShl(RNG* rng,
               const BitVectorDomain& domain,
               BitVectorNode* child0,
               BitVectorNode* child1);

  Kind get_kind() const override { return SHL; }

  void evaluate() override;

  /**
   * IC:
   *   w/o const bits (IC_wo):
   *       pos_x = 0: (t >> s) << s = t
   *       pos_x = 1: ctz(s) <= ctz(t) &&
   *                  ((t = 0) || (s << (ctz(t) - ctz(s))) = t)
   *
   *   with const bits:
   *       pos_x = 0: IC_wo && mfb(x << s, t)
   *       pos_x = 1: IC_wo &&
   *                  ((t = 0) => (x_hi >= ctz(t) - ctz(s) || (s = 0))) &&
   *                  ((t != 0) => mfb(x, ctz(t) - ctz(s)))
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *   with const bits:
   *     pos_x = 0: \exists y. (y <= ctz(t) && mfb(x << y, t))
   *     pos_x = 1: t = 0 || \exists y. (y <= ctz(t) && mfb(x, y))
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();
};

std::ostream& operator<<(std::ostream& out, const BitVectorShl& node);

/* -------------------------------------------------------------------------- */

class BitVectorShr : public BitVectorNode
{
 public:
  /**
   * Additional interface / helper for is_invertible.
   * Cached result is stored in 'inverse_value'.
   */
  static bool is_invertible(RNG* rng,
                            const BitVector& t,
                            const BitVector& s,
                            const BitVectorDomain& x,
                            uint32_t pos_x,
                            bool is_essential_check,
                            std::unique_ptr<BitVector>* inverse_value);
  /**
   * Additional interface / helper for inverse_value.
   * Cached result is stored in 'inverse_value'.
   */
  static void inverse_value(RNG* rng,
                            const BitVector& t,
                            const BitVector& s,
                            const BitVectorDomain& x,
                            uint32_t pos_x,
                            std::unique_ptr<BitVector>& inverse_value);
  /** Constructors. */
  BitVectorShr(RNG* rng,
               uint32_t size,
               BitVectorNode* child0,
               BitVectorNode* child1);
  BitVectorShr(RNG* rng,
               const BitVectorDomain& domain,
               BitVectorNode* child0,
               BitVectorNode* child1);

  Kind get_kind() const override { return SHR; }

  void evaluate() override;

  /**
   * IC:
   *   w/o const bits (IC_wo):
   *       pos_x = 0: (t << s) >> s = t
   *       pos_x = 1: clz(s) <= clz(t) &&
   *                  ((t = 0) || (s >> (clz(t) - clz(s))) = t)
   *
   *   with const bits:
   *       pos_x = 0: IC_wo && mfb(x >> s, t)
   *       pos_x = 1: IC_wo &&
   *                  ((t = 0) => (x_hi >= clz(t) - clz(s) || (s = 0))) &&
   *                  ((t != 0) => mfb(x, clz(t) - clz(s)))
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *   with const bits:
   *     pos_x = 0: \exists y. (y <= clz(t) && mfb(x >> y, t))
   *     pos_x = 1: t = 0 || \exists y. (y <= clz(t) && mfb(x, y))
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();
};

std::ostream& operator<<(std::ostream& out, const BitVectorShr& node);

/* -------------------------------------------------------------------------- */

class BitVectorAshr : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorAshr(RNG* rng,
                uint32_t size,
                BitVectorNode* child0,
                BitVectorNode* child1);
  BitVectorAshr(RNG* rng,
                const BitVectorDomain& domain,
                BitVectorNode* child0,
                BitVectorNode* child1);

  Kind get_kind() const override { return ASHR; }

  void evaluate() override;

  /**
   * IC:
   *   w/o const bits (IC_wo):
   *       pos_x = 0: (s < bw(s) => (t << s) >>a s = t) &&
   *                  (s >= bw(s) => (t = ones || t = 0))
   *       pos_x = 1: (s[msb] = 0 => IC_shr(s >> x = t) &&
   *                  (s[msb] = 1 => IC_shr(~s >> x = ~t))
   *
   *   with const bits:
   *       pos_x = 0: IC_wo && mfb(x >>a s, t)
   *       pos_x = 1: IC_wo &&
   *                  (s[msb ] = 0 => IC_shr) &&
   *                  (s[msb] = 1 => IC_shr(~s >> x = ~t))
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *   with const bits:
   *     pos_x = 0:
   *     ((t = 0 || t = ones) => \exists y. (y[msb] = t[msb] && mfb(x, y))) &&
   *     ((t != 0 && t != ones) => \exists y. (
   *        c => y <= clo(t) && ~c => y <= clz(t) && mfb(x, y))
   *     with c = ((t << y)[msb] = 1)
   *
   *     pos_x = 1:
   *     t = 0 || t = ones ||
   *     \exists y. (c => y < clo(t) && ~c => y < clz(t) && mfb(x, y)
   *     with c = (t[msb] = 1)
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();
};

std::ostream& operator<<(std::ostream& out, const BitVectorAshr& node);

/* -------------------------------------------------------------------------- */

class BitVectorUdiv : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorUdiv(RNG* rng,
                uint32_t size,
                BitVectorNode* child0,
                BitVectorNode* child1);
  BitVectorUdiv(RNG* rng,
                const BitVectorDomain& domain,
                BitVectorNode* child0,
                BitVectorNode* child1);

  Kind get_kind() const override { return UDIV; }

  void evaluate() override;

  /**
   * IC:
   *   w/o const bits (IC_wo):
   *       pos_x = 0: (s * t) / s = t
   *       pos_x = 1: s / (s / t) = t
   *
   *   with const bits:
   *       pos_x = 0: IC_wo &&
   *                  (t = 0 => x_lo < s) &&
   *                  ((t != 0 && s != 0 ) => \exists y. (
   *                    mfb(x, y) && (~c => y < s * t + 1) && (c => y <= ones)))
   *                  with c = umulo(s, t + 1) && uaddo(t, 1)
   *       pos_x = 1: IC_wo &&
   *                  (t != ones => x_hi > 0) &&
   *                  ((s != 0 || t != 0) => (s / x_hi <= t) && \exists y. (
   *                      mfb(x, y) &&
   *                      (t = ones => y <= s / t) &&
   *                      (t != ones => y > t + 1 && y <= s / t)))
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *
   *   with const bits:
   *     pos_x = 0:
   *       (t != ones => x_hi >= t) && (t = 0 => x_lo != ones) &&
   *       ((t != 0 && t != ones && t != 1 && !mfb(x, t)) =>
   *        (!mulo(2, t) && \exists y,o.(mfb(x, y*t + o) && y >= 1 && o <= c
   *         && !mulo(y, t) && !addo(y * t, o))))
   *     with c = min(y − 1, x_hi − y * t)
   *
   *     pos_x = 1:
   *       (t = ones => (mfb(x, 0) || mfb(x, 1))) &&
   *       (t != ones => (!mulo(x_lo, t) &&
   *                  \exists y. (y > 0 && mfb(x, y) && !mulo(y, t))))
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();

  /**
   * Try to find a consistent value for pos_x = 0 other than x = t.
   * Returns a null bit-vector if no such value can be found.
   */
  BitVector consistent_value_pos0_aux(const BitVector& t);
};

std::ostream& operator<<(std::ostream& out, const BitVectorUdiv& node);

/* -------------------------------------------------------------------------- */

class BitVectorUlt : public BitVectorNode
{
 public:
  /**
   * Constructor.
   * @param rng The associated random number generator.
   * @param size The bit-width of this node.
   * @param child0 The operand at index 0.
   * @param child1 The operand at index 1.
   * @param opt_concat_sext True to enable optimization for inverse_value
   *                        computation of concat and sign extension operands.
   */
  BitVectorUlt(RNG* rng,
               uint32_t size,
               BitVectorNode* child0,
               BitVectorNode* child1,
               bool opt_concat_sext = false);
  /**
   * Constructor.
   * @param rng The associated random number generator.
   * @param domain The underlying bit-vector domain.
   * @param child0 The operand at index 0.
   * @param child1 The operand at index 1.
   * @param opt_concat_sext True to enable optimization for inverse_value
   *                        computation of concat and sign extension operands.
   */
  BitVectorUlt(RNG* rng,
               const BitVectorDomain& domain,
               BitVectorNode* child0,
               BitVectorNode* child1,
               bool opt_concat_sext = false);

  Kind get_kind() const override { return ULT; }

  void evaluate() override;

  /**
   * IC:
   *   w/o const bits (IC_wo):
   *       pos_x = 0: t = 0 || s != 0
   *       pos_x = 1: t = 0 || s != ones
   *
   *   with const bits:
   *       pos_x = 0: t = 1 => (s != 0 && x_lo < s) && t = 0 => (x_hi >= s)
   *       pos_x = 1: t = 1 => (s != ones && x_hi > s) && t = 0 => (x_lo <= s)
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *   with const bits: pos_x = 0: t = false || x_lo != ones
   *                    pos_x = 1: t = false || x_hi != 0
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();
  /**
   * Helper for is_invertible().
   * @param d The domain representing operand 'x'.
   * @param s The value of the other operand.
   * @param t The target value of this node.
   * @param pos_x The index of operand `x`.
   * @param find_inverse True to cache one (out of possibly many) inverse if
   *                     there is one.
   * @param is_essential_check True if called to determine is_essential(). For
   *                           is_essential() checks, we don't consider bounds
   *                           derived from top-level inequalities since this
   *                           may trap us in a cycle (see is_essential()).
   */
  bool _is_invertible(const BitVectorDomain* d,
                      const BitVector& s,
                      bool t,
                      uint32_t pos_x,
                      bool find_inverse,
                      bool is_essential_check);
  /**
   * Helper to compute the min and max bounds for `x` with respect to the
   * current min/max bounds of this node, if any.
   * @param s The value of the other operand.
   * @param t The target value of this node.
   * @param pos_x The index of operand `x`.
   * @param min The resulting lower bound.
   * @param max The resulting upper bound.
   * @param is_essential_check True if called to determine is_essential(). For
   *                           is_essential() checks, we don't consider bounds
   *                           derived from top-level inequalities since this
   *                           may trap us in a cycle (see is_essential()).
   */
  void compute_min_max_bounds(const BitVector& s,
                              bool t,
                              uint32_t pos_x,
                              BitVector& min,
                              BitVector& max,
                              bool is_essential_check = false);
  /**
   * Helper for concat-specific (when x is a concat) inverse value computation.
   * Attempts to find an inverse value by only changing the value of one of
   * the children of the concat.
   * @param s The value of the other operand.
   * @param t The target value of this node.
   * @param pos_x The index of operand `x`, which is a concat node.
   * @return The inverse value.
   */
  BitVector* inverse_value_concat(bool t, uint32_t pos_x, uint32_t pos_s);
  /**
   * Helper for sext-specific (when x is a sign extension) inverse value
   * computation. @param s The value of the other operand.
   * @param t The target value of this node.
   * @param pos_x The index of operand `x`, which is a sign extension node.
   * @return The inverse value.
   */
  BitVector* inverse_value_sext(bool t, uint32_t pos_x, uint32_t pos_s);
  /**
   * Helper for inverse_value_concat() and inverse_value_sext() to generate a
   * new random value with respect to the given domain and within given min/max
   * range.
   * @param d The domain.
   * @param min The lower bound of the range.
   * @param max The upper bound of the range.
   * @return A random value within the given range, if there is one, else
   *         a null BitVector.
   */
  BitVector inverse_value_concat_sext_new_random(const BitVectorDomain& d,
                                                 const BitVector& min,
                                                 const BitVector& max);
  /**
   * True to enable optimization for inverse_value computation of concat
   * and sign extension operands.
   */
  bool d_opt_concat_sext = false;
};

std::ostream& operator<<(std::ostream& out, const BitVectorUlt& node);

/* -------------------------------------------------------------------------- */

class BitVectorSlt : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorSlt(RNG* rng,
               uint32_t size,
               BitVectorNode* child0,
               BitVectorNode* child1,
               bool opt_concat_sext = false);
  BitVectorSlt(RNG* rng,
               const BitVectorDomain& domain,
               BitVectorNode* child0,
               BitVectorNode* child1,
               bool opt_concat_sext = false);

  Kind get_kind() const override { return SLT; }

  void evaluate() override;

  /**
   * IC:
   *   w/o const bits (IC_wo):
   *       pos_x = 0: t = 0 || s != min_signed_value
   *       pos_x = 1: t = 0 || s != max_signed_value
   *
   *   with const bits:
   *       pos_x = 0: t = 1 => (s != min_signed_value &&
   *                   ((MSB(x) = 0 && x_lo < s) ||
   *                    (MSB(x) != 0 && 1 o x_lo[bw-2:0] < s))) &&
   *                  t = 0 => ((MSB(x) = 1 && x_hi >= s) ||
   *                            (MSB(x) != 1 && 0 o x_hi[bw-2:0] >= s))))
   *       pos_x = 1: t = 1 => (s != max_signed_value &&
   *                            ((MSB(x) = 1 && s < x_hi) ||
   *                             (MSB(x) != 1 && s < 0 o x_hi[bw-2:0])))
   *                  t = 0 => ((MSB(x) = 0 && s >= x_lo) ||
   *                            (MSB(x) != 0 && s >= 1 o x_lo[bw-2:0])))
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *   with const bits: pos_x = 0: t = false || (const(x) => x_lo != smax)
   *                    pos_x = 1: t = false || (const(x) => x_lo != smin)
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();
  /**
   * Helper for is_invertible().
   * @param d The domain representing operand 'x'.
   * @param s The value of the other operand.
   * @param t The target value of this node.
   * @param pos_x The index of operand `x`.
   * @param find_inverse True to cache one (out of possibly many) inverse if
   *                     there is one.
   * @param is_essential_check True if called to determine is_essential(). For
   *                           is_essential() checks, we don't consider bounds
   *                           derived from top-level inequalities since this
   *                           may trap us in a cycle (see is_essential()).
   */
  bool _is_invertible(const BitVectorDomain* d,
                      const BitVector& s,
                      bool t,
                      uint32_t pos_x,
                      bool find_inverse,
                      bool is_essential_check);
  /**
   * Helper to compute the min and max bounds for `x` with respect to the
   * current min/max bounds of this node, if any.
   * @param s The value of the other operand.
   * @param t The target value of this node.
   * @param pos_x The index of operand `x`.
   * @param min The resulting lower bound.
   * @param max The resulting upper bound.
   * @param is_essential_check True if called to determine is_essential(). For
   *                           is_essential() checks, we don't consider bounds
   *                           derived from top-level inequalities since this
   * may trap us in a cycle (see is_essential()).
   */
  void compute_min_max_bounds(const BitVector& s,
                              bool t,
                              uint32_t pos_x,
                              BitVector& min,
                              BitVector& max,
                              bool is_essential_check = false);
  /**
   * Helper for concat-specific (when x is a concat) inverse value computation.
   * Attempts to find an inverse value by only changing the value of one of
   * the children of the concat.
   * @param s The value of the other operand.
   * @param t The target value of this node.
   * @param pos_x The index of operand `x`, which is a concat node.
   * @return The inverse value.
   */
  BitVector* inverse_value_concat(bool t, uint32_t pos_x, uint32_t pos_s);
  /**
   * Helper for sext-specific (when x is a sign extension) inverse value
   * computation. @param s The value of the other operand.
   * @param t The target value of this node.
   * @param pos_x The index of operand `x`, which is a sign extension node.
   * @return The inverse value.
   */
  BitVector* inverse_value_sext(bool t, uint32_t pos_x, uint32_t pos_s);
  /**
   * Helper for inverse_value_concat() and inverse_value_sext() to generate a
   * new random value with respect to the given domain and within given min/max
   * range.
   * @param d The domain.
   * @param min The lower bound of the range.
   * @param max The upper bound of the range.
   * @return A random value within the given range, if there is one, else
   *         a null BitVector.
   */
  BitVector inverse_value_concat_sext_new_random(const BitVectorDomain& d,
                                                 const BitVector& min,
                                                 const BitVector& max);
  /**
   * True to enable optimization for inverse_value computation of concat
   * and sign extension operands.
   */
  bool d_opt_concat_sext = false;
};

std::ostream& operator<<(std::ostream& out, const BitVectorSlt& node);

/* -------------------------------------------------------------------------- */

class BitVectorUrem : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorUrem(RNG* rng,
                uint32_t size,
                BitVectorNode* child0,
                BitVectorNode* child1);
  BitVectorUrem(RNG* rng,
                const BitVectorDomain& domain,
                BitVectorNode* child0,
                BitVectorNode* child1);

  Kind get_kind() const override { return UREM; }

  void evaluate() override;

  /**
   * IC:
   *   w/o const bits (IC_wo):
   *       pos_x = 0: ~(-s) >= t
   *       pos_x = 1: (t + t - s) & s >= t
   *
   *   with const bits:
   *       pos_x = 0: IC_wo &&
   *                  ((s = 0 || t = ones) => mfb(x, t)) &&
   *                  ((s != 0 && t != ones) => \exists y. (
   *                      mfb(x, s * y + t) && !umulo(s, y) && !uaddo(s *y, t)))
   *       pos_x = 1: IC_wo &&
   *                  (s = t => (x_lo = 0 || x_hi > t)) &&
   *                  (s != t => \exists y. (
   *                      mfb(x, y) && y > t && (s - t) mod y = 0)
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *  w/o  const bits: true
   *
   *  with const bits:
   *     pos_x = 0: (t = ones => mfb(x, ones)) &&
   *                (t != ones =>
   *                  (t > (ones - t) => mfb (x, t)) &&
   *                  (t < (ones - t) => mfb(x, t) ||
   *                   \exists y. (mfb(x, y) && y> 2*t))
   *
   *     pos_x = 1: mfb(x, 0) ||
   *                ((t = ones => mfb(x, 0)) &&
   *                 (t != ones => \exists y. (mfb(x, y) && y > t)))
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;
  const BitVector& inverse_value(const BitVector& t,
                                 uint32_t pos_x,
                                 uint32_t n_tries);

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();

  /**
   * Pick a consistent value for pos_x = 0 with x > t.
   * Returns a null bit-vector if no such value can be found.
   */
  BitVector consistent_value_pos0_aux(const BitVector& t);
  /** Cached inverse_value result. */
  std::unique_ptr<BitVectorDomain> d_inverse_domain;
};

std::ostream& operator<<(std::ostream& out, const BitVectorUrem& node);

/* -------------------------------------------------------------------------- */

class BitVectorXor : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorXor(RNG* rng,
               uint32_t size,
               BitVectorNode* child0,
               BitVectorNode* child1);
  BitVectorXor(RNG* rng,
               const BitVectorDomain& domain,
               BitVectorNode* child0,
               BitVectorNode* child1);

  Kind get_kind() const override { return XOR; }

  void evaluate() override;

  /**
   * IC:
   *   w/o  const bits: true
   *   with const bits: mfb(x, s^t)
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *   with const bits: true
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();
};

std::ostream& operator<<(std::ostream& out, const BitVectorXor& node);

/* -------------------------------------------------------------------------- */

class BitVectorIte : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorIte(RNG* rng,
               uint32_t size,
               BitVectorNode* child0,
               BitVectorNode* child1,
               BitVectorNode* child2);
  BitVectorIte(RNG* rng,
               const BitVectorDomain& domain,
               BitVectorNode* child0,
               BitVectorNode* child1,
               BitVectorNode* child2);

  Kind get_kind() const override { return ITE; }

  void evaluate() override;

  bool is_essential(const BitVector& t, uint32_t pos_x) override;

  /**
   * ite(_c, _t, _e)
   *
   * IC:
   *   w/o const bits (IC_wo):
   *       pos_x = 0: s0 == t || s1 == t
   *                  with s0 the value for '_t' branch
   *                  and s1 the value for '_e'
   *       pos_x = 1: s0 == true
   *                  with s0 the value for '_c'
   *       pos_x = 2: s0 == false
   *                  with s0 the value for '_c'
   *
   *   with const bits:
   *       pos_x = 0: (!is_fixed(x) && (s0 = t || s1 = t)) ||
   *                  (is_fixed_true(x) && s0 = t) ||
   *                  (is_fixed_false(x) && s1 = t)
   *                  with s0 the value for '_t' and s1 the value for '_e'
   *       pos_x = 1: s0 = true && mfb(x, t)
   *                  with s0 the value for '_c'
   *       pos_x = 2: s0 == false && mfb(x, t)
   *                  with s0 the value for '_c'
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *   with const bits: true
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  uint32_t select_path(const BitVector& t) override;

  std::string to_string() const override;

 private:
  int32_t select_path_non_const(std::vector<uint32_t>& inputs) const override;
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();
};

std::ostream& operator<<(std::ostream& out, const BitVectorIte& node);

/* -------------------------------------------------------------------------- */

class BitVectorNot : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorNot(RNG* rng, uint32_t size, BitVectorNode* child0);
  BitVectorNot(RNG* rng, const BitVectorDomain& domain, BitVectorNode* child0);

  Kind get_kind() const override { return NOT; }

  void evaluate() override;

  bool is_essential(const BitVector& t, uint32_t pos_x) override;

  /**
   * IC:
   *   w/o  const bits: true
   *   with const bits: mfb(x, ~t)
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *   with const bits: IC
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();
};

std::ostream& operator<<(std::ostream& out, const BitVectorNot& node);

/* -------------------------------------------------------------------------- */

class BitVectorExtract : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorExtract(
      RNG* rng, uint32_t size, BitVectorNode* child0, uint32_t hi, uint32_t lo);
  BitVectorExtract(RNG* rng,
                   const BitVectorDomain& domain,
                   BitVectorNode* child0,
                   uint32_t hi,
                   uint32_t lo);

  Kind get_kind() const override { return EXTRACT; }

  void evaluate() override;

  bool is_essential(const BitVector& t, uint32_t pos_x) override;

  /**
   * IC:
   *   w/o  const bits: true
   *   with const bits: mfb(x[hi:lo], t)
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *   with const bits: IC
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Probability for keeping the current value of don't care bits (rather than
   * of fully randomizing all of them).
   */
  static constexpr uint32_t s_prob_keep = 500;

  /** The upper index. */
  uint32_t d_hi;
  /** The lower index. */
  uint32_t d_lo;
  /**
   * Left part of don't care bits, that is, all bits > d_hi.
   * Nullptr if d_hi = msb.
   * Cache for inverse_value.
   */
  std::unique_ptr<BitVectorDomain> d_x_slice_left;
  /**
   * Right part of don't care bits, that is, all bits < d_lo.
   * Nullptr if d_lo = 0.
   * Cache for inverse_value.
   */
  std::unique_ptr<BitVectorDomain> d_x_slice_right;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();
};

std::ostream& operator<<(std::ostream& out, const BitVectorExtract& node);

/* -------------------------------------------------------------------------- */

class BitVectorSignExtend : public BitVectorNode
{
 public:
  /** Constructors. */
  BitVectorSignExtend(RNG* rng,
                      uint32_t size,
                      BitVectorNode* child0,
                      uint32_t n);
  BitVectorSignExtend(RNG* rng,
                      const BitVectorDomain& domain,
                      BitVectorNode* child0,
                      uint32_t n);

  /**
   * Get the number of extension bits.
   * @return The number of extension bits.
   */
  uint32_t get_n() const { return d_n; }

  Kind get_kind() const override { return SEXT; }

  void evaluate() override;

  bool is_essential(const BitVector& t, uint32_t pos_x) override;

  /**
   * IC:
   *   w/o  const bits (IC_wo): t_ext == ones || t_ext == zero
   *                            and t_x   = t[t_size - 1 - n : 0]
   *                            and t_ext = t[t_size - 1, t_size - 1 - n]
   *                            (i.e., it includes MSB of t_x)
   *
   *   with const bits: IC_wo && mfb(x, t_x)
   */
  bool is_invertible(const BitVector& t,
                     uint32_t pos_x,
                     bool find_inverse       = true,
                     bool is_essential_check = false) override;

  /**
   * CC:
   *   w/o  const bits: true
   *   with const bits: IC
   */
  bool is_consistent(const BitVector& t, uint32_t pos_x) override;

  const BitVector& inverse_value(const BitVector& t, uint32_t pos_x) override;

  const BitVector& consistent_value(const BitVector& t,
                                    uint32_t pos_x) override;

  std::string to_string() const override;

 private:
  /**
   * Evaluate the assignment of this node.
   * Helper to allow evaluating the assignment on construction (evaluate() is
   * virtual and cannot be called from the constructor).
   */
  void _evaluate();
  /**
   * Wrapper for evaluating the assignment and fixing the domain on construction
   * when all operands are constant.
   */
  void _evaluate_and_set_domain();

  /** The number of bits to extend with. */
  uint32_t d_n;
};

std::ostream& operator<<(std::ostream& out, const BitVectorSignExtend& node);

/* -------------------------------------------------------------------------- */

}  // namespace ls
}  // namespace bzla
#endif
