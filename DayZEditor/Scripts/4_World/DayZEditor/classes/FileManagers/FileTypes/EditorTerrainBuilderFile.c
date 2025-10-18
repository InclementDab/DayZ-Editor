//DayZ Editor export to Terrain Builder
//Coded by FarTooBaked
//Shout to Dab and team for making DayZ Editor awesome!

class EditorMatrix3
{
    float elements[9];

    void EditorMatrix3()
    {
        IdentityMatrix();
    }

    void IdentityMatrix()
    {
        elements[0] = 1; elements[1] = 0; elements[2] = 0;
        elements[3] = 0; elements[4] = 1; elements[5] = 0;
        elements[6] = 0; elements[7] = 0; elements[8] = 1;
    }

    int Index(int row, int col)
    {
        return row * 3 + col;
    }

    EditorMatrix3 Multiply(EditorMatrix3 other)
    {
        EditorMatrix3 result = new EditorMatrix3();
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                result.elements[Index(i, j)] = elements[Index(i, 0)] * other.elements[Index(0, j)] + elements[Index(i, 1)] * other.elements[Index(1, j)] + elements[Index(i, 2)] * other.elements[Index(2, j)];
            }
        }
        return result;
    }

    vector ToYawPitchRoll()
    {
        float pitchRad = Math.Asin(elements[Index(0, 2)]);
        float pitchDeg = pitchRad * Math.RAD2DEG;
        float cosPitch = Math.Cos(pitchRad);
        float yawRad, rollRad;
        if (Math.AbsFloat(cosPitch) > 0.001)
        {
            yawRad = Math.Atan2(-elements[Index(1, 2)], elements[Index(2, 2)]);
            rollRad = Math.Atan2(-elements[Index(0, 1)], elements[Index(0, 0)]);
        }
        else
        {
            yawRad = Math.Atan2(elements[Index(2, 0)], elements[Index(2, 1)]);
            rollRad = 0;
        }
        float yawDeg = yawRad * Math.RAD2DEG;
        float rollDeg = rollRad * Math.RAD2DEG;
        return Vector(yawDeg, pitchDeg, rollDeg);
    }
}

class EditorTerrainBuilderFile: EditorFileType
{
    override void Export(EditorSaveData data, string file, ExportSettings settings, eDialogExtraSetting dialog_setting)
    {
        EditorLog.Trace("EditorTerrainBuilderFile::Export");
		FileHandle handle = OpenFile(file, FileMode.WRITE);
		if (!handle)
		{
			EditorLog.Error("File in use %1", file);
			return;
		}

		foreach (EditorObjectData editor_object: data.EditorObjects)
		{
			vector object_POS = editor_object.Position; //weird value when calling WorldObject instead
			vector object_ROT = editor_object.Orientation; //weird value when calling WorldObject instead
			vector object_BC = editor_object.WorldObject.GetBoundingCenter();
			vector object_mat[4];
			editor_object.WorldObject.GetTransform(object_mat);
			object_BC = object_BC.Multiply3(object_mat);

			string type = editor_object.Type;
			float scale = editor_object.WorldObject.GetScale();

			//Wonky way to export Statics and Brushed Objects to Terrain Builder that works for 99% of objects 
			string model_name = editor_object.WorldObject.GetShapeName();
			if (model_name == "UNKNOWN_P3D_FILE")
			{
				model_name = editor_object.Type;
			}

			if (model_name != "")
			{
				array<string> split_path = {};
				model_name.Replace("/", "\\");
				model_name.Split("\\", split_path);
				if (split_path.Count() > 0)
				{
					model_name = split_path[split_path.Count() - 1];
				}
				if (model_name.Contains(".p3d"))
				{
					model_name.Replace(".p3d", "");
				}
				if (model_name.Contains("Land_"))
				{
					model_name.Replace("Land_", "");
				}
			}
			if (model_name == "")
			{
				model_name = "UNKNOWN_OBJECT";
			}

			string position_x_string = object_POS[0].ToString();
			array<string> split = {};
			position_x_string.Split(".", split);
			int cnt = split[0].Length();
			position_x_string = String("200000").Substring(0, 6 - cnt) + position_x_string;

			object_POS[1] = object_POS[1] - object_BC[1];

			EditorMatrix3 matExtrinsic = BuildRotationMatrix(object_ROT[0], object_ROT[1], object_ROT[2]);
			vector oriExtrinsic = matExtrinsic.ToYawPitchRoll();
            string extrinsic_line = string.Format("\"%1\";%2;%3;%4;%5;%6;%7;%8", model_name, position_x_string, object_POS[2], oriExtrinsic[0], oriExtrinsic[1], oriExtrinsic[2], scale, object_POS[1]);
			FPrintln(handle, extrinsic_line);
		}
		CloseFile(handle);
    }

    override string GetExtension()
    {
        return ".txt";
    }

    override void GetValidExtensions(notnull inout array<ref Param2<string, string>> valid_extensions)
    {
        super.GetValidExtensions(valid_extensions);
        valid_extensions.Insert(new Param2<string, string>("Text File", "*.txt"));
    }

    static EditorMatrix3 BuildRotationMatrix(float yawDeg, float pitchDeg, float rollDeg)
    {
        EditorMatrix3 R_xYaw = RotationX(yawDeg);
        EditorMatrix3 R_yPitch = RotationY(pitchDeg);
        EditorMatrix3 R_zRoll = RotationZ(rollDeg);
        EditorMatrix3 A, B, C;
        
        A = R_zRoll; B = R_yPitch; C = R_xYaw;

        EditorMatrix3 tmp = A.Multiply(B);
        return tmp.Multiply(C);
    }

    static EditorMatrix3 RotationX(float angleDeg)
    {
        EditorMatrix3 result = new EditorMatrix3();
        float rad = Math.DEG2RAD * angleDeg;
        float c = Math.Cos(rad);
        float s = Math.Sin(rad);

        result.elements[0] = 1; result.elements[1] = 0; result.elements[2] = 0;
        result.elements[3] = 0; result.elements[4] = c; result.elements[5] = -s;
        result.elements[6] = 0; result.elements[7] = s; result.elements[8] = c;

        return result;
    }

    static EditorMatrix3 RotationY(float angleDeg)
    {
        EditorMatrix3 result = new EditorMatrix3();
        float rad = Math.DEG2RAD * angleDeg;
        float c = Math.Cos(rad);
        float s = Math.Sin(rad);

        result.elements[0] = c;  result.elements[1] = 0; result.elements[2] = s;
        result.elements[3] = 0;  result.elements[4] = 1; result.elements[5] = 0;
        result.elements[6] = -s; result.elements[7] = 0; result.elements[8] = c;

        return result;
    }

    static EditorMatrix3 RotationZ(float angleDeg)
    {
        EditorMatrix3 result = new EditorMatrix3();
        float rad = Math.DEG2RAD * angleDeg;
        float c = Math.Cos(rad);
        float s = Math.Sin(rad);

        result.elements[0] = c;  result.elements[1] = -s; result.elements[2] = 0;
        result.elements[3] = s;  result.elements[4] = c;  result.elements[5] = 0;
        result.elements[6] = 0;  result.elements[7] = 0;  result.elements[8] = 1;

        return result;
    }
}
