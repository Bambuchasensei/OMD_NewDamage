modded class DayZPlayerImplementAiming
{

	override protected float CalculateWeight(int stance_index, float current_stamina, float camera_sway_modifier, bool holding_breath)
	{
		float base;
		base = super.CalculateWeight(stance_index, current_stamina, camera_sway_modifier, holding_breath);
		PlayerBase player;
		player = PlayerBase.Cast(m_PlayerPb);
		if (!player)
		{
			return base;
		}

		float shake;
		shake = player.OMD_NewDamage_GetAimShakeMult();
		if (shake <= 1.0)
		{
			return base;
		}

		return base * shake;
	}

};
