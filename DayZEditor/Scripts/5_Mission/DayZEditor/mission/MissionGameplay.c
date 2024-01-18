modded class MissionGameplay
{	
	override void OnUpdate(float timeslice)
	{		
		// Handle commands, and if we return TRUE, then dont run the rest of the missions update thread
		if (GetEditor()) {
			map<string, Command> commands = GetEditor().GetCommandShortcutMap();
			foreach (string input_name, Command command: commands) {		
				if (GetFocus() && GetFocus().IsInherited(EditBoxWidget)) {
					continue;
				}
						
				if (!command || !command.CanExecute()) {
					continue;
				}
				
				switch (command.GetShortcutType()) {
					case ShortcutKeyType.PRESS: {
						if (GetGame().GetInput().LocalPress(input_name)) {
							command.Execute(true);
						}
						
						break;
					}
					
					case ShortcutKeyType.DOUBLE: {
						if (GetGame().GetInput().LocalDbl(input_name)) {
							command.Execute(true);
						}
						
						break;
					}
					
					case ShortcutKeyType.HOLD: {
						if (GetGame().GetInput().LocalHold(input_name)) {
							command.Execute(true);
						}
						
						break;
					}
				}
			}
		}
		
		super.OnUpdate(timeslice);
		
		bool player_control_enabled = GetEditor() && GetEditor().GetCurrentControl() && GetEditor().GetCurrentControl().IsInherited(Man);
		m_HudRootWidget.Show(player_control_enabled);
	}
	
	override void OnInit()
	{
		super.OnInit();
						
		/* Kill the weather! */
		GetGame().GetWeather().GetFog().Set(0.1, 0, 30);
		GetGame().GetWeather().GetOvercast().Set(Math.RandomFloat01() * 0.8, 0, 30);
		GetGame().GetWeather().GetRain().Set(Math.RandomFloat01() * 0.1, 0, 30);
		
		GetGame().GetWeather().SetWind(vector.Zero);
		GetGame().GetWeather().SetWindSpeed(Math.RandomFloat01() * 15.0);
	}
	
	override void OnMissionStart()
	{
		super.OnMissionStart();		
		g_Game.ReportProgress("Loading Mission");
		
		CF.ObjectManager.UnhideAllMapObjects();
	}
	
	override void OnMissionFinish()
	{
		super.OnMissionFinish();		
		
		delete g_Editor;
	}
		
	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();		
		
		g_Game.ReportProgress("Loading DayZ Editor...");
		
		/*
		g_Editor = new Editor(Editor.GenerateSafeStartPosition());
		g_Game.ReportProgress("DayZ Editor Loading complete");*/
	}
		
	override void OnMouseButtonPress(int button)
	{
		if (!GetEditor() || !GetEditor().OnMouseDown(button)) {			
			super.OnMouseButtonPress(button);
		} 
	}
	
	override void ShowInventory()
	{
		GetGame().GetPlayer().GetHumanInventory().UnlockInventory(LOCK_FROM_SCRIPT);
		super.ShowInventory();
		m_Hud.InventoryShown = true;
	}
	
	override void HideInventory()
	{
		super.HideInventory();
		m_Hud.InventoryShown = false;
	}
}