using System.Reflection;

namespace DirectDrawConverter
{
    public struct Vec2D
    {
        public int X { get; set; }
        public int Y { get; set; }

        public Vec2D() 
        {
        }
        public Vec2D(int x, int y) 
        { 
            X = x;
            Y = y;
        }

        public override string ToString()
        {
            return $"{X} {Y}";
        }
    }

    public class EnfusionConfigBase
    {
        public string? Name;
        
        public EnfusionConfigBase()
        {
        }

        public EnfusionConfigBase(string? name)
        {
            Name = name;
        }

        public virtual bool DeclareNameBeforeScope()
        {
            return true;
        }

        public virtual string ToStringy(int tab_count = 0)
        {
            var tab_generator = (int tab_count) => {
                return String.Concat(Enumerable.Repeat("\t", tab_count));
            };
            
            string result = $"{tab_generator(tab_count)}{GetType().Name}Class {(DeclareNameBeforeScope() ? Name : "")} {{\n";
            foreach (FieldInfo field_info in GetType().GetFields().Reverse()) {
                object field_value = field_info.GetValue(this);
                if (field_value == null) {
                    continue;
                }

                if (field_value is IEnumerable<EnfusionConfigBase>) {
                    result += $"{tab_generator(tab_count + 1)}{field_info.Name} {{\n";

                    foreach (EnfusionConfigBase field_list_item in field_value as IEnumerable<EnfusionConfigBase>) {
                        result += field_list_item.ToStringy(tab_count + 2);
                    }

                    result += $"{tab_generator(tab_count + 1)}}}\n";
                    continue;
                }
                
                if (field_value is string) {
                    result += $"{tab_generator(tab_count + 1)}{field_info.Name} \"{field_value}\"\n";
                    continue;
                }

                result += $"{tab_generator(tab_count + 1)}{field_info.Name} {field_value}\n";
            }

            result += $"{tab_generator(tab_count)}}}\n";
            return result;
        }
    }

    public class ImageSet: EnfusionConfigBase
    {
        public List<ImageSetGroup> Groups = new();
        public List<ImageSetDef> Images = new();
        public List<ImageSetTexture> Textures = new();
        public Vec2D RefSize;       

        public class ImageSetTexture: EnfusionConfigBase
        {
            public string path;
            public int mpix;
        }

        public class ImageSetGroup: EnfusionConfigBase
        {
            public List<ImageSetDef> Images = new();
        }

        public class ImageSetDef: EnfusionConfigBase
        {
            public ImageSetFlags Flags = 0;
            public Vec2D Size;
            public Vec2D Pos;

            public enum ImageSetFlags
            {
                ISHorizontalTile = 1,
                ISVerticalTile = 2,
            }
        }

        public override bool DeclareNameBeforeScope()
        {
            return false;
        }
    }


    public class MetaFile: EnfusionConfigBase
    {
        public string Author;
        public int ChangeDate;
        public override bool DeclareNameBeforeScope()
        {
            return false;
        }
    }
}