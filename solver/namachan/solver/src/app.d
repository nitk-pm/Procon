module procon28.app;

import std.getopt : getopt;
import std.algorithm.iteration : splitter;
import std.file : readText;

import procon28.decoder;
import procon28.encoder;
import procon28.search;
import procon28.eval;
import procon28.trigger;
import procon28.accumurator;
import procon28.cmdopt : Option, parse_arg;

import std.stdio : stderr, writeln, File, write;
import std.meta : AliasSeq;
import core.thread : exit;

alias simple = AliasSeq!(
	acc!(0.5f, 2),
	3,
	//頂点衝突 1乗, 1倍
	point_conflict, always, 1, 1.0f
);

alias model_jinriki = AliasSeq!(
	acc!(0.5f, 2),
	3,
	point_conflict,               always, 2,  10.0f,
	consective_point_conflict!2 , always, 1,  5.0f,
	tooshort_segments!(3.0, 2.0), always, 1, -10.0f,
	area_size,                    always, 1, 0.1f,
);

void main(string[] args){
	auto opt = args[1..$].parse_arg;
	auto situation = beam_search!model_jinriki
		(
			opt.piece_name.readText.decode_piece,
			opt.frame_name.readText.decode_frame,
			opt.beam_width, opt.time_limit,
			opt.enable_parallel
		);
	auto output_file = File ("output.json", "w");
	output_file.write(situation.situation_pp);
}
