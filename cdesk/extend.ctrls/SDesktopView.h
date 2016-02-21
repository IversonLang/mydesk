#pragma once
#include <core/Swnd.h>
#include <control/SCmnCtrl.h>

#include <list>

namespace SOUI
{
	class SFencesView;

	class SDesktopView : public SWindow
	{
		SOUI_CLASS_NAME(SDesktopView, L"desktopview")
		friend class SFencesView;

	public: 
		SDesktopView();
		~SDesktopView();

	protected:
		virtual BOOL CreateChildren(pugi::xml_node xmlNode);
		virtual void UpdateChildrenPosition();

		int OnCreate(LPVOID);
		void OnDestroy();

		SOUI_MSG_MAP_BEGIN()
			MSG_WM_CREATE(OnCreate)
			MSG_WM_DESTROY(OnDestroy)
			SOUI_MSG_MAP_END()

			SOUI_ATTRS_BEGIN()
			SOUI_ATTRS_END()

	public:
		/**
		* addFencesView
		* @brief    添加栅栏
		* @return   BOOL 是否添加成功
		*
		* Describe  提供给外部添加栅栏接口
		*/
		bool addFencesView();

		/**
		* removeFencesView
		* @brief    移除栅栏
		* @return   BOOL 是否移除成功
		*
		* Describe  提供给外部移除栅栏接口
		*/
		bool removeFencesView();

	private:
		std::list<SFencesView *> m_fencesViewList; /**< 栅栏集合 */


	};

}

