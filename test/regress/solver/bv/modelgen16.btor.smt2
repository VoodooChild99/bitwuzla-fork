(set-logic QF_BV)
(set-info :status sat)
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 2))
(assert (= #b1 (ite (= v0 (bvand (bvnot (ite (bvult (bvnot v1) v1) #b1 #b0)) (bvnot (ite (bvult v1 (bvnot v1)) #b1 #b0)))) #b1 #b0)))
(check-sat)
