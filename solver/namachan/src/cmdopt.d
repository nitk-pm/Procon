module procon28.cmdopt;

import std.exception;
import std.format : format;
import std.algorithm.searching : any;
import std.conv : to, ConvException;

class ArgException : Exception {
	this (in string msg, in string file, in size_t line) {
		super(msg, file, line);
	}
}

struct Option {
	immutable int beam_width;
	immutable int time_limit;
	immutable string frame_name;
	immutable string piece_name;
	immutable bool enable_parallel;
}

T get_value (T)(in string[] args, in size_t pos, in string type_description) {
	if (pos + 1 >= args.length)
		throw new ArgException (format("%sは%sの値が必要です", args[pos], type_description), __FILE__, __LINE__);
	T val;
	try {
		val = args[pos+1].to!T;
	}
	catch (ConvException e) {
		throw new ArgException(format("%sは%sの値が必要です", args[pos], type_description), __FILE__, __LINE__);
	}
	return val;
}
unittest {
	assert (get_value!int(["-p", "124", "-f"], 0, "") == 124);
	assert (get_value!int(["--enable_parallel", "-p", "124", "-f"], 1, "") == 124);
	assertThrown(get_value!int(["-p", "124", "-f"], 1, ""));
	assertThrown(get_value!int(["-p", "-f"], 0, ""));
	assertThrown(get_value!int(["-p"], 0, ""));
}

Option parse_arg (in string[] args) {
	string piece_name = "piece.json", frame_name = "frame.json";
	int time = -1, width = -1;
	bool enable_parallel;
	for (size_t n; n < args.length;) {
		if (args[n] == "-p" || args[n] == "--piece") {
			piece_name = args.get_value!string(n, "string");
			n += 2;
		}
		else if (args[n] == "-f" || args[n] == "--frame") {
			frame_name = args.get_value!string(n, "string");
			n += 2;
		}
		else if (args[n] == "-w" || args[n] == "--width") {
			if (time >= 0)
				throw new ArgException("ビーム幅と制限時間は同時に指定することは出来ません", __FILE__, __LINE__);
			width = args.get_value!int(n, "interger");
			n += 2;
		}
		else if (args[n] == "-t" || args[n] == "--time") {
			if (width >= 0)
				throw new ArgException("ビーム幅と制限時間は同時に指定することは出来ません", __FILE__, __LINE__);
			time = args.get_value!int(n, "interger");
			n += 2;
		}
		else if (args[n] == "--parallel") {
			enable_parallel = true;
			n += 1;
		}
		else {
			if (args[n].length > 0 && args[n][0] == '-')
				throw new ArgException(format("サポートしてないフラグ : %s", args[n]), __FILE__, __LINE__);
			throw new ArgException(format("予期しない値 : %s", args[n]), __FILE__, __LINE__);
		}
	}
	if (width < 0) width = 1;
	return Option(width, time, frame_name, piece_name, enable_parallel);
}
unittest {
	assert (["-p", "p.json", "--frame", "f.json", "--enable_parallel"].parse_arg == Option (1, -1, "f.json", "p.json", true));
}
