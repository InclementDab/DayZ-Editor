modded class MissionGameplay
{	
	override void OnUpdate(float timeslice)
	{
		bool player_control_enabled = GetEditor() && GetGame().GetPlayer() == GetEditor().GetCurrentControl();
		
		m_HudRootWidget.Show(player_control_enabled);
		
		// Handle commands, and if we return TRUE, then dont run the rest of the missions update thread
		if (GetEditor()) {
			map<string, EditorCommand> commands = GetEditor().CommandManager.GetCommandShortcutMap();
			foreach (string input_name, EditorCommand command: commands) {				
				if (command && command.CanExecute()) {
					bool execute;
					switch (command.GetShortcutType()) {
						case EditorShortcutKeyType.PRESS: {
							if (GetGame().GetInput().LocalPress(input_name)) {
								execute = true;
							}
							
							break;
						}
						
						case EditorShortcutKeyType.DOUBLE: {
							if (GetGame().GetInput().LocalDbl(input_name)) {
								execute = true;
							}
							
							break;
						}
						
						case EditorShortcutKeyType.HOLD: {
							if (GetGame().GetInput().LocalHold(input_name)) {
								execute = true;
							}
							
							break;
						}
					}
					
					if (execute) {
						EditorLog.Debug("Hotkeys Pressed for %1", command.ToString());
						CommandArgs args = new CommandArgs();
						args.Context = GetEditor().GetEditorHud();
						if (command.Execute(GetEditor().CommandManager, args)) {
							return; // mucho importante
						}
					}
				}
			}
		}
		
		super.OnUpdate(timeslice);
	}
	
	override void OnInit()
	{
		super.OnInit();
				
		GetUApi().GetInputByName("UACOTModuleToggleCOT").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleButtons").ForceDisable(true);
		GetUApi().GetInputByName("UACOTTogglePlayer").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleCamera").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleESP").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleMap").ForceDisable(true);
		GetUApi().GetInputByName("UACameraToolSpeedIncrease").ForceDisable(true);
		GetUApi().GetInputByName("UACameraToolSpeedDecrease").ForceDisable(true);
		GetUApi().GetInputByName("UAGear").ForceDisable(true);
		GetUApi().UpdateControls();
		
		/* Kill the weather! */
		GetGame().GetWeather().GetFog().Set(0, 0, 0);
		GetGame().GetWeather().GetFog().SetLimits(0, 0);
		GetGame().GetWeather().GetOvercast().Set(0, 0, 0);
		GetGame().GetWeather().GetOvercast().SetLimits(0, 0);
		GetGame().GetWeather().GetRain().Set(0, 0, 0);
		GetGame().GetWeather().GetRain().SetLimits(0, 0);
		
		GetGame().GetWeather().SetWind(vector.Zero);
		GetGame().GetWeather().SetWindSpeed(0);
	}
	
	override void OnMissionStart()
	{
		super.OnMissionStart();		
		g_Game.ReportProgress("Loading Mission");
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
		
		g_Editor = new Editor(Editor.GenerateSafeStartPosition());
		g_Game.ReportProgress("DayZ Editor Loading complete");
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