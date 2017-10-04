module procon28.util;

auto qsort (alias comp, T) (in T[] ary) {
	if (ary.length < 2) return ary;
	auto pivot = ary[0];
	const(T)[] small, large;
	foreach (e; ary[1..$]) {
		if (comp (pivot, e))
			large ~= e;
		else
			small ~= e;
	}
	if (small.length == 0)
		return pivot ~ qsort!comp (large);
	if (large.length == 0)
		return qsort!comp(small) ~ pivot;
	return qsort!comp (small ~ pivot) ~ qsort!comp(large);
}
unittest {
	assert ([5,2,1,4,3].qsort!((a,b) => a < b) == [1,2,3,4,5]);
	assert ([5,2,1,4,3, 2, 2].qsort!((a,b) => a < b) == [1,2,2,2,3,4,5]);
	assert ([0,0,0,0,0].qsort!((a,b) => a < b) == [0,0,0,0,0]);
}
