(set-info :smt-lib-version 2.6)
(set-logic QF_BV)
(set-info :status sat)
(declare-fun x () (_ BitVec 32))
(declare-fun y () (_ BitVec 32))
(assert (bvuge x y))
(assert (bvule (bvadd x (_ bv1 32)) y))
(check-sat)
(exit)
