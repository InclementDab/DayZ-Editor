modded class DayZGame
{		
	ref array<ref Param3<string, vector, float>> DebugTexts = {};
	
	void DebugDrawText(string text, vector pos, float size)
	{
		DebugTexts.Insert(new Param3<string, vector, float>(text, pos, size));
	}
	
	// VERY handy when changing layouts :)
	void Recompile()
	{
		m_Editor = new Editor(GetPlayer().GetIdentity(), GetPlayer());
	}
	
	// server!
	protected ref map<string, ref EditorNode> m_Nodes = new map<string, ref EditorNode>();
	
	protected ref map<typename, ref Command> m_Commands = new map<typename, ref Command>();
	protected ref map<string, Command> m_CommandShortcutMap = new map<string, Command>();
			
	void DayZGame()
	{
		foreach (typename command_type: RegisterCommand.Instances) {		
			Command command = Command.Cast(command_type.Spawn());
			if (!command) {
				Error("Invalid command");
				continue;
			}
			
			m_Commands[command_type] = command;
			
			if (command.GetShortcut() != string.Empty) {
				m_CommandShortcutMap[command.GetShortcut()] = command;
			}
		}	
	}
	
	override void OnUpdate(bool doSim, float timeslice)
	{
		super.OnUpdate(doSim, timeslice);
						
		if (IsLeftCtrlDown()) {
			foreach (string input_name, Command command: m_CommandShortcutMap) {		
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
		
		if (m_Editor) {
			m_Editor.Update(doSim, timeslice);
		}
	}
		
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{				
		// RPC is editor online session
		if (Editor.RPC_ALL.Find(rpc_type) != -1) {			
			m_Editor.OnRPC(sender, rpc_type, ctx);
		}
		
		super.OnRPC(sender, target, rpc_type, ctx);
	}
	
	void StartEditor(notnull PlayerIdentity identity, notnull DayZPlayer player)
	{
		m_Editor = new Editor(identity, player);
	}
	
	Command GetCommand(typename command)
	{
		return m_Commands[command];
	}
	
	static bool IsForbiddenItem(string model)
	{
		//! In theory should be safe but just in case
		if (model.Contains("Fx")) return true;
		if (model == "ItemOptics") return true;

		//! Cursed items
		if (model == "AKM_TESTBED") return true;
		if (model == "Red9") return true;
		if (model == "QuickieBow") return true;
		if (model == "LargeTentBackPack") return true;
		if (model == "SurvivorMale_Base" || model == "SurvivorFemale_Base") return true;
		if (model == "Land_VASICore" || model == "FlagCarrierCore") return true;
		if (GetGame().IsKindOf(model, "GP25Base")) return true;
		if (GetGame().IsKindOf(model, "M203Base")) return true;
		if (model == "ItemOptics_Base") return true;
		
		//! Everything is fine... I hope... :pain:
		return false;
	}
	
#ifndef EDITOR_DECLARE
#define EDITOR_DECLARE
	protected ref Editor m_Editor;
	
	Editor GetEditor()
	{
		return m_Editor;
	}
#endif
}