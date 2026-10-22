class OMD_NewDamage_ActionPressWoundTarget : ActionSingleUseBase
{

	void OMD_NewDamage_ActionPressWoundTarget()
	{
		m_CommandUID = -1;
		m_FullBody = false;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT + DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "#STR_OMD_ND_PRESS_WOUND_TARGET";
	}


	override bool IsInstant()
	{
		return true;
	}


	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}


	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone();
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}


	override bool HasProgress()
	{
		return false;
	}


	override bool UseMainItem()
	{
		return false;
	}


	override ActionData CreateActionData()
	{
		OMD_NewDamage_PressWoundActionData action_data;
		action_data = new OMD_NewDamage_PressWoundActionData;
		return action_data;
	}


	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		string woundKey;
		woundKey = "";
		OMD_NewDamage_PressWoundVariantData variantData;
		variantData = player.OMD_NewDamage_GetPressWoundVariantData();
		if (variantData)
		{
			woundKey = variantData.GetTargetWoundAtIndex(m_VariantID);
		}

		m_Text = player.OMD_NewDamage_GetPressWoundLabel(woundKey);
	}


	protected void OMD_NewDamage_PressCondFailTarget(string reason)
	{
		if (!OMD_NewDamageConfig.IsDebugEnabled())
		{
			return;
		}

		string side;
		side = "client";
		if (GetGame().IsServer())
		{
			side = "server";
		}

		string line;
		line = "[OMD_NewDamage][PRESS] cond fail target side=" + side;
		line = line + " reason=" + reason;
		Print(line);
	}


	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		OMD_NewDamageConfigData cfg;
		cfg = OMD_NewDamageConfig.Get();
		if (!cfg)
		{
			OMD_NewDamage_PressCondFailTarget("no_cfg");
			return false;
		}

		if (!cfg.holdPressureEnabled)
		{
			OMD_NewDamage_PressCondFailTarget("hold_disabled");
			return false;
		}

		if (!cfg.holdPressureTargetEnabled)
		{
			OMD_NewDamage_PressCondFailTarget("target_disabled");
			return false;
		}

		if (player.OMD_NewDamage_IsHolderPressModeActive())
		{
			OMD_NewDamage_PressCondFailTarget("holder_mode_active");
			return false;
		}

		if (cfg.holdPressureRequiresEmptyHands)
		{
			if (item)
			{
				OMD_NewDamage_PressCondFailTarget("item_in_action");
				return false;
			}

			if (player.GetItemInHands())
			{
				OMD_NewDamage_PressCondFailTarget("item_in_hands");
				return false;
			}
		}

		PlayerBase ntarget;
		ntarget = PlayerBase.Cast(target.GetObject());
		if (!ntarget)
		{
			OMD_NewDamage_PressCondFailTarget("no_target");
			return false;
		}

		if (!ntarget.IsAlive())
		{
			OMD_NewDamage_PressCondFailTarget("target_dead");
			return false;
		}

		if (!ntarget.OMD_NewDamage_CanPressWound())
		{
			OMD_NewDamage_PressCondFailTarget("can_press_false");
			return false;
		}

		return true;
	}


	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL)
	{
		if (!super.SetupAction(player, target, item, action_data, extra_data))
		{
			return false;
		}

		OMD_NewDamage_PressWoundActionData pressData;
		pressData = OMD_NewDamage_PressWoundActionData.Cast(action_data);
		if (!pressData)
		{
			return false;
		}

		if (!GetGame().IsDedicatedServer())
		{
			string woundKey;
			woundKey = "";
			OMD_NewDamage_PressWoundVariantData variantData;
			variantData = player.OMD_NewDamage_GetPressWoundVariantData();
			if (variantData)
			{
				woundKey = variantData.GetTargetWoundAtIndex(m_VariantID);
			}

			PlayerBase ntarget;
			ntarget = PlayerBase.Cast(target.GetObject());
			if (woundKey == "" && ntarget)
			{
				woundKey = ntarget.OMD_NewDamage_SelectMostDangerousWound(OMD_NewDamageConfig.Get());
			}

			pressData.m_WoundKey = woundKey;
		}

		return true;
	}


	override void WriteToContext(ParamsWriteContext ctx, ActionData action_data)
	{
		super.WriteToContext(ctx, action_data);
		OMD_NewDamage_PressWoundActionData pressData;
		pressData = OMD_NewDamage_PressWoundActionData.Cast(action_data);
		if (pressData)
		{
			ctx.Write(pressData.m_WoundKey);
		}
		else
		{
			ctx.Write("");
		}
	}


	override bool ReadFromContext(ParamsReadContext ctx, out ActionReciveData action_recive_data)
	{
		action_recive_data = new OMD_NewDamage_PressWoundActionReciveData;
		if (!super.ReadFromContext(ctx, action_recive_data))
		{
			return false;
		}

		string woundKey;
		woundKey = "";
		if (!ctx.Read(woundKey))
		{
			return false;
		}

		OMD_NewDamage_PressWoundActionReciveData.Cast(action_recive_data).m_WoundKey = woundKey;
		return true;
	}


	override void HandleReciveData(ActionReciveData action_recive_data, ActionData action_data)
	{
		super.HandleReciveData(action_recive_data, action_data);
		OMD_NewDamage_PressWoundActionReciveData recv;
		recv = OMD_NewDamage_PressWoundActionReciveData.Cast(action_recive_data);
		OMD_NewDamage_PressWoundActionData pressData;
		pressData = OMD_NewDamage_PressWoundActionData.Cast(action_data);
		if (recv && pressData)
		{
			pressData.m_WoundKey = recv.m_WoundKey;
		}
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
	}


	override void OnStartClient(ActionData action_data)
	{
		super.OnStartClient(action_data);

		if (OMD_NewDamageConfig.IsDebugEnabled())
		{
			Print("[OMD_NewDamage][PRESS] enter action start target side=client");
		}
	}

};
