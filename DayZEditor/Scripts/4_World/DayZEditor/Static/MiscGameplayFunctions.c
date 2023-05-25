modded class MiscGameplayFunctions
{
	static override void RemoveAllAttachedChildrenByTypename(notnull EntityAI parent, array<typename> listOfTypenames)
	{
		if (listOfTypenames.Count() > 0)
		{
			Object child = Object.Cast(parent.GetChildren());
			while (child)
			{
				//! get rid of lookup for ParticleSource and others
				if (!child.IsInherited(EntityAI))
					return;
	
				if (child.IsAnyInherited(listOfTypenames)) {
					vector pos = child.GetPosition();
					Object childToDelete = child;
					child = Object.Cast(child.GetSibling());
					parent.RemoveChild(childToDelete, false);
	
					vector m4[4];
					Math3D.MatrixIdentity4(m4);
					m4[3] = pos;
					childToDelete.SetTransform(m4);
					childToDelete.PlaceOnSurface();
				} else {
					child = Object.Cast(child.GetSibling());
				}
			}
		}
	}
}