(set-logic QF_ABV)
(set-info :status unsat)
(declare-const a0 (Array (_ BitVec 1) (_ BitVec 1) ))
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 1))
(declare-const v2 (_ BitVec 1))
(declare-const v3 (_ BitVec 1))
(assert (= #b1 (bvnot (bvor (bvnot (ite (= (bvand v0 v1) (bvnot (bvand v2 v3))) #b1 #b0)) (ite (= (select a0 (bvand v0 v1)) (select a0 (bvnot (bvand v2 v3)))) #b1 #b0)))))
(check-sat)
