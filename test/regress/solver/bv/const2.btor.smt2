(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 1))
(assert (= #b1 (ite (= v0 (bvnot v0)) #b1 #b0)))
(check-sat)
