(set-logic QF_BV)
(set-info :status sat)
(declare-const v0 (_ BitVec 2))
(declare-const v1 (_ BitVec 2))
(declare-const v2 (_ BitVec 2))
(assert (= #b1 (bvand (ite (= v0 (bvmul v1 v2)) #b1 #b0) (ite (distinct v0 (bvnot (_ bv0 2))) #b1 #b0))))
(check-sat)
