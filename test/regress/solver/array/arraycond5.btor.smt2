(set-logic QF_ABV)
(set-info :status unsat)
(declare-const a0 (Array (_ BitVec 1) (_ BitVec 1) ))
(declare-const a1 (Array (_ BitVec 1) (_ BitVec 1) ))
(declare-const a2 (Array (_ BitVec 1) (_ BitVec 1) ))
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 1))
(declare-const v2 (_ BitVec 1))
(declare-const v3 (_ BitVec 1))
(assert (= #b1 (bvand (bvand (bvand (bvnot (bvand (bvnot (select a0 #b0)) (bvnot (select a0 #b1)))) (bvnot (bvand (select a0 #b0) (select a0 #b1)))) (bvand (bvnot (bvand (bvnot (select a1 #b0)) (bvnot (select a1 #b1)))) (bvnot (bvand (select a1 #b0) (select a1 #b1))))) (bvand (bvand (bvnot (bvand (bvnot (select a2 #b0)) (bvnot (select a2 #b1)))) (bvnot (bvand (select a2 #b0) (select a2 #b1)))) (bvand (select (ite (= (bvand v2 v3) #b1) (ite (= (bvand (bvnot v0) (bvnot v1)) #b1) a1 a0) a2) #b0) (select (ite (= (bvand v2 v3) #b1) (ite (= (bvand (bvnot v0) (bvnot v1)) #b1) a1 a0) a2) #b1))))))
(check-sat)
