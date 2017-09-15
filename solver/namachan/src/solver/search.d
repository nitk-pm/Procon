module procon28.solver.search;

import procon28.basic_data : P, BitField, Situation, PlacedShape, Pos;
import procon28.solver.datamanip : merge, move;
import procon28.solver.eval : eval_basic;

import std.algorithm.iteration : map;
import std.conv;
import std.range : array;
import std.stdio;
import std.array : join;

/++
 + なぜphobosのソートアルゴリズムは破壊的なのか。
 + その謎を探るため我々は南米の奥地へ飛んだ...
 +/
@safe
nothrow pure auto sort(alias compare, T)(inout T[] ops) {
	import std.traits;
	auto merge (inout T[] ops1, inout T[] ops2) {
		inout(T)[] merged;
		size_t ops1_idx, ops2_idx;
		for (;ops1_idx < ops1.length || ops2_idx < ops2.length;) {
			if (ops2_idx >= ops2.length || (ops1_idx < ops1.length && compare(ops1[ops1_idx], ops2[ops2_idx]))) {
				merged ~= ops1[ops1_idx];
				++ops1_idx;
			}
			else {
				merged ~= ops2[ops2_idx];
				++ops2_idx;
			}
		}
		return merged;
	}
	if (ops.length < 2)
		return ops;
	if (ops.length == 2) {
		if (compare(ops[0], ops[1]))
			return ops;
		else
			return [ops[1], ops[0]];
	}
	auto center_idx = ops.length / 2;
	return merge (sort!compare(ops[0..center_idx]), sort!compare(ops[center_idx..$]));
}

@safe
pure const(Situation)[] eval_all(alias EvalFunc)(in P[][][] pieces,in Situation acc) {
	const(Situation)[] situaions; 
	foreach (piece_idx, piece; pieces) {
		if (acc.used_pieces[cast(int)piece_idx]) continue;
		BitField!128 new_mask = acc.used_pieces;
		new_mask[piece_idx] = true;
		foreach (spin_level, shape; piece) {
			foreach (piece_vertex; shape) {
				foreach (frame_idx, frame; acc.frames) {
					foreach (frame_vertex; frame) {
						auto diff = frame_vertex - piece_vertex;
						auto moved = shape.move(diff);
						auto reply = EvalFunc (frame, moved);
						if (reply[0] == -float.infinity) continue;
						auto merged_frames = reply[1] ~ acc.frames[0..frame_idx] ~ acc.frames[frame_idx+1..$];
						auto placed_shape = PlacedShape (diff.x.to!byte, diff.y.to!byte, piece_idx.to!ubyte, spin_level.to!ubyte);
						situaions ~=
							const(Situation) (acc.shapes ~ placed_shape, new_mask, merged_frames, reply[0]);
					}
				}
			}
		}
	}
	return situaions;
}

@safe
pure const(Situation) beam_search(alias EvalFunc)(P[][][] pieces, P[][] frames, size_t beam_width) {
	BitField!128 mask_base;
	foreach (idx; pieces.length..128)
		mask_base[idx] = true;
	auto sorted = eval_all!EvalFunc(pieces, Situation([], mask_base, frames, 1.0f))
		.sort!((a, b) => a.val > b.val)
		.array;
	for (;;) {
		if (sorted.length > beam_width)
			sorted = sorted[0..beam_width];
		foreach (procedure; sorted) {
			if (procedure.used_pieces.all)
				return procedure;
		}
		const(Situation)[] evaled;
		foreach (situation; sorted) {
			evaled ~= eval_all!EvalFunc(pieces, situation);
		}
		sorted = evaled.sort!((a,b) => a.val > b.val);
		if (sorted.length == 0) throw new Exception("could'nt solve.");
	}
}

unittest {
	auto p1 = [[P(0,0), P(20,0), P(0,20)]];
	auto p2 = [[P(20,0), P(20,20), P(0,20)]];
	auto p3 = [[P(0,0), P (20, 0), P(20,20),P(0,20)]];
	auto frames = [[P(0,0), P(20,0), P(20,40), P(0,40)]];
	auto ops = beam_search!eval_basic([p1, p2, p3], frames, 1);
	assert (ops.shapes.length == 3);
}
