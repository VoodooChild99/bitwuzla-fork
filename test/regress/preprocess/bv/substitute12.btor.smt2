(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 8))
(assert (= #b1 (bvand (bvnot (bvredor (bvand v0 #b00000001))) (bvnot ((_ extract 0 0) (bvnot v0))))))
(check-sat)
