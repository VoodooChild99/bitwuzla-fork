(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 4))
(declare-const v2 (_ BitVec 4))
(declare-const v3 (_ BitVec 4))
(assert (= #b1 (bvor (bvor (bvnot (ite (= (ite (= v0 #b1) (bvand v1 v2) (bvand v1 v3)) (bvand (ite (= v0 #b1) v2 v3) v1)) #b1 #b0)) (bvnot (ite (= (ite (= v0 #b1) (bvand v1 v3) (bvand v2 v3)) (bvand (ite (= v0 #b1) v1 v2) v3)) #b1 #b0))) (bvor (bvnot (ite (= (ite (= v0 #b1) (bvand v1 v2) (bvand v2 v3)) (bvand (ite (= v0 #b1) v1 v3) v2)) #b1 #b0)) (bvnot (ite (= (ite (= v0 #b1) (bvand v2 v3) (bvand v1 v2)) (bvand (ite (= v0 #b1) v3 v1) v2)) #b1 #b0))))))
(check-sat)
