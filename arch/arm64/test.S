main:
	mov x4, #0x4000
	mov x1, #65
	/*
	 * welcomm to univmm!
	 * 119, 101, 108, 99, 111, 109, 109, 32, 116, 111, 32, 117, 110, 105, 118, 109, 109, 33,
	 */
	mov x2, #119
	str x2, [x4]
	mov x2, #101
	str x2, [x4]
	mov x2, #108
	str x2, [x4]
	mov x2, #99
	str x2, [x4]
	mov x2, #111
	str x2, [x4]
	mov x2, #109
	str x2, [x4]
	mov x2, #109
	str x2, [x4]
	mov x2, #32
	str x2, [x4]
	mov x2, #116
	str x2, [x4]
	mov x2, #111
	str x2, [x4]
	mov x2, #32
	str x2, [x4]
	mov x2, #117
	str x2, [x4]
	mov x2, #110
	str x2, [x4]
	mov x2, #105
	str x2, [x4]
	mov x2, #118
	str x2, [x4]
	mov x2, #109
	str x2, [x4]
	mov x2, #109
	str x2, [x4]
	mov x2, #33
	str x2, [x4]
	mov x2, #10
	str x2, [x4]
	mov x2, #7
	str x2, [x4]
	hlt 0
