(set-logic QF_ABV)
(set-info :status unsat)
(declare-const a0 (Array (_ BitVec 8) (_ BitVec 8) ))
(declare-const v0 (_ BitVec 8))
(assert (= #b1 (bvnot (ite (= (select (store a0 (bvadd v0 #b00000001) (bvadd v0 #b00000001)) v0) (select a0 v0)) #b1 #b0))))
(check-sat)
