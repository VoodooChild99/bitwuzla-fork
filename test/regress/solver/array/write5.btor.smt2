(set-logic QF_ABV)
(set-info :status sat)
(declare-const a0 (Array (_ BitVec 1) (_ BitVec 1) ))
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 1))
(declare-const v2 (_ BitVec 1))
(declare-const v3 (_ BitVec 1))
(declare-const v4 (_ BitVec 1))
(declare-const v5 (_ BitVec 1))
(assert (= #b1 (select (store a0 (bvand v0 v1) (bvnot (bvand (bvnot v2) (bvnot v3)))) (bvand (bvnot (bvand (bvnot v4) (bvnot v5))) (bvnot (bvand v4 v5))))))
(check-sat)
