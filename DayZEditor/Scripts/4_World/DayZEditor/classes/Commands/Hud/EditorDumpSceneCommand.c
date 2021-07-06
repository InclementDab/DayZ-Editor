class EditorDumpSceneCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		string camera_data;
		
		vector pos = GetEditor().GetCamera().GetPosition();
		vector ori = GetEditor().GetCamera().GetOrientation();
		vector player_pos = GetGame().GetPlayer().GetPosition();
		vector player_ori = GetGame().GetPlayer().GetOrientation();
		EditorCamera camera = GetEditor().GetCamera();
		int year, month, day, hour, minute;
		GetGame().GetWorld().GetDate(year, month, day, hour, minute);
		float over = GetGame().GetWeather().GetOvercast().GetActual();
		float rain = GetGame().GetWeather().GetRain().GetActual();
		float fog = GetGame().GetWeather().GetFog().GetActual();
		float wind = GetGame().GetWeather().GetWindSpeed();
		
		camera_data += string.Format("CameraPosition[]={%1, %2, %3};\n", pos[0], pos[1], pos[2]);
		camera_data += string.Format("CameraOrientation[]={%1, %2, %3};\n", ori[0], ori[1], ori[2]);
		camera_data += string.Format("PlayerPosition[]={%1, %2, %3};\n", player_pos[0], player_pos[1], player_pos[2]);
		camera_data += string.Format("PlayerOrientation[]={%1, %2, %3};\n", player_ori[0], player_ori[1], player_ori[2]);
		camera_data += string.Format("fov=%1;\n", camera.FOV);
		camera_data += string.Format("DOFDistance=%1;\n", camera.DOFDistance);
		camera_data += string.Format("DOFBlur=%1;\n", camera.DOFBlur);
		camera_data += string.Format("Blur=%1;\n", camera.Blur);
		camera_data += string.Format("Vignette=%1;\n", camera.Vignette);
		camera_data += string.Format("Sharpness=%1;\n", camera.Sharpness);
		camera_data += string.Format("NearPlane=%1;\n", camera.NearPlane);
		
		camera_data += string.Format("date[]={%1, %2, %3, %4, %5};\n", year, month, day, hour, minute);
		camera_data += string.Format("overcast=%1;\n", over);
		camera_data += string.Format("rain=%1;\n", rain);
		camera_data += string.Format("fog=%1;\n", fog);
		camera_data += string.Format("windspeed=%1;\n", wind);
		
		GetGame().CopyToClipboard(camera_data);
		
		GetEditor().GetEditorHud().CreateNotification("Camera Data copied to clipboard!");
	}

	override string GetName() 
	{
		return "#STR_EDITOR_CMD_DUMP_CAMERA";
	}
}