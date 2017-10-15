module procon28.decoder;

import procon28.data : Pos, P, PlacedShape, Shapes;
import procon28.geometry : zoom;

import std.conv : to;
import std.json : parseJSON, JSONValue;

@system
P decode_p (in JSONValue json) {
	auto x = json.object["x"].integer.to!int;
	auto y = json.object["y"].integer.to!int;
	return P(x,y);
}

@system
P[] decode_shape (in JSONValue json) {
	Pos[] vertexies;
	foreach (pos; json.array) {
		vertexies ~= decode_p(pos);
	}
	return vertexies;
}

///jsonフォーマットの文字列をShapeにデコード
@system
P[][] decode_shapes (in JSONValue json) {
	P[][] shape;
	foreach (shape_json; json.array) {
		shape ~= decode_shape (shape_json);
	}
	return shape;
}

///jsonフォーマットの文字列をPieceの集合にデコード
@system
P[][][] decode_piece (in JSONValue json) {
	P[][][] piecies;
	foreach (piece_json; json.array) {
		piecies ~= decode_shapes(piece_json["shapes"]);
	}
	return piecies;
}

@system
Shapes parse_shapes (in string str) {
	auto json = str.parseJSON;
	auto pieces = decode_piece(json["piece"]);
	auto frame  = decode_shapes(json["frame"]);
	return Shapes(pieces, frame);
}

@system
PlacedShape decode_placedshape (in JSONValue json) {
	auto piece_idx = json["piece_id"].integer.to!ubyte;
	auto spin_level = json["shape_id"].integer.to!ubyte;
	auto p = decode_p(json["pos"]);
	return PlacedShape(p.x.to!ubyte, p.y.to!ubyte, piece_idx, spin_level);
}

@system
PlacedShape[] decode_output (in string str) {
	auto json = str.parseJSON;
	PlacedShape[] ret;
	foreach (placedshape; json["operations"].array) {
		ret ~= decode_placedshape(placedshape);
	}
	return ret;
}
