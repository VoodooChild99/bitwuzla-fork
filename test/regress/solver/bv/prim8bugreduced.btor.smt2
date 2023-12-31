(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 1))
(assert (= #b1 (ite (= v0 (bvnot v1)) #b1 #b0)))
(assert (= #b1 (ite (= (_ bv0 1) (bvnot v1)) #b1 #b0)))
(assert (= #b1 (bvand (ite (bvult v0 (bvnot v1)) #b1 #b0) (ite (= (bvnot v1) (_ bv0 1)) #b1 #b0))))
(check-sat)
