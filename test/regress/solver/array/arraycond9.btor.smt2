(set-logic QF_ABV)
(set-info :status unsat)
(declare-const a0 (Array (_ BitVec 8) (_ BitVec 8) ))
(declare-const a1 (Array (_ BitVec 8) (_ BitVec 8) ))
(declare-const v0 (_ BitVec 1))
(assert (= #b1 (bvand (bvnot (ite (= a0 (ite (= v0 #b1) a0 a1)) #b1 #b0)) (bvnot (ite (= a1 (ite (= v0 #b1) a0 a1)) #b1 #b0)))))
(check-sat)
