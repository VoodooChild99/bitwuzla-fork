(set-info :status sat)
(set-option :produce-models true)
(set-info :status sat)
(set-logic QF_BV)
(declare-fun a () Bool)
(assert (not a))
(check-sat)
(get-model)
