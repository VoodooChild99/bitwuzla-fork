(set-logic QF_ABV)
(set-info :status unsat)
(declare-const a0 (Array (_ BitVec 1) (_ BitVec 1) ))
(declare-const a1 (Array (_ BitVec 1) (_ BitVec 1) ))
(assert (= #b1 (bvnot (ite (= (bvand (ite (= (select a0 #b0) (select a1 #b0)) #b1 #b0) (ite (= (select a0 #b1) (select a1 #b1)) #b1 #b0)) (ite (= a0 a1) #b1 #b0)) #b1 #b0))))
(check-sat)
