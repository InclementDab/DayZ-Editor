

class InclementDabLightning: ScriptedLightBase
{    
    void InclementDabLightning()
    {
		SetLightType(LightSourceType.SpotLight);
        SetVisibleDuringDaylight( true );
        SetRadius(500);
        SetBrightness(1);
    }
}

private static const string BOLT_TYPES[2] = { "ThunderboltNorm", "ThunderboltHeavy" };
private static const string SOUND_TYPES[4] = { "ThunderNorm_Near_SoundSet", "ThunderNorm_Far_SoundSet", "ThunderHeavy_Near_SoundSet", "ThunderHeavy_Far_SoundSet"};


	