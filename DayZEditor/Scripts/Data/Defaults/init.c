static void SpawnObject(string type, vector position, vector orientation)
{
    auto obj = GetGame().CreateObjectEx(type, position, ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS);
    obj.SetPosition(position);
    obj.SetOrientation(orientation);
    obj.SetOrientation(obj.GetOrientation());
    obj.SetFlags(EntityFlags.STATIC, false);
    obj.Update();
	obj.SetAffectPathgraph(true, false);
	if (obj.CanAffectPathgraph()) GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj);
}

static ref map<int, Object> WorldObjects;
static void DeleteObject(int object_id)
{
    // Adds all map objects to the WorldObjects array
    if (!WorldObjects) {
        WorldObjects = new map<int, Object>();
		ref array<Object> objects = {};
		ref array<CargoBase> cargos = {};
		GetGame().GetObjectsAtPosition(Vector(7500, 0, 7500), 20000, objects, cargos);
		foreach (Object o: objects) {
			WorldObjects.Insert(o.GetID(), o);
		}
    }

    CF_ObjectManager.HideMapObject(WorldObjects[object_id]);
}