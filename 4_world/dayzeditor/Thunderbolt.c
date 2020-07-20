

class InclementDabLightning extends PointLightBase
{    
    void InclementDabLightning()
    {
        SetVisibleDuringDaylight( true );
        SetRadius( 500 );
        SetBrightness( 500 );
        SetDiffuseColor( 1, 1, 0.8 );
        SetAmbientColor( 1, 1, 0.8 );
        SetAmbientAlpha( 1 );
        SetCastShadow( true );
        SetEnabled( true );
        SetFlareVisible( true );
    }
}

class LightingBolt
{
		
	private static const string BOLT_TYPES[2] = { "ThunderboltNorm", "ThunderboltHeavy" };
	private static const string SOUND_TYPES[4] = { "ThunderNorm_Near_SoundSet", "ThunderNorm_Far_SoundSet", "ThunderHeavy_Near_SoundSet", "ThunderHeavy_Far_SoundSet"};
	
	static vector this_game_is_string;
	
	static void CreateLightning(vector position, int intensity)
	{
		int count = Math.RandomFloat(1, 3) * intensity;
		this_game_is_string = position;
		for (int i = 0; i < count; i++) {
		
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(CreateBolt, Math.RandomInt(0, 350), false);
		}
		
	}
	
	static ScriptedLightBase light;
	static void CreateBolt()
	{
		
		this_game_is_string[0] = this_game_is_string[0] + Math.RandomFloat(-15, 15);
		this_game_is_string[2] = this_game_is_string[2] + Math.RandomFloat(-15, 15);
		
		Object bolt = GetGame().CreateObject(BOLT_TYPES[Math.RandomInt(0, 1)], this_game_is_string);
		bolt.SetOrientation(Vector(0, Math.RandomFloat(0, 360), 0));
		SEffectManager.PlaySound(SOUND_TYPES[Math.RandomInt(0, 3)], this_game_is_string);
		
		light = ScriptedLightBase.CreateLight(InclementDabLightning, this_game_is_string);
		
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DestroyBolt, Math.RandomInt(15, 150), false, bolt);
	}
	
	static void DestroyBolt(Object bolt)
	{
		SEffectManager.PlaySound(SOUND_TYPES[Math.RandomInt(0, 3)], this_game_is_string);
		light.Destroy();
		GetGame().ObjectDelete(bolt);
		
	}
	
}