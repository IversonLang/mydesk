#pragma once
#include <core/Swnd.h>
#include <control/SCmnCtrl.h>

#include "SDesktopView.h"

namespace SOUI
{

	class SFencesView : public SWindow
	{
		SOUI_CLASS_NAME(SFencesView, L"fencesview")
		friend class SDesktopView;

	public:
		SFencesView(SDesktopView *pHost = NULL);
		~SFencesView();

	public:
		SOUI_MSG_MAP_BEGIN()
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			SOUI_MSG_MAP_END()

	protected:		
		void OnLButtonDown(UINT nFlags, CPoint point);
		void OnLButtonUp(UINT nFlags, CPoint point);
		void OnMouseMove(UINT nFlags, CPoint point);
		void OnPaint( IRenderTarget * pRT );

		UINT OnWndNcHitTest(CPoint point);

	private:
		bool isDragable(UINT nFlags);

		/**
		* dragFences
		* @brief    拖动窗口
		* @param    CRect rectWnd --  窗口当前位置
		* @param    CPoint point  --  鼠标位置
		* @void
		*
		* Describe  拖动窗口实现
		*/
		void dragFences(CRect rectWnd, CPoint point);

		/**
		* zoomFences
		* @brief    缩放窗口
		* @param    CRect rectWnd --  窗口当前位置
		* @param    CPoint point  --  鼠标相对屏幕位置
		* @param    UINT mouseLoc --  鼠标所在窗口位置
		* @void
		*
		* Describe  缩放窗口实现
		*/
		void zoomFences(CRect rectWnd, CPoint point, UINT mouseLoc);

		void setFencesCursor(UINT mouseLoc);


	private:
		CPoint  m_ptDrag;                 /**< 鼠标左键按下位置  */ 
		bool    m_bDrag;                  /**< 窗口是否正在被拖动 */
		bool    m_bZoom;                  /**< 窗口是否正在缩放 */
		UINT    m_nStartZoomMouseLoc;     /**< 开始缩放时,鼠标所在窗口位置 */
		SDesktopView *m_pHost;            /**< 父窗口实例 */

	};

	class SFencesViewEx : public SFencesView
	{
		SOUI_CLASS_NAME(SFencesViewEx, L"fencesviewex")
	public:
		SFencesViewEx()
		{

		}
	};
}

