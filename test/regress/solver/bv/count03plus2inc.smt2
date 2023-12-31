(set-logic QF_BV)
(declare-fun s0 () (_ BitVec 3))
(declare-fun zero () (_ BitVec 3))
(declare-fun one () (_ BitVec 3))
(declare-fun goal () (_ BitVec 3))
(declare-fun lim () (_ BitVec 3))
(declare-fun r1 () (_ BitVec 1))
(declare-fun e1 () (_ BitVec 1))
(declare-fun s1 () (_ BitVec 3))
(declare-fun i1 () (_ BitVec 3))
(declare-fun r2 () (_ BitVec 1))
(declare-fun e2 () (_ BitVec 1))
(declare-fun s2 () (_ BitVec 3))
(declare-fun i2 () (_ BitVec 3))
(declare-fun r3 () (_ BitVec 1))
(declare-fun e3 () (_ BitVec 1))
(declare-fun s3 () (_ BitVec 3))
(declare-fun i3 () (_ BitVec 3))
(declare-fun r4 () (_ BitVec 1))
(declare-fun e4 () (_ BitVec 1))
(declare-fun s4 () (_ BitVec 3))
(declare-fun i4 () (_ BitVec 3))
(declare-fun r5 () (_ BitVec 1))
(declare-fun e5 () (_ BitVec 1))
(declare-fun s5 () (_ BitVec 3))
(declare-fun i5 () (_ BitVec 3))
(declare-fun r6 () (_ BitVec 1))
(declare-fun e6 () (_ BitVec 1))
(declare-fun s6 () (_ BitVec 3))
(declare-fun i6 () (_ BitVec 3))
(declare-fun r7 () (_ BitVec 1))
(declare-fun e7 () (_ BitVec 1))
(declare-fun s7 () (_ BitVec 3))
(declare-fun i7 () (_ BitVec 3))
(assert (= goal (_ bv7 3)))
(assert (= zero (_ bv0 3)))
(assert (= one (_ bv1 3)))
(assert (= lim (_ bv2 3)))
(assert (= s0 zero))
(push 1)
(assert (= s0 goal))
(set-info :status unsat)
(check-sat)
(pop 1)

(assert (bvule i1 lim))
(assert (= s1 (ite (= r1 (_ bv1 1)) zero (ite (= e1 (_ bv1 1)) (bvadd s0 i1) s0))))
(push 1)
(assert (= s1 goal))
(set-info :status unsat)
(check-sat)
(pop 1)

(assert (bvule i2 lim))
(assert (= s2 (ite (= r2 (_ bv1 1)) zero (ite (= e2 (_ bv1 1)) (bvadd s1 i2) s1))))
(push 1)
(assert (= s2 goal))
(set-info :status unsat)
(check-sat)
(pop 1)

(assert (bvule i3 lim))
(assert (= s3 (ite (= r3 (_ bv1 1)) zero (ite (= e3 (_ bv1 1)) (bvadd s2 i3) s2))))
(push 1)
(assert (= s3 goal))
(set-info :status unsat)
(check-sat)
(pop 1)

(assert (bvule i4 lim))
(assert (= s4 (ite (= r4 (_ bv1 1)) zero (ite (= e4 (_ bv1 1)) (bvadd s3 i4) s3))))
(push 1)
(assert (= s4 goal))
(set-info :status sat)
(check-sat)
(pop 1)

(assert (bvule i5 lim))
(assert (= s5 (ite (= r5 (_ bv1 1)) zero (ite (= e5 (_ bv1 1)) (bvadd s4 i5) s4))))
(push 1)
(assert (= s5 goal))
(set-info :status sat)
(check-sat)
(pop 1)

(assert (bvule i6 lim))
(assert (= s6 (ite (= r6 (_ bv1 1)) zero (ite (= e6 (_ bv1 1)) (bvadd s5 i6) s5))))
(push 1)
(assert (= s6 goal))
(set-info :status sat)
(check-sat)
(pop 1)

(assert (bvule i7 lim))
(assert (= s7 (ite (= r7 (_ bv1 1)) zero (ite (= e7 (_ bv1 1)) (bvadd s6 i7) s6))))
(push 1)
(assert (= s7 goal))
(set-info :status sat)
(check-sat)
(pop 1)

