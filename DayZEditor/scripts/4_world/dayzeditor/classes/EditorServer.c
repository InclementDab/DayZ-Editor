
enum EditorServerModuleRPC
{
	INVALID = 35114,
	CREATEOBJECT,
	CREATECAMERA,
	COUNT
};


class EditorServerModule: JMModuleBase
{
	
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
			
			case EditorServerModuleRPC.CREATEOBJECT: {
				RPC_CreateObject(ctx, sender, target);
				break;
			}
			
			case EditorServerModuleRPC.CREATECAMERA: {
				RPC_CreateCamera(ctx, sender, target);
				break;
			}
			
			
		}
		
	}
	
	override bool IsClient()
	{
		return false;
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

