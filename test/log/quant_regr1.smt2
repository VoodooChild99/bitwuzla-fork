(declare-const x Bool)
(declare-fun f () Bool)
(assert (forall ((y Bool)) (= (_ bv0 16) (ite (ite x y f) (_ bv0 16) (_ bv1 16)))))
(check-sat)
