(set-logic QF_ABV)
(set-info :status sat)
(declare-const x (_ BitVec 32))
(declare-const y (_ BitVec 8))
(declare-const a (Array (_ BitVec 8) (_ BitVec 32)))
(assert (= a ((as const (Array (_ BitVec 8) (_ BitVec 32))) x)))
(check-sat)
(exit)
