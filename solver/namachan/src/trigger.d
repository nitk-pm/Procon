module procon28.trigger;

import procon28.data : P;

@safe @nogc
pure nothrow bool always (in int times, in P[] frame, in P[] piece, in P[][] merged) {
	return true;
}

@safe @nogc
pure nothrow bool n_placed (alias N)(in int times, in P[] frame, in P[] piece, in P[][] merged) {
	return times >= N;
}

@safe @nogc
pure nothrow bool splited_frame (alias N)(in int times, in P[] frame, in P[] piece, in P[][] merged) {
	return merged.frame >= N;
}

//F : trigger
//G : trigger
@safe @nogc
pure nothrow bool and (alias F, alias G)(in int times, in P[] frame, in P[] piece, in P[][] merged) {
	return F(times, frame, piece, merged) && G(times, frame, piece, merged);
}

//F : trigger
//G : trigger
@safe @nogc
pure nothrow bool or (alias F, alias G)(in int times, in P[] frame, in P[] piece, in P[][] merged) {
	return F(times, frame, piece, merged) || G(times, frame, piece, merged);
}

//F : eval
@safe @nogc
pure nothrow bool more (alias F, alias Val)(in int times, in P[] frame, in P[] piece, in P[][] merged) {
	return F(frame, piece, merged) >= Val;
}

@safe @nogc
pure nothrow bool not (alias F)(in int times, in P[] frame, in P[] piece, in P[][] merged) {
	return !F(times, frame, piece, merged);
}
