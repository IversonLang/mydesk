#include "StdAfx.h"
#include "SDesktopView.h"
#include "SFencesView.h"

namespace SOUI
{

	SDesktopView::SDesktopView()
	{
		
	}

	SDesktopView::~SDesktopView()
	{

	}		

	int SDesktopView::OnCreate( LPVOID )
	{
		int nRet = __super::OnCreate(NULL);
		return nRet;
	}

	void SDesktopView::OnDestroy()
	{
		__super::OnDestroy();
	}


	BOOL SDesktopView::CreateChildren( pugi::xml_node xmlNode )
	{
		return TRUE;
	}

	void SDesktopView::UpdateChildrenPosition()
	{
		
	}

	bool SDesktopView::addFencesView()
	{
		return false;
	}

	bool SDesktopView::removeFencesView()
	{
		return false;
	}

}
