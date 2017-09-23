module procon28.app;

import std.getopt : getopt;
import std.algorithm.iteration : splitter;
import std.file : readText;

import procon28.decoder;
import procon28.encoder;
import procon28.solver.search;
import procon28.solver.eval;

import std.stdio : stderr, writeln, File, write;
import core.thread : exit;

void main(string[] args){
	string piece_file_name = "";
	string frame_file_name = "";
	int width = 100;
	int target_time = -1;
	args.getopt ("p", &piece_file_name, "f", &frame_file_name, "w", &width, "t", &target_time);
	if (frame_file_name == "") {
		stderr.writeln("Error : フレームファイルを指定してください");
		exit(-1);
	}
	auto frame = frame_file_name.readText.decode_frame;
	if (piece_file_name == "") {
		stderr.writeln("Error : ピースファイルを指定してください");
		exit(-1);
	}
	auto piece = piece_file_name.readText.decode_piece;
	if (width == -1 && target_time == -1) {
		stderr.writeln("Error : 目標時間かビーム幅のどちらかを指定してください");
		exit(-1);
	}
	auto situation = beam_search!simple_is_best(piece, frame, width, target_time);
	auto output_file = File ("output.json", "w");
	output_file.write(situation.situation_pp);
}
