(set-info :status unsat)
(declare-const x RoundingMode)
(declare-const x2 RoundingMode)
(assert (not (= x x2)))
(push 1)
(assert (= x x2))
(assert (= x2 RTP))
(assert (= x RTP))
(check-sat)
