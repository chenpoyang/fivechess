// FiveChessView.cpp : implementation of the CFiveChessView class
//

#include "stdafx.h"
#include "FiveChess.h"
#include "FiveChessDoc.h"
#include "FiveChessView.h"
#include "stdio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* 棋盘 */
ChessBoard board;
CRect start(675, 100, 740, 118);
CRect takeback(675, 150, 740, 168);
CRect restart(675, 200, 740, 218);
int my_color;	/* 自己棋子的颜色 */

/////////////////////////////////////////////////////////////////////////////
// CFiveChessView

IMPLEMENT_DYNCREATE(CFiveChessView, CView)

BEGIN_MESSAGE_MAP(CFiveChessView, CView)
	//{{AFX_MSG_MAP(CFiveChessView)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_GAME_START, OnGameStart)
	ON_WM_KEYUP()
	ON_COMMAND(ID_GAME_RESTART, OnGameRestart)
	ON_COMMAND(ID_GAME_TAKEBACK, OnGameTakeback)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFiveChessView construction/destruction

CFiveChessView::CFiveChessView()
{
	// TODO: add construction code here
	/* 视图背影大小 */
	VIEW_WIDTH = 800;
	VIEW_HEIGHT = 652;
	
	/* 设置人的棋子的颜色 */
	my_color = 1;	/* 默认人选黑色 */
	board.SetPlayer(my_color);
	BEGIN = false;
	box_shown = false;	/* 还没弹出过窗口 */
}

CFiveChessView::~CFiveChessView()
{
}

BOOL CFiveChessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	/* 设置窗口属性 */

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFiveChessView drawing

/* 设置窗口背景 */
void CFiveChessView::setBackGround(CDC* pDC)
{
	/* 用刷子设置背影颜色 */
	CBrush bru;
	bru.CreateSolidBrush(RGB(85, 123, 205));	/* 设置颜色 */
	CRect rect(0, 0, VIEW_WIDTH, VIEW_HEIGHT);	/* 要刷的矩形 */
	pDC->FillRect(rect, &bru);					/* 填育颜色 */
}

/* 区域功能视图分块 */
void CFiveChessView::split(CDC* pDC)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));	/* 笔的属性 */
	pDC->SelectObject(&pen);					/* 使用这支笔 */
	pDC->MoveTo(620, 0);	/* 将画图工具的起点移动到点(650, 0) */
	pDC->LineTo(620, 650);	/* 在当前点, 画一条到指定点的线 */
}

/* 重画 */
void CFiveChessView::OnDraw(CDC* pDC)
{
	CFiveChessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	setBackGround(pDC);	/* 设置背景颜色为豆沙绿 */
	split(pDC);			/* 区域分块 */

	board.DrawBoard(pDC);	/* 画棋盘 */
	board.DrawChesses(pDC);	/* 画棋盘上的棋子 */
	if (board.GetWinner() == BLACK && !box_shown)	/* 黑赢, 且没弹出窗口 */
	{
		SetStart(false);	/* 下一局还没开始 */
		MessageBox("	黑子胜利!	", "GameOver!", MB_OK);
		ReFleshBoard(board);		/* 全部刷新 */
		board.InitMap();	/* 重新初始化棋盘 */
		box_shown = true;	/* 已弹出 */
	}
	else if (board.GetWinner() == WHITE && !box_shown)	/* 白赢 */
	{
		SetStart(false);	/* 下一局还没开始 */
		MessageBox("	白子胜利!	", "GameOver!", MB_OK);
		ReFleshBoard(board);		/* 全部刷新 */
		board.InitMap();	/* 重新初始化棋盘 */
	}

	DrawButton(pDC, start);
	DrawButton(pDC, takeback);
	DrawButton(pDC, restart);

	pDC->DrawText("start", start, DT_CENTER | DT_BOTTOM);
	pDC->DrawText("takeback", takeback, DT_CENTER | DT_BOTTOM);
	pDC->DrawText("restart", restart, DT_CENTER | DT_BOTTOM);

	// TODO: add draw code for native data here
}

void CFiveChessView::DrawButton(CDC *pDC, const CRect &rect)		/* 画扭钮 */
{
	/* 用刷子设置背影颜色 */
	CBrush bru;
	CPen pen;

	pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));	/* 笔的属性 */
	pDC->SelectObject(&pen);					/* 使用这支笔 */
	bru.CreateSolidBrush(RGB(255, 255, 255));	/* 设置颜色 */
	pDC->FillRect(rect, &bru);					/* 填育颜色 */

	pDC->MoveTo(rect.left, rect.top);	/* 将画图工具的起点移动到点(650, 110) */
	pDC->LineTo(rect.left, rect.bottom);	/* 在当前点, 画一条到指定点的线 */
	pDC->MoveTo(rect.left, rect.bottom);	/* 将画图工具的起点移动到点(650, 110) */
	pDC->LineTo(rect.right, rect.bottom);	/* 在当前点, 画一条到指定点的线 */
	pDC->MoveTo(rect.right, rect.bottom);	/* 将画图工具的起点移动到点(650, 110) */
	pDC->LineTo(rect.right, rect.top);	/* 在当前点, 画一条到指定点的线 */
	pDC->MoveTo(rect.right, rect.top);	/* 将画图工具的起点移动到点(650, 110) */
	pDC->LineTo(rect.left, rect.top);	/* 在当前点, 画一条到指定点的线 */
}

/////////////////////////////////////////////////////////////////////////////
// CFiveChessView diagnostics

#ifdef _DEBUG
void CFiveChessView::AssertValid() const
{
	CView::AssertValid();
}

void CFiveChessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFiveChessDoc* CFiveChessView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFiveChessDoc)));
	return (CFiveChessDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFiveChessView message handlers

BOOL CFiveChessView::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CView::DestroyWindow();
}

void CFiveChessView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int x, y, me, enemy;
	CPoint dest;
	CRect rect;

	if (!IsBegin())	/* 还没按开始 */
	{
		return;
	}

	me = board.GetFolkColor();
	enemy = board.GetCompColor();

	/* 鼠标单击的点为 point 后, 返加精确的位置 */
	dest = board.GetChessLocation(point);

	/* 增加一棋子, 棋子坐标合, 且对应的棋盘上无棋子, 
	 * 视为些坐标上对应的棋盘上的位置合法 
	 */
	if (board.IsLegal(dest))
	{
		/* 向棋盘上指定坐标上添加一棋子 */
		board.AddChess(dest, me);
		/* 在坐标指定的棋盘位置上添加棋子标记 */
		board.SetChessFlg(dest, me);

		/* 刷新棋子位置, 以便立即显示给用户 */
		x = dest.x;
		y = dest.y;
		/* 刷新的矩形 */
		rect = CRect(x - GRID_SIZE / 2, y - GRID_SIZE / 2, 
			x - GRID_SIZE / 2 + GRID_SIZE, y - GRID_SIZE / 2 + GRID_SIZE);
		/* 刷新 */
		InvalidateRect(rect, true);

		/* 判断输赢(刚下棋的一方) */
		board.JudgementOutcome(dest, me);

		Sleep(200);	/* 人下棋0.2秒后, 轮到电脑下 */
		x = (dest.x - DEFAULT_X) / GRID_SIZE;
		y = (dest.y - DEFAULT_Y) / GRID_SIZE;
		board.ComputerStep(x, y);	/* 返回x, y的值(电脑下棋位置) */
		dest = board.FlgToLocation(x, y);	/* 棋盘上对应的坐标 */
		/* 向棋盘上指定坐标上添加一棋子 */
		board.AddChess(dest, enemy);
		/* 在坐标指定的棋盘位置上添加棋子标记 */
		board.SetChessFlg(dest, enemy);

		/* 刷新棋子位置, 以便立即显示给用户 */
		x = dest.x;
		y = dest.y;
		/* 刷新的矩形 */
		rect = CRect(x - GRID_SIZE / 2, y - GRID_SIZE / 2, 
			x - GRID_SIZE / 2 + GRID_SIZE, y - GRID_SIZE / 2 + GRID_SIZE);
		/* 刷新 */
		InvalidateRect(rect, true);

		/* 判断输赢(刚下棋的一方) */
		board.JudgementOutcome(dest, enemy);
	}

	CView::OnLButtonUp(nFlags, point);
}

void CFiveChessView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	/* 重新开始 */
	if (point.x >= restart.left && point.x <= restart.right &&
		point.y >= restart.top && point.y <= restart.bottom && 
		IsBegin())
	{
		ReFleshBoard(board);
		board.InitMap();
	}

	/* 开始 */
	if (point.x >= start.left && point.x <= start.right &&
		point.y >= start.top && point.y <= start.bottom && 
		!IsBegin())
	{
		SetStart(true);
		ReFleshBoard(board);
		board.InitMap();
	}

	/* 悔棋 */
	if (point.x >= takeback.left && point.x <= takeback.right &&
		point.y >= takeback.top && point.y <= takeback.bottom && 
		board.GetChessesNum() >= 2)
	{
		Sleep(200);
		CRect rect_a, rect_b;
		board.SubChess(rect_a, rect_b);	/* 减少两棋子, 且移除标记 */

		/* 刷新 */
		InvalidateRect(rect_a, true);
		InvalidateRect(rect_b, true);
	}

	CView::OnLButtonDown(nFlags, point);
}

bool CFiveChessView::IsBegin()	/* 判断游戏是否开始了 */
{
	return BEGIN;
}

void CFiveChessView::SetStart(const bool &start)	/* 开始游戏 */
{
	BEGIN = start;
}

/* 刷新棋盘 */
void CFiveChessView::ReFleshBoard(ChessBoard &board)
{
	int i, len;
	CRect rect;
	Chess chess;

	len = board.GetChessesNum();
	for (i = 0; i < len; ++i)
	{
		chess = board.GetChess(i);
		rect = chess.ReFleshChess();
		InvalidateRect(rect, true);
	}
}


void CFiveChessView::OnGameStart() 
{
	if (!IsBegin())
	{
		SetStart(true);
		ReFleshBoard(board);
		board.InitMap();
	}
}

void CFiveChessView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (nChar == VK_F2 && !IsBegin())	/* 游戏开始 */
	{
		OnGameStart();
	}
	else if (nChar == VK_F3 && IsBegin())	/* 游戏重新开始 */
	{
		ReFleshBoard(board);
		board.InitMap();
	}

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CFiveChessView::OnGameRestart() 
{
	// TODO: Add your command handler code here
	if (IsBegin())	/* 游戏已经开始, 重新开始 */
	{
		ReFleshBoard(board);
		board.InitMap();
	}
}

void CFiveChessView::OnGameTakeback() 
{
	// TODO: Add your command handler code here
	/* 悔棋 */
	if (board.GetChessesNum() >= 2)
	{
		Sleep(200);
		CRect rect_a, rect_b;
		board.SubChess(rect_a, rect_b);	/* 减少两棋子, 且移除标记 */

		/* 刷新 */
		InvalidateRect(rect_a, true);
		InvalidateRect(rect_b, true);
	}
}
