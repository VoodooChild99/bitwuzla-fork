(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 8))
(declare-const v2 (_ BitVec 8))
(declare-const v3 (_ BitVec 8))
(assert (= #b1 (ite (distinct (ite (= (bvnot (ite (= v0 #b1) v1 v2)) (bvnot (ite (= v0 #b1) v1 v3))) #b1 #b0) (ite (= v0 #b1) (ite (= (bvnot v1) (bvnot v1)) #b1 #b0) (ite (= (bvnot v2) (bvnot v3)) #b1 #b0))) #b1 #b0)))
(check-sat)
