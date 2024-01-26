modded class DayZGame
{
	// Handled in DayZGame
	static const int RPC_REQUEST_SYNC = 54364;
	static const int RPC_NODE_SYNC = 54365;
	
	protected ref map<typename, ref Command> m_Commands = new map<typename, ref Command>();
	protected ref map<string, Command> m_CommandShortcutMap = new map<string, Command>();
	
	// Created on client AND server, assumed existence always. RPC_SYNC and Synchronize will be relying on this
	protected ref EditorNode m_Master = new EditorNode(string.Empty, string.Empty, string.Empty);
			
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
		
	override void SetMissionPath(string path)
	{
		super.SetMissionPath(path);
		
		m_Master.Add(new EditorServer("SERVER", "Editors", Symbols.CAMERA_SECURITY));
		m_Master.Synchronize();
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
		
		if (GetEditor()) {
			GetEditor().Update(doSim, timeslice);
		}
	}
		
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{				
		switch (rpc_type) {		
			case RPC_REQUEST_SYNC: {
				m_Master.Synchronize(sender);
				break;
			}
				
			case RPC_NODE_SYNC: {	
				int tree_depth;
				if (!ctx.Read(tree_depth)) {
					break;
				}

				EditorNode current = m_Master;
				for (int i = 0; i < tree_depth; i++) {
					string uuid;
					ctx.Read(uuid);
					
					string type;
					ctx.Read(type);
					
					EditorNode node = current[uuid];
					if (!node) {
						node = EditorNode.Cast(type.ToType().Spawn());
						if (!node) {
							Error("Invalid node type " + type);
							continue;
						}
						
						current[uuid] = node;
						node.SetParent(current[uuid]);
					}
					
					current = current[uuid];
				}
								
				current.Read(ctx, 0);
				
				// Who do we sync back to?
				if (GetGame().IsDedicatedServer()) {
					array<PlayerIdentity> identities = {};
					GetGame().GetPlayerIndentities(identities);
					foreach (PlayerIdentity identity: identities) {
						
						// The client that sent the original RPC will not recieve it back - they are the most recent commit
						if (sender.GetId() != identity.GetId()) {
							current.Synchronize(identity);
						}
					}	
				}
				
				current.OnSynchronized();
				break;
			}
		}
		
		super.OnRPC(sender, target, rpc_type, ctx);
	}
	
	Command GetCommand(typename command)
	{
		return m_Commands[command];
	}
	
	ref array<ref Param3<string, vector, float>> DebugTexts = {};
	
	void DebugDrawText(string text, vector pos, float size)
	{
		DebugTexts.Insert(new Param3<string, vector, float>(text, pos, size));
	}
	
	// VERY handy when changing layouts :)
	void Recompile()
	{
		PlayerIdentity identity = GetPlayer().GetIdentity();
		delete m_Master["SERVER"][identity.GetPlainId()];
		
		Editor editor = new Editor(identity.GetPlainId(), identity.GetFullName(), Symbols.CAMERA.Regular(), identity, GetPlayer());
		m_Master["SERVER"][identity.GetPlainId()] = editor;
		
		m_Master.Synchronize();
	}
	
	EditorNode GetMaster()
	{
		return m_Master;
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
	
	Editor GetEditor()
	{		
		if (IsServer()) {
			return Editor.Cast(m_Master["SERVER"]);
		}
		
		if (!m_Master["SERVER"] || !GetUserManager() || !GetUserManager().GetTitleInitiator()) {
			return null;
		}
				
		return Editor.Cast(m_Master["SERVER"][GetUserManager().GetTitleInitiator().GetUid()]);
	}
}