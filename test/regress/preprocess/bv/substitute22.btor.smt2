(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 8))
(assert (= #b1 (bvand (bvredor (bvand v0 #b00001110)) (ite (= (bvnot (_ bv0 3)) ((_ extract 3 1) (bvnot v0))) #b1 #b0))))
(check-sat)
