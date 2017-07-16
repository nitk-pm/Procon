module procon28.app;

import std.getopt : getopt;
import std.algorithm.iteration : splitter;
import std.file : readText;

import procon28.decoder;

void main(string[] args){
	string piece_file_name = "";
	string frame_file_name = "";
	args.getopt ("p", &piece_file_name, "f", &frame_file_name);
	if (piece_file_name == "" || frame_file_name == "") throw new Exception ("solver need piece and frame defination file");
	auto frame = frame_file_name.readText.decode_frame;
	auto piece = piece_file_name.readText.decode_piece;
}
