class EditorFileType : Managed
{
    EditorSaveData Import(string file, ImportSettings settings);
    void Export(EditorSaveData data, string file, ExportSettings settings);

    string GetExtension()
    {
        return ".txt";
    }

    void GetValidExtensions(notnull inout array<ref Param2<string, string>> valid_extensions)
    {
        valid_extensions.Insert(new Param2<string, string>("Any", "*.*"));
    }

    bool CanDoDeletion()
    {
        return false;
    }

    static vector GetOffset(string name)
    {
        Object object = GetGame().CreateStaticObjectUsingP3D(name, vector.Zero, vector.Zero, 1);
        if (!object)
        {
            return vector.Zero;
        }
        vector offset = object.GetBoundingCenter();
        object.Delete();
        return offset;
    }
}