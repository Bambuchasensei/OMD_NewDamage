modded class ActionBase
{

	override bool Can(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (player)
		{
			if (player.OMD_NewDamage_IsHolderPressModeActive())
			{
				return false;
			}
		}

		return super.Can(player, target, item);
	}

};
