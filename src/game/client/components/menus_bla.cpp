/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <base/color.h>
#include <base/math.h>

#include <engine/engine.h>
#include <engine/graphics.h>
#include <engine/storage.h>
#include <engine/textrender.h>
#include <engine/external/json-parser/json.h>
#include <engine/shared/config.h>

#include <generated/protocol.h>
#include <generated/client_data.h>

#include <game/client/components/sounds.h>
#include <game/client/ui.h>
#include <game/client/render.h>
#include <game/client/gameclient.h>
#include <game/client/animstate.h>
#include <game/version.h>

#include "binds.h"
#include "countryflags.h"
#include "menus.h"
#include "skins.h"
#include "gskins.h"
#include "pskins.h"
#include "eskins.h"
#include "cskins.h"

void CMenus::RenderBlaSettingsGeneral(CUIRect MainView)
{
    CUIRect Label, Button, Functions, Effect, BottomView;
	// cut view
	MainView.HSplitBottom(80.0f, &MainView, &BottomView);
	BottomView.HSplitTop(20.f, 0, &BottomView);
	// render game menu backgrounds
	int NumOptions = 8.0f;
	float ButtonHeight = 20.0f;
	float Spacing = 2.0f;
	float BackgroundHeight = (float)(NumOptions+1)*ButtonHeight+(float)NumOptions*Spacing;
	MainView.HSplitTop(20.0f, 0, &MainView);
	MainView.HSplitTop(BackgroundHeight, &Functions, &MainView);
	RenderTools()->DrawUIRect(&Functions, vec4(0.0f, 0.0f, 0.0f, 0.25f), CUI::CORNER_ALL, 5.0f);

	MainView.HSplitTop(10.0f, 0, &MainView);
	MainView.HSplitTop(BackgroundHeight, &Effect, &MainView);
	RenderTools()->DrawUIRect(&Effect, vec4(0.0f, 0.0f, 0.0f, 0.25f), CUI::CORNER_ALL, 5.0f);
	// render game menu
	Functions.HSplitTop(ButtonHeight, &Label, &Functions);
	Label.y += 2.0f;
	UI()->DoLabel(&Label, Localize("Functions"), ButtonHeight*ms_FontmodHeight*0.8f, CUI::ALIGN_CENTER);

    Functions.HSplitTop(Spacing, 0, &Functions);
	Functions.HSplitTop(ButtonHeight, &Button, &Functions);
	static CButtonContainer s_Bdadash;
	if(DoButton_CheckBox(&s_Bdadash, Localize("Bdadash feature (Pew Pew!)"), g_Config.m_Bdadash, &Button))
		g_Config.m_Bdadash ^= 1;

	Functions.HSplitTop(Spacing, 0, &Functions);
	Functions.HSplitTop(ButtonHeight, &Button, &Functions);
    static CButtonContainer s_Admin;
	if(DoButton_CheckBox(&s_Admin, Localize("Show admin icon in scoreboard"), g_Config.m_AdminIcon, &Button))
		g_Config.m_AdminIcon ^= 1;

	Functions.HSplitTop(Spacing, 0, &Functions);
	Functions.HSplitTop(ButtonHeight, &Button, &Functions);
	static CButtonContainer s_HealthBar;
	if(DoButton_CheckBox(&s_HealthBar, Localize("Show health & ammo bar over tee "), g_Config.m_HealthBar, &Button))
		g_Config.m_HealthBar ^= 1;

    Functions.HSplitTop(Spacing, 0, &Functions);
	Functions.HSplitTop(ButtonHeight, &Button, &Functions);
	static CButtonContainer s_GamerHUD;
	if(DoButton_CheckBox(&s_GamerHUD, Localize("Gamer HUD"), g_Config.m_ClGhud, &Button))
		g_Config.m_ClGhud ^= 1;

	Functions.HSplitTop(Spacing, 0, &Functions);
	Functions.HSplitTop(ButtonHeight, &Button, &Functions);

	// render effect menu
	Effect.HSplitTop(ButtonHeight, &Label, &Effect);
	Label.y += 2.0f;
	UI()->DoLabel(&Label, Localize("Effects"), ButtonHeight*ms_FontmodHeight*0.8f, CUI::ALIGN_CENTER);
	Effect.HSplitTop(Spacing, 0, &Effect);
	Effect.HSplitTop(ButtonHeight, &Button, &Effect);
	static CButtonContainer s_Blood;
	if(DoButton_CheckBox(&s_Blood, Localize("Death blood"), g_Config.m_Blood, &Button))
		g_Config.m_Blood ^= 1;

    Effect.HSplitTop(Spacing, 0, &Effect);
    Effect.HSplitTop(ButtonHeight, &Button, &Effect);
    Button.VSplitLeft(ButtonHeight, 0, &Button);
    static CButtonContainer s_BloodDamage;
    if(DoButton_CheckBox(&s_BloodDamage, Localize("Damage blood"), g_Config.m_BloodDamage, &Button))
        g_Config.m_BloodDamage ^= 1;

	Effect.HSplitTop(Spacing, 0, &Effect);
	Effect.HSplitTop(ButtonHeight, &Button, &Effect);
	static int s_RainbowBody = 0;
	if(DoButton_CheckBox(&s_RainbowBody, Localize("Rainbow body"), g_Config.m_ClRainbowBody, &Button))
		g_Config.m_ClRainbowBody ^= 1;

	Effect.HSplitTop(Spacing, 0, &Effect);
	Effect.HSplitTop(ButtonHeight, &Button, &Effect);
	static int s_RainbowFeet = 0;
	if(DoButton_CheckBox(&s_RainbowFeet, Localize("Rainbow feet"), g_Config.m_ClRainbowFeet, &Button))
		g_Config.m_ClRainbowFeet ^= 1;

	Effect.HSplitTop(Spacing, 0, &Effect);
	Effect.HSplitTop(ButtonHeight, &Button, &Effect);
	static int s_RainbowHands = 0;
	if(DoButton_CheckBox(&s_RainbowHands, Localize("Rainbow hands"), g_Config.m_ClRainbowHands, &Button))
		g_Config.m_ClRainbowHands ^= 1;

	Effect.HSplitTop(Spacing, 0, &Effect);
	Effect.HSplitTop(ButtonHeight, &Button, &Effect);
	static int s_RainbowDecoration = 0;
	if(DoButton_CheckBox(&s_RainbowDecoration, Localize("Rainbow decoration"), g_Config.m_ClRainbowDecoration, &Button))
		g_Config.m_ClRainbowDecoration ^= 1;

	Effect.HSplitTop(Spacing, 0, &Effect);
	Effect.HSplitTop(ButtonHeight, &Button, &Effect);
	static int s_RainbowMarking = 0;
	if(DoButton_CheckBox(&s_RainbowMarking, Localize("Rainbow marking"), g_Config.m_ClRainbowMarking, &Button))
		g_Config.m_ClRainbowMarking ^= 1;

    Effect.HSplitTop(Spacing, 0, &Effect);
	Effect.HSplitTop(ButtonHeight, &Button, &Effect);
	static int s_Lightsabers = 0;
	if(DoButton_CheckBox(&s_Lightsabers, Localize("Lightsabers"), g_Config.m_ClYoda, &Button))
		g_Config.m_ClYoda ^= 1;

	MainView.HSplitTop(10.0f, 0, &MainView);

}
 void CMenus::RenderBlaSettingsExtras(CUIRect MainView)
{
    CUIRect Label, Button, Extras, BottomView;
	// cut view
	MainView.HSplitBottom(80.0f, &MainView, &BottomView);
	BottomView.HSplitTop(20.f, 0, &BottomView);
	// render game menu backgrounds
	int NumOptions = 17.0f;
	float ButtonHeight = 20.0f;
	float Spacing = 2.0f;
	float BackgroundHeight = (float)(NumOptions+1)*ButtonHeight+(float)NumOptions*Spacing;
	MainView.HSplitTop(20.0f, 0, &MainView);
	MainView.HSplitTop(BackgroundHeight, &Extras, &MainView);
	RenderTools()->DrawUIRect(&Extras, vec4(0.0f, 0.0f, 0.0f, 0.25f), CUI::CORNER_ALL, 5.0f);

	// render extra menu
	Extras.HSplitTop(ButtonHeight, &Label, &Extras);
	Label.y += 2.0f;
	UI()->DoLabel(&Label, Localize("Extras"), ButtonHeight*ms_FontmodHeight*0.8f, CUI::ALIGN_CENTER);
    Extras.HSplitTop(Spacing, 0, &Extras);
	Extras.HSplitTop(ButtonHeight, &Button, &Extras);
	static CButtonContainer s_ChatFeatures;
	if(DoButton_CheckBox(&s_ChatFeatures, Localize("Eye candy chat"), g_Config.m_ChatFeatures, &Button))
		g_Config.m_ChatFeatures ^= 1;
	Extras.HSplitTop(Spacing, 0, &Extras);
	Extras.HSplitTop(ButtonHeight, &Button, &Extras);
	static CButtonContainer s_DisableQuads;
	if(DoButton_CheckBox(&s_DisableQuads, Localize("Disable quads"), g_Config.m_DisableQuads, &Button))
		g_Config.m_DisableQuads ^= 1;
	Extras.HSplitTop(Spacing, 0, &Extras);
	Extras.HSplitTop(ButtonHeight, &Button, &Extras);
	static CButtonContainer s_RenderTime;
	if(DoButton_CheckBox(&s_RenderTime, Localize("Show local time ingame"), g_Config.m_RenderTime, &Button))
		g_Config.m_RenderTime ^= 1;
	Extras.HSplitTop(Spacing, 0, &Extras);
	Extras.HSplitTop(ButtonHeight, &Button, &Extras);
}

void CMenus::RenderBlaSettingsHud(CUIRect MainView)
{
    CUIRect Label, Button, General, HUD, BottomView;
	// cut view
	MainView.HSplitBottom(80.0f, &MainView, &BottomView);
	BottomView.HSplitTop(20.f, 0, &BottomView);
	// render game menu backgrounds
	int NumOptions = 8.0f;
	float ButtonHeight = 20.0f;
	float Spacing = 2.0f;
	float BackgroundHeight = (float)(NumOptions+1)*ButtonHeight+(float)NumOptions*Spacing;
	MainView.HSplitTop(20.0f, 0, &MainView);
	MainView.HSplitTop(BackgroundHeight, &General, &MainView);
	RenderTools()->DrawUIRect(&General, vec4(0.0f, 0.0f, 0.0f, 0.25f), CUI::CORNER_ALL, 5.0f);
	MainView.HSplitTop(10.0f, 0, &MainView);
	MainView.HSplitTop(BackgroundHeight, &HUD, &MainView);
	RenderTools()->DrawUIRect(&HUD, vec4(0.0f, 0.0f, 0.0f, 0.25f), CUI::CORNER_ALL, 5.0f);

	// render general menu
	General.HSplitTop(ButtonHeight, &Label, &General);
	Label.y += 2.0f;
	UI()->DoLabel(&Label, Localize("General"), ButtonHeight*ms_FontmodHeight*0.8f, CUI::ALIGN_CENTER);
    General.HSplitTop(Spacing, 0, &General);
	General.HSplitTop(ButtonHeight, &Button, &General);
	static CButtonContainer s_gChat;
	if(DoButton_CheckBox(&s_gChat, Localize("Render chat if menu is active"), g_Config.m_RenderChatIfMenu, &Button))
		g_Config.m_RenderChatIfMenu ^= 1;
    General.HSplitTop(Spacing, 0, &General);
	General.HSplitTop(ButtonHeight, &Button, &General);
	static CButtonContainer s_gMenu;
	if(DoButton_CheckBox(&s_gMenu, Localize("Render hud if menu is active"), g_Config.m_RenderHudIfMenu, &Button))
		g_Config.m_RenderHudIfMenu ^= 1;


    // render hud menu
	HUD.HSplitTop(ButtonHeight, &Label, &HUD);
	Label.y += 2.0f;
	UI()->DoLabel(&Label, Localize("Nothing"), ButtonHeight*ms_FontmodHeight*0.8f, CUI::ALIGN_CENTER);
	HUD.HSplitTop(Spacing, 0, &HUD);
	HUD.HSplitTop(ButtonHeight, &Button, &HUD);
}
void CMenus::RenderBlaSettingsDummy(CUIRect MainView)
{
    CUIRect Label, Text, Button, Dummy, BottomView;
	// cut view
	MainView.HSplitBottom(80.0f, &MainView, &BottomView);
	BottomView.HSplitTop(20.f, 0, &BottomView);
	// render game menu backgrounds
	int NumOptions = 17.0f;
	float ButtonHeight = 20.0f;
	float Spacing = 2.0f;
	float BackgroundHeight = (float)(NumOptions+1)*ButtonHeight+(float)NumOptions*Spacing;
	MainView.HSplitTop(20.0f, 0, &MainView);
	MainView.HSplitTop(BackgroundHeight, &Dummy, &MainView);
	RenderTools()->DrawUIRect(&Dummy, vec4(0.0f, 0.0f, 0.0f, 0.25f), CUI::CORNER_ALL, 5.0f);
	MainView.HSplitTop(10.0f, 0, &MainView);

	// render general menu
	Dummy.HSplitTop(ButtonHeight, &Label, &Dummy);
	Label.y += 2.0f;
	UI()->DoLabel(&Label, Localize("Placeholder"), ButtonHeight*ms_FontmodHeight*0.8f, CUI::ALIGN_CENTER);
}
void CMenus::RenderBlaSettingsTexture(CUIRect MainView)
{
    CUIRect Button, Left, Right, TopView, Label, TabBar;

	static int s_ControlPage = 0;
	// render page
	if(s_ControlPage == 0)
		g_Config.m_Texture = 0;
	else if(s_ControlPage == 1)
		g_Config.m_Texture = 1;
	else if(s_ControlPage == 2)
		g_Config.m_Texture = 2;
	else if(s_ControlPage == 3)
		g_Config.m_Texture = 3;
	// render game menu backgrounds
	float ButtonHeight = 20.0f;
	float Spacing = 2.0f;
	float BackgroundHeight = 2.0f*ButtonHeight+Spacing;
	MainView.HSplitTop(20.0f, 0, &MainView);
	MainView.HSplitBottom(80.0f, &MainView, 0);
    MainView.HSplitTop(20.0f, &TabBar, &MainView);
	MainView.Margin(10.0f, &MainView);
	// tab bar
	{
		TabBar.VSplitLeft(TabBar.w/4, &Button, &TabBar);
		static CButtonContainer s_Button0;
		if(DoButton_MenuTab(&s_Button0, Localize("Gameskin"), s_ControlPage == 0, &Button, 0))
			s_ControlPage = 0;
		TabBar.VSplitLeft(TabBar.w/3, &Button, &TabBar);
		static CButtonContainer s_Button1;
		if(DoButton_MenuTab(&s_Button1, Localize("Emoticons"), s_ControlPage == 1, &Button, 0))
			s_ControlPage = 1;
		TabBar.VSplitMid(&Button, &TabBar);
		static CButtonContainer s_Button2;
		if(DoButton_MenuTab(&s_Button2, Localize("Cursor"), s_ControlPage == 2, &Button, 0))
			s_ControlPage = 2;
		static CButtonContainer s_Button3;
		if(DoButton_MenuTab(&s_Button3, Localize("Particles"), s_ControlPage == 3, &TabBar, 0))
			s_ControlPage = 3;
	}

  if(g_Config.m_Texture == 0)
  {
	static bool s_InitSkinlist = true;
	static sorted_array<const CgSkins::CgSkin *> s_paSkinList;
	if(s_InitSkinlist)
	{
		s_paSkinList.clear();
		for(int i = 0; i < m_pClient->m_pgSkins->Num(); ++i)
		{
			const CgSkins::CgSkin *s = m_pClient->m_pgSkins->Get(i);
			// no special skins
			if(s->m_aName[0] == 'x' && s->m_aName[1] == '_')
				continue;
			s_paSkinList.add(s);
		}
		s_InitSkinlist = false;
	}
	MainView.HSplitTop(10.0f, 0, &MainView);
	static float s_ScrollValue = 0.0f;
	int OldSelected = -1;
	UiDoListboxHeader(&MainView, Localize("Texture"), 20.0f, 2.0f);
	UiDoListboxStart(&s_ScrollValue, 160.0f, 0, s_paSkinList.size(), 3, OldSelected, s_ScrollValue);
    for(int i = 0; i < s_paSkinList.size(); ++i)
	{
		const CgSkins::CgSkin *s = s_paSkinList[i];
		if(s == 0)
			continue;
		if(str_comp(s->m_aName, g_Config.m_GameTexture) == 0)
			OldSelected = i;
		CListboxItem Item = UiDoListboxNextItem(&s_paSkinList[i], OldSelected == i);
		if(Item.m_Visible)
		{
			CUIRect Label;
			Item.m_Rect.Margin(5.0f, &Item.m_Rect);
			Item.m_Rect.HSplitBottom(10.0f, &Item.m_Rect, &Label);
			IGraphics::CTextureHandle m_Texture;
			char gName[512];
			str_format(gName, sizeof(gName), "%s", s->m_aName);
			Graphics()->TextureSet(s_paSkinList[i]->m_Texture);
			Graphics()->QuadsBegin();
			IGraphics::CQuadItem QuadItem(Item.m_Rect.x+Item.m_Rect.w/2 - 120.0f, Item.m_Rect.y+Item.m_Rect.h/2 - 60.0f, 240.0f, 120.0f);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
			Graphics()->QuadsEnd();
	        UI()->DoLabel(&Label, gName, 10.0f, CUI::ALIGN_CENTER);
		}
	}
	const int NewSelected = UiDoListboxEnd(&s_ScrollValue, 0);
	if(OldSelected != NewSelected)
	{
		mem_copy(g_Config.m_GameTexture, s_paSkinList[NewSelected]->m_aName, sizeof(g_Config.m_GameTexture));
		g_pData->m_aImages[IMAGE_GAME].m_Id = s_paSkinList[NewSelected]->m_Texture;
	}
  }

  else if(g_Config.m_Texture == 1)
  {
	static bool s_InitSkinlist = true;
	static sorted_array<const CeSkins::CeSkin *> s_paSkinList;
	if(s_InitSkinlist)
	{
		s_paSkinList.clear();
		for(int i = 0; i < m_pClient->m_peSkins->Num(); ++i)
		{
			const CeSkins::CeSkin *s = m_pClient->m_peSkins->Get(i);
			// no special skins
			if(s->m_aName[0] == 'x' && s->m_aName[1] == '_')
				continue;
			s_paSkinList.add(s);
		}
		s_InitSkinlist = false;
	}
	MainView.HSplitTop(10.0f, 0, &MainView);
	static float s_ScrollValue = 0.0f;
	int OldSelected = -1;
	UiDoListboxHeader(&MainView, Localize("Emoticons"), 20.0f, 2.0f);
	UiDoListboxStart(&s_ScrollValue, 160.0f, 0, s_paSkinList.size(), 3, OldSelected, s_ScrollValue);
    for(int i = 0; i < s_paSkinList.size(); ++i)
	{
		const CeSkins::CeSkin *s = s_paSkinList[i];
		if(s == 0)
			continue;
		if(str_comp(s->m_aName, g_Config.m_GameEmoticons) == 0)
			OldSelected = i;
		CListboxItem Item = UiDoListboxNextItem(&s_paSkinList[i], OldSelected == i);
		if(Item.m_Visible)
		{
			CUIRect Label;
			Item.m_Rect.Margin(5.0f, &Item.m_Rect);
			Item.m_Rect.HSplitBottom(10.0f, &Item.m_Rect, &Label);
			IGraphics::CTextureHandle m_Texture;
			char gName[512];
			str_format(gName, sizeof(gName), "%s", s->m_aName);
			Graphics()->TextureSet(s_paSkinList[i]->m_Texture);
			Graphics()->QuadsBegin();
			IGraphics::CQuadItem QuadItem(Item.m_Rect.x+Item.m_Rect.w/2 - 60.0f, Item.m_Rect.y+Item.m_Rect.h/2 - 60.0f, 120.0f, 120.0f);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
			Graphics()->QuadsEnd();
	        UI()->DoLabel(&Label, gName, 10.0f, CUI::ALIGN_CENTER);
		}
	}
	const int NewSelected = UiDoListboxEnd(&s_ScrollValue, 0);
	if(OldSelected != NewSelected)
	{
	    mem_copy(g_Config.m_GameEmoticons, s_paSkinList[NewSelected]->m_aName, sizeof(g_Config.m_GameEmoticons));
		g_pData->m_aImages[IMAGE_EMOTICONS].m_Id = s_paSkinList[NewSelected]->m_Texture;
	}
  }

  else if(g_Config.m_Texture == 2)
  {
	static bool s_InitSkinlist = true;
	static sorted_array<const CcSkins::CcSkin *> s_paSkinList;
	if(s_InitSkinlist)
	{
		s_paSkinList.clear();
		for(int i = 0; i < m_pClient->m_pcSkins->Num(); ++i)
		{
			const CcSkins::CcSkin *s = m_pClient->m_pcSkins->Get(i);
			// no special skins
			if(s->m_aName[0] == 'x' && s->m_aName[1] == '_')
				continue;
			s_paSkinList.add(s);
		}
		s_InitSkinlist = false;
	}
	MainView.HSplitTop(10.0f, 0, &MainView);
	static float s_ScrollValue = 0.0f;
	int OldSelected = -1;
	UiDoListboxHeader(&MainView, Localize("Cursor"), 20.0f, 2.0f);
	UiDoListboxStart(&s_ScrollValue, 160.0f, 0, s_paSkinList.size(), 3, OldSelected, s_ScrollValue);
    for(int i = 0; i < s_paSkinList.size(); ++i)
	{
		const CcSkins::CcSkin *s = s_paSkinList[i];
		if(s == 0)
			continue;
		if(str_comp(s->m_aName, g_Config.m_GameCursor) == 0)
			OldSelected = i;
		CListboxItem Item = UiDoListboxNextItem(&s_paSkinList[i], OldSelected == i);
		if(Item.m_Visible)
		{
				CUIRect Label;
			Item.m_Rect.Margin(5.0f, &Item.m_Rect);
			Item.m_Rect.HSplitBottom(10.0f, &Item.m_Rect, &Label);
			IGraphics::CTextureHandle m_Texture;
			char gName[512];
			str_format(gName, sizeof(gName), "%s", s->m_aName);
			Graphics()->TextureSet(s_paSkinList[i]->m_Texture);
			Graphics()->QuadsBegin();
			IGraphics::CQuadItem QuadItem(Item.m_Rect.x+Item.m_Rect.w/2 - 60.0f, Item.m_Rect.y+Item.m_Rect.h/2 - 60.0f, 120.0f, 120.0f);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
			Graphics()->QuadsEnd();
	        UI()->DoLabel(&Label, gName, 10.0f, CUI::ALIGN_CENTER);
		}
	}
	const int NewSelected = UiDoListboxEnd(&s_ScrollValue, 0);
	if(OldSelected != NewSelected)
	{
	    mem_copy(g_Config.m_GameCursor, s_paSkinList[NewSelected]->m_aName, sizeof(g_Config.m_GameCursor));
		g_pData->m_aImages[IMAGE_CURSOR].m_Id = s_paSkinList[NewSelected]->m_Texture;
	}
}

  else if(g_Config.m_Texture == 3)
  {
	static bool s_InitSkinlist = true;
	static sorted_array<const CpSkins::CpSkin *> s_paSkinList;
	if(s_InitSkinlist)
	{
		s_paSkinList.clear();
		for(int i = 0; i < m_pClient->m_ppSkins->Num(); ++i)
		{
			const CpSkins::CpSkin *s = m_pClient->m_ppSkins->Get(i);
			// no special skins
			if(s->m_aName[0] == 'x' && s->m_aName[1] == '_')
				continue;
			s_paSkinList.add(s);
		}
		s_InitSkinlist = false;
	}
	MainView.HSplitTop(10.0f, 0, &MainView);
	static float s_ScrollValue = 0.0f;
	int OldSelected = -1;
	UiDoListboxHeader(&MainView, Localize("Particles"), 20.0f, 2.0f);
	UiDoListboxStart(&s_ScrollValue, 160.0f, 0, s_paSkinList.size(), 3, OldSelected, s_ScrollValue);
    for(int i = 0; i < s_paSkinList.size(); ++i)
	{
		const CpSkins::CpSkin *s = s_paSkinList[i];
		if(s == 0)
			continue;
		if(str_comp(s->m_aName, g_Config.m_GameParticles) == 0)
			OldSelected = i;
		CListboxItem Item = UiDoListboxNextItem(&s_paSkinList[i], OldSelected == i);
		if(Item.m_Visible)
		{
				CUIRect Label;
			Item.m_Rect.Margin(5.0f, &Item.m_Rect);
			Item.m_Rect.HSplitBottom(10.0f, &Item.m_Rect, &Label);
			IGraphics::CTextureHandle m_Texture;
			char gName[512];
			str_format(gName, sizeof(gName), "%s", s->m_aName);
			Graphics()->TextureSet(s_paSkinList[i]->m_Texture);
			Graphics()->QuadsBegin();
			IGraphics::CQuadItem QuadItem(Item.m_Rect.x+Item.m_Rect.w/2 - 60.0f, Item.m_Rect.y+Item.m_Rect.h/2 - 60.0f, 120.0f, 120.0f);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
			Graphics()->QuadsEnd();
	        UI()->DoLabel(&Label, gName, 10.0f, CUI::ALIGN_CENTER);
		}
	}
	const int NewSelected = UiDoListboxEnd(&s_ScrollValue, 0);
	if(OldSelected != NewSelected)
	{
	   mem_copy(g_Config.m_GameParticles, s_paSkinList[NewSelected]->m_aName, sizeof(g_Config.m_GameParticles));
	   g_pData->m_aImages[IMAGE_PARTICLES].m_Id = s_paSkinList[NewSelected]->m_Texture;
	}
  }
}
void CMenus::RenderBlaSettingsInfo(CUIRect MainView)
{
    CUIRect Label, Info, BottomView;
	// cut view
	MainView.HSplitBottom(80.0f, &MainView, &BottomView);
	BottomView.HSplitTop(20.f, 0, &BottomView);
	// render game menu backgrounds
	int NumOptions = 17.0f;
	float ButtonHeight = 20.0f;
	float Spacing = 2.0f;
	float BackgroundHeight = (float)(NumOptions+1)*ButtonHeight+(float)NumOptions*Spacing;
	MainView.HSplitTop(20.0f, 0, &MainView);
	MainView.HSplitTop(BackgroundHeight, &Info, &MainView);
	RenderTools()->DrawUIRect(&Info, vec4(0.0f, 0.0f, 0.0f, 0.25f), CUI::CORNER_ALL, 5.0f);
	MainView.HSplitTop(10.0f, 0, &MainView);

	// render general menu
	Info.HSplitTop(ButtonHeight, &Label, &Info);
	Label.y += 2.0f;
	UI()->DoLabel(&Label, Localize("Info"), ButtonHeight*ms_FontmodHeight*0.8f, CUI::ALIGN_CENTER);
    char aBuf[32];
	str_format(aBuf, sizeof(aBuf), "\n\n%s %s %s", Localize("Compiled"), __DATE__, __TIME__, BLA_VERSION);
	UI()->DoLabel(&Label, aBuf, ButtonHeight*ms_FontmodHeight*0.8f, CUI::ALIGN_CENTER);
    char aClient[64];
    Label.y += 20.0f;
	str_format(aClient, sizeof(aClient), "\n\n%s%s for Teeworlds v%s", Localize("Bla-Client v"), BLA_VERSION, GAME_VERSION);
	UI()->DoLabel(&Label, aClient, ButtonHeight*ms_FontmodHeight*0.8f, CUI::ALIGN_CENTER);
    Label.y += 60.0f;
     char aText[64];
	str_format(aText, sizeof(aClient), "\n\n%s", Localize("Thanks to everyone who made this client and his features possible"));
	UI()->DoLabel(&Label, aText, ButtonHeight*ms_FontmodHeight*0.8f, CUI::ALIGN_CENTER);
}
void CMenus::RenderBla(CUIRect MainView)
{
	// handle which page should be rendered
	if(g_Config.m_UiSettingsPage == BLA_SETTINGS_GENERAL)
		RenderBlaSettingsGeneral(MainView);
	else if(g_Config.m_UiSettingsPage == BLA_SETTINGS_EXTRAS)
		RenderBlaSettingsExtras(MainView);
	else if(g_Config.m_UiSettingsPage == BLA_SETTINGS_HUD)
		RenderBlaSettingsHud(MainView);
	else if(g_Config.m_UiSettingsPage == BLA_SETTINGS_DUMMY)
		RenderBlaSettingsDummy(MainView);
	else if(g_Config.m_UiSettingsPage == BLA_SETTINGS_TEXTURE)
		RenderBlaSettingsTexture(MainView);
	else if(g_Config.m_UiSettingsPage == BLA_SETTINGS_INFO)
		RenderBlaSettingsInfo(MainView);
	MainView.HSplitBottom(60.0f, 0, &MainView);
	RenderBackButton(MainView);
}
