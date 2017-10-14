import geom;
import std.algorithm.comparison;
import std.algorithm.sorting;

struct Point {
	int x, y;
	real diff;
}

@safe
pure set_origin (in Vec[] shape, in Vec orig) {
	auto ret = shape.dup;
	foreach (ref p; ret) {
		p = p - orig;
	}
	return ret;
}

Vec[] rotate (in Vec[] shape, in Matrix4 mat) {
	auto ret = new Vec[shape.length];
	foreach (i, p; shape) {
		ret[i] = mat * p;
	}
	return ret;
}

real eval(in Vec[] shape) {
	import std.math;
	real score = 0.0;
	foreach (p; shape) {
		score += (p.x - round(p.x)) ^^ 2;
		score += (p.y - round(p.y)) ^^ 2;
	}
	return score;
}

struct Result {
	real score;
	Vec[] shape;
}

Result[] search(in Vec[] shape) {
	auto normalized = shape.set_origin(shape[0]);
	auto sample = shape[1];
	auto len = shape[1].norm2;
	Point[] candidates;
	foreach (x; 0..(cast(int)len + 1)) {
		foreach (y; 0..(cast(int)len + 1)) {
			auto diff = Vec(x, y).norm2 - len;
			if (diff < 1.0)
				candidates ~= Point(x, y, diff);
		}
	}
	Result[] results;
	foreach (cand; candidates) {
		auto rotate_mat = gen_rotate_mat (Vec(cand.x, cand.y), sample);
		auto rotated    = normalized.rotate(rotate_mat);
		results ~= Result(eval(rotated), rotated);
	}
	results.sort!((a, b) => a.score < b.score);
	return results;
}
