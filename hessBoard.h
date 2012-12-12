// hessBoard.h: interface for the ChessBoard class.
//
//////////////////////////////////////////////////////////////////////
#include "hess.h"

#if !defined(AFX_HESSBOARD_H__3FF45F9A_50FD_41E4_8D8E_CA8981F3D258__INCLUDED_)
#define AFX_HESSBOARD_H__3FF45F9A_50FD_41E4_8D8E_CA8981F3D258__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define DEFAULT_X 22	/* 棋盘默认的横坐标 */
#define DEFAULT_Y 12	/* 棋盘默认的纵坐标 */
#define GRID_SIZE 24	/* 网格大小 */
#define MAX_LEN 25	/* 可下棋的最大长度: MAX_LEN * MAX_LEN形矩阵 */
#define MAX_CHESSES MAX_LEN * MAX_LEN	/* 最多棋子数 */
#define NO_PLAYER 0	/* 没有玩家 */
#define BLACK 1	/* 白子 */
#define WHITE 2	/* 黑子 */
#define WON 5	/* 胜利 */

/* 棋盘 */
class ChessBoard  
{
public:
	ChessBoard();
	virtual ~ChessBoard();
public:
	/* 画棋盘 */
	void DrawBoard(CDC *pDC);
	/* 画所有在棋盘的棋子 */
	void DrawChesses(CDC *pDC);
	/* 向棋盘上指定坐标上添加指定颜色的棋子 */
	void AddChess(const CPoint &point, const int &color);
	/* 棋(color)下在(x, y), 判断输赢 */
	int CheckWin(const CPoint src, const int x, const int y, const int color);

	/* 获取位置和大小 */
	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();

	/* 初始化地图 */
	void InitMap();
	/* 设置玩家棋子颜色 */
	void SetPlayer(const int &my_color);
	/* 判断有没有棋子 */
	bool HasChess(const int &i, const int &j);
	/* 设置棋子标记, MAP[i][j] = color */
	void SetChessFlg(const CPoint &point, const int &color);
	/* 设置优胜者 */
	void SetWinner(const int &count, const int &color);
	/* 获取优胜者 */
	int GetWinner();
	/* 获取下棋的位置 */
	CPoint GetChessLocation(const CPoint &point);
	/* 获取指定下标的棋子 */
	Chess GetChess(const int &index);
	/* 判断位置是否合法 */
	bool IsLegal(const CPoint &point);
	/* 棋下在 point 后, 判断胜负(判断四条线方向) */
	void JudgementOutcome(const CPoint &point, const int &color);
	/* 判断地图情况, 便于计算出电脑下棋的位置 */
	void JudgeMap();
	/* 人下point, 返回电脑下棋的位置 */
	void ComputerStep(int &x, int &y);
	/* 给出棋盘相应的位置, 获取坐标位置 */
	CPoint FlgToLocation(const int &i, const int &j);
	/* 获取人的颜色 */
	int GetFolkColor();
	/* 获取电脑的颜色 */
	int ChessBoard::GetCompColor();
	/* 悔棋 */
	void TakeBack();
	/* 减少一棋子 */
	void SubChess(CRect &rect_a, CRect &rect_b);
	/* 删除标记 */
	void RemoveFlg(Chess chess);
	/* 获取棋子数 */
	int GetChessesNum();

private:
	Chess chs[MAX_CHESSES];	/* 棋盘上的棋子 */
	int chs_len;	/* 棋子数 */

	/* 地图 */
	int MAP[MAX_LEN][MAX_LEN];
	short Com[MAX_LEN][MAX_LEN][4];
	short Pla[MAX_LEN][MAX_LEN][4];

	int winner;	/* 优胜者 */
	int me, enemy;	/* 我和敌方 */

private:
	int x, y, width, height, size;
};

#endif // !defined(AFX_HESSBOARD_H__3FF45F9A_50FD_41E4_8D8E_CA8981F3D258__INCLUDED_)
