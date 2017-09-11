exception Parse_error of string

type pos_t = int * int

(*セミコロンでピース毎に文字列を分割*)
(*string -> string list*)
let split_piece str =
	str
	|> Str.split (Str.regexp ":")

(*空白区切りの文字列を整数のリストに変換*)
(*string -> int list*)
let ints_of_string str = str
	|> Str.split (Str.regexp " ")
	|> List.filter (fun x -> (String.length x) != 0)
	|> List.map (fun x -> int_of_string x)

(*xとyが交互に並んだリストからpos_tのリストに変換*)
(*int list -> pos_t list*)
let rec parse_piece = function
	| [] -> []
	| x :: y :: rest ->
		(x, y) :: parse_piece rest
	| _ :: [] ->
		raise (Parse_error "number of number is even!")

(*2x2 x 2x1の行列積を計算*)
(* int * int * int * int -> pos_t -> pos_t *)
let matrix_mul_2x2_2x1 m2x2 m2x1 = match (m2x2, m2x1) with
	((a11, a12, a21, a22), (b11, b21)) ->
		(a11 * b11 + a12 * b21, a21 * b11 + a22 * b21)

(*回転させるための行列の生成 (intなので90度刻みの回転しか出来ない*)
(* float -> int * int * int * int *)
let gen_matrix4rotate angle = (
		cos angle |> int_of_float, -. sin angle |> int_of_float,
		sin angle |> int_of_float,    cos angle |> int_of_float
	)

(*円周率の定義 (標準ライブラリに無いため)*)
let pi = 4.0 *. atan 1.0;;

(*回転後座標計算関数*)
(*pos_t -> pos_t *)
let rotate_90  = matrix_mul_2x2_2x1 (gen_matrix4rotate (pi *. 0.5))
let rotate_180 = matrix_mul_2x2_2x1 (gen_matrix4rotate (pi *. 1.0))
let rotate_270 = matrix_mul_2x2_2x1 (gen_matrix4rotate (pi *. 1.5))
(*反転後座標計算関数*)
(*pos_t -> pos_t *)
let turn_over  = matrix_mul_2x2_2x1 (-1, 0, 0, 1)

(*座標のリストを反転させる*)
(* pos_t list -> pos_t list list*)
let add_turn_over pos_list =
	(*反転させた場合、回転方向が逆になるのでリストを逆順にする*)
	[pos_list; pos_list |> List.map turn_over |> List.rev]

(*形状に90度ずつ回転したものを追加*)
(* pos_t list -> pos_t list list*)
let add_rotated pos_list = [
		pos_list;
		List.map rotate_90  pos_list;
		List.map rotate_180 pos_list;
		List.map rotate_270 pos_list;
	]

(*裏表、回転4バージョンで計8パターンの回転状態を計算*)
(*(int*int) list -> (int * int) list list*)
let add_patterns pos_list =
	pos_list
	|> add_turn_over
	|> List.map add_rotated
	|> List.flatten

(*形状を表すjson valueを構築*)
(* pos_t list -> Yojson.json*)
let construct_shape_json shape =
	`List (
		shape
		|> List.map (function
			(x, y) -> `Assoc [("x", `Int x); ("y", `Int y)])
	)

(*複数の形状を表すjson valueを構築*)
(*pos_t list list -> Yojson.json*)
let construct_shapes_json shapes =
	`Assoc [
		("shapes", `List (
			shapes |> List.map construct_shape_json
		))
	]

(*ピースを表すjson valueを構築*)
(*pos_t list list list -> Yojson.json*)
let construct_piece_json pieces =
	`Assoc [
		("pieces", `List (
			pieces
			|> List.map construct_shapes_json
		))
	]

(*x,yそれぞれの最小座標を抽出*)
(*pos_t list -> pos_t*)
let extract_min_pos shape =
	let folder p min_p = match (p, min_p) with
		((p_x, p_y), (min_p_x, min_p_y)) ->
			(min p_x min_p_x, min p_y min_p_y) in
	List.fold_left folder (max_int, max_int) shape

(*x,yの座標が必ず正になるように*)
(*pos_t list -> pos_t list*)
let normalize shape =
	let (min_x, min_y) = extract_min_pos shape in
	List.map (function (x,y) -> (x - min_x, y - min_y)) shape

(*frameを表すjson valueの構築(中身はconstruct_shapes_jsonと同じで良い)*)
(*pos_t list list -> Yojson.json*)
let construct_frame_json = construct_shapes_json

(*int list -> Yojson.json*)
let output_pieces pieces =
	let piece_channel = open_out "piece.json" in
	pieces
	|> List.map parse_piece
	|> List.map add_patterns
	|> List.map (List.map normalize)
	|> construct_piece_json
	|> Yojson.pretty_to_string
	|> output_string piece_channel

(*int list -> Yojson.json*)
let output_frame frames =
	let frame_channel = open_out "frame.json" in
	frames
	|> List.map parse_piece
	|> List.map normalize
	|> construct_frame_json
	|> Yojson.pretty_to_string
	|> output_string frame_channel

let rec slice l b e =
	let rec take n = function
		| [] -> []
		| h :: t -> if n = 0 then [] else h :: take (n - 1) t
	in
	let rec drop n = function
		| [] -> []
		| h :: t as l -> if n = 0 then l else drop (n - 1) t
	in
	drop b l |> take (e - b)

(*標準入力から読み込み,フレームのリストと形状のリストを分ける*)
(* unit -> int list list * int list list*)
let rec read_all () =
	(*標準入力から読み込み、':'を区切りに整数のリストのリストに変換*)
	let splited_input =
		read_line ()
		|> split_piece
		|> List.map ints_of_string
	in match splited_input with
	(*グリッド数情報の切り落とし*)
	| [pieces_num] :: shapes_with_grid_num ->
		let shapes = List.map List.tl shapes_with_grid_num in
		let shape_length = List.length shapes in
		let pieces = slice shapes 0 pieces_num in
		let frames = slice shapes pieces_num shape_length in
		(*まだ入力があれば次の入力の結果と結合*)
		begin try
			let (pieces_next, frames_next) = read_all () in
			(pieces_next @ pieces, frames_next @ frames)
		with
		(*入力が終わった場合はそのまま出力*)
		| End_of_file ->
			(pieces, frames)
		end
	| _ ->
		raise (Parse_error "shortage of shape info")

(*メイン関数*)
let main () =
	let (pieces, frames) = read_all () in
	pieces |> output_pieces;
	frames |> output_frame
	
(*エントリポイント*)
let () = main ()
