(set-logic QF_BV)
(set-info :status sat)
(declare-const v0 (_ BitVec 32))
(assert (= #b1 (bvnot (bvand (bvnot (bvand ((_ extract 31 31) #b00000000000000000000000000001010) (bvnot ((_ extract 31 31) v0)))) (bvand (bvnot (bvand (bvand (bvnot ((_ extract 31 31) #b00000000000000000000000000001010)) (bvnot ((_ extract 31 31) v0))) (ite (bvult ((_ extract 30 0) #b00000000000000000000000000001010) ((_ extract 30 0) v0)) #b1 #b0))) (bvnot (bvand (bvand ((_ extract 31 31) #b00000000000000000000000000001010) ((_ extract 31 31) v0)) (ite (bvult ((_ extract 30 0) #b00000000000000000000000000001010) ((_ extract 30 0) v0)) #b1 #b0))))))))
(check-sat)
