module procon28.encoder;

import std.json : JSONValue;
import procon28.data : Situation, P, PlacedShape;

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
	json["pos"] = pos2json(P(shape.x, shape.y));
	return json;
}

@safe
pure JSONValue construct_json (in Situation situation) {
	import std.algorithm.iteration : map;
	import std.range : array;
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
