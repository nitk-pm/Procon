module procon28.decoder;

import armos.math.vector;

import procon28.basic_data : Segment;
import procon28.solver.data : Piece, Shape, vertexies2shape;

import std.conv : to;
import std.json : parseJSON;

///jsonフォーマットの文字列をShapeにデコード
Shape decode_frame (in string str) {
	auto json = str.parseJSON;
	Vector2i[] vertexies;
	foreach (pair; json["frame"].object["shape"].array) {
		auto x = pair.array[0].integer.to!int;
		auto y = pair.array[1].integer.to!int;
		vertexies ~= Vector2i(x, y);
	}
	return vertexies2shape (vertexies);
}

///jsonフォーマットの文字列をPieceの集合にデコード
Piece[] decode_piece (in string str) {
	auto json = str.parseJSON;

	Piece[] piecies;
	foreach (piece_json; json["piece"].array) {
		foreach (shape_json; piece_json.array) {
			Piece piece;
			Vector2i[] vertexies;
			foreach (pair; shape_json.array) {
				auto x = pair.array[0].integer.to!int;
				auto y = pair.array[1].integer.to!int;
				vertexies ~= Vector2i(x, y);
			}
			auto shape = vertexies2shape (vertexies);
			piece ~= shape;
		}
	}
	return piecies;
}
