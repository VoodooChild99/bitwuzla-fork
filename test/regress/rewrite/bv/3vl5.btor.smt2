(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 6))
(declare-const v1 (_ BitVec 3))
(declare-const v2 (_ BitVec 2))
(declare-const v3 (_ BitVec 1))
(assert (= #b1 (bvnot (ite (= ((_ extract 7 3) (bvudiv (concat (_ bv0 2) v0) (concat (concat (concat (concat v1 (_ bv1 1)) v2) (_ bv1 1)) v3))) (_ bv0 5)) #b1 #b0))))
(check-sat)
