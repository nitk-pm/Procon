module procon28.accumurator;

@safe @nogc
nothrow pure float acc (alias old_ratio, alias degree) (in float old, in float now) {
	return old * old_ratio + now ^^ degree;
}
