class Cartesian
{
	protected EntityAI CartesianObject;

    void Cartesian(vector position)
    {
        CartesianObject = GetGame().CreateObjectEx("m_3DWidget", position, ECE_KEEPHEIGHT | ECE_NOSURFACEALIGN | ECE_TRACE);
        CartesianObject.SetOrientation(vector.Up);
    }

    void ~Cartesian()
    {
        GetGame().ObjectDelete(CartesianObject);
    }

    static Cartesian CreateOnObject(Object obj)
    {
        vector size = GetObjectSize(obj);
        vector position = obj.GetPosition();

        position[1] = position[1] + size[1] / 2;
        Cartesian c = new Cartesian(position);
        vector c_size = GetObjectSize(c.CartesianObject);
        vector c_pos = c.CartesianObject.GetPosition();
        c_pos[0] = c_pos[0] + c_size[0] / 2;
        c_pos[1] = c_pos[1] + c_size[1] / 2;
        c_pos[2] = c_pos[2] + c_size[2] / 2;
        c.CartesianObject.SetPosition(c_pos);

        return c;
    }
}
