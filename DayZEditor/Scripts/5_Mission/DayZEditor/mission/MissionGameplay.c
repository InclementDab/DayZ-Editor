modded class MissionGameplay
{	
	override void OnUpdate(float timeslice)
	{		
		// Handle commands, and if we return TRUE, then dont run the rest of the missions update thread
		if (GetEditor()) {
			map<string, EditorCommand> commands = GetEditor().CommandManager.GetCommandShortcutMap();
			foreach (string input_name, EditorCommand command: commands) {		
				if (GetFocus() && GetFocus().IsInherited(EditBoxWidget)) {
					continue;
				}
						
				if (!command || !command.CanExecute()) {
					continue;
				}
				
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
					
					// hate enf
					execute = false;
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
		GetGame().GetWeather().GetFog().Set(Math.RandomFloat01(), 0, 30);
		GetGame().GetWeather().GetOvercast().Set(Math.RandomFloat01(), 0, 30);
		GetGame().GetWeather().GetRain().Set(Math.RandomFloat01(), 0, 30);
		
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