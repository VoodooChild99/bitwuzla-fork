(set-logic QF_BV)
(set-info :status unsat)
(declare-fun s () (_ BitVec 4))
(declare-fun t () (_ BitVec 4))
(assert (not (= (bvnor s t) (bvnot (bvor s t)))))
(check-sat)
(exit)
