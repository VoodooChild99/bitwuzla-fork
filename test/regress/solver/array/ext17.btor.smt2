(set-logic QF_ABV)
(set-info :status sat)
(declare-const a0 (Array (_ BitVec 1) (_ BitVec 1) ))
(declare-const a1 (Array (_ BitVec 1) (_ BitVec 1) ))
(assert (= true (and (ite (distinct a0 a1) true false) (and (bvule (select a0 #b0) (select a0 #b1)) (bvule (select a1 #b0) (select a1 #b1))))))
(check-sat)
