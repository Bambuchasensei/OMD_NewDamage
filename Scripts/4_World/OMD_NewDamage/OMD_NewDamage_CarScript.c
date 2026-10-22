// Vanilla DamageCrew: crew.AddHealth("", "Health", -hp) — bypasses transferToGlobalCoef and EEHitBy.
modded class CarScript
{

	override void DamageCrew(float dmg)
	{
		super.DamageCrew(dmg);
	}

	override void OnVehicleJumpOutServer(GetOutTransportActionData gotActionData)
	{
		super.OnVehicleJumpOutServer(gotActionData);
	}

};
