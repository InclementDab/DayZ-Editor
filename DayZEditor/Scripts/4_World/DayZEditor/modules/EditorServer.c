
enum EditorServerModuleRPC
{
	INVALID = 35114,
	EDITOR_CLIENT_UPDATE,
	EDITOR_CLIENT_CREATED,
	EDITOR_CLIENT_DESTROYED,
	COUNT
};


class EditorServerModule: JMModuleBase
{
	
	private ref map<int, EntityAI> m_EditorClientModels;
	
	override void OnInit()
	{
		m_EditorClientModels = new map<int, EntityAI>();
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
			
			case EditorServerModuleRPC.EDITOR_CLIENT_UPDATE: {
				EditorLog.Debug("EditorServerModule::EDITOR_SERVER_UPDATE");
				RPC_UpdateEditor(ctx, sender, target);
				break;
			}
			
			case EditorServerModuleRPC.EDITOR_CLIENT_CREATED: {
				EditorLog.Debug("EditorServerModule::EDITOR_CLIENT_CREATED");
				m_EditorClientModels.Insert(sender.GetPlayerId(), GetGame().CreateObjectEx("DSLRCamera", vector.Zero, ECE_NONE));
				break;
			}
			
			case EditorServerModuleRPC.EDITOR_CLIENT_DESTROYED: {
				EditorLog.Debug("EditorServerModule::EDITOR_CLIENT_DESTROYED");
				m_EditorClientModels.Remove(sender.GetPlayerId());
				break;
			}
		}
	}
	
	override bool IsClient() { return false; }
	
	private void RPC_UpdateEditor(ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		vector pos, ori;
		ctx.Read(pos);
		ctx.Read(ori);
		
		EntityAI cam = m_EditorClientModels.Get(senderRPC.GetPlayerId());
		vector transform[4];

		ori.RotationMatrixFromAngles(transform);
		transform[3] = pos;

		cam.MoveInTime(transform, 0);
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
	}
	
}

