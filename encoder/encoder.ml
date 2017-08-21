exception Parse_error of string

type pos_t = int * int

let split_piece str =
	str
	|> Core.String.split ~on:':'

let ints_of_string str = str
	|> Core.String.split ~on:' '
	|> Core.List.filter ~f:(fun x -> (Core.String.length x) != 0)
	|> Core.List.map ~f:(fun x -> int_of_string x)

let rec parse_piece = function
	| [] -> []
	| x :: y :: rest ->
		(x, y) :: parse_piece rest
	| _ :: [] ->
		raise (Parse_error "number of number is even!")

let matrix_mul_2x2_2x1 m2x2 m2x1 = match (m2x2, m2x1) with
	((a11, a12, a21, a22), (b11, b21)) ->
		(a11 * b11 + a12 * b21, a21 * b11 + a22 * b21)

let gen_matrix4rotate angle = (
		cos angle |> int_of_float, -. sin angle |> int_of_float,
		sin angle |> int_of_float,    cos angle |> int_of_float
	)

let pi = 4.0 *. atan 1.0;;

let rotate_90  = matrix_mul_2x2_2x1 (gen_matrix4rotate (pi *. 0.5))
let rotate_180 = matrix_mul_2x2_2x1 (gen_matrix4rotate (pi *. 1.0))
let rotate_270 = matrix_mul_2x2_2x1 (gen_matrix4rotate (pi *. 1.5))
let turn_over  = matrix_mul_2x2_2x1 (-1, 0, 0, 1)

let add_turn_over pos_list =
	[pos_list; Core.List.map pos_list turn_over]

let add_rotated pos_list = [
		pos_list;
		Core.List.map pos_list rotate_90;
		Core.List.map pos_list rotate_180;
		Core.List.map pos_list rotate_270;
	]

let add_patterns pos_list =
	pos_list
	|> add_turn_over
	|> Core.List.map ~f:add_rotated
	|> Core.List.join

let construct_shape_json shape =
	`List (
		shape
		|> Core.List.map ~f:(function
			(x, y) -> `Assoc [("x", `Int x); ("y", `Int y)])
	)

let construct_shapes_json shapes =
	`Assoc [
		("shapes", `List (
			shapes |> Core.List.map ~f:construct_shape_json
		))
	]

let construct_piece_json pieces =
	`Assoc [
		("pieces", `List (
			pieces
			|> Core.List.map ~f:construct_shapes_json
		))
	]

let construct_frame_json = construct_shapes_json

let output_pieces pieces =
	let piece_channel = open_out "piece.json" in
	pieces
	|> Core.List.map ~f:parse_piece
	|> Core.List.map ~f:add_patterns
	|> construct_piece_json
	|> Yojson.to_string
	|> output_string piece_channel

let output_frame frame =
	let frame_channel = open_out "frame.json" in
	frame
	|> Core.List.map ~f:parse_piece
	|> construct_frame_json
	|> Yojson.to_string
	|> output_string frame_channel

let rec read_all () =
	let splited_input =
	read_line ()
	|> split_piece
	|> Core.List.map ~f:ints_of_string
	in match splited_input with
	| [pieces_num] :: shapes ->
		let shape_length = Core.List.length shapes in
		let pieces = Core.List.slice shapes 0 (pieces_num - 1) in
		let frames = Core.List.slice shapes pieces_num shape_length in
		Printf.printf "%d %d\n" (Core.List.length pieces) (Core.List.length frames);
		begin try
			let (pieces_next, frames_next) = read_all () in
			(pieces_next @ pieces, frames_next @ frames)
		with
		| End_of_file ->
			(pieces, shapes)
		end
	| _ ->
		raise (Parse_error "shortage of shape info")

let main () =
	let (pieces, frames) = read_all () in
	pieces |> output_pieces;
	frames |> output_frame
	
let () = main ()
