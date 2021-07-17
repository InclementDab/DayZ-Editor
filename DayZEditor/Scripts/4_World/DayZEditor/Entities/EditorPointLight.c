class EditorScriptedLight: ScriptedLightBase
{
	bool CastShadow = GetCastShadow();
	bool EnableSpecular = true;
	bool EnableLinear = true;
	float PulseCoef = 0.0;
	bool VisibleDuringDay = IsVisibleDuringDaylight();
	float Radius = GetRadius();
	
	// Heat Haze
	bool HeatHaze = false;
	float HeatHazeRadius = GetHeatHazeRadius();
	float HeatHazePower = GetHeatHazePower();
	
	// Colors and brightness
	int DiffuseColor = COLOR_WHITE; // todo default these
	int AmbientColor = COLOR_WHITE;
	float Brightness = GetBrightness();
	
	// Flare
	bool FlareVisible = IsFlareVisible();
	vector FlareRelativePosition = GetFlareRelPosition();
	
	// Spotlight
	float SpotLightAngle = GetSpotLightAngle();
	
	void EditorScriptedLight()
	{
		SetLightType(LightSourceType.NotDef);
	}
	
	void PropertyChanged(string property_name)
	{
		float a, r, g, b;
		switch (property_name) {
			case "CastShadow": {
				SetCastShadow(CastShadow);
				break;
			}
			
			case "EnableSpecular": {
				EnableSpecular(EnableSpecular);
				break;
			}
			
			case "EnableLinear": {
				EnableLinear(EnableLinear);
				break;
			}
			
			case "PulseCoef": {
				SetPulseCoef(PulseCoef);
				break;
			}
			
			case "VisibleDuringDay": {
				SetVisibleDuringDaylight(VisibleDuringDay);
				break;
			}
			
			case "Radius": {
				SetRadiusTo(Radius);
				break;
			}
			
			case "HeatHaze": {
				EnableHeatHaze(HeatHaze);
				break;
			}
			
			case "HeatHazeRadius": {
				SetHeatHazeRadius(HeatHazeRadius);
				break;
			}
			
			case "HeatHazePower": {
				SetHeatHazePower(HeatHazePower);
				break;
			}
			
			case "DiffuseColor": {
				InverseARGBF(DiffuseColor, a, r, g, b);
				SetDiffuseColor(r, g, b);
				SetDiffuseAlpha(a);
				break;
			}			
			
			case "AmbientColor": {
				InverseARGBF(AmbientColor, a, r, g, b);
				SetAmbientColor(r, g, b);
				SetAmbientAlpha(a);
				break;
			}
			
			case "Brightness": {
				SetBrightnessTo(Brightness);
				break;
			}
			
			case "FlareVisible": {
				SetFlareVisible(FlareVisible);
				break;
			}
			
			case "FlareRelativePosition": {
				SetFlareRelPosition(FlareRelativePosition);
				break;
			}
			
			case "SpotLightAngle": {
				SetSpotLightAngle(SpotLightAngle);
				break;
			}
		}
	}
}

class EditorPointLight: EditorScriptedLight
{
	void EditorPointLight()
	{
		SetLightType(LightSourceType.PointLight);
	}
}

class EditorSpotLight: EditorScriptedLight
{
	void EditorSpotLight()
	{
		SetLightType(LightSourceType.SpotLight);
	}
}