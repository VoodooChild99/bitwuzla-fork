(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 32))
(declare-const v2 (_ BitVec 32))
(assert (= #b1 (bvor (bvnot (ite (= (ite (= (bvnot (ite (= v0 #b1) v1 v2)) v2) #b1 #b0) (bvand v0 (ite (= (bvnot v1) v2) #b1 #b0))) #b1 #b0)) (bvnot (ite (= (bvand v0 (ite (= (bvnot v1) v2) #b1 #b0)) (ite (= v2 (bvnot (ite (= v0 #b1) v1 v2))) #b1 #b0)) #b1 #b0)))))
(check-sat)
