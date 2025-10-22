class EditorNetActionManager
{
    void SendObjectUpdate(EditorObject obj)
    {
        if (GetGame().IsMultiplayer() && obj.Uuid != string.Empty)
        {
            Print(string.Format("[CLIENT | PERSISTENCE-SEND] Sending full OBJECT_UPDATE for UUID %1 at final position %2", obj.Uuid, obj.GetPosition().ToString()));
            ScriptRPC rpc = new ScriptRPC();
            rpc.Write(1); // Count
            rpc.Write(obj.Uuid);
            obj.GetData().Write(rpc, int.MAX);
            rpc.Send(null, EditorRPC.OBJECT_UPDATE, true);
        }
    }

    void SendTransformUpdate(array<EditorObject> objects)
    {
        if (!GetGame().IsMultiplayer() || objects.Count() == 0) return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(objects.Count());

        int packed[4];
        foreach (EditorObject obj : objects)
        {
            EditorNetUtils.PackTransform(obj.GetPosition(), obj.GetOrientation(), obj.GetScale(), packed);
            rpc.Write(obj.Uuid);
            rpc.Write(packed[0]);
            rpc.Write(packed[1]);
            rpc.Write(packed[2]);
            rpc.Write(packed[3]);
        }

        rpc.Send(null, EditorRPC.BATCH_UPDATE_TRANSFORM_PACKED, true);
    }

    void SendDragSessionStart(EditorObject parent, array<EditorObject> children)
    {
        if (!GetGame().IsMultiplayer()) return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(eDragPhase.START);
        rpc.Write(parent.Uuid);
        rpc.Write(children.Count());
        foreach (EditorObject child : children)
        {
            rpc.Write(child.Uuid);
        }
        rpc.Send(null, EditorRPC.DRAG_SESSION, true);
    }

    void SendDragSessionUpdate(string parentUUID, int packedData[4])
    {
        if (!GetGame().IsMultiplayer()) return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(eDragPhase.UPDATE);
        rpc.Write(parentUUID);
        rpc.Write(packedData[0]);
        rpc.Write(packedData[1]);
        rpc.Write(packedData[2]);
        rpc.Write(packedData[3]);
        rpc.Send(null, EditorRPC.DRAG_SESSION, false); // Unreliable
    }

    void SendDragSessionEnd(string parentUUID, int packedData[4])
    {
        if (!GetGame().IsMultiplayer()) return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(eDragPhase.END);
        rpc.Write(parentUUID);
        rpc.Write(packedData[0]);
        rpc.Write(packedData[1]);
        rpc.Write(packedData[2]);
        rpc.Write(packedData[3]);
        rpc.Send(null, EditorRPC.DRAG_SESSION, true); // Reliable
    }
}
