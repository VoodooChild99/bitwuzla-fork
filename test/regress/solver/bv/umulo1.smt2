(set-logic QF_BV)
(set-info :status sat)
(declare-const v (_ BitVec 3))
(assert (not (bvumulo v v)))
(check-sat)
