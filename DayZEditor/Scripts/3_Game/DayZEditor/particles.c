modded class ParticleList 
{
	static const int SNOW = RegisterParticle("DayZEditor/Editor/particles/", "new_snow");
}

class Snow : EffectParticle 
{
	void Snow()
	{
		SetParticleID(ParticleList.SNOW);
	}
}
