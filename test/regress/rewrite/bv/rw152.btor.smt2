(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 47))
(declare-const v1 (_ BitVec 47))
(assert (= #b1 (bvor (bvnot (ite (= (ite (= (bvnot v1) (bvadd (bvnot v0) (bvnot v1))) #b1 #b0) (ite (= (bvnot v0) (_ bv0 47)) #b1 #b0)) #b1 #b0)) (bvnot (ite (= (ite (= (bvadd (bvnot v0) (bvnot v1)) (bvnot v1)) #b1 #b0) (ite (= (bvnot v0) (_ bv0 47)) #b1 #b0)) #b1 #b0)))))
(check-sat)
