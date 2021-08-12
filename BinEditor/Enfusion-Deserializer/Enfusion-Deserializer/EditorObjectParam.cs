using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Enfusion_Deserializer
{
    public abstract class EditorObjectParam
    {
        public List<string> Types = new();

        public string GetSerializableType()
        {
            string type = GetType().ToString();
            type = type.Replace("Enfusion_Deserializer.", "");

            // Formatted already
            if (Types.Count == 0) {
                return type;
            }

            type += "<";
            type += string.Join(", ", Types);
            type += ">";

            return type;
        }

        public abstract bool Write(EnfusionSerializer stream);

        public abstract bool Read(EnfusionSerializer stream);
    }

    public class EditorObjectParam1 : EditorObjectParam
    {
        public object param1;

        public override bool Write(EnfusionSerializer stream)
        {
            return true;
        }

        public override bool Read(EnfusionSerializer stream)
        {
            foreach (string type in Types) {
                switch (type) {
                    case "string\0":
                        param1 = stream.ReadString();
                        Console.WriteLine(param1);
                        return true;

                    case "int\0":
                        param1 = stream.ReadInt();
                        return true;

                    case "float\0":
                        param1 = stream.ReadFloat();
                        return true;

                    case "bool\0":
                        param1 = stream.ReadBool();
                        return true;

                    default: throw new NotSupportedException();
                }
            }

            return false;
        }
    }
}
