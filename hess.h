// hess.h: interface for the Chess class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_HESS_H__512E1886_862A_49F1_AD35_452B96BC6343__INCLUDED_)
#define AFX_HESS_H__512E1886_862A_49F1_AD35_452B96BC6343__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SIZE 20

class Chess  
{
public:
	Chess();
	Chess(int x, int y, int color);
	Chess(int a, int b, int wid, int hei, int color);
	virtual ~Chess();

public:
	void DrawChess(CDC *pDC);	/* 画棋子 */
	void SetColor(int col);		/* 设置棋子颜色 */
	void SetLocation(int x, int y);	/* 设置棋子位置 */
	int GetX();	/* 获取X坐标 */
	int GetY();	/* 获取Y坐标 */
	int GetSize();	/* 获取棋子大小 */
	bool IsInChessBoard(int x, int y);	/* 判断棋子是否在棋盘上 */
	int GetColor();	/* 获取颜色 */
	CRect ReFleshChess();	/* 刷新棋子, 以便立即显示 */

private:
	int x, y, width, height, size, color;
	bool is_in_chess_board;	/* 判断是否在棋盘上 */
};

#endif // !defined(AFX_HESS_H__512E1886_862A_49F1_AD35_452B96BC6343__INCLUDED_)
