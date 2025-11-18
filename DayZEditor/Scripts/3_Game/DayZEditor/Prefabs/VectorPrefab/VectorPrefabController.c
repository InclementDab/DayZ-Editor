modded class VectorPrefabController
{
    override void PropertyChanged(string property_name)
    {
        if (property_name == "X" || property_name == "Y" || property_name == "Z")
        {
            vector v = Value;
            
            if (property_name == "X") v[0] = X.Parse();
            if (property_name == "Y") v[1] = Y.Parse();
            if (property_name == "Z") v[2] = Z.Parse();
            
            Value = v;
            
            PrefabBase<vector> parent = PrefabBase<vector>.Cast(GetParent());
            if (parent) 
            {
                parent.PrefabPropertyChanged("Value");
            }
            
            return;
        }
        
        super.PropertyChanged(property_name);
    }
}