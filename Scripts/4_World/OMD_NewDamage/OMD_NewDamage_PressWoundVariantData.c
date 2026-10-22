class OMD_NewDamage_PressWoundVariantData
{
	protected PlayerBase m_Player;
	protected ref ActionVariantManager m_SelfVariantManager;
	protected ref ActionVariantManager m_TargetVariantManager;
	protected ref array<string> m_SelfWounds;
	protected ref array<string> m_TargetWounds;


	void OMD_NewDamage_PressWoundVariantData(PlayerBase player)
	{
		m_Player = player;
		m_SelfWounds = new array<string>;
		m_TargetWounds = new array<string>;

		if (!GetGame())
		{
			return;
		}

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			m_SelfVariantManager = ActionManagerClient.GetVariantManager(OMD_NewDamage_ActionPressWoundSelf);
			if (m_SelfVariantManager)
			{
				m_SelfVariantManager.GetOnUpdateInvoker().Clear();
				m_SelfVariantManager.GetOnUpdateInvoker().Insert(OnUpdateSelfActions);
			}

			m_TargetVariantManager = ActionManagerClient.GetVariantManager(OMD_NewDamage_ActionPressWoundTarget);
			if (m_TargetVariantManager)
			{
				m_TargetVariantManager.GetOnUpdateInvoker().Clear();
				m_TargetVariantManager.GetOnUpdateInvoker().Insert(OnUpdateTargetActions);
			}
		}
	}


	void ~OMD_NewDamage_PressWoundVariantData()
	{
		if (!GetGame())
		{
			return;
		}

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			if (m_SelfVariantManager)
			{
				m_SelfVariantManager.GetOnUpdateInvoker().Remove(OnUpdateSelfActions);
			}

			if (m_TargetVariantManager)
			{
				m_TargetVariantManager.GetOnUpdateInvoker().Remove(OnUpdateTargetActions);
			}
		}
	}


	void OnUpdateSelfActions(Object item, Object target, int component_index)
	{
		m_SelfWounds.Clear();
		if (!m_Player)
		{
			if (m_SelfVariantManager)
			{
				m_SelfVariantManager.Clear();
			}
			return;
		}

		PlayerBase localPlayer;
		localPlayer = PlayerBase.Cast(GetGame().GetPlayer());
		if (localPlayer != m_Player)
		{
			return;
		}

		m_Player.OMD_NewDamage_CollectPressWoundsSorted(m_Player, m_SelfWounds);
		if (m_SelfVariantManager)
		{
			m_SelfVariantManager.SetActionVariantCount(m_SelfWounds.Count());
		}
	}


	void OnUpdateTargetActions(Object item, Object target, int component_index)
	{
		m_TargetWounds.Clear();
		PlayerBase localPlayer;
		localPlayer = PlayerBase.Cast(GetGame().GetPlayer());
		if (localPlayer != m_Player)
		{
			return;
		}

		PlayerBase ntarget;
		ntarget = PlayerBase.Cast(target);
		if (!ntarget)
		{
			if (m_TargetVariantManager)
			{
				m_TargetVariantManager.Clear();
			}
			return;
		}

		ntarget.OMD_NewDamage_CollectPressWoundsSorted(ntarget, m_TargetWounds);
		if (m_TargetVariantManager)
		{
			m_TargetVariantManager.SetActionVariantCount(m_TargetWounds.Count());
		}
	}


	string GetSelfWoundAtIndex(int index)
	{
		if (index < 0)
		{
			return "";
		}

		if (index >= m_SelfWounds.Count())
		{
			return "";
		}

		return m_SelfWounds.Get(index);
	}


	string GetTargetWoundAtIndex(int index)
	{
		if (index < 0)
		{
			return "";
		}

		if (index >= m_TargetWounds.Count())
		{
			return "";
		}

		return m_TargetWounds.Get(index);
	}


	int GetSelfWoundCount()
	{
		return m_SelfWounds.Count();
	}


	int GetTargetWoundCount()
	{
		return m_TargetWounds.Count();
	}

};
