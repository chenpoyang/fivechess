// hessBoard.cpp: implementation of the ChessBoard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FiveChess.h"
#include "hessBoard.h"
#include "hess.h"
#include <iostream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/* 棋盘位置和大小 */
ChessBoard::ChessBoard()
{
	/* 棋盘坐标 */
	x = DEFAULT_X;
	y = DEFAULT_Y;

	/* 棋盘大小 */
	width = 576;
	height = 576;

	/* 网格大小 */
	size = GRID_SIZE;

	/* 棋盘上棋子数为零 */
	chs_len = 0;

	/* 初始化棋盘上棋子状态(是否有棋子) */
	InitMap();

	winner = NO_PLAYER;	/* 设置优胜者, 默认为颜色2(非黑非白) */
}

ChessBoard::~ChessBoard()
{

}

/* 向棋盘指定坐标上添加一棋子 */
void ChessBoard::AddChess(const CPoint &point, const int &color)
{
	Chess chess(point.x, point.y, color);

	chs[chs_len++] = chess;
}

/* 画出所有棋子 */
void ChessBoard::DrawChesses(CDC *pDC)
{
	int i;

	/* 逐渐画出所有棋子 */
	for (i = 0; i < chs_len; ++i)
	{
		chs[i].DrawChess(pDC);
	}	
}

/* 画出棋盘 */
void ChessBoard::DrawBoard(CDC *pDC)
{
	int i, row, col;
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(155, 155, 155));
	pDC->SelectObject(&pen);

	row = height / size;
	col = width / size;
	
	for (i = 0; i <= row; ++i)
	{
		pDC->MoveTo(x, y + i * size);
		pDC->LineTo(x + width, y + i * size);
	}

	for (i = 0; i <= col; ++i)
	{
		pDC->MoveTo(x + i * size, y);
		pDC->LineTo(x + i * size, y + height);
	}
}

/* 获取位置 */
int ChessBoard::GetX()
{
	return x;
}

int ChessBoard::GetY()
{
	return y;
}

/* 获取宽度 */
int ChessBoard::GetWidth()
{
	return width;
}

/* 获取高度 */
int ChessBoard::GetHeight()
{
	return height;
}

/* 初始化地图 */
void ChessBoard::InitMap()
{
	int i, j;

	chs_len = 0;/* 棋盘上的棋子重新归零 */
	winner = NO_PLAYER;	/* 重置优胜方 */
	for (i = 0; i <= MAX_LEN; ++i)
	{
		for (j = 0; j <= MAX_LEN; ++j)
		{
			MAP[i][j] = NO_PLAYER;	/* 未确定颜色 */
		}
	}
}

/* 设置玩家棋子颜色 */
void ChessBoard::SetPlayer(const int &my_color)
{
	me = my_color;
	if (me == BLACK)
	{
		enemy = WHITE;
	}
	else
	{
		enemy = BLACK;
	}
}

/* 判断有没有棋子 */
bool ChessBoard::HasChess(const int &x, const int &y)
{
	/* 1 表示黑棋子, 2表示白棋子, 0无棋子  */
	return MAP[x][y] != 0;
}

/* 设置棋子标记, MAP[i][j] = color */
void ChessBoard::SetChessFlg(const CPoint &point, const int &color)
{
	int i, j;

	/* 增加棋子标记到棋盘 */
	i = (point.x - DEFAULT_X) / GRID_SIZE + 1;
	j = (point.y - DEFAULT_Y) / GRID_SIZE + 1;

	/* MAP[i][j] = color; *///下标从1 ~ MAX_LEN
	MAP[i - 1][j - 1] = color;
}

/* 下棋后, 棋(color)在(x, y)方向上 判断输赢 
 * src:		下棋的点
 * (x, y):	判断的方向(双向用:累加或累减)
 * chess_color:	下棋的颜色
 */
int ChessBoard::CheckWin(const CPoint src, 
			const int x, const int y, 
			const int chess_color)
{
	int i, j, color, count;

	/* (i, j): 地图起点标记 */
	i = (src.x - DEFAULT_X) / GRID_SIZE;
	j = (src.y - DEFAULT_Y) / GRID_SIZE;
	color = chess_color;
	count = 0;

	/* 棋子没有超出范围, 且棋子颜色与刚下的棋子颜色一致 */
	while (i >= 0 && i < MAX_LEN &&
			j >= 0 && j < MAX_LEN &&
			MAP[i][j] == color)
	{
		++count;	/* 找到连续同一颜色的棋子 */
		/* 找下一个棋子(正向) */
		i += x;	
		j += y;
	}

	i = (src.x - DEFAULT_X) / GRID_SIZE;
	j = (src.y - DEFAULT_Y) / GRID_SIZE;
	color = chess_color;

	/* 棋子没有超出范围, 且棋子颜色与刚下的棋子颜色一致 */
	while (i >= 0 && i < MAX_LEN &&
			j >= 0 && j < MAX_LEN &&
			MAP[i][j] == color)
	{
		++count;	/* 找到连续同一颜色的棋子 */
		/* 找下一个棋子(反向) */
		i -= x;	
		j -= y;
	}

	return --count;	/* 源点重复累加 */
}

/* 设置优胜者 */
void ChessBoard::SetWinner(const int &count, const int &color)
{
	if (count == WON)
	{
		winner = color;	/* 胜利者的颜色 */
	}
}
/* 获取优胜者 */
int ChessBoard::GetWinner()
{
	return winner;
}

/* 单击鼠标的位置在point后, 获取下棋的准确位置 */
CPoint ChessBoard::GetChessLocation(const CPoint &point)
{
	int wid, hei, x, y;
	CPoint dest;	/* 最终确定要下棋的位置 */

	/* 棋盘上确定的最近的坐标, 最小宽度和高度 */
	wid = point.x - DEFAULT_X;
	hei = point.y - DEFAULT_Y;

	/* 最近的点 */
	x = DEFAULT_X + wid / GRID_SIZE * GRID_SIZE;
	y = DEFAULT_Y + hei / GRID_SIZE * GRID_SIZE;
	CPoint tmp(x, y);
	/* 其余三点 */
	CPoint right(x + GRID_SIZE, y);	/* tmp 点右边的点 */
	CPoint ri_down(x + GRID_SIZE, y + GRID_SIZE);	/* tmp 点右下的点 */
	CPoint down(x, y + GRID_SIZE);	/* tmp 点正下方的点 */

	/* 鼠标单击的点 */
	x = point.x;
	y = point.y;
	/* 精确点(tmp, right, ri_down, down 其中一个) */
	if (x >= right.x - GRID_SIZE / 2 && 
		x <= right.x + GRID_SIZE / 2 && 
		y >= right.y - GRID_SIZE / 2 && 
		y <= right.y + GRID_SIZE / 2)
	{
		dest = right;
	}
	else if (x >= ri_down.x - GRID_SIZE / 2 && 
		x <= ri_down.x + GRID_SIZE / 2 && 
		y >= ri_down.y - GRID_SIZE / 2 && 
		y <= ri_down.y + GRID_SIZE / 2)
	{
		dest = ri_down;
	}
	else if (x >= down.x - GRID_SIZE / 2 && 
		x <= down.x + GRID_SIZE / 2 && 
		y >= down.y - GRID_SIZE / 2 && 
		y <= down.y + GRID_SIZE / 2)
	{
		dest = down;
	}
	else
	{
		dest = tmp;
	}

	return dest;
}

/* 获取指定下标的棋子 */
Chess ChessBoard::GetChess(const int &index)
{
	return chs[index];
}

/* 判断位置是否合法 */
bool ChessBoard::IsLegal(const CPoint &point)
{
	int i, j;

	/* 棋子坐标对应的地图标记 */
	i = (point.x - DEFAULT_X) / GRID_SIZE;
	j = (point.y - DEFAULT_Y) / GRID_SIZE;

	/* 坐标合法, 且在棋盘上无棋子 */
	if (point.x >= DEFAULT_X && 
		point.x <= DEFAULT_X + GetWidth() && 
		point.y >= DEFAULT_Y && 
		point.y <= DEFAULT_Y + GetHeight() && 
		!HasChess(i, j))
	{
		return true;
	}

	return false;
}

/* 棋下在 point 后, 判断胜负(判断四条线方向) */
void ChessBoard::JudgementOutcome(const CPoint &point, const int &color)
{
	/* 判断输赢(刚下棋的一方) */
	if (WON == CheckWin(point, 1, 0, color))	/* 横方向 */
	{
		SetWinner(WON, color);	/* 设置胜利者 */
	}
	else if (WON == CheckWin(point, 0, 1, color))	/* 竖方向 */
	{
		SetWinner(WON, color);	/* 设置胜利者 */
	}
	else if (WON == CheckWin(point, 1, 1, color))	/* 反斜线方向 "\" */
	{
		SetWinner(WON, color);	/* 设置胜利者 */
	}
	else if (WON == CheckWin(point, -1, 1, color))	/* 正斜线方向 "/" */
	{
		SetWinner(WON, color);	/* 设置胜利者 */
	}
}

/* 判断地图情况, 便于计算出电脑下棋的位置 */
void ChessBoard::JudgeMap()
{
	int i, j, a, now = 0, last = 0, left, up, right, down;
	
	for (i = 0; i < MAX_LEN; ++i)
	{
		for (j = 0; j < MAX_LEN; ++j)
		{
			for (a = 0; a < 4; ++a)
			{
				Com[i][j][a] = 0;
				Pla[i][j][a] = 0;
			}
		}
	}
	
	for (i = 0; i < MAX_LEN; ++i)
	{
		for (j = 0; j < MAX_LEN; ++j)
		{
			if (MAP[i][j] == 0)
			{
				/* left */
				now = 0;
				last = 0;
				left = j - 1;
				for (; left >= 0; --left)
				{
					if (MAP[i][left] == me)
					{
						now = me;
						if (left != j - 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Com[i][j][0];
						last = me;
					}
					else if (MAP[i][left] == enemy)
					{
						now = enemy;
						if (left != j - 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Pla[i][j][0];
						last = enemy;
					}
					else
					{
						break;
					}
				}
				
				/* right */
				now = 0;
				last = 0;
				right = j + 1;
				for (; right < MAX_LEN; ++right)
				{
					if (MAP[i][right] == me)
					{
						now = me;
						if (right != j + 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Com[i][j][0];
						last = me;
					}
					else if (MAP[i][right] == enemy)
					{
						now = enemy;
						if (right != j + 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Pla[i][j][0];
						last = enemy;
					}
					else
					{
						break;
					}
				}
				
				/* up */
				up = i - 1;
				now = 0;
				last = 0;
				for (; up >= 0; --up)
				{
					if (MAP[up][j] == me)
					{
						now = me;
						if (up != i - 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Com[i][j][1];
						last = me;
					}
					else if (MAP[up][j] == enemy)
					{
						now = enemy;
						if (up != i - 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Pla[i][j][1];
						last = enemy;
					}
					else
					{
						break;
					}
				}
				
				down = i + 1;
				now = 0;
				last = 0;
				for (; down < MAX_LEN; ++down)
				{
					if (MAP[down][j] == me)
					{
						now = me;
						if (down != i + 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Com[i][j][1];
						last = me;
					}
					else if (MAP[down][j] == enemy)
					{
						now = enemy;
						if (down != i + 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Pla[i][j][1];
						last = enemy;
					}
					else
					{
						break;
					}
				}
				
				/* down */
				right = j + 1;
				up = i - 1;
				now = 0;
				last = 0;
				while (right < MAX_LEN && up >= 0)
				{
					if (MAP[up][right] == me)
					{
						now = me;
						if (right != j + 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Com[i][j][2];
						last = me;
					}
					else if (MAP[up][right] == enemy)
					{
						now = enemy;
						if (right != j + 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Pla[i][j][2];
						last = enemy;
					}
					else
					{
						break;
					}
					
					++right;
					--up;
				}
				
				left = j - 1;
				down = i + 1;
				now = 0;
				last = 0;
				while (left >= 0 && down < MAX_LEN)
				{
					if (MAP[down][left] == me)
					{
						now = me;
						if (left != j - 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Com[i][j][2];
						last = me;
					}
					else if (MAP[down][left] == enemy)
					{
						now = enemy;
						if (left != j - 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Pla[i][j][2];
						last = enemy;
					}
					else
					{
						break;
					}
					--left;
					++down;
				}
				
				left = j - 1;
				up = i - 1;
				now = 0;
				last = 0;
				while (left >= 0 && up >= 0)
				{
					if (MAP[up][left] == me)
					{
						now = me;
						if (left != j - 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Com[i][j][3];
						last = me;
					}
					else if (MAP[up][left] == enemy)
					{
						now = enemy;
						if (left != j - 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Pla[i][j][3];
						last = enemy;
					}
					else
					{
						break;
					}
					--left;
					--up;
				}
				
				right = j + 1;
				down = i + 1;
				now = 0;
				last = 0;
				while (right < MAX_LEN && down < MAX_LEN)
				{
					if (MAP[down][right] == me)
					{
						now = me;
						if (right != j + 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Com[i][j][3];
						last = me;
					}
					else if (MAP[down][right] == enemy)
					{
						now = enemy;
						if (right != j + 1)
						{
							if (now != last)
							{
								break;
							}
						}
						
						++Pla[i][j][3];
						last = enemy;
					}
					else
					{
						break;
					}
					++right;
					++down;
				}
			}
		}
	}
}

/* 人下point, 返回电脑下棋的位置 */
void ChessBoard::ComputerStep(int &x, int &y)
{
	int i, j, temp, max = 0;
	
	x = y = MAX_LEN / 2 + 1;
	JudgeMap();
	for (i = 0; i < MAX_LEN; ++i)
	{
		for (j = 0; j < MAX_LEN; ++j)
		{
			temp = Com[i][j][0] + Com[i][j][1] + Com[i][j][2] + Com[i][j][3];
			if (temp > max)
			{
				x = i;
				y = j;
				max = temp;
			}
			temp = Pla[i][j][0] + Pla[i][j][1] + Pla[i][j][2] + Pla[i][j][3];
			if (temp > max)
			{
				x = i;
				y = j;
				max = temp;
			}
			
			temp = Com[i][j][0];
			
			if (temp > max)
			{	
				max = temp;
				x = i;
				y = j;
			}
			
			if (temp == 4)
			{
				x = i;
				y = j; 
				MAP[x][y] = me;
				return;
			}
			
			temp = Com[i][j][1];
			if (temp > max)
			{	
				max = temp;
				x = i;
				y = j;
			}
			
			if (temp == 4)
			{
				x = i;
				y = j; 
				MAP[x][y] = me;
				return;
			}
			
			temp = Com[i][j][2];
			if (temp > max)
			{
				max = temp;
				x = i;
				y = j;
			}
			
			if (temp == 4)
			{
				x = i;
				y = j; 
				MAP[x][y] = me;
				return;
			}
			
			temp = Com[i][j][3];
			if (temp > max)
			{
				max = temp;
				x = i;
				y = j;
			}
			
			if (temp == 4)
			{
				x = i;
				y = j;
				MAP[x][y] = me;
				return;
			}
			
			temp = Pla[i][j][0];
			if (temp > max)
			{
				max = temp;
				x = i;
				y = j;
			}
			
			if (temp == 4)
			{
				x = i;
				y = j;
				MAP[x][y] = me;
				return;
			}
			
			temp = Pla[i][j][1];
			if (temp > max)
			{
				max = temp;
				x = i;
				y = j;
			}
			
			if (temp == 4)
			{
				x = i;
				y = j; 
				MAP[x][y] = me;
				return;
			}
			
			temp = Pla[i][j][2];
			if (temp > max)
			{
				max = temp;
				x = i;
				y = j;
			}
			
			if (temp == 4)
			{
				x = i;
				y = j; 
				MAP[x][y] = me;
				return;
			}
			
			temp = Pla[i][j][3];
			if (temp > max)
			{
				max = temp;
				x = i;
				y = j;
			}
			
			if (temp ==4)
			{
				x = i;
				y = j; 
				MAP[x][y] = me;
				return;
			}
		}
	}

	for (i = 0; i < MAX_LEN; ++i) {
		for (j = 0; j < MAX_LEN; ++j)
		{
			temp = Com[i][j][0];
			if (temp == 3)
			{
				if (j - 1 >= 0 && j + 4 < MAX_LEN && 
					MAP[i][j-1] == 0 && 
					MAP[i][j+1] == me && 
					MAP[i][j+2] == me && 
					MAP[i][j+3] == me && 
					MAP[i][j+4]==0)
				{
					x = i;
					y = j; 
					MAP[x][y] = me;
					return;
				}
				else if (j + 1 < MAX_LEN && j - 4 >= 0 && 
					MAP[i][j+1] == 0 && 
					MAP[i][j-1] == me && 
					MAP[i][j-2] == me && 
					MAP[i][j-3] == me && 
					MAP[i][j-4]==0)
				{
					x = i;
					y = j; 
					MAP[x][y] = me;
					return;
				}
				else if (j + 2 < MAX_LEN && j - 3 >= 0 && 
					MAP[i][j-1] == me && 
					MAP[i][j-2] == me && 
					MAP[i][j-3] == 0 && 
					MAP[i][j+1] == me && 
					MAP[i][j+2]==0)
				{
					x = i;
					y = j; 
					MAP[x][y] = me;
					return;
				}
				else if (j + 3 < MAX_LEN && j - 2 >= 0 && 
					MAP[i][j+1] == me && 
					MAP[i][j+2] == me && 
					MAP[i][j+3] == 0 && 
					MAP[i][j-1] == me && 
					MAP[i][j-2]==0)
				{
					x = i;
					y = j; 
					MAP[x][y] = me;
					return;
				}
			}
			
			temp = Com[i][j][1];
			if (temp == 3)
			{
				if (i - 1 >= 0 && i + 4 < MAX_LEN && 
					MAP[i-1][j] == 0 && 
					MAP[i+1][j] == me && 
					MAP[i+2][j] == me && 
					MAP[i+3][j] == me && 
					MAP[i+4][j]==0)
				{
					x = i;
					y = j; 
					MAP[x][y] = me;
					return;
				}
				else if (i + 1 < MAX_LEN && i - 4 >= 0 && 
					MAP[i+1][j] == 0 && 
					MAP[i-1][j] == me && 
					MAP[i-2][j] == me && 
					MAP[i-3][j] == me && 
					MAP[i-4][j]==0)
				{
					x = i;
					y = j; 
					MAP[x][y] = me;
					return;
				}
				else if (i + 2 < MAX_LEN && i - 3 >= 0 && 
					MAP[i-1][j] == me && 
					MAP[i-2][j] == me && 
					MAP[i-3][j] == 0 && 
					MAP[i+1][j] == me && 
					MAP[i+2][j]==0)
				{
					x = i;
					y = j; 
					MAP[x][y] = me;
					return;
				}
				else if (i + 3 < MAX_LEN && i - 2 >= 0 && 
					MAP[i+1][j] == me && 
					MAP[i+2][j] == me && 
					MAP[i+3][j] == 0 && 
					MAP[i-1][j] == me && 
					MAP[i-2][j]==0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
			}
			
			temp = Com[i][j][2];
			if (temp == 3)
			{
				if (i + 4 < MAX_LEN && i - 1 >= 0 && 
					j + 1 < MAX_LEN && j - 4 >= 0 && 
					MAP[i-1][j+1] == 0 && 
					MAP[i+1][j-1] == me && 
					MAP[i+2][j-2] == me && 
					MAP[i+3][j-3] == me && 
					MAP[i+4][j-4] == 0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (i - 4 >= 0 && i + 1 < MAX_LEN && 
					j + 4 < MAX_LEN && j - 1 >= 0 && 
					MAP[i+1][j-1] == 0 && 
					MAP[i-1][j+1] == me && 
					MAP[i-2][j+2] == me && 
					MAP[i-3][j+3] == me && 
					MAP[i-4][j+4]==0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				} else if (i + 2 < MAX_LEN && i - 3 >= 0 && 
					j + 3 < MAX_LEN && j - 2 >= 0 && 
					MAP[i+2][j-2] == 0 && 
					MAP[i+1][j-1] == me && 
					MAP[i-1][j+1] == me && 
					MAP[i-2][j+2] == me && 
					MAP[i-3][j+3] == 0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (i + 3 < MAX_LEN && i - 2 >= 0 && 
					j - 3 >= 0 && j + 2 < MAX_LEN && 
					MAP[i+3][j-3] == 0 && 
					MAP[i+2][j-2] == me && 
					MAP[i+1][j-1] == me && 
					MAP[i-1][j+1] == me && 
					MAP[i-2][j+2]==0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				
			}
			
			temp=Com[i][j][3];
			if (temp == 3)
			{
				if (i + 4 < MAX_LEN && i - 1 >= 0 && 
					j + 4 < MAX_LEN && j - 1 >= 0 && 
					MAP[i-1][j-1] == 0 && 
					MAP[i+1][j+1] == me && 
					MAP[i+2][j+2] == me && 
					MAP[i+3][j+3] == me && 
					MAP[i+4][j+4]==0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (i - 4 >= 0 && i + 1 < MAX_LEN && 
					j + 1 < MAX_LEN && j - 4 >= 0 && 
					MAP[i+1][j+1] == 0 && 
					MAP[i-1][j-1] == me && 
					MAP[i-2][j-2] == me && 
					MAP[i-3][j-3] == me && 
					MAP[i-4][j-4]==0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (i + 2 < MAX_LEN && i - 3 >= 0 && 
					j + 2 < MAX_LEN && j - 3 >= 0 && 
					MAP[i+2][j+2] == 0 && 
					MAP[i+1][j+1] == me && 
					MAP[i-1][j-1] == me && 
					MAP[i-2][j-2] == me && 
					MAP[i-3][j-3] == 0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (i + 3 < MAX_LEN && i - 2 >= 0 && 
					j - 2 >= 0 && j + 3 < MAX_LEN && 
					MAP[i+3][j+3] == 0 && 
					MAP[i+2][j+2] == me && 
					MAP[i+1][j+1] == me && 
					MAP[i-1][j-1] == me && 
					MAP[i-2][j-2]==0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
			}
			
			temp = Pla[i][j][0];
			if (temp == 3)
			{
				if (j - 1 >= 0 && j + 4 < MAX_LEN && 
					MAP[i][j-1] == 0 && 
					MAP[i][j+1] == enemy && 
					MAP[i][j+2] == enemy && 
					MAP[i][j+3] == enemy && 
					MAP[i][j+4]==0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (j + 1 < MAX_LEN && j - 4 >= 0 && 
					MAP[i][j+1] == 0 && 
					MAP[i][j-1] == enemy && 
					MAP[i][j-2] == enemy && 
					MAP[i][j-3] == enemy && 
					MAP[i][j-4] == 0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (j + 2 < MAX_LEN && j - 3 >= 0 && 
					MAP[i][j-1] == enemy && 
					MAP[i][j-2] == enemy && 
					MAP[i][j-3] == 0 && 
					MAP[i][j+1] == enemy && 
					MAP[i][j+2]==0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (j + 3 < MAX_LEN && j - 2 >= 0 && 
					MAP[i][j+1] == enemy && 
					MAP[i][j+2] == enemy && 
					MAP[i][j+3] == 0 && 
					MAP[i][j-1] == enemy && 
					MAP[i][j-2]==0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
			}
			
			temp = Pla[i][j][1];
			if (temp == 3)
			{
				if (i - 1 >= 0 && i + 4 < MAX_LEN && 
					MAP[i-1][j] == 0 && 
					MAP[i+1][j] == enemy && 
					MAP[i+2][j] == enemy && 
					MAP[i+3][j] == enemy && 
					MAP[i+4][j]==0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (i + 1 < MAX_LEN && i - 4 >= 0 && 
					MAP[i+1][j] == 0 && 
					MAP[i-1][j] == enemy && 
					MAP[i-2][j] == enemy && 
					MAP[i-3][j] == enemy && 
					MAP[i-4][j]==0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (i + 2 < MAX_LEN && i - 3 >= 0 && 
					MAP[i-1][j] == enemy && 
					MAP[i-2][j] == enemy && 
					MAP[i-3][j] == 0 && 
					MAP[i+1][j] == enemy && 
					MAP[i+2][j] == 0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (i + 3 < MAX_LEN && i - 2 >= 0 && 
					MAP[i+1][j] == enemy && 
					MAP[i+2][j] == enemy && 
					MAP[i+3][j] == 0 && 
					MAP[i-1][j] == enemy && 
					MAP[i-2][j]==0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
			}

			temp = Pla[i][j][2];
			if (temp == 3)
			{
				if (i + 4 < MAX_LEN && i - 1 >= 0 && 
					j + 1 < MAX_LEN && j - 4 >= 0 && 
					MAP[i-1][j+1] == 0 && 
					MAP[i+1][j-1] == enemy && 
					MAP[i+2][j-2] == enemy && 
					MAP[i+3][j-3] == enemy && 
					MAP[i+4][j-4] == 0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (i - 4 >= 0 && i + 1 < MAX_LEN && 
					j + 4 < MAX_LEN && j - 1 >= 0 && 
					MAP[i+1][j-1] == 0 && 
					MAP[i-1][j+1] == enemy && 
					MAP[i-2][j+2] == enemy && 
					MAP[i-3][j+3] == enemy && 
					MAP[i-4][j+4] == 0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (i + 2 < MAX_LEN && i - 3 >= 0 && 
					j + 3 < MAX_LEN && j - 2 >= 0 && 
					MAP[i+2][j-2] == 0 && 
					MAP[i+1][j-1] == enemy && 
					MAP[i-1][j+1] == enemy && 
					MAP[i-2][j+2] == enemy && 
					MAP[i-3][j+3] == 0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (i + 3 < MAX_LEN && i - 2 >= 0 && 
					j - 3 >= 0 && j + 2 < MAX_LEN && 
					MAP[i+3][j-3] == 0 && 
					MAP[i+2][j-2] == enemy && 
					MAP[i+1][j-1] == enemy && 
					MAP[i-1][j+1] == enemy && 
					MAP[i-2][j+2] == 0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				
			}
			
			temp = Pla[i][j][3];
			if (temp == 3)
			{
				if (i + 4 < MAX_LEN && i - 1 >= 0 && 
					j + 4 < MAX_LEN && j - 1 >= 0 && 
					MAP[i-1][j-1] == 0 && 
					MAP[i+1][j+1] == enemy && 
					MAP[i+2][j+2] == enemy && 
					MAP[i+3][j+3] == enemy && 
					MAP[i+4][j+4] == 0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (i - 4 >= 0 && i + 1 < MAX_LEN && 
					j + 1 < MAX_LEN && j - 4 >= 0 && 
					MAP[i+1][j+1] == 0 && 
					MAP[i-1][j-1] == enemy && 
					MAP[i-2][j-2] == enemy && 
					MAP[i-3][j-3] == enemy && 
					MAP[i-4][j-4] == 0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (i + 2 < MAX_LEN && i - 3 >= 0 && 
					j + 2 < MAX_LEN && j - 3 >= 0 && 
					MAP[i+2][j+2] == 0 && 
					MAP[i+1][j+1] == enemy && 
					MAP[i-1][j-1] == enemy && 
					MAP[i-2][j-2] == enemy && 
					MAP[i-3][j-3] == 0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
				else if (i + 3 < MAX_LEN && i - 2 >= 0 && 
					j - 2 >= 0 && j + 3 < MAX_LEN && 
					MAP[i+3][j+3] == 0 && 
					MAP[i+2][j+2] == enemy && 
					MAP[i+1][j+1] == enemy && 
					MAP[i-1][j-1] == enemy && 
					MAP[i-2][j-2] == 0)
				{
					x = i;
					y = j;
					MAP[x][y] = me;
					return;
				}
			}
			
		}
	} 
	
	MAP[x][y]=me; 
}

/* 给出棋盘相应的位置, 获取坐标位置 */
CPoint ChessBoard::FlgToLocation(const int &i, const int &j)
{
	int x, y;
	CPoint dest;

	x = i * GRID_SIZE + DEFAULT_X;
	y = j * GRID_SIZE + DEFAULT_Y;
	dest = CPoint(x, y);

	return dest;
}

/* 获取人的颜色 */
int ChessBoard::GetFolkColor()
{
	return me;
}

/* 获取电脑的颜色 */
int ChessBoard::GetCompColor()
{
	return enemy;
}

/* 悔棋 */
void ChessBoard::TakeBack()
{
}

/* 减少一棋子 */
void ChessBoard::SubChess(CRect &rect_a, CRect &rect_b)
{
	CRect rect;
	if (chs_len >= 2)
	{
		--chs_len;
		rect = chs[chs_len].ReFleshChess();
		rect_a = rect;	/* 返回一个区域 */
		RemoveFlg(chs[chs_len]);

		--chs_len;
		rect = chs[chs_len].ReFleshChess();
		rect_b = rect;	/* 返回另一个区域 */
		RemoveFlg(chs[chs_len]);
	}
}

/* 删除标记 */
void ChessBoard::RemoveFlg(Chess chess)
{
	int i, j;

	i = (chess.GetX() - DEFAULT_X) / GRID_SIZE;
	j = (chess.GetY() - DEFAULT_Y) / GRID_SIZE;
	MAP[i][j] = NO_PLAYER;
}

/* 获取棋子数 */
int ChessBoard::GetChessesNum()
{
	return chs_len;
}
