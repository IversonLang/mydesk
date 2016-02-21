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
		* @brief    ���դ��
		* @return   BOOL �Ƿ���ӳɹ�
		*
		* Describe  �ṩ���ⲿ���դ���ӿ�
		*/
		bool addFencesView();

		/**
		* removeFencesView
		* @brief    �Ƴ�դ��
		* @return   BOOL �Ƿ��Ƴ��ɹ�
		*
		* Describe  �ṩ���ⲿ�Ƴ�դ���ӿ�
		*/
		bool removeFencesView();

	private:
		std::list<SFencesView *> m_fencesViewList; /**< դ������ */


	};

}

