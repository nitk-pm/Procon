#pragma once
#include <vector>
#include <deque>
#include "data.h"

bool isInFrame(Piece p, std::vector<Frame> frameList) {
	/*/
	//�d�S��winding number
	std::vector<int> wn_(frameList.size(), 0);
	Position centerOfGravity(p.center);

	for (int i = 0; i < frameList.size(); ++i) {//�g�̐��ɑΉ����郋�[�v
	//std::cout << p.vertexPositionList.at(i).x << "," << p.vertexPositionList.at(i).y << std::endl;
	for (int k = 0; k < frameList.at(i).vertexPositionList.size() - 1; ++k) {//current�Șg�̒��_���ɑΉ����郋�[�v
	double vt = (double)(centerOfGravity.y - frameList.at(i).vertexPositionList.at(k).y) / (frameList.at(i).vertexPositionList.at(k + 1).y - frameList.at(i).vertexPositionList.at(k).y);//k,k+1�̂Ȃ������̌X��
	//std::cout << "pos: " << frameList.at(i).vertexPositionList.at(k).y << "," << frameList.at(i).vertexPositionList.at(k).x << std::endl;
	//std::cout << "pos: " << frameList.at(i).vertexPositionList.at(k + 1).y << "," << frameList.at(i).vertexPositionList.at(k + 1).x << std::endl;

	if ((frameList.at(i).vertexPositionList.at(k).y <= centerOfGravity.y) //�n�_�ȏ�I�_������y�l
	&& (frameList.at(i).vertexPositionList.at(k + 1).y > centerOfGravity.y)) {
	if (centerOfGravity.x < (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))))
	{

	//std::cout << "a " << p.vertexPositionList.at(j).x << "," << (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))) << std::endl;
	wn_.at(i)++;
	}
	}
	else if ((frameList.at(i).vertexPositionList.at(k).y > centerOfGravity.y)//���邢�͏I�_�ȉ��n�_���߂�y�l
	&& (frameList.at(i).vertexPositionList.at(k + 1).y <= centerOfGravity.y)) {
	if (centerOfGravity.x < (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))))
	{
	//std::cout << "b " << p.vertexPositionList.at(j).x << "," << (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))) << std::endl;
	wn_.at(i)--;
	}
	}
	}
	}
	bool tmp3 = true;

	tmp3 &= (wn_[0] != 0);

	for (int i = 1; i < frameList.size(); ++i)
	tmp3 &= (wn_[i] == 0);

	if (tmp3) {
	/*/
	std::vector<std::vector<int>> wn(frameList.size(), std::vector<int>(p.vertexPositionList.size()));//�ӏ�̓_�����o���邽�ߐ�ɐ錾���Ă���

																									  //�����̌�������ɂ����O����
	bool tmp1 = false;
	for (int i = 0; i < p.vertexPositionList.size() - 1; ++i) {//�s�[�X�̂Ȃ������̃��[�v
		Position a(p.vertexPositionList.at(i).x, p.vertexPositionList.at(i).y), b(p.vertexPositionList.at(i + 1).x, p.vertexPositionList.at(i + 1).y);

		for (int j = 0; j < frameList.size(); ++j) {//Frame�̐��̃��[�v
			for (int k = 0; k < frameList.at(j).vertexPositionList.size() - 1; ++k) {//current��Frame�̂Ȃ������̃��[�v
				Position c(frameList.at(j).vertexPositionList.at(k).x, frameList.at(j).vertexPositionList.at(k).y), d(frameList.at(j).vertexPositionList.at(k + 1).x, frameList.at(j).vertexPositionList.at(k + 1).y);

				int
					tc = (a.x - b.x)*(c.y - a.y) + (a.y - b.y)*(a.x - c.x),
					td = (a.x - b.x)*(d.y - a.y) + (a.y - b.y)*(a.x - d.x),
					ta = (c.x - d.x)*(a.y - c.y) + (c.y - d.y)*(c.x - a.x),
					tb = (c.x - d.x)*(b.y - c.y) + (c.y - d.y)*(c.x - b.x);

				if (ta == 0)
					if (wn[j][i]>80)
						wn[j][i] -= 99;
				else wn[j][i] += 99;
				if (tb == 0)
					if (wn[j][i+1]>80)
						wn[j][i+1] -= 99;
					else wn[j][i+1] += 99;

				tmp1 |= ta*tb < 0 && tc*td < 0;

			}
		}
	}


	//widing number algorithm�ɂ��S�_�̓��O����

	/*std::cout <<p.vertexPositionList.size();*/

	if (!tmp1) {

		for (int i = 0; i < frameList.size(); ++i)
			for (int j = 0; j < p.vertexPositionList.size() - 1; ++j)
				wn[i][j] = 0;

		for (int i = 0; i < frameList.size(); ++i) {//�g�̐��ɑΉ����郋�[�v
			for (int j = 0; j < p.vertexPositionList.size(); ++j) {//�s�[�X�̒��_���ɑΉ����郋�[�v
																   //std::cout << p.vertexPositionList.at(i).x << "," << p.vertexPositionList.at(i).y << std::endl;
				for (int k = 0; k < frameList.at(i).vertexPositionList.size() - 1; ++k) {//current�Șg�̒��_���ɑΉ����郋�[�v
					double vt = (double)(p.vertexPositionList.at(j).y - frameList.at(i).vertexPositionList.at(k).y) / (frameList.at(i).vertexPositionList.at(k + 1).y - frameList.at(i).vertexPositionList.at(k).y);//k,k+1�̂Ȃ������̌X��
																																																					//std::cout << "pos: " << frameList.at(i).vertexPositionList.at(k).y << "," << frameList.at(i).vertexPositionList.at(k).x << std::endl;
																																																					//std::cout << "pos: " << frameList.at(i).vertexPositionList.at(k + 1).y << "," << frameList.at(i).vertexPositionList.at(k + 1).x << std::endl;

					if ((frameList.at(i).vertexPositionList.at(k).y <= p.vertexPositionList.at(j).y) //�n�_�ȏ�I�_������y�l
						&& (frameList.at(i).vertexPositionList.at(k + 1).y > p.vertexPositionList.at(j).y)) {
						if (p.vertexPositionList.at(j).x < (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))))
						{

							//std::cout << "a " << p.vertexPositionList.at(j).x << "," << (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))) << std::endl;
							wn.at(i).at(j)++;
						}
					}
					else if ((frameList.at(i).vertexPositionList.at(k).y > p.vertexPositionList.at(j).y)//���邢�͏I�_�ȉ��n�_���߂�y�l
						&& (frameList.at(i).vertexPositionList.at(k + 1).y <= p.vertexPositionList.at(j).y)) {
						if (p.vertexPositionList.at(j).x < (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))))
						{
							//std::cout << "b " << p.vertexPositionList.at(j).x << "," << (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))) << std::endl;
							wn.at(i).at(j)--;
						}
					}
				}
			}
		}

		bool tmp2 = true;
		for (int j = 0; j < p.vertexPositionList.size(); ++j) {
			tmp2 &= (wn[0][j] != 0);
		}
		if (tmp2)
			for (int i = 1; i < frameList.size(); ++i) {
				for (int j = 0; j < p.vertexPositionList.size(); ++j) {
					tmp2 &= (wn[i][j] == 0);
				}
			}

		return tmp2;
	}
	//}
	return false;
}

