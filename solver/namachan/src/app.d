module procon28.app;

import std.getopt : getopt;
import std.algorithm.iteration : splitter;
import std.file : readText;

import procon28.decoder;
import procon28.encoder;
import procon28.search;
import procon28.eval;
import procon28.cmdopt : Option, parse_arg;

import std.stdio : stderr, writeln, File, write;
import core.thread : exit;

void main(string[] args){
	auto opt = args[1..$].parse_arg;
	auto situation = beam_search!simple_is_best(opt.piece_name.readText.decode_piece, opt.frame_name.readText.decode_frame, opt.beam_width, opt.time_limit, opt.enable_parallel);
	auto output_file = File ("output.json", "w");
	output_file.write(situation.situation_pp);
}
