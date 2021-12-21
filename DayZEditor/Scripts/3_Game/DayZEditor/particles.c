modded class ParticleList 
{
	static const int SNOW = RegisterParticle("DayZEditor/Editor/particles/", "new_snow");
	static const int SNOWFOG = RegisterParticle("DayZEditor/Editor/particles/", "snow_fog");
	static const int FIREWORK = RegisterParticle("DayZEditor/Editor/particles/", "firework");
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

class FireWork : EffectParticle
{
	void FireWork()
	{
		SetParticleID(ParticleList.FIREWORK);
	}
}