(declare-const O (_ BitVec 1))
(declare-const x Bool)
(declare-fun I () (Array (_ BitVec 32) (_ BitVec 32)))
(assert (= (_ bv0 32) (ite (= (_ bv0 32) ((_ zero_extend 31) O)) (_ bv1 32) (_ bv0 32))))
(push 1)
(assert (and (distinct (_ bv0 32) (ite (= (_ bv0 32) ((_ zero_extend 31) O)) (_ bv1 32) (_ bv0 32))) x (bvule (select (store I ((_ zero_extend 31) O) (_ bv0 32)) (_ bv0 32)) (_ bv0 32))))
(set-info :status unsat)
(check-sat)
