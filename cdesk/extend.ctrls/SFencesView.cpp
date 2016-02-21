#include "StdAfx.h"
#include "SFencesView.h"

namespace SOUI
{

	SFencesView::SFencesView(SDesktopView *pHost)
		:m_pHost(pHost)
		,m_bDrag(false)	
		,m_bZoom(false)
		,m_nStartZoomMouseLoc(0)
	{
		
	}

	SFencesView::~SFencesView()
	{

	}	

	void SFencesView::OnLButtonDown( UINT nFlags, CPoint point )
	{
		SWindow::OnLButtonDown(nFlags,point);

		UINT nMouseLoc = OnWndNcHitTest(point);
		setFencesCursor(nMouseLoc);

		BringWindowToTop();
		m_ptDrag = point;
		m_bDrag  = false;
	}

	void SFencesView::OnLButtonUp( UINT nFlags, CPoint point )
	{
		SWindow::OnLButtonUp(nFlags, point);
		if (m_bDrag && m_pHost != NULL)  
		{
			m_pHost->UpdateChildrenPosition();
		}

		m_bDrag = false;
		m_bZoom = false;
		m_nStartZoomMouseLoc = 0;
	}

	void SFencesView::OnMouseMove( UINT nFlags, CPoint point )
	{				
		CRect rcWnd = GetWindowRect();
		UINT nMouseLoc = OnWndNcHitTest(point);

		// 拖动窗口
		if (isDragable(nFlags) && HTCLIENT == nMouseLoc && !m_bZoom)
		{			
			dragFences(rcWnd, point);
		}
		// 缩放窗口
		else 
		{
			zoomFences(rcWnd, point, nMouseLoc);
		}				
	}

	void SFencesView::OnPaint( IRenderTarget * pRT )
	{
		SPainter painter;
		BeforePaint(pRT, painter);
		CRect rcList = GetWindowRect();
		pRT->PushClipRect(&rcList);
		
		// 绘制背景
		COLORREF crBg(0xffffff64);
		CAutoRefPtr<IBrush> brush,oldBrush;
		pRT->CreateSolidColorBrush(crBg, &brush);			
		pRT->SelectObject(brush,(IRenderObj**)&oldBrush);
		pRT->FillRectangle(rcList);
		pRT->SelectObject(oldBrush);

		pRT->PopClip();
		AfterPaint(pRT, painter);
	}

	UINT SFencesView::OnWndNcHitTest(CPoint point)
	{
		CRect rcMargin(5, 5, 5, 5);
		CRect rcWindow = GetClientRect();

		if (point.x > rcWindow.right - rcMargin.right)
		{
			if (point.y > rcWindow.bottom - rcMargin.bottom)
			{
				return HTBOTTOMRIGHT;
			}
			else if (point.y < (rcWindow.top+rcMargin.top))
			{
				return HTTOPRIGHT;
			}
			return HTRIGHT;
		}
		else if (point.x < rcWindow.left+rcMargin.left)
		{
			if (point.y > rcWindow.bottom - rcMargin.bottom)
			{
				return HTBOTTOMLEFT;
			}
			else if (point.y < (rcWindow.top+rcMargin.top))
			{
				return HTTOPLEFT;
			}
			return HTLEFT;
		}
		else if (point.y > rcWindow.bottom - rcMargin.bottom)
		{
			return HTBOTTOM;
		}
		else if (point.y < (rcWindow.top+rcMargin.top))
		{
			return HTTOP;
		}

		return HTCLIENT;
	}

	bool SFencesView::isDragable(UINT nFlags)
	{
		m_bDrag = (nFlags & MK_LBUTTON);
		return m_bDrag;
	}

	void SFencesView::dragFences(CRect rectWnd, CPoint point)
	{
		rectWnd.OffsetRect(point.x-m_ptDrag.x, point.y-m_ptDrag.y);				
		Move(rectWnd);
		m_ptDrag = point;		
	}

	void SFencesView::zoomFences(CRect rectWnd, CPoint point, UINT mouseLoc)
	{
		CPoint topLeftPt = rectWnd.TopLeft();
		CPoint bottomRightPt = rectWnd.BottomRight();
		int nStartZoomMouseLoc = mouseLoc;

		if (m_bDrag && m_nStartZoomMouseLoc == 0)
		{
			m_nStartZoomMouseLoc = mouseLoc;
		}
		
		if (m_bDrag && m_nStartZoomMouseLoc != 0)
		{
			nStartZoomMouseLoc = m_nStartZoomMouseLoc;
		}

		switch (nStartZoomMouseLoc)
		{
		case HTLEFT:
			topLeftPt.Offset(point.x-m_ptDrag.x, 0);			
			break;
		case HTRIGHT:
			bottomRightPt.Offset(point.x-m_ptDrag.x, 0);		
			break;
		case HTTOP:
			topLeftPt.Offset(0, point.y-m_ptDrag.y);		
			break;
		case HTBOTTOM:
			bottomRightPt.Offset(0, point.y-m_ptDrag.y);	
			break;
		case HTTOPLEFT:
			topLeftPt.Offset(point.x-m_ptDrag.x, point.y-m_ptDrag.y);
			break;
		case HTTOPRIGHT:
			{
				topLeftPt.Offset(0, point.y-m_ptDrag.y);
				bottomRightPt.Offset(point.x-m_ptDrag.x, 0);		
			}	
			break;
		case HTBOTTOMLEFT:
			{
				topLeftPt.Offset(point.x-m_ptDrag.x, 0);
				bottomRightPt.Offset(0, point.y-m_ptDrag.y);		
			}	
			break;
		case HTBOTTOMRIGHT:
			bottomRightPt.Offset(point.x-m_ptDrag.x, point.y-m_ptDrag.y);
			break;
		default :
			break;
		}

		m_bZoom = m_bDrag;
		if (m_bZoom)
		{
			rectWnd.SetRect(topLeftPt, bottomRightPt);	
			Move(rectWnd);
		}

		m_ptDrag = point;
		setFencesCursor(nStartZoomMouseLoc);
	}

	void SFencesView::setFencesCursor(UINT mouseLoc)
	{
		HCURSOR hcur = NULL;

		switch (mouseLoc)
		{
		case HTLEFT:		
			hcur = ::LoadCursor(NULL, IDC_SIZEWE);
			break;
		case HTRIGHT:	
			hcur = ::LoadCursor(NULL, IDC_SIZEWE);
			break;
		case HTTOP:
			hcur = ::LoadCursor(NULL, IDC_SIZENS);			
			break;
		case HTBOTTOM:
			hcur = ::LoadCursor(NULL, IDC_SIZENS);
			break;
		case HTTOPLEFT:
			hcur = ::LoadCursor(NULL, IDC_SIZENWSE);	
			break;
		case HTTOPRIGHT:
			hcur = ::LoadCursor(NULL, IDC_SIZENESW);	
			break;
		case HTBOTTOMLEFT:
			hcur = ::LoadCursor(NULL, IDC_SIZENESW);	
			break;
		case HTBOTTOMRIGHT:
			hcur = ::LoadCursor(NULL, IDC_SIZENWSE);	
			break;
		default :
			break;
		}

		if (hcur != NULL)
		{
			::SetCursor(hcur);			
		}
	}
}
