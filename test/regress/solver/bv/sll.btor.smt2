(set-logic QF_BV)
(set-info :status sat)
(declare-const v0 (_ BitVec 32))
(declare-const v1 (_ BitVec 5))
(assert (distinct (_ bv0 32) (bvshl v0 ((_ zero_extend 27) v1))))
(check-sat)
