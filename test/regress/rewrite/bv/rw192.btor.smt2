(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 4))
(declare-const v1 (_ BitVec 4))
(declare-const v2 (_ BitVec 4))
(assert (= #b1 (ite (distinct (ite (bvult (bvnot (concat v0 v1)) (bvnot (concat v0 v2))) #b1 #b0) (ite (bvult (bvnot v1) (bvnot v2)) #b1 #b0)) #b1 #b0)))
(check-sat)
