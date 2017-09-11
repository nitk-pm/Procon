module procon28.app;

import std.getopt : getopt;
import std.algorithm.iteration : splitter;
import std.file : readText;

import procon28.decoder;
import procon28.solver.search;
import procon28.solver.eval;

void main(string[] args){
	string piece_file_name = "";
	string frame_file_name = "";
	int width = 100;
	args.getopt ("p", &piece_file_name, "f", &frame_file_name, "w", &width);
	if (piece_file_name == "" || frame_file_name == "") throw new Exception ("solver need piece and frame defination file");
	auto frame = frame_file_name.readText.decode_frame;
	auto piece = piece_file_name.readText.decode_piece;
	beam_search!eval_basic(piece, frame, width);
}
