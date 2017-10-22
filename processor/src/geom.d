import std.math;

struct Vec {
	real x, y;
	

	@safe @nogc
	pure nothrow Vec opSub (in Vec v) const {
		return Vec (x - v.x, y - v.y);
	}
	@safe @nogc
	pure nothrow Vec opAdd (in Vec v) const {
		return Vec (x + v.x, y + v.y);
	}

	@safe @nogc
	pure nothrow real norm2() const {
		return x * x + y * y;
	}

	@safe @nogc
	pure nothrow real norm() const {
		return sqrt (norm2);
	}
	
	@safe @nogc
	pure nothrow Vec normalized () const {
		auto norm = norm();
		if (norm == 0) return Vec(0,0);
		return Vec (x/norm, y/norm);
	}
}
unittest {
	auto v1 = Vec(4, 2);
	auto v2 = Vec(-6,1);
	assert (v1 + v2 == Vec(-2, 3));
	assert (v2 - v1 == Vec(-10, -1));
	assert (v1.norm2 == 20);
}

struct Pos {
	int x, y;
}

struct Matrix4 {
	real[2][2] mat;
	Vec opMul (in Vec vec) const {
		auto x = mat[0][0] * vec.x + mat[0][1] * vec.y;
		auto y = mat[1][0] * vec.x + mat[1][1] * vec.y;
		return Vec(x, y);
	}
}
unittest {
	auto v = Vec(3,1);
	auto mat = Matrix4([
		[-3, 2],
		[ 5, 8]
	]);
	assert (mat * v == Vec (-7, 23));
}

@safe @nogc
pure nothrow Matrix4 gen_rotate_mat (in Vec now, in Vec target) {
	auto tv = target.normalized;
	auto nv = now.normalized;
	//cosの加法定理より
	auto cos = tv.x * nv.x + tv.y * nv.y;
	//sinの加法定理より
	auto sin = tv.y * nv.x - tv.x * nv.y;
	real[2][2] mat = [
		[cos, -sin],
		[sin,  cos]
	];
	return Matrix4(mat);
}
