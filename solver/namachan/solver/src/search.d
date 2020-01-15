module procon28.search;

import procon28.data : P, BitField, Situation, PlacedShape, Pos, Shapes;
import procon28.geometry: merge, move, zoom;
import procon28.eval;
import procon28.accumurator : acc;
import procon28.util : qsort;

import core.time : TickDuration;

import std.algorithm.iteration : map;
import std.conv;
import std.range : array;
import std.stdio;
import std.array : join;

version(unittest) {
	enum ENABLE_PARALLEL = false;
}
else version(LDC) {
	enum ENABLE_PARALLEL = false;
}
else {
	enum ENABLE_PARALLEL = true;
	import std.parallelism : TaskPool;
}

@safe
const(Situation)[] eval_all(alias Accumurator, alias Pruning_Level, Param...)(in int times, in P[][][] pieces,in Situation acc) {
	const(Situation)[] situaions; 
	foreach (piece_idx, piece; pieces) {
		if (acc.used_pieces[cast(int)piece_idx]) continue;
		BitField!128 new_mask = acc.used_pieces;
		new_mask[piece_idx] = true;
		foreach (spin_level, shape; piece) {
			const(Situation)[] buf;
			foreach (piece_vertex; shape) {
				foreach (frame_idx, frame; acc.frames) {
					foreach (frame_vertex; frame) {
						auto diff = frame_vertex - piece_vertex;
						auto moved = shape.move(diff);
						auto reply = eval!Param(times, frame, moved);
						if (reply[0] == -float.infinity) continue;
						auto merged_frames = reply[1] ~ acc.frames[0..frame_idx] ~ acc.frames[frame_idx+1..$];
						auto placed_shape = PlacedShape (diff.x.to!ubyte/2, diff.y.to!ubyte/2, piece_idx.to!ubyte, spin_level.to!ubyte);
						buf ~=
							const(Situation) (acc.shapes ~ placed_shape, new_mask, merged_frames, Accumurator(acc.val, reply[0]));
					}
				}
			}
			auto sorted = buf.qsort!((a,b) => a.val > b.val);
			situaions ~= sorted.length > Pruning_Level ? sorted[0..Pruning_Level] : sorted;
		}
	}
	return situaions;
}

@safe
pure string show_duration (in TickDuration dur) {
	import std.format : format;
	return format("%4s s %4s msecs", dur.msecs / 1000, dur.msecs % 1000);
}

@trusted
const(Situation) beam_search (alias Accumurator, alias Pruning_Level, Param...)
	(in Shapes shapes, in int beam_width, in int target_time, in bool parallel) {
	auto pieces = shapes.pieces.map!(piece => piece.map!(shape => shape.zoom(2)).array).array;
	auto frames = shapes.frames.map!(shape => shape.zoom(2)).array;
	import std.datetime : StopWatch;
	import std.stdio : writefln;
	BitField!128 mask_base;
	foreach (idx; pieces.length..128)
		mask_base[idx] = true;
	const(Situation)[] sorted = [const(Situation)([], mask_base, frames, 1.0f)];
	const(Situation)[] best;
	int total_width;
	TickDuration total_time;
	int piece_cnt;
	//sec -> msec conversion
	immutable target_time_msecs = target_time * 1000;
	static if (ENABLE_PARALLEL) {
		auto pool = new TaskPool;
		scope(exit)
			pool.finish;
	}
	int next_width = beam_width;
	for (;;) {
		++piece_cnt;
		StopWatch sw;
		sw.start;
		const(Situation)[] evaled;
		static if (ENABLE_PARALLEL) {
			if (parallel) {
				foreach (situation; pool.parallel(sorted, 1)) {
					evaled ~= eval_all!(Accumurator, Pruning_Level, Param)(piece_cnt, pieces, situation);
				}
			}
			else {
				foreach (situation; sorted) {
					evaled ~= eval_all!(Accumurator, Pruning_Level, Param)(piece_cnt, pieces, situation);
				}
			}
		}
		else {
			foreach (situation; sorted) {
				evaled ~= eval_all!(Accumurator, Pruning_Level, Param)(piece_cnt, pieces, situation);
			}
		}
		sw.stop;
		total_time += sw.peek;
		writefln ("%2s/%s | %4s 探索幅 | %6s 盤面数 |  %s 計算時間 | %s 計",
			piece_cnt,
			pieces.length,
			next_width,
			evaled.length,
			sw.peek.show_duration,
			total_time.show_duration);
		//次のビーム幅の計算
		if (target_time_msecs > 0) {
			total_width += next_width;
			auto width_per_time = cast(float)total_width / cast(float)total_time.msecs;
			next_width = cast(int)(width_per_time * (target_time_msecs - total_time.msecs));
			if (next_width < 1)
				next_width = 1;
		}
		
		sorted = evaled.qsort!((a,b) => a.val > b.val);
		if (sorted.length > next_width)
			sorted = sorted[0..next_width];
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
	auto ops = beam_search!(acc!(0.0, 1), 3, simple_is_best)([p1, p2, p3], frames, 1, -1, false);
	assert (ops.shapes.length == 3);
}
