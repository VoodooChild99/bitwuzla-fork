(set-logic QF_BV)
(set-info :status sat)
(set-option :incremental false)
(assert (= (bvlshr (_ bv0 2) (_ bv0 2)) (_ bv0 2)))
(assert (= (bvlshr (_ bv0 2) (_ bv1 2)) (_ bv0 2)))
(assert (= (bvlshr (_ bv0 2) (_ bv2 2)) (_ bv0 2)))
(assert (= (bvlshr (_ bv0 2) (_ bv3 2)) (_ bv0 2)))
(assert (= (bvlshr (_ bv1 2) (_ bv0 2)) (_ bv1 2)))
(assert (= (bvlshr (_ bv1 2) (_ bv1 2)) (_ bv0 2)))
(assert (= (bvlshr (_ bv1 2) (_ bv2 2)) (_ bv0 2)))
(assert (= (bvlshr (_ bv1 2) (_ bv3 2)) (_ bv0 2)))
(assert (= (bvlshr (_ bv2 2) (_ bv0 2)) (_ bv2 2)))
(assert (= (bvlshr (_ bv2 2) (_ bv1 2)) (_ bv1 2)))
(assert (= (bvlshr (_ bv2 2) (_ bv2 2)) (_ bv0 2)))
(assert (= (bvlshr (_ bv2 2) (_ bv3 2)) (_ bv0 2)))
(assert (= (bvlshr (_ bv3 2) (_ bv0 2)) (_ bv3 2)))
(assert (= (bvlshr (_ bv3 2) (_ bv1 2)) (_ bv1 2)))
(assert (= (bvlshr (_ bv3 2) (_ bv2 2)) (_ bv0 2)))
(assert (= (bvlshr (_ bv3 2) (_ bv3 2)) (_ bv0 2)))
(check-sat)

