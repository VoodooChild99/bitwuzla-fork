(set-logic QF_BV)
(set-info :status sat)
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 1))
(declare-const v2 (_ BitVec 1))
(declare-const v3 (_ BitVec 1))
(declare-const v4 (_ BitVec 2))
(assert (= #b1 (bvand (ite (= (concat (bvnot (ite (= v0 v1) #b1 #b0)) (ite (= v2 v3) #b1 #b0)) v4) #b1 #b0) (bvand (ite (= v0 v1) #b1 #b0) (ite (= v2 v3) #b1 #b0)))))
(check-sat)
