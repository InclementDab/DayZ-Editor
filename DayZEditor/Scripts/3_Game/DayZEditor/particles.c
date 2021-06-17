modded class ParticleList 
{
	static const int SNOW = RegisterParticle("DayZEditor/Editor/particles/", "snow");
}

class Snow : EffectParticle 
{
	void Snow()
	{
		SetParticleID(ParticleList.SNOW);
	}
}
