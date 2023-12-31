(set-logic QF_FPLRA)
(set-info :status sat)
(declare-const x Float16)
(declare-const y Float16)
(assert (= (fp.add RNA x y) ((_ to_fp 5 11) RNA (/ 1 1000000))))
(assert (distinct (fp.sub RNA x y) ((_ to_fp 5 11) RNE (/ 1.2 5))))
(assert (distinct (fp.div RNA x y) ((_ to_fp 5 11) RNA (/ 1.2 3.25))))
(check-sat)
