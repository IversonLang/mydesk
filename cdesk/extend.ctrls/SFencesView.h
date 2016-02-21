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
		* @brief    �϶�����
		* @param    CRect rectWnd --  ���ڵ�ǰλ��
		* @param    CPoint point  --  ���λ��
		* @void
		*
		* Describe  �϶�����ʵ��
		*/
		void dragFences(CRect rectWnd, CPoint point);

		/**
		* zoomFences
		* @brief    ���Ŵ���
		* @param    CRect rectWnd --  ���ڵ�ǰλ��
		* @param    CPoint point  --  ��������Ļλ��
		* @param    UINT mouseLoc --  ������ڴ���λ��
		* @void
		*
		* Describe  ���Ŵ���ʵ��
		*/
		void zoomFences(CRect rectWnd, CPoint point, UINT mouseLoc);

		void setFencesCursor(UINT mouseLoc);


	private:
		CPoint  m_ptDrag;                 /**< ����������λ��  */ 
		bool    m_bDrag;                  /**< �����Ƿ����ڱ��϶� */
		bool    m_bZoom;                  /**< �����Ƿ��������� */
		UINT    m_nStartZoomMouseLoc;     /**< ��ʼ����ʱ,������ڴ���λ�� */
		SDesktopView *m_pHost;            /**< ������ʵ�� */

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

