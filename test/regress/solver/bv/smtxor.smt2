(set-info :status sat)
(set-logic QF_BV)
(declare-fun a () Bool)
(declare-fun b () Bool)
(assert (and a (xor a b)))
(check-sat)
