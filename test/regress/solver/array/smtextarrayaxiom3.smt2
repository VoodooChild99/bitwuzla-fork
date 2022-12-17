(set-info :status unsat)
(set-option :incremental false)
(set-logic QF_AUFBV)
(declare-fun a () (Array (_ BitVec 3) (_ BitVec 3)))
(declare-fun b () (Array (_ BitVec 3) (_ BitVec 3)))
(assert (= (select a (_ bv0 3)) (select b (_ bv0 3))))
(assert (= (select a (_ bv1 3)) (select b (_ bv1 3))))
(assert (= (select a (_ bv2 3)) (select b (_ bv2 3))))
(assert (= (select a (_ bv3 3)) (select b (_ bv3 3))))
(assert (= (select a (_ bv4 3)) (select b (_ bv4 3))))
(assert (= (select a (_ bv5 3)) (select b (_ bv5 3))))
(assert (= (select a (_ bv6 3)) (select b (_ bv6 3))))
(assert (= (select a (_ bv7 3)) (select b (_ bv7 3))))
(assert (not (= a b)))
(check-sat)

