(set-logic QF_BV)
(set-info :status unsat)
(declare-fun b () Bool)
(declare-fun x () (_ BitVec 4))
(declare-fun y () (_ BitVec 4))
(declare-fun z () (_ BitVec 4))
(assert (distinct (ite b (concat x y) (concat x z)) (concat x (ite b y z))))
(check-sat)
(exit)
