(set-info :status unsat)
(set-option :produce-unsat-cores true)
(set-logic QF_AUFBV)
(declare-fun a () (Array (_ BitVec 4) (_ BitVec 4)))
(declare-fun b () (Array (_ BitVec 4) (_ BitVec 4)))
(assert (! (= (select a (_ bv0 4)) (select b (_ bv0 4))) :named a0))
(assert (! (= (select a (_ bv1 4)) (select b (_ bv1 4))) :named a1))
(assert (! (= (select a (_ bv2 4)) (select b (_ bv2 4))) :named a2))
(assert (! (= (select a (_ bv3 4)) (select b (_ bv3 4))) :named a3))
(assert (! (= (select a (_ bv4 4)) (select b (_ bv4 4))) :named a4))
(assert (! (= (select a (_ bv5 4)) (select b (_ bv5 4))) :named a5))
(assert (! (= (select a (_ bv6 4)) (select b (_ bv6 4))) :named a6))
(assert (! (= (select a (_ bv7 4)) (select b (_ bv7 4))) :named a7))
(assert (! (= (select a (_ bv8 4)) (select b (_ bv8 4))) :named a8))
(assert (! (= (select a (_ bv9 4)) (select b (_ bv9 4))) :named a9))
(assert (! (= (select a (_ bv10 4)) (select b (_ bv10 4))) :named a10))
(assert (! (= (select a (_ bv11 4)) (select b (_ bv11 4))) :named a11))
(assert (! (= (select a (_ bv12 4)) (select b (_ bv12 4))) :named a12))
(assert (! (= (select a (_ bv13 4)) (select b (_ bv13 4))) :named a13))
(assert (! (= (select a (_ bv14 4)) (select b (_ bv14 4))) :named a14))
(assert (! (= (select a (_ bv15 4)) (select b (_ bv15 4))) :named a15))
(assert (! (not (= a b)) :named a16))
(check-sat)
(get-unsat-core)
