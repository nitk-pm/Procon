module procon28.encoder;

import std.json : JSONValue;
import procon28.data : Situation, P, PlacedShape;
import std.algorithm.iteration : map;
import std.range : array;

@safe
pure JSONValue pos2json (in P p) {
	JSONValue json;
	json["x"] = p.x;
	json["y"] = p.y;
	return json;
}

@safe
pure JSONValue placed_shape2json (in PlacedShape shape) {
	JSONValue json;
	json["piece_id"] = shape.piece_idx;
	json["shape_id"] = shape.spin_level;
	//デコード時に2倍してたので1/2にする
	json["pos"] = pos2json(P(shape.x, shape.y)/2);
	return json;
}

@safe
pure JSONValue construct_json (in Situation situation) {
	JSONValue json;
	json["operations"] =
		situation
		.shapes
		.map!(shape => placed_shape2json(shape))
		.array;
	return json;
}

@safe
string situation_pp (in Situation situation) {
	return situation.construct_json.toPrettyString;
}

@safe
pure JSONValue[] construct_shape (in P[] shape) {
	return shape.map!(a => a.pos2json).array;
}

@safe
pure JSONValue construct_shapes (in P[][] shapes) {
	JSONValue json;
	json["shapes"] = shapes.map!(a => a.construct_shape).array;
	return json;
}

@safe
pure JSONValue construct_piece (in P[][][] pieces) {
	JSONValue json;
	json["pieces"] = pieces.map!(a => a.construct_shapes).array;
	return json;
}

@safe
string pieces_pp(in P[][][] pieces){
	return construct_piece(pieces).toPrettyString;
}
