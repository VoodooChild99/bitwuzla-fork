(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 1))
(assert (= #b1 (ite (distinct (bvand (bvnot (bvand (bvnot v0) v1)) (bvnot (bvand v0 (bvnot v1)))) (ite (= v0 v1) #b1 #b0)) #b1 #b0)))
(check-sat)
