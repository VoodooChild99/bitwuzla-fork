(set-info :status unknown)
(declare-const b (_ BitVec 64))
(assert (not (= (_ bv0 32) ((_ extract 63 32) b))))
(assert (not (= (_ bv0 64) (bvlshr (_ bv1 64) b))))
;(assert (not (= (_ bv0 32) ((_ extract 31 0) b))))
(check-sat)
