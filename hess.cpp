// hess.cpp: implementation of the Chess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FiveChess.h"
#include "hessBoard.h"
#include "hess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEFAULT_X 22
#define DEFAULT_Y 12

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Chess::Chess()
{
	x = DEFAULT_X;
	y = DEFAULT_Y;
	is_in_chess_board = false;	/* 棋子不在棋盘上 */
	color = BLACK;	/* 默认黑色 */
	width = height = size = SIZE;
}

Chess::Chess(int a, int b, int col)
{
	/* 位置 */
	x = a;
	y = b;
	is_in_chess_board = false;	/* 棋子不在棋盘上 */
	color = col;	/* 0 代表黑色, 1代表白色 */
	width = height = size = SIZE;	/* 大小 */
}

/* 位置和大小 */
Chess::Chess(int a, int b, int wid, int hei, int col)
{
	x = a;
	y = b;
	is_in_chess_board = false;	/* 棋子不在棋盘上 */
	color = col;
	width = height = size = SIZE;
}

/* 设置棋子颜色 */
void Chess::SetColor(int col)
{
	color = col;
}

/* 设置位置 */
void Chess::SetLocation(int a, int b)
{
	x = a;
	y = b;
}

/* 获取位置 */
int Chess::GetX()
{
	return x;
}

int Chess::GetY()
{
	return y;
}

/* 画棋子 */
void Chess::DrawChess(CDC *pDC)
{
	CBrush *brush;
	COLORREF color;
	CPen pen;
	int a, b;

	a = x - SIZE / 2;
	b = y - SIZE / 2;

	/* 选择颜色 */
	if (this->color == WHITE)	/* 白色 */
	{
		color = RGB(255, 255, 255);
		pen.CreatePen(PS_SOLID, 1, color);
	}
	else	/* 黑色 */
	{
		color = RGB(0, 0, 0);
		pen.CreatePen(PS_SOLID, 1, color);
	}

	brush = new CBrush(color);	/* 笔的属性 */
	pDC->SelectObject(brush);			/* 使用这支笔 */
	pDC->SelectObject(&pen);
	pDC->Ellipse(a, b, a + size, b + size);	/* 画圆 */
	brush->DeleteObject();
	pen.DeleteObject();
}

Chess::~Chess()
{

}

/* 获取棋子大小 */
int Chess::GetSize()
{
	return size;
}

/* 判断棋子是否在棋盘上 */
bool Chess::IsInChessBoard(int x, int y)
{
	return is_in_chess_board;
}

int Chess::GetColor()	/* 获取颜色 */
{
	return color;
}

CRect Chess::ReFleshChess()	/* 刷新棋子, 以便立即显示 */
{
	CRect rect;

	/* 刷新的矩形 */
	rect = CRect(x - GRID_SIZE / 2, y - GRID_SIZE / 2, 
		x - GRID_SIZE / 2 + GRID_SIZE, y - GRID_SIZE / 2 + GRID_SIZE);

	return rect;	/* 返回 */
}