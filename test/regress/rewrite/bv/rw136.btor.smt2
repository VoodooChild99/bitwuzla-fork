(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 4))
(declare-const v1 (_ BitVec 4))
(declare-const v2 (_ BitVec 4))
(assert (= #b1 (bvor (bvor (bvnot (ite (= (ite (= (bvadd v0 v1) (bvadd v0 v2)) #b1 #b0) (ite (= v1 v2) #b1 #b0)) #b1 #b0)) (bvnot (ite (= (ite (= (bvadd v0 v1) (bvadd v1 v2)) #b1 #b0) (ite (= v0 v2) #b1 #b0)) #b1 #b0))) (bvor (bvnot (ite (= (ite (= (bvadd v0 v2) (bvadd v1 v2)) #b1 #b0) (ite (= v0 v1) #b1 #b0)) #b1 #b0)) (bvnot (ite (= (ite (= v0 v2) #b1 #b0) (ite (= (bvadd v1 v2) (bvadd v0 v1)) #b1 #b0)) #b1 #b0))))))
(check-sat)
