(set-logic QF_BV)
(set-info :status unsat)
(declare-fun x () (_ BitVec 4))
(declare-fun y () (_ BitVec 4))
(assert (distinct (bvand (bvnot (concat y #xf)) (bvnot (concat #x0 x))) (concat (bvnot y) #x0)))
(check-sat)
(exit)
