(set-logic QF_ABV)
(set-info :status unsat)
(declare-const a0 (Array (_ BitVec 1) (_ BitVec 1) ))
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 1))
(declare-const v2 (_ BitVec 1))
(assert (= #b1 (bvand (ite (= v0 v2) #b1 #b0) (bvnot (ite (= v1 (select (store a0 v0 v1) v2)) #b1 #b0)))))
(check-sat)
