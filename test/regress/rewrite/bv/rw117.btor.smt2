(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 8))
(declare-const v1 (_ BitVec 8))
(declare-const v2 (_ BitVec 1))
(declare-const v3 (_ BitVec 1))
(assert (= #b1 (bvnot (ite (= (ite (= v3 #b1) v1 (ite (= v2 #b1) v0 v1)) (ite (= (bvand v2 (bvnot v3)) #b1) v0 v1)) #b1 #b0))))
(check-sat)
