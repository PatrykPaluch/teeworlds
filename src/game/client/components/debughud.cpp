/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <engine/shared/config.h>
#include <engine/graphics.h>
#include <engine/textrender.h>

#include <generated/protocol.h>
#include <generated/client_data.h>

#include <game/layers.h>

#include <game/client/gameclient.h>
#include <game/client/animstate.h>
#include <game/client/render.h>

//#include "controls.h"
//#include "camera.h"
#include "debughud.h"

void CDebugHud::RenderNetCorrections()
{
	if(!g_Config.m_Debug || g_Config.m_DbgGraphs || !m_pClient->m_Snap.m_pLocalCharacter || !m_pClient->m_Snap.m_pLocalPrevCharacter)
		return;

	float Width = 300*Graphics()->ScreenAspect();
	Graphics()->MapScreen(0, 0, Width, 300);

	/*float speed = distance(vec2(netobjects.local_prev_character->x, netobjects.local_prev_character->y),
		vec2(netobjects.local_character->x, netobjects.local_character->y));*/

	float Velspeed = length(vec2(m_pClient->m_Snap.m_pLocalCharacter->m_VelX/256.0f, m_pClient->m_Snap.m_pLocalCharacter->m_VelY/256.0f))*50;
	float Ramp = VelocityRamp(Velspeed, m_pClient->m_Tuning.m_VelrampStart, m_pClient->m_Tuning.m_VelrampRange, m_pClient->m_Tuning.m_VelrampCurvature);

	const char *paStrings[] = {"velspeed:", "velspeed*ramp:", "ramp:", "Pos", " x:", " y:", "netmsg failed on:", "netobj num failures:", "netobj failed on:"};
	const int Num = sizeof(paStrings)/sizeof(char *);
	const float LineHeight = 6.0f;
	const float Fontsize = 5.0f;

	float x = Width-100.0f, y = 50.0f;
	for(int i = 0; i < Num; ++i)
		TextRender()->Text(0, x, y+i*LineHeight, Fontsize, paStrings[i], -1);

	x = Width-10.0f;
	char aBuf[128];
	str_format(aBuf, sizeof(aBuf), "%.0f", Velspeed/32);
	float w = TextRender()->TextWidth(0, Fontsize, aBuf, -1);
	TextRender()->Text(0, x-w, y, Fontsize, aBuf, -1);
	y += LineHeight;
	str_format(aBuf, sizeof(aBuf), "%.0f", Velspeed/32*Ramp);
	w = TextRender()->TextWidth(0, Fontsize, aBuf, -1);
	TextRender()->Text(0, x-w, y, Fontsize, aBuf, -1);
	y += LineHeight;
	str_format(aBuf, sizeof(aBuf), "%.2f", Ramp);
	w = TextRender()->TextWidth(0, Fontsize, aBuf, -1);
	TextRender()->Text(0, x-w, y, Fontsize, aBuf, -1);
	y += 2*LineHeight;
	str_format(aBuf, sizeof(aBuf), "%d", m_pClient->m_Snap.m_pLocalCharacter->m_X/32);
	w = TextRender()->TextWidth(0, Fontsize, aBuf, -1);
	TextRender()->Text(0, x-w, y, Fontsize, aBuf, -1);
	y += LineHeight;
	str_format(aBuf, sizeof(aBuf), "%d", m_pClient->m_Snap.m_pLocalCharacter->m_Y/32);
	w = TextRender()->TextWidth(0, Fontsize, aBuf, -1);
	TextRender()->Text(0, x-w, y, Fontsize, aBuf, -1);
	y += LineHeight;
	w = TextRender()->TextWidth(0, Fontsize, m_pClient->NetmsgFailedOn(), -1);
	TextRender()->Text(0, x-w, y, Fontsize, m_pClient->NetmsgFailedOn(), -1);
	y += LineHeight;
	str_format(aBuf, sizeof(aBuf), "%d", m_pClient->NetobjNumFailures());
	w = TextRender()->TextWidth(0, Fontsize, aBuf, -1);
	TextRender()->Text(0, x-w, y, Fontsize, aBuf, -1);
	y += LineHeight;
	w = TextRender()->TextWidth(0, Fontsize, m_pClient->NetobjFailedOn(), -1);
	TextRender()->Text(0, x-w, y, Fontsize, m_pClient->NetobjFailedOn(), -1);
}


void CDebugHud::MakeButton(CUIRect Area, const char* pStr, float x, float y, float w, float h)
{
	CUIRect Button = {Area.x + x, Area.y + y, w, h};
	// Area.VSplitLeft(w, &Button, &Area);
	// Area.HSplitTop(h, &Button, &Area);
	RenderTools()->DrawUIRect(&Button, vec4(0.9f, 0.9f, 0.9f, 0.66f) , CUI::CORNER_ALL, 4.0f);
  	TextRender()->TextOutlineColor(1.0f, 1.0f, 1.0f, 0.25f);
	TextRender()->TextColor(0.0f, 0.0f, 0.0f, 1.0f);
	Button.y += Button.h/2-4.0f;
	UI()->DoLabel(&Button, pStr, 6.0f, CUI::ALIGN_CENTER);
}

void CDebugHud::MakeLabel(CUIRect Area, const char* pStr, float x, float y, float w, float h)
{
	CUIRect Label = {Area.x + x, Area.y + y, w, h};
  	TextRender()->TextOutlineColor(0.0f, 0.0f, 0.0f, 0.3f);
	TextRender()->TextColor(1.0f, 1.0f, 1.0f, 1.0f);
	UI()->DoLabel(&Label, pStr, 6.0f, CUI::ALIGN_CENTER);
}

void CDebugHud::RenderTuning()
{
	// render tuning debugging
	if(!g_Config.m_DbgTuning)
	{
		const float Height = 300.0f;
		const float Width = Height*Graphics()->ScreenAspect();
		Graphics()->MapScreen(0.0f, 0.0f, Width, Height);

		CUIRect Area;
		Area.x = Width/6.f;
		Area.y = Height-70.0f;
		Area.w = 4*Width/6.f;
		Area.h = 50.f;
		RenderTools()->DrawUIRect(&Area, vec4(0.0f, 0.0f, 0.0f, 0.5f) , CUI::CORNER_ALL, 5.0f);
		
		Area.HMargin(10.0f, &Area);
		Area.VMargin(10.0f, &Area);

		const float ButtonSize = 12.0f;
		const float Spacing = ButtonSize+1.0f;
		float x = 10.f;
		float y = 10.f;

		MakeLabel(Area, Localize("Move"), x, y+Spacing, Spacing*2);
		MakeButton(Area, "Q", x, y);
		// x += Spacing;
		// MakeButton(Area, "Z", x, y-Spacing);
		x += Spacing;
		MakeButton(Area, "D", x, y);

		x+= Spacing*2;

		MakeButton(Area, "Space", x, y, ButtonSize*2.f);
		MakeLabel(Area, Localize("Jump"), x, y+Spacing, 2*ButtonSize);
		x+= ButtonSize*3;

		x+= Spacing;

		MakeLabel(Area, Localize("Change weapon"), x, y+Spacing, Spacing*5);
		MakeButton(Area, "1", x, y);
		x += Spacing;
		MakeButton(Area, "2", x, y);
		x += Spacing;
		MakeButton(Area, "3", x, y);
		x += Spacing;
		MakeButton(Area, "4", x, y);
		x += Spacing;
		MakeButton(Area, "5", x, y);
		x += Spacing;

		x+= Spacing;

		// const vec4 CRCWhite(1, 1, 1, 0.25);
		// const vec4 CRCTeam(0.4, 1, 0.4, 0.4);
		// const vec4 CRCWhisper(0, 0.5, 1, 0.5);


		MakeLabel(Area, Localize("Chat"), x, y+Spacing, Spacing*3);
		{
			CUIRect Button = {Area.x + x, Area.y + y-Spacing, 12.f, 12.f};
			Graphics()->TextureSet(g_pData->m_aImages[IMAGE_EMOTICONS].m_Id);
			Graphics()->QuadsBegin();
			RenderTools()->SelectSprite(SPRITE_DOTDOT);
			IGraphics::CQuadItem QuadItem(Button.x, Button.y, Button.w, Button.h);
			Graphics()->SetColor(1, 1, 1, 0.25);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
			Graphics()->QuadsEnd();
		}
		MakeButton(Area, "T", x, y);
		x += Spacing;			
		// MakeLabel(Area, Localize("Team"), x, y-Spacing, Spacing);
		{
			CUIRect Button = {Area.x + x, Area.y + y-Spacing, 12.f, 12.f};
			Graphics()->TextureSet(g_pData->m_aImages[IMAGE_EMOTICONS].m_Id);
			Graphics()->QuadsBegin();
			RenderTools()->SelectSprite(SPRITE_DOTDOT);
			IGraphics::CQuadItem QuadItem(Button.x, Button.y, Button.w, Button.h);
			Graphics()->SetColor(0.4, 1, 0.4, 0.4);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
			Graphics()->QuadsEnd();
		}
		MakeButton(Area, "Y", x, y);
		x += Spacing;
		// MakeLabel(Area, Localize("Whisper"), x, y-Spacing, Spacing);
		{
			CUIRect Button = {Area.x + x, Area.y + y-Spacing, 12.f, 12.f};
			Graphics()->TextureSet(g_pData->m_aImages[IMAGE_CHATWHISPER].m_Id);
			Graphics()->QuadsBegin();
			Graphics()->QuadsSetSubset(1, 0, 0, 1);			// QuadIcon = IGraphics::CQuadItem(Area.x + x, Area.y + y+Spacing, 12.f, 12.f);
			IGraphics::CQuadItem QuadItem(Button.x, Button.y+6.f, Button.w, Button.h/2.f);
			Graphics()->SetColor(0, 0.5, 1, 0.5);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
			Graphics()->QuadsEnd();
		}

		MakeButton(Area, "X", x, y);
		x += Spacing;

		x += Spacing;

		MakeLabel(Area, Localize("Menu"), x, y+Spacing, Spacing);
		MakeButton(Area, "Esc", x, y);
		x += Spacing;
		
  		TextRender()->TextOutlineColor(0.0f, 0.0f, 0.0f, 0.3f);
		TextRender()->TextColor(1.0f, 1.0f, 1.0f, 1.0f);
		return;
	}

	CTuningParams StandardTuning;

	Graphics()->MapScreen(0, 0, 300*Graphics()->ScreenAspect(), 300);

	float y = 50.0f;
	int Count = 0;
	for(int i = 0; i < m_pClient->m_Tuning.Num(); i++)
	{
		char aBuf[128];
		float Current, Standard;
		m_pClient->m_Tuning.Get(i, &Current);
		StandardTuning.Get(i, &Standard);

		if(Standard == Current)
			TextRender()->TextColor(1,1,1,1.0f);
		else
			TextRender()->TextColor(1,0.25f,0.25f,1.0f);

		float w;
		float x = 5.0f;

		str_format(aBuf, sizeof(aBuf), "%.2f", Standard);
		x += 20.0f;
		w = TextRender()->TextWidth(0, 5, aBuf, -1);
		TextRender()->Text(0x0, x-w, y+Count*6, 5, aBuf, -1);

		str_format(aBuf, sizeof(aBuf), "%.2f", Current);
		x += 20.0f;
		w = TextRender()->TextWidth(0, 5, aBuf, -1);
		TextRender()->Text(0x0, x-w, y+Count*6, 5, aBuf, -1);

		x += 5.0f;
		TextRender()->Text(0x0, x, y+Count*6, 5, m_pClient->m_Tuning.m_apNames[i], -1);

		Count++;
	}

	y = y+Count*6;

	Graphics()->TextureClear();
	Graphics()->BlendNormal();
	Graphics()->LinesBegin();
	float Height = 50.0f;
	float pv = 1;
	IGraphics::CLineItem Array[100];
	for(int i = 0; i < 100; i++)
	{
		float Speed = i/100.0f * 3000;
		float Ramp = VelocityRamp(Speed, m_pClient->m_Tuning.m_VelrampStart, m_pClient->m_Tuning.m_VelrampRange, m_pClient->m_Tuning.m_VelrampCurvature);
		float RampedSpeed = (Speed * Ramp)/1000.0f;
		Array[i] = IGraphics::CLineItem((i-1)*2, y+Height-pv*Height, i*2, y+Height-RampedSpeed*Height);
		//Graphics()->LinesDraw((i-1)*2, 200, i*2, 200);
		pv = RampedSpeed;
	}
	Graphics()->LinesDraw(Array, 100);
	Graphics()->LinesEnd();
	TextRender()->TextColor(1,1,1,1);
}

void CDebugHud::OnRender()
{
	RenderTuning();
	RenderNetCorrections();
}
