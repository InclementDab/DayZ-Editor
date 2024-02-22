modded class EditorObjectData
{
	ObjectNode CovertToNode()
	{
		vector matrix[4];
		Math3D.MatrixOrthogonalize4(matrix);
		Object object = EditorNode.CreateObject(Type, matrix);
		if (!object) {
			return null;
		}
		
		object.SetPosition(Position);
		object.SetOrientation(Orientation);
		object.SetScale(Scale);
		return new ObjectNode(UUID.Generate(), object.GetDebugName(), Symbols.TREE, LinearColor.WHITE, object);
	}
}