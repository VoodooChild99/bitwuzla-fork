(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 8))
(assert (= #b1 (bvor (bvnot (ite (= (bvand (bvand #b01000101 v0) #b10101010) (bvand (_ bv0 8) v0)) #b1 #b0)) (bvnot (ite (= (bvand #b10101010 (bvand #b01000101 v0)) (bvand (_ bv0 8) v0)) #b1 #b0)))))
(check-sat)
