(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 8))
(assert (= #b1 (bvand (bvnot (bvredor (bvand v0 #b00001000))) (bvnot ((_ extract 3 3) (bvnot v0))))))
(check-sat)
