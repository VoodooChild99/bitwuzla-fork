(set-info :status unsat)
(set-logic QF_BV)
(declare-fun a () (_ BitVec 4))
(declare-fun b () (_ BitVec 4))
(declare-fun c () (_ BitVec 4))
(assert (not (= (bvmul a (bvmul b c)) (bvmul (bvmul a b) c))))
(check-sat)
