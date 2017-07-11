module procon28.solver.data;

import procon28.basic_data;

alias Point = Vector;

alias Shape = Segment[];
alias Piece = Shape[];

Shape merge (Shape x, Shape y) {
	auto x_hit = new bool[x.length];
	auto y_hit = new bool[y.length];
	Shape buf;
	for (size_t i; i < x.length; ++i) {
		for (size_t j = i; j < y.length; ++j) {
			if (x[i].addable (y[j])) {
				buf ~= x[i].split(y[j]);
				x_hit[i] = true;
				y_hit[j] = true;
			}
		}
	}
	foreach (idx, seg; x) {
		if (!x_hit[idx]) buf ~= seg;
	}
	foreach (idx, seg; y) {
		if (!y_hit[idx]) buf ~= seg;
	}

	int[Segment] cnt;
	foreach (seg; buf)
		++cnt[seg];
	Shape result;
	foreach (seg; cnt.keys) {
		if (cnt[seg] < 2)
			result ~= seg;
	}
	return result;
}
