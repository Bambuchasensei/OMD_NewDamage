modded class ActionConstructor
{

	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);
		actions.Insert(OMD_NewDamage_ActionPressWoundSelf);
		actions.Insert(OMD_NewDamage_ActionPressWoundTarget);
	}

};
