(push 1)
(declare-const x (_ BitVec 2))
(assert (and (= x (bvadd (_ bv1 2) (_ bv1 2))) (= x (_ bv2 2))))
(set-info :status sat)
(check-sat)
(pop 1)
