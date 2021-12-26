modded class ParticleList 
{
	static const int EDITOR_SNOW = RegisterParticle("DayZEditor/Editor/particles/", "editor_snow");
	static const int EDITOR_SNOWFOG_THIN = RegisterParticle("DayZEditor/Editor/particles/", "editor_snow_fog_thin");
	static const int EDITOR_SNOWFOG_MEDIUM = RegisterParticle("DayZEditor/Editor/particles/", "editor_snow_fog_medium");
	static const int EDITOR_FIREWORK_RED = RegisterParticle("DayZEditor/Editor/particles/", "editor_firework_red");
	static const int EDITOR_FIREWORK_BLUE = RegisterParticle("DayZEditor/Editor/particles/", "editor_firework_blue");
	static const int EDITOR_FIREWORK_CYAN = RegisterParticle("DayZEditor/Editor/particles/", "editor_firework_cyan");
	static const int EDITOR_FIREWORK_GREEN = RegisterParticle("DayZEditor/Editor/particles/", "editor_firework_green");
	static const int EDITOR_FIREWORK_ORANGE = RegisterParticle("DayZEditor/Editor/particles/", "editor_firework_orange");
	static const int EDITOR_FIREWORK_PURPLE = RegisterParticle("DayZEditor/Editor/particles/", "editor_firework_purple");
	static const int EDITOR_FIREWORK_SILVER = RegisterParticle("DayZEditor/Editor/particles/", "editor_firework_silver");
	static const int EDITOR_FIREWORK_WHITE = RegisterParticle("DayZEditor/Editor/particles/", "editor_firework_white");
	static const int EDITOR_FIREWORK_YELLOW = RegisterParticle("DayZEditor/Editor/particles/", "editor_firework_yellow");
}

class Editor_Snow : EffectParticle 
{
	void Editor_Snow()
	{
		SetParticleID(ParticleList.EDITOR_SNOW);
	}
}

class Editor_SnowFog_Thin : EffectParticle
{
	void Editor_SnowFog_Thin()
	{
		SetParticleID(ParticleList.EDITOR_SNOWFOG_THIN);
	}
}

class Editor_SnowFog_Medium : EffectParticle
{
	void Editor_SnowFog_Medium()
	{
		SetParticleID(ParticleList.EDITOR_SNOWFOG_MEDIUM);
	}
}

class Editor_FireWorkRed : EffectParticle
{
	void Editor_FireWorkRed()
	{
		SetParticleID(ParticleList.EDITOR_FIREWORK_RED);
	}
}

class Editor_FireWorkBlue : EffectParticle
{
	void Editor_FireWorkBlue()
	{
		SetParticleID(ParticleList.EDITOR_FIREWORK_BLUE);
	}
}

class Editor_FireWorkCyan : EffectParticle
{
	void Editor_FireWorkCyan()
	{
		SetParticleID(ParticleList.EDITOR_FIREWORK_CYAN);
	}
}

class Editor_FireWorkGreen : EffectParticle
{
	void Editor_FireWorkGreen()
	{
		SetParticleID(ParticleList.EDITOR_FIREWORK_GREEN);
	}
}

class Editor_FireWorkOrange : EffectParticle
{
	void Editor_FireWorkOrange()
	{
		SetParticleID(ParticleList.EDITOR_FIREWORK_ORANGE);
	}
}

class Editor_FireWorkPurple : EffectParticle
{
	void Editor_FireWorkPurple()
	{
		SetParticleID(ParticleList.EDITOR_FIREWORK_PURPLE);
	}
}

class Editor_FireWorkSilver : EffectParticle
{
	void Editor_FireWorkSilver()
	{
		SetParticleID(ParticleList.EDITOR_FIREWORK_SILVER);
	}
}

class Editor_FireWorkWhite : EffectParticle
{
	void Editor_FireWorkWhite()
	{
		SetParticleID(ParticleList.EDITOR_FIREWORK_WHITE);
	}
}

class Editor_FireWorkYellow : EffectParticle
{
	void Editor_FireWorkYellow()
	{
		SetParticleID(ParticleList.EDITOR_FIREWORK_YELLOW);
	}
}