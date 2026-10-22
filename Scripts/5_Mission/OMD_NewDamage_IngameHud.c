#ifndef SERVER
modded class IngameHud
{
	Widget m_OMD_ND_Layout;
	Widget m_OMD_ND_BadgesPanelWidget;
	ref map<string, ref Widget> m_OMD_ND_Widgets = new map<string, ref Widget>;
	bool m_OMD_ND_WidgetsInitialized;
	float m_OMD_ND_UpdateLastTime;
	int m_OMD_ND_InitTries;


	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OMD_ND_InitWidgets, 200, false, hud_panel_widget);
	}


	override void Update(float timeslice)
	{
		super.Update(timeslice);
		if (m_OMD_ND_WidgetsInitialized)
		{
			OMD_ND_UpdateBadges(timeslice);
		}
	}


	void OMD_ND_InitWidgets(Widget hudPanelWidget)
	{
		if (m_OMD_ND_WidgetsInitialized)
		{
			return;
		}

		if (!hudPanelWidget)
		{
			return;
		}

		if (!m_OMD_ND_Layout)
		{
			m_OMD_ND_Layout = GetGame().GetWorkspace().CreateWidgets("OMD_NewDamage/gui/layouts/omd_newdamage_badges.layout");
			if (!m_OMD_ND_Layout)
			{
				m_OMD_ND_InitTries = m_OMD_ND_InitTries + 1;
				if (m_OMD_ND_InitTries <= 10)
				{
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OMD_ND_InitWidgets, 200, false, hudPanelWidget);
				}

				return;
			}
		}

		if (!m_OMD_ND_BadgesPanelWidget)
		{
			if (m_Badges)
			{
				m_OMD_ND_BadgesPanelWidget = m_Badges;
			}
			else
			{
				m_OMD_ND_BadgesPanelWidget = hudPanelWidget.FindAnyWidget("BadgesPanel");
			}

			if (!m_OMD_ND_BadgesPanelWidget)
			{
				return;
			}
		}

		OMD_ND_RegisterLevelFrame("omd_heart_frame");
		OMD_ND_RegisterLevelFrame("omd_spleen_frame");
		OMD_ND_RegisterLevelFrame("omd_lungL_frame");
		OMD_ND_RegisterLevelFrame("omd_lungR_frame");
		OMD_ND_RegisterLevelFrame("omd_guts_frame");
		OMD_ND_RegisterLevelFrame("omd_liver_frame");
		OMD_ND_RegisterLevelFrame("omd_face_frame");
		OMD_ND_RegisterLevelFrame("omd_brain_frame");
		OMD_ND_RegisterArteryFrame("omd_artery_head_frame");
		OMD_ND_RegisterArteryFrame("omd_artery_torso_frame");
		OMD_ND_RegisterArteryFrame("omd_artery_arms_frame");
		OMD_ND_RegisterArteryFrame("omd_artery_legs_frame");
		OMD_ND_RegisterTintFrame("omd_tint_head_frame");
		OMD_ND_RegisterTintFrame("omd_tint_torso_frame");
		OMD_ND_RegisterTintFrame("omd_tint_arms_frame");
		OMD_ND_RegisterTintFrame("omd_tint_legs_frame");

		m_OMD_ND_WidgetsInitialized = true;
	}


	protected void OMD_ND_RegisterLevelFrame(string frameName)
	{
		Widget frameWidget;
		frameWidget = m_OMD_ND_Layout.FindAnyWidget(frameName);
		if (!frameWidget)
		{
			return;
		}

		m_OMD_ND_Widgets.Set(frameName, frameWidget);
		m_OMD_ND_Widgets.Set(frameName + "_y", frameWidget.FindAnyWidget("IconY"));
		m_OMD_ND_Widgets.Set(frameName + "_o", frameWidget.FindAnyWidget("IconO"));
		m_OMD_ND_Widgets.Set(frameName + "_r", frameWidget.FindAnyWidget("IconR"));
		m_OMD_ND_BadgesPanelWidget.AddChild(frameWidget);
	}


	protected void OMD_ND_RegisterArteryFrame(string frameName)
	{
		Widget frameWidget;
		frameWidget = m_OMD_ND_Layout.FindAnyWidget(frameName);
		if (!frameWidget)
		{
			return;
		}

		m_OMD_ND_Widgets.Set(frameName, frameWidget);
		m_OMD_ND_Widgets.Set(frameName + "_artery", frameWidget.FindAnyWidget("Artery"));
		m_OMD_ND_BadgesPanelWidget.AddChild(frameWidget);
	}


	protected void OMD_ND_RegisterTintFrame(string frameName)
	{
		Widget frameWidget;
		frameWidget = m_OMD_ND_Layout.FindAnyWidget(frameName);
		if (!frameWidget)
		{
			return;
		}

		m_OMD_ND_Widgets.Set(frameName, frameWidget);
		m_OMD_ND_Widgets.Set(frameName + "_tint", frameWidget.FindAnyWidget("Tint"));
		m_OMD_ND_BadgesPanelWidget.AddChild(frameWidget);
	}


	void OMD_ND_UpdateBadges(float timeslice)
	{
		if ((GetGame().GetTickTime() - m_OMD_ND_UpdateLastTime) < 0.5)
		{
			return;
		}

		m_OMD_ND_UpdateLastTime = GetGame().GetTickTime();

		PlayerBase player;
		player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
		{
			return;
		}

		OMD_ND_RenderLevelBadge("omd_heart_frame", player.OMD_NewDamage_GetHudHeartLevel());
		OMD_ND_RenderLevelBadge("omd_spleen_frame", player.OMD_NewDamage_GetHudSpleenLevel());
		OMD_ND_RenderLevelBadge("omd_lungL_frame", player.OMD_NewDamage_GetHudLungLLevel());
		OMD_ND_RenderLevelBadge("omd_lungR_frame", player.OMD_NewDamage_GetHudLungRLevel());
		OMD_ND_RenderLevelBadge("omd_guts_frame", player.OMD_NewDamage_GetHudGutsLevel());
		OMD_ND_RenderLevelBadge("omd_liver_frame", player.OMD_NewDamage_GetHudLiverLevel());
		OMD_ND_RenderLevelBadge("omd_face_frame", player.OMD_NewDamage_GetHudFaceLevel());
		OMD_ND_RenderLevelBadge("omd_brain_frame", player.OMD_NewDamage_GetHudBrainLevel());
		OMD_ND_RenderArteryBadge("omd_artery_head_frame", player.OMD_NewDamage_GetHudArteryLevelForLimb("head"));
		OMD_ND_RenderArteryBadge("omd_artery_torso_frame", player.OMD_NewDamage_GetHudArteryLevelForLimb("torso"));
		OMD_ND_RenderArteryBadge("omd_artery_arms_frame", player.OMD_NewDamage_GetHudArteryLevelForLimb("arms"));
		OMD_ND_RenderArteryBadge("omd_artery_legs_frame", player.OMD_NewDamage_GetHudArteryLevelForLimb("legs"));
		OMD_ND_RenderTint("omd_tint_head_frame", player.OMD_NewDamage_GetHudLimbTintLevel("head"));
		OMD_ND_RenderTint("omd_tint_torso_frame", player.OMD_NewDamage_GetHudLimbTintLevel("torso"));
		OMD_ND_RenderTint("omd_tint_arms_frame", player.OMD_NewDamage_GetHudLimbTintLevel("arms"));
		OMD_ND_RenderTint("omd_tint_legs_frame", player.OMD_NewDamage_GetHudLimbTintLevel("legs"));
	}


	protected void OMD_ND_RenderLevelBadge(string frameName, int level)
	{
		Widget frameWidget;
		frameWidget = m_OMD_ND_Widgets.Get(frameName);
		if (!frameWidget)
		{
			return;
		}

		Widget iconY;
		iconY = m_OMD_ND_Widgets.Get(frameName + "_y");
		Widget iconO;
		iconO = m_OMD_ND_Widgets.Get(frameName + "_o");
		Widget iconR;
		iconR = m_OMD_ND_Widgets.Get(frameName + "_r");
		if (level <= 0)
		{
			OMD_ND_Hide(frameWidget);
			OMD_ND_Hide(iconY);
			OMD_ND_Hide(iconO);
			OMD_ND_Hide(iconR);
			return;
		}

		OMD_ND_Show(frameWidget);
		OMD_ND_Hide(iconY);
		OMD_ND_Hide(iconO);
		OMD_ND_Hide(iconR);
		if (level == 1)
		{
			OMD_ND_Show(iconY);
		}
		else
		{
			if (level == 2)
			{
				OMD_ND_Show(iconO);
			}
			else
			{
				OMD_ND_Show(iconR);
			}
		}
	}


	protected void OMD_ND_RenderArteryBadge(string frameName, int level)
	{
		Widget frameWidget;
		frameWidget = m_OMD_ND_Widgets.Get(frameName);
		if (!frameWidget)
		{
			return;
		}

		Widget artery;
		artery = m_OMD_ND_Widgets.Get(frameName + "_artery");
		if (level <= 0)
		{
			OMD_ND_Hide(frameWidget);
			OMD_ND_Hide(artery);
			return;
		}

		OMD_ND_Show(frameWidget);
		OMD_ND_Show(artery);
	}


	protected void OMD_ND_RenderTint(string frameName, int level)
	{
		Widget frameWidget;
		frameWidget = m_OMD_ND_Widgets.Get(frameName);
		if (!frameWidget)
		{
			return;
		}

		ImageWidget tint;
		tint = ImageWidget.Cast(m_OMD_ND_Widgets.Get(frameName + "_tint"));
		if (level <= 0)
		{
			OMD_ND_Hide(frameWidget);
			return;
		}

		OMD_ND_Show(frameWidget);
		if (!tint)
		{
			return;
		}

		if (level == 1)
		{
			tint.SetColor(ARGB(140, 230, 200, 74));
		}
		else
		{
			if (level == 2)
			{
				tint.SetColor(ARGB(160, 224, 122, 47));
			}
			else
			{
				tint.SetColor(ARGB(180, 198, 40, 40));
			}
		}
	}


	protected void OMD_ND_Show(Widget w)
	{
		if (w)
		{
			w.Show(true);
		}
	}


	protected void OMD_ND_Hide(Widget w)
	{
		if (w)
		{
			w.Show(false);
		}
	}

};
#endif
