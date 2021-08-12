using System;

namespace Enfusion_Deserializer
{
    public class vector
    {
        public float X, Y, Z;

        public vector(float x = 0, float y = 0, float z = 0)
        {
            X = x; Y = y; Z = z;
        }

        public float this[int index] {
            get {
                return index switch {
                    0 => X,
                    1 => Y,
                    2 => Z,
                    _ => throw new IndexOutOfRangeException(),
                };
            }

            set {
                switch (index) {
                    case 0: X = value; break;
                    case 1: Y = value; break;
                    case 2: Z = value; break;
                    default: throw new IndexOutOfRangeException();
                }
            }
        }

        public static readonly vector Zero = new();

        public override string ToString()
        {
            return $"{X}, {Y}, {Z}";
        }
    }
}
