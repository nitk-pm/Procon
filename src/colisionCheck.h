#pragma once਍⌀椀渀挀氀甀搀攀 㰀瘀攀挀琀漀爀㸀ഀ
#include <deque>਍⌀椀渀挀氀甀搀攀 ∀搀愀琀愀⸀栀∀ഀ
਍戀漀漀氀 椀猀䤀渀䘀爀愀洀攀⠀倀椀攀挀攀 瀀Ⰰ 猀琀搀㨀㨀瘀攀挀琀漀爀㰀䘀爀愀洀攀㸀 昀爀愀洀攀䰀椀猀琀⤀ 笀ഀ
਍        猀琀搀㨀㨀瘀攀挀琀漀爀㰀猀琀搀㨀㨀瘀攀挀琀漀爀㰀椀渀琀㸀㸀 眀渀⠀昀爀愀洀攀䰀椀猀琀⸀猀椀稀攀⠀⤀Ⰰ 猀琀搀㨀㨀瘀攀挀琀漀爀㰀椀渀琀㸀⠀瀀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀猀椀稀攀⠀⤀⤀⤀㬀⼀⼀먀ඏ上の点を検出するため先に宣言しておく਍ഀ
                                                                                                                                                                                                          //線分の交差判定による内外判定਍        戀漀漀氀 琀洀瀀㄀ 㴀 昀愀氀猀攀㬀ഀ
        for (int i = 0; i < p.vertexPositionList.size() - 1; ++i) {//ピースのなす線分のループ਍                倀漀猀椀琀椀漀渀 愀⠀瀀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀椀⤀⸀砀Ⰰ 瀀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀椀⤀⸀礀⤀Ⰰ 戀⠀瀀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀椀 ⬀ ㄀⤀⸀砀Ⰰ 瀀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀椀 ⬀ ㄀⤀⸀礀⤀㬀ഀ
਍                昀漀爀 ⠀椀渀琀 樀 㴀 　㬀 樀 㰀 昀爀愀洀攀䰀椀猀琀⸀猀椀稀攀⠀⤀㬀 ⬀⬀樀⤀ 笀⼀⼀䘀爀愀洀攀渀瀰湥ﰰ휰ര
                        for (int k = 0; k < frameList.at(j).vertexPositionList.size() - 1; ++k) {//currentなFrameのなす線分のループ਍                                倀漀猀椀琀椀漀渀 挀⠀昀爀愀洀攀䰀椀猀琀⸀愀琀⠀樀⤀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀欀⤀⸀砀Ⰰ 昀爀愀洀攀䰀椀猀琀⸀愀琀⠀樀⤀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀欀⤀⸀礀⤀Ⰰ 搀⠀昀爀愀洀攀䰀椀猀琀⸀愀琀⠀樀⤀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀欀 ⬀ ㄀⤀⸀砀Ⰰ 昀爀愀洀攀䰀椀猀琀⸀愀琀⠀樀⤀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀欀 ⬀ ㄀⤀⸀礀⤀㬀ഀ
਍                                椀渀琀ഀ
                                        tc = (a.x - b.x)*(c.y - a.y) + (a.y - b.y)*(a.x - c.x),਍                                        琀搀 㴀 ⠀愀⸀砀 ⴀ 戀⸀砀⤀⨀⠀搀⸀礀 ⴀ 愀⸀礀⤀ ⬀ ⠀愀⸀礀 ⴀ 戀⸀礀⤀⨀⠀愀⸀砀 ⴀ 搀⸀砀⤀Ⰰഀ
                                        ta = (c.x - d.x)*(a.y - c.y) + (c.y - d.y)*(c.x - a.x),਍                                        琀戀 㴀 ⠀挀⸀砀 ⴀ 搀⸀砀⤀⨀⠀戀⸀礀 ⴀ 挀⸀礀⤀ ⬀ ⠀挀⸀礀 ⴀ 搀⸀礀⤀⨀⠀挀⸀砀 ⴀ 戀⸀砀⤀㬀ഀ
਍                                椀昀 ⠀琀愀 㴀㴀 　⤀ 笀ഀ
                                        if (wn[j][i] > 80)਍                                                眀渀嬀樀崀嬀椀崀 ⴀ㴀 㤀㤀㬀ഀ
਍                                紀ഀ
                                if (tb == 0) {਍                                        椀昀 ⠀眀渀嬀樀崀嬀椀 ⬀ ㄀崀 㸀 㠀　⤀ഀ
                                                wn[j][i + 1] -= 99;਍                                紀ഀ
                                                tmp1 |= ta*tb < 0 && tc*td < 0;਍ഀ
                        }਍                紀ഀ
        }਍        椀昀 ⠀℀琀洀瀀㄀⤀ 笀ഀ
਍ഀ
਍                ⼀⼀眀椀搀椀渀最 渀甀洀戀攀爀 愀氀最漀爀椀琀栀洀欀蠰謰栰륑湰蔰ᙑ⑙驒൛
਍ഀ
                for (int i = 0; i < frameList.size(); ++i)਍                        昀漀爀 ⠀椀渀琀 樀 㴀 　㬀 樀 㰀 瀀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀猀椀稀攀⠀⤀ ⴀ ㄀㬀 ⬀⬀樀⤀ഀ
                                wn[i][j] = 0;਍ഀ
                for (int i = 0; i < frameList.size(); ++i) {//枠の数に対応するループ਍                        昀漀爀 ⠀椀渀琀 樀 㴀 　㬀 樀 㰀 瀀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀猀椀稀攀⠀⤀㬀 ⬀⬀樀⤀ 笀⼀⼀퐀ﰰ뤰渰Ȱ릘灰步︰�奟謰ﰰ휰ര
                                                                                                                                   //std::cout << p.vertexPositionList.at(i).x << "," << p.vertexPositionList.at(i).y << std::endl;਍                                昀漀爀 ⠀椀渀琀 欀 㴀 　㬀 欀 㰀 昀爀愀洀攀䰀椀猀琀⸀愀琀⠀椀⤀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀猀椀稀攀⠀⤀ ⴀ ㄀㬀 ⬀⬀欀⤀ 笀⼀⼀挀甀爀爀攀渀琀樀ꀰ湧Ȱ릘灰步︰�奟謰ﰰ휰ര
                                        double vt = (double)(p.vertexPositionList.at(j).y - frameList.at(i).vertexPositionList.at(k).y) / (frameList.at(i).vertexPositionList.at(k + 1).y - frameList.at(i).vertexPositionList.at(k).y);//k,k+1のなす線分の傾き਍                                                                                                                                                                                                                                                                                                                                                                                                                                        ⼀⼀猀琀搀㨀㨀挀漀甀琀 㰀㰀 ∀瀀漀猀㨀 ∀ 㰀㰀 昀爀愀洀攀䰀椀猀琀⸀愀琀⠀椀⤀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀欀⤀⸀礀 㰀㰀 ∀Ⰰ∀ 㰀㰀 昀爀愀洀攀䰀椀猀琀⸀愀琀⠀椀⤀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀欀⤀⸀砀 㰀㰀 猀琀搀㨀㨀攀渀搀氀㬀ഀ
                                                                                                                                                                                                                                                                                                                                                                                                                                        //std::cout << "pos: " << frameList.at(i).vertexPositionList.at(k + 1).y << "," << frameList.at(i).vertexPositionList.at(k + 1).x << std::endl;਍ഀ
                                        if ((frameList.at(i).vertexPositionList.at(k).y <= p.vertexPositionList.at(j).y) //始点以਍䉎륽⩰聧湮礰␀൐
                                                && (frameList.at(i).vertexPositionList.at(k + 1).y > p.vertexPositionList.at(j).y)) {਍                                                椀昀 ⠀瀀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀樀⤀⸀砀 㰀 ⠀昀爀愀洀攀䰀椀猀琀⸀愀琀⠀椀⤀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀欀⤀⸀砀 ⬀ ⠀瘀琀 ⨀ ⠀昀爀愀洀攀䰀椀猀琀⸀愀琀⠀椀⤀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀欀 ⬀ ㄀⤀⸀砀 ⴀ 昀爀愀洀攀䰀椀猀琀⸀愀琀⠀椀⤀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀欀⤀⸀砀⤀⤀⤀⤀ഀ
                                                {਍ഀ
                                                        //std::cout << "a " << p.vertexPositionList.at(j).x << "," << (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))) << std::endl;਍                                                        眀渀⸀愀琀⠀椀⤀⸀愀琀⠀樀⤀⬀⬀㬀ഀ
                                                }਍                                        紀ഀ
                                        else if ((frameList.at(i).vertexPositionList.at(k).y > p.vertexPositionList.at(j).y)//あるいは終点以下始点超過のy値਍                                                ☀☀ ⠀昀爀愀洀攀䰀椀猀琀⸀愀琀⠀椀⤀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀欀 ⬀ ㄀⤀⸀礀 㰀㴀 瀀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀愀琀⠀樀⤀⸀礀⤀⤀ 笀ഀ
                                                if (p.vertexPositionList.at(j).x < (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))))਍                                                笀ഀ
                                                        //std::cout << "b " << p.vertexPositionList.at(j).x << "," << (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))) << std::endl;਍                                                        眀渀⸀愀琀⠀椀⤀⸀愀琀⠀樀⤀ⴀⴀ㬀ഀ
                                                }਍                                        紀ഀ
                                }਍                        紀ഀ
                }਍ഀ
                bool tmp2 = true;਍                昀漀爀 ⠀椀渀琀 樀 㴀 　㬀 樀 㰀 瀀⸀瘀攀爀琀攀砀倀漀猀椀琀椀漀渀䰀椀猀琀⸀猀椀稀攀⠀⤀㬀 ⬀⬀樀⤀ 笀ഀ
                        tmp2 &= (wn[0][j] != 0);਍                紀ഀ
                if (tmp2)਍                        昀漀爀 ⠀椀渀琀 椀 㴀 ㄀㬀 椀 㰀 昀爀愀洀攀䰀椀猀琀⸀猀椀稀攀⠀⤀㬀 ⬀⬀椀⤀ 笀ഀ
                                for (int j = 0; j < p.vertexPositionList.size(); ++j) {਍                                        琀洀瀀㈀ ☀㴀 ⠀眀渀嬀椀崀嬀樀崀 㴀㴀 　⤀㬀ഀ
                                }਍                        紀ഀ
਍                爀攀琀甀爀渀 琀洀瀀㈀㬀ഀ
਍        紀ഀ
        return false;਍紀ഀ
਍�