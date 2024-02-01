modded class DayZGame
{
	// Handled in DayZGame
	static const int RPC_REQUEST_SYNC = 54364;
	static const int RPC_NODE_SYNC = 54365;
	
	protected ref TreeNode m_Server = new TreeNode("SERVER", "Editors", Symbols.CAMERA_SECURITY);
	protected ref EditorNode m_Editor;
		
	void DayZGame()
	{
#ifdef WORKBENCH
		m_Editor = new EditorNode("WORKBENCH", "Debug", Symbols.CAMERA_SECURITY, null, null);
#endif
	}
		
	EditorNode GetEditor()
	{
#ifdef WORKBENCH
		return m_Editor;
#endif
		
#ifdef SERVER
		return m_Server;
#else		
		return EditorNode.Cast(m_Server.Children[GetUserManager().GetTitleInitiator().GetUid()]);
#endif
	}
			
	override bool OnInitialize()
	{
		BiosUserManager manager = GetGame().GetUserManager();
		if (manager && manager.GetTitleInitiator()) {
			manager.SelectUserEx(manager.GetTitleInitiator());
		}
		
		if (manager && manager.GetSelectedUser()) {
			SetPlayerName(manager.GetSelectedUser().GetName());
		}
		
		return super.OnInitialize();
	}
	
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{				
		switch (rpc_type) {			
			case RPC_REQUEST_SYNC: {
				m_Server.Synchronize(sender);
				break;
			}
				
			case RPC_NODE_SYNC: {	
				int tree_depth;
				if (!ctx.Read(tree_depth)) {
					break;
				}

				TreeNode current = m_Server;
				for (int i = 0; i < tree_depth; i++) {
					string uuid;
					ctx.Read(uuid);
					
					string type;
					ctx.Read(type);
					
					TreeNode node = current[uuid];
					if (!node) {
						node = TreeNode.Cast(type.ToType().Spawn());
						if (!node) {
							Error("Invalid node type " + type);
							continue;
						}
						
						current[uuid] = node;
						node.Parent = current[uuid];
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
				} else {
					current.OnSynchronized();
				}
				break;
			}
		}
		
		super.OnRPC(sender, target, rpc_type, ctx);
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
		delete m_Server[identity.GetPlainId()];
		
		EditorNode editor = new EditorNode(identity.GetPlainId(), identity.GetFullName(), Symbols.CAMERA.Regular(), identity, GetPlayer());
		m_Server[identity.GetPlainId()] = editor;
		
		editor.OnSynchronized();
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
}