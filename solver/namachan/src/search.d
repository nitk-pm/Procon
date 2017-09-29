module procon28.search;

import procon28.data : P, BitField, Situation, PlacedShape, Pos;
import procon28.geometry: merge, move;
import procon28.eval : eval_basic;

import core.time : TickDuration;

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
pure string show_duration (in TickDuration dur) {
	import std.format : format;
	return format("%4s s %4s msecs", dur.msecs / 1000, dur.msecs % 1000);
}

@safe
const(Situation) beam_search(alias EvalFunc)(P[][][] pieces, P[][] frames, int beam_width, int target_time) {
	import std.datetime : StopWatch;
	import std.stdio : writefln;
	BitField!128 mask_base;
	foreach (idx; pieces.length..128)
		mask_base[idx] = true;
	const(Situation)[] sorted = [Situation([], mask_base, frames, 1.0f)];
	const(Situation)[] best;
	int total_width;
	TickDuration total_time;
	size_t piece_cnt;
	//sec -> msec conversion
	target_time *= 1000;

	for (;;) {
		++piece_cnt;
		StopWatch sw;
		sw.start;
		const(Situation)[] evaled;
		foreach (i,situation; sorted) {
			evaled ~= eval_all!EvalFunc(pieces, situation);
		}
		sw.stop;
		total_time += sw.peek;
		writefln ("%2s/%s | %4s 探索幅 | %6s 盤面数 |  %s 計算時間 | %s 計",
			piece_cnt,
			pieces.length,
			beam_width,
			evaled.length,
			sw.peek.show_duration,
			total_time.show_duration);
		//次のビーム幅の計算
		if (target_time > 0) {
			total_width += beam_width;
			auto width_per_time = cast(float)total_width / cast(float)total_time.msecs;
			beam_width = cast(int)(width_per_time * (target_time - total_time.msecs));
			if (beam_width < 1)
				beam_width = 1;
		}
		
		sorted = evaled.sort!((a,b) => a.val > b.val);
		if (sorted.length > beam_width)
			sorted = sorted[0..beam_width];
		foreach (procedure; sorted) {
			if (procedure.used_pieces.all)
				return procedure;
		}
		if (sorted.length == 0) {
			writeln("Warning : 完全解未発見");
			return best[0];
		}
		else
			best = sorted[0..1];
	}
}

unittest {
	auto p1 = [[P(0,0), P(20,0), P(0,20)]];
	auto p2 = [[P(20,0), P(20,20), P(0,20)]];
	auto p3 = [[P(0,0), P (20, 0), P(20,20),P(0,20)]];
	auto frames = [[P(0,0), P(20,0), P(20,40), P(0,40)]];
	auto ops = beam_search!eval_basic([p1, p2, p3], frames, 1, -1);
	assert (ops.shapes.length == 3);
}
