module procon28.decoder;

import procon28.solver.data;
import procon28.basic_data;

import std.json : parseJSON;
import std.conv : to;

import armos.math.vector;

Segment[] vertexies2segments (Vector2i[] vertexies) {
	Segment[] segments;
	foreach (idx, vertex; vertexies[0..$-1]) {
		segments ~= Segment (vertex, vertexies[idx+1]);
	}
	return segments;
}

///jsonフォーマットの文字列をShapeにデコード
Shape decode_frame (in string str) {
	auto json = str.parseJSON;
	Vector2i[] vertexies;
	foreach (pair; json["frame"].object["shape"].array) {
		auto x = pair.array[0].integer.to!int;
		auto y = pair.array[1].integer.to!int;
		vertexies ~= Vector2i(x, y);
	}
	return vertexies2segments (vertexies);
}

///jsonフォーマットの文字列をPieceの集合にデコード
Piece[] decode_piece (in string str) {
	auto json = str.parseJSON;

	Piece[] piecies;
	foreach (piece_json; json["piece"].array) {
		Piece piece;
		foreach (shape_json; piece_json.array) {
			Vector2i[] vertexies;
			foreach (pair; shape_json.array) {
				auto x = pair.array[0].integer.to!int;
				auto y = pair.array[1].integer.to!int;
				vertexies ~= Vector2i(x, y);
			}
			auto shape = vertexies2segments (vertexies);
			piece ~= shape;
		}
	}
	return piecies;
}
