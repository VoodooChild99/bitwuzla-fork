(set-logic QF_AUFBV)
(declare-sort |pfcache_s| 0)
(declare-const x (_ BitVec 2))
(declare-const y (_ BitVec 2))
(define-fun |pfcache_u| ((state0 |pfcache_s|) (state1 |pfcache_s|)) Bool (distinct state0 state1))
(declare-fun s0 () |pfcache_s|)
(declare-fun s1 () |pfcache_s|)
(assert (|pfcache_u| s0 s1))
(set-info :status sat)
(check-sat)
(exit)
