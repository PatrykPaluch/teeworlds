/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_CLIENT_COMPONENTS_DEBUGHUD_H
#define GAME_CLIENT_COMPONENTS_DEBUGHUD_H
#include <game/client/component.h>

class CDebugHud : public CComponent
{
	void RenderNetCorrections();
	void RenderTuning();
public:
	virtual void OnRender();
	void MakeButton(CUIRect Area, const char* pStr, float x, float y, float w = 12.f, float h = 12.f);
	void MakeLabel(CUIRect Area, const char* pStr, float x, float y, float w = 50.f, float h = 12.f);
};

#endif
