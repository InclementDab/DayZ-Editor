
enum EditorServerModuleRPC
{
	INVALID = 35114,
	EDITOR_SERVER_UPDATE,
	EDITOR_CLIENT_CREATE,
	EDITOR_CLIENT_DESTROY,
	COUNT
};


class EditorServerModule: JMModuleBase
{
	
	private ref map<int, ref EditorClientModule> m_EditorClients;
	
	override void OnInit()
	{
		m_EditorClients = new map<int, ref EditorClientModule>();
	}
	
	
	override int GetRPCMin()
	{
		return EditorServerModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return EditorServerModuleRPC.COUNT;
	}
	
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx)
	{
		switch (rpc_type) {
			
			case EditorServerModuleRPC.EDITOR_SERVER_UPDATE: {
				EditorLog.Debug("EditorServerModule::EDITOR_SERVER_UPDATE");
				RPC_UpdateEditor(ctx, sender, target);
				break;
			}
			
			case EditorServerModuleRPC.EDITOR_CLIENT_CREATE: {
				EditorLog.Debug("EditorServerModule::EDITOR_CLIENT_CREATE");
				EditorClientModule m = new EditorClientModule();
				ctx.Read(m);
				m_EditorClients.Insert(sender.GetPlayerId(), m);
				
				ScriptRPC create_rpc = new ScriptRPC();
				create_rpc.Write(sender.GetPlayerId());
				create_rpc.Send(null, EditorClientModuleRPC.EDITOR_CLIENT_CREATED, true);
				
				break;
			}
			
			case EditorServerModuleRPC.EDITOR_CLIENT_DESTROY: {
				EditorLog.Debug("EditorServerModule::EDITOR_CLIENT_DESTROY");
				m_EditorClients.Remove(sender.GetPlayerId());
				
				ScriptRPC destroy_rpc = new ScriptRPC();
				destroy_rpc.Write(sender.GetPlayerId());
				destroy_rpc.Send(null, EditorClientModuleRPC.EDITOR_CLIENT_DESTROYED, true);
				break;
			}

			
			
		}
		
	}
	
	override bool IsClient() { return false; }
	
	private void RPC_UpdateEditor(ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		EditorLog.Trace("EditorServerModule::RPC_UpdateEditor");
		
		
		vector pos, ori;
		ctx.Read(pos);
		ctx.Read(ori);
		
		//EditorClientModule client = m_EditorClients.Get(senderRPC.GetPlayerId());
		
		foreach (int id, ref EditorClientModule client: m_EditorClients) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(id);
			rpc.Write(pos);
			rpc.Write(ori);
			rpc.Send(null, EditorClientModuleRPC.EDITOR_CLIENT_UPDATE, true);
		}
	}
		
	private void RPC_CreateObject(ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		EditorLog.Trace("EditorServer::RPC_CreateObject");
		EditorObjectData data = new EditorObjectData();
		data.OnRead(ctx);
		
		GetGame().CreateObjectEx(data.Type, data.Position, ECE_NONE);
	}
	
	private void RPC_CreateCamera(ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		EditorLog.Trace("EditorServer::RPC_CreateCamera");

		vector pos;
		ctx.Read(pos);
	
		Object world_object = GetGame().CreateObjectEx("DSLRCamera", pos, ECE_NONE);
		Print(world_object);
		Print(world_object.GetPosition());
		//camera.SetWorldObject(world_object);
	}
	
}

