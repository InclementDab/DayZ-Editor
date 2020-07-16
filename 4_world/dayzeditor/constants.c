
static string layout_dir = "P:/DayZ_Server/dev/DayZEditor/Addons/Editor/Layouts/";





modded class Animal_GallusGallusDomesticus 
{
    
	void Animal_GallusGallusDomesticus()
    {
        SetEventMask(EntityEvent.POSTFRAME);
    }
	
	
    override void EOnPostFrame(IEntity other, int extra)
    {
		float scale = 1700;
        float xScale = scale;
        float yScale = scale;
        float zScale = scale;
        vector mat[4];
        GetTransform(mat);

        mat[0] = mat[0].Normalized() * xScale;
        mat[1] = mat[1].Normalized() * yScale;
        mat[2] = mat[2].Normalized() * zScale;

        SetTransform(mat);
    }
}