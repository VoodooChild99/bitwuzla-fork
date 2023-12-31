(set-info :status unsat)
(set-option :produce-unsat-cores true)
(set-logic QF_AUFBV)
(declare-fun a () (Array (_ BitVec 3) (_ BitVec 3)))
(declare-fun b () (Array (_ BitVec 3) (_ BitVec 3)))
(assert (! (= (select a (_ bv0 3)) (select b (_ bv0 3))) :named a0))
(assert (! (= (select a (_ bv1 3)) (select b (_ bv1 3))) :named a1))
(assert (! (= (select a (_ bv2 3)) (select b (_ bv2 3))) :named a2))
(assert (! (= (select a (_ bv3 3)) (select b (_ bv3 3))) :named a3))
(assert (! (= (select a (_ bv4 3)) (select b (_ bv4 3))) :named a4))
(assert (! (= (select a (_ bv5 3)) (select b (_ bv5 3))) :named a5))
(assert (! (= (select a (_ bv6 3)) (select b (_ bv6 3))) :named a6))
(assert (! (= (select a (_ bv7 3)) (select b (_ bv7 3))) :named a7))
(assert (! (not (= a b)) :named a8))
(check-sat)
(get-unsat-core)
