modded class ParticleList 
{
	static const int SNOW = RegisterParticle("DayZEditor/Editor/particles/", "new_snow");
	static const int SNOWFOG = RegisterParticle("DayZEditor/Editor/particles/", "fog_maybe");
}

class Snow : EffectParticle 
{
	void Snow()
	{
		SetParticleID(ParticleList.SNOW);
	}
}

class SnowFog : EffectParticle
{
	void SnowFog()
	{
		SetParticleID(ParticleList.SNOWFOG);
	}
}
