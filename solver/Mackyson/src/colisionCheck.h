#pragma once
#include "data.h"

bool isInFrame (Piece p, std::vector<Frame> frameList) {

	std::vector<std::vector<int>> wn (frameList.size (), std::vector<int> (p.vertexPositionList.size ()));


	//widing number algorithmによる全点の内外判定(判定は後)

	for (int i = 0; i < frameList.size (); ++i)
		for (int j = 0; j < p.vertexPositionList.size () - 1; ++j)
			wn[i][j] = 0;

	for (int i = 0; i < frameList.size (); ++i) {//枠の数に対応するループ
		for (int j = 0; j < p.vertexPositionList.size (); ++j) {//ピースの頂点数に対応するループ
																//std::cout << p.vertexPositionList.at(i).x << "," << p.vertexPositionList.at(i).y << std::endl;
			for (int k = 0; k < frameList.at (i).vertexPositionList.size () - 1; ++k) {//currentな枠の頂点数に対応するループ
				double vt = (double)(p.vertexPositionList.at (j).y - frameList.at (i).vertexPositionList.at (k).y) / (frameList.at (i).vertexPositionList.at (k + 1).y - frameList.at (i).vertexPositionList.at (k).y);//k,k+1のなす線分の傾き

				if ((frameList.at (i).vertexPositionList.at (k).y <= p.vertexPositionList.at (j).y) //始点以上終点未満のy値
					&& (frameList.at (i).vertexPositionList.at (k + 1).y > p.vertexPositionList.at (j).y)) {
					if (p.vertexPositionList.at (j).x < (frameList.at (i).vertexPositionList.at (k).x + (vt * (frameList.at (i).vertexPositionList.at (k + 1).x - frameList.at (i).vertexPositionList.at (k).x))))
					{
						wn.at (i).at (j)++;
					}
				}
				else if ((frameList.at (i).vertexPositionList.at (k).y > p.vertexPositionList.at (j).y)//あるいは終点以下始点超過のy値
					&& (frameList.at (i).vertexPositionList.at (k + 1).y <= p.vertexPositionList.at (j).y)) {
					if (p.vertexPositionList.at (j).x < (frameList.at (i).vertexPositionList.at (k).x + (vt * (frameList.at (i).vertexPositionList.at (k + 1).x - frameList.at (i).vertexPositionList.at (k).x))))
					{
						wn.at (i).at (j)--;
					}
				}
			}
		}
	}
	//線分の交差判定による内外判定
	bool tmp1 = false;
	for (int i = 0; i < p.vertexPositionList.size () - 1; ++i) {//ピースのなす線分のループ
		Position a (p.vertexPositionList.at (i).x, p.vertexPositionList.at (i).y), b (p.vertexPositionList.at (i + 1).x, p.vertexPositionList.at (i + 1).y);

		for (int j = 0; j < 1; ++j) {//初期フレームだけはWNの判定が違うので別の挙動
			for (int k = 0; k < frameList.at (j).vertexPositionList.size () - 1; ++k) {//currentなFrameのなす線分のループ
				Position c (frameList.at (j).vertexPositionList.at (k).x, frameList.at (j).vertexPositionList.at (k).y), d (frameList.at (j).vertexPositionList.at (k + 1).x, frameList.at (j).vertexPositionList.at (k + 1).y);

				int
					tc = (a.x - b.x)*(c.y - a.y) + (a.y - b.y)*(a.x - c.x),
					td = (a.x - b.x)*(d.y - a.y) + (a.y - b.y)*(a.x - d.x),
					ta = (c.x - d.x)*(a.y - c.y) + (c.y - d.y)*(c.x - a.x),
					tb = (c.x - d.x)*(b.y - c.y) + (c.y - d.y)*(c.x - b.x);

				if (ta == 0 && (((c.x <= a.x && d.x >= a.x) || (c.x >= a.x&&d.x <= a.x)) && ((c.y <= a.y && d.y >= a.y) || (c.y >= a.y&&d.y <= a.y)))) {
					wn[0][i] += 90;
				}
				tmp1 |= ta*tb < 0 && tc*td < 0;

			}
		}
	}
	if (!tmp1) {
		for (int i = 0; i < p.vertexPositionList.size () - 1; ++i) {//ピースのなす線分のループ
			Position a (p.vertexPositionList.at (i).x, p.vertexPositionList.at (i).y), b (p.vertexPositionList.at (i + 1).x, p.vertexPositionList.at (i + 1).y);

			for (int j = 1; j < frameList.size (); ++j) {//Frameの数のループ
				for (int k = 0; k < frameList.at (j).vertexPositionList.size () - 1; ++k) {//currentなFrameのなす線分のループ
					Position c (frameList.at (j).vertexPositionList.at (k).x, frameList.at (j).vertexPositionList.at (k).y), d (frameList.at (j).vertexPositionList.at (k + 1).x, frameList.at (j).vertexPositionList.at (k + 1).y);

					int
						tc = (a.x - b.x)*(c.y - a.y) + (a.y - b.y)*(a.x - c.x),
						td = (a.x - b.x)*(d.y - a.y) + (a.y - b.y)*(a.x - d.x),
						ta = (c.x - d.x)*(a.y - c.y) + (c.y - d.y)*(c.x - a.x),
						tb = (c.x - d.x)*(b.y - c.y) + (c.y - d.y)*(c.x - b.x);

					if (ta == 0 && (((c.x <= a.x && d.x >= a.x) || (c.x >= a.x&&d.x <= a.x)) && ((c.y <= a.y && d.y >= a.y) || (c.y >= a.y&&d.y <= a.y)))) {
						wn[j][i] = 0;
					}
					tmp1 |= ta*tb < 0 && tc*td < 0;

				}
			}
		}
	}
	if (!tmp1) {

		bool tmp2 = true;
		for (int j = 0; j < p.vertexPositionList.size () - 1; ++j) {
			tmp2 &= (wn[0][j] != 0);
		}
		if (tmp2)
			for (int i = 1; i < frameList.size (); ++i) {
				for (int j = 0; j < p.vertexPositionList.size (); ++j) {
					tmp2 &= (wn[i][j] == 0);
				}
			}

		return tmp2;

	}
	return false;
}

bool isInFrameBit (std::unordered_set<Position> p, Position pos, std::bitset<101 * 65> f) {

	for (auto itr = p.begin (); itr != p.end (); ++itr) {
		if (!f[(itr->y + pos.y) * 101 + itr->x + pos.x])
			return false;
	}
	return true;
}
