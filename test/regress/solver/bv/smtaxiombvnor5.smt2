(set-logic QF_BV)
(set-info :status unsat)
(declare-fun s () (_ BitVec 5))
(declare-fun t () (_ BitVec 5))
(assert (not (= (bvnor s t) (bvnot (bvor s t)))))
(check-sat)
(exit)
