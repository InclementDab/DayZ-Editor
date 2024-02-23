class AfterlifeToggle: NamedNode
{	
	protected ref array<Object> m_UnhiddenObjects = {};
	
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
		EditorNode editor = EditorNode.Cast(FindAncestor(EditorNode));
		if (!GetGame().IsServer() || !editor) {
			Print(editor);
			Error("Wyhat");
			return;
		}

		if (node_state.IsActive()) {
			array<ref SuppressedObject> suppressed_objects = GetDayZGame().GetSuppressedObjectManager().GetObjects();			
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(suppressed_objects.Count());
			
			if (state) {
				foreach (auto suppressed_object: suppressed_objects) {
					rpc.Write(suppressed_object.GetObject());
					m_UnhiddenObjects.Insert(suppressed_object.GetObject());
				}
				
				rpc.Send(null, SuppressedObjectManager.RPC_UNSUPPRESS, true, editor.Identity);
			} else {
				foreach (auto unsuppressed_object: m_UnhiddenObjects) {
					rpc.Write(unsuppressed_object);
				}
				
				m_UnhiddenObjects.Clear();
				rpc.Send(null, SuppressedObjectManager.RPC_SUPPRESS, true, editor.Identity);
			}			
		} 
	}
	
	override void OnInteract(NodeInteractType interact_type, Widget widget = null)
	{
		super.OnInteract(interact_type, widget);
		
		ToggleState(NodeState.ACTIVE);
		SetSynchDirty();
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.PRESS;
	}
}