module procon28.decoder;

import procon28.data : Pos, P;
import procon28.geometry : zoom;

import std.conv : to;
import std.json : parseJSON, JSONValue;

@system
P[] decode_shape (in JSONValue json) {
	Pos[] vertexies;
	foreach (pos; json.array) {
		auto x = pos.object["x"].integer.to!int;
		auto y = pos.object["y"].integer.to!int;
		vertexies ~= Pos(x, y);
	}
	//2倍して全ての頂点座標の値を偶数にしておくと中点をとっても必ず丁度整数になる
	return vertexies.zoom(2);
}

///jsonフォーマットの文字列をShapeにデコード
@system
P[][] decode_frame (in string str) {
	auto json = str.parseJSON;
	P[][] shape;
	foreach (shape_json; json["shapes"].array) {
		shape ~= decode_shape (shape_json);
	}
	return shape;
}

///jsonフォーマットの文字列をPieceの集合にデコード
@system
P[][][] decode_piece (in string str) {
	auto json = str.parseJSON;
	P[][][] piecies;
	foreach (piece_json; json["pieces"].array) {
		P[][] shapes;
		foreach (shape_json; piece_json["shapes"].array) {
			shapes ~= decode_shape (shape_json);
		}
		piecies ~= shapes;
	}
	return piecies;
}
