modded class DayZGame
{		
#ifdef DIAG_DEVELOPER
	
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
#endif
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
	
	override void OnEvent(EventType eventTypeId, Param params)
	{
		super.OnEvent(eventTypeId, params);
		
		switch (eventTypeId) {
			case MPSessionPlayerReadyEventTypeID: {
				// Client -> Server
				m_Editor = new Editor(GetGame().GetPlayer().GetIdentity(), GetGame().GetPlayer());	
				break;
			}
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

	override void OnRespawnEvent(int time)
	{
	}
	
	override void EnterLoginQueue(UIMenuPanel parent)
	{
	}
	
	override void EnterLoginTime(UIMenuPanel parent)
	{
	}
		
	// the weird loading screen order is
	// game starts loading into server. creates the login queue
	// after a period in queue, it deletes the login queue and creates login timer
	// then after login timer is done, loading is done
	
	// Game Loading Screen
	// this function gets called CONSTANTLY while loading in 
	override void UpdateLoginQueue(float timeslice)
	{
		// probably need to make sure we are in the main menu here somehow
	}
	
	// this function only gets called one time when you get loaded into the server,
	// the loginTime is the amount of seconds until the engine will put you in
	override void OnLoginTimeEvent(int loginTime)
	{
		//LoginTimeEvent.Invoke(loginTime);
	}
	
	override void LoginTimeCountdown()
	{
	}
	
	// appears to be the best place to delete the loading screen
	override void CancelLoginTimeCountdown()
	{
		
	}
	
	override void CancelLoginQueue()
	{
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

modded class DayZGame
{
	override void RegisterConversionTemplates(out TypeConversionHashMap type_conversions)
	{
		super.RegisterConversionTemplates(type_conversions);
		type_conversions.Insert(EditorBrushData, TypeConversionBrush);
		type_conversions.Insert(EditorBrushObject, TypeConversionBrushObject);
		type_conversions.Insert(DropdownListPrefabItemBase, DropdownListPrefabItemConverter);
		type_conversions.Insert(EditorFile, TypeConversionEditorFile);
	}
}

class TypeConversionBrush: TypeConversionTemplate<EditorBrushData>
{	
	override void SetString(string value) 
	{
		m_Value.Type = value;
	}
	
	override string GetString() 
	{
		return m_Value.Type;
	}
}

class TypeConversionBrushObject: TypeConversionTemplate<EditorBrushObject>
{
	override void SetString(string value) 
	{
		m_Value.Type = value;
	}
	
	override string GetString() 
	{
		return m_Value.Type;
	}
	
	override Widget GetWidget() 
	{
		return GetGame().GetWorkspace().CreateWidgets("Editor/gui/Layouts/options/EditorDialogOptionEditBrush.layout");
	}
}


class TypeConversionEditorFile: TypeConversionTemplate<EditorFile>
{
	override void SetString(string value) 
	{
		if (m_Value) {
			m_Value.FileName = value;
		}
	}
	
	override string GetString() 
	{
		if (m_Value) {
			return m_Value.FileName;
		}
		
		return string.Empty;
	}
}

class DropdownListPrefabItemConverter: TypeConversionTemplate<DropdownListPrefabItemBase>
{
	override string GetString() 
	{
		if (m_Value) {
			return m_Value.GetText();
		}
		
		return string.Empty;
	}
	
	override Widget GetWidget() 
	{
		// Todo: why can this be null? not sure
		if (m_Value) {
			return m_Value.GetLayoutRoot();
		}
		
		return null;
	}
	
	override void SetWidget(Widget value) 
	{
		m_Value.OnWidgetScriptInit(value);
	}
}