import std.stdio;
import std.conv : to;
import std.file;
import procon28.decoder;
import procon28.encoder;
import procon28.geometry;
import std.algorithm.searching;
import std.algorithm.iteration;

void main(string[] args) {
	auto output = args[1].readText.decode_output;
	auto piece  = args[2].readText.decode_piece;
	auto new_piece = args[3];
	auto merge_ids = args[4..$].to!(int[]);
	P[] merged;
	auto piece_id = output[merge_ids[0]].piece_idx;
	foreach (id; merge_ids) {
		writeln(output[id].piece_idx);
		merged = merge_piece(piece[output[id].piece_idx][output[id].spin_level].move(output[id].x, output[id].y), merged)[0];
	}
	P[][] spin_all = [merged];
	foreach (_;0..7) {
		merged = rotate90(merged);
		spin_all ~= merged;
	}
	P[][][] replaced ;
	foreach(i, p; piece) {
		if (i == piece_id)
			replaced ~= spin_all;
		else if (merge_ids[1..$].map!(a => output[a].piece_idx).any!(a => a == i))
			replaced ~= [[]];
		else
			replaced ~= p;
	}
	auto output_file = File (new_piece, "w");
	output_file.write(replaced.pieces_pp);
}
