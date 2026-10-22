// Vanilla DamageCrew: crew.AddHealth("", "Health", -hp) — bypasses transferToGlobalCoef and EEHitBy.
modded class MotorbikeScript
{

	override protected void DamageCrew(float dmg)
	{
		super.DamageCrew(dmg);
	}

};
