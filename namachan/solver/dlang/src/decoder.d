module procon28.decoder;

import procon28.solver.data;
import procon28.basic_data;

import std.json : parseJSON;
import std.conv : to;

Segment[] vertexies2segments (Vector[] vertexies) {
	Segment[] segments;
	foreach (idx, vertex; vertexies[0..$-1]) {
		segments ~= Segment (vertex, vertexies[idx+1]);
	}
	return segments;
}

Shape decode_frame (in string str) {
	auto json = str.parseJSON;
	Vector[] vertexies;
	foreach (pair; json["frame"].object["shape"].array) {
		auto x = pair.array[0].integer.to!int;
		auto y = pair.array[1].integer.to!int;
		vertexies ~= Vector(x, y);
	}
	return vertexies2segments (vertexies);
}

Piece[] decode_piece (in string str) {
	auto json = str.parseJSON;

	Piece[] piecies;
	foreach (piece_json; json["piece"].array) {
		Piece piece;
		foreach (shape_json; piece_json.array) {
			Vector[] vertexies;
			foreach (pair; shape_json.array) {
				auto x = pair.array[0].integer.to!int;
				auto y = pair.array[1].integer.to!int;
				vertexies ~= Vector(x, y);
			}
			auto shape = vertexies2segments (vertexies);
			piece ~= shape;
		}
	}
	return piecies;
}
