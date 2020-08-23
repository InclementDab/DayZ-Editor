


class PropertyInfo
{
	typename Type;
	string Name;
	
	void PropertyInfo(typename type, string name)
	{
		Type = type; Name = name;
	}
}

class DataBindingHashMap: map<string, ref DataBindingBase> {}