(set-logic QF_ABV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 8))
(declare-const v2 (_ BitVec 8))
(declare-const a0 (Array (_ BitVec 8) (_ BitVec 8) ))
(assert (= #b1 (bvnot (ite (= (ite (= v0 #b1) (store a0 v1 v2) a0) (store a0 v1 (ite (= v0 #b1) v2 (select a0 v1)))) #b1 #b0))))
(check-sat)
