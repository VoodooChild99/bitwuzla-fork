(declare-const z (_ BitVec 1))
(declare-fun n () (_ BitVec 8))
(declare-fun B ((_ BitVec 8) (_ BitVec 8)) (_ BitVec 8))
(assert (= n (B (_ bv0 8) ((_ zero_extend 7) z))))
(assert (= (_ bv1 8) (B (_ bv0 8) (_ bv0 8))))
(set-info :status sat)
(check-sat)
