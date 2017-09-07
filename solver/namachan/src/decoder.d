module procon28.decoder;

import procon28.basic_data : Segment, Piece, Shape, Pos;
import procon28.solver.datamanip : vertexies2shape;

import std.conv : to;
import std.json : parseJSON, JSONValue;

@system
pure Shape decode_shape (in JSONValue json) {
	Pos[] vertexies;
	foreach (pos; json.array) {
		auto x = pos.object["x"].integer.to!int;
		auto y = pos.object["y"].integer.to!int;
		vertexies ~= Pos(x, y);
	}
	return vertexies2shape(vertexies);
}

///jsonフォーマットの文字列をShapeにデコード
@system
pure Shape decode_frame (in string str) {
	auto json = str.parseJSON;
	Shape shape;
	foreach (shape_json; json["shapes"].array) {
		shape ~= decode_shape (shape_json);
	}
	return shape;
}

///jsonフォーマットの文字列をPieceの集合にデコード
@system
pure Piece[] decode_piece (in string str) {
	auto json = str.parseJSON;
	Piece[] piecies;
	foreach (piece_json; json["pieces"].array) {
		Shape[] shapes;
		foreach (shape_json; piece_json["shapes"].array) {
			shapes ~= decode_shape (shape_json);
		}
		piecies ~= shapes;
	}
	return piecies;
}
