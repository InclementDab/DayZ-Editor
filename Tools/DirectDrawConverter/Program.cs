// was publishing wiht 'dotnet publish -r win-x64 --sc false' but idk how to reliably get it running on other machines

using System.Drawing;
using DdsFileTypePlus;
using DirectDrawConverter;
using PaintDotNet;
using Svg;


const int IMAGE_MAX_SIZE = 512; // this is the space allocated to each image
const int IMAGE_SIZE = 256;

string folder_output = "P:\\Editor\\GUI\\icons";
DirectoryInfo dest_directory = Directory.CreateDirectory(folder_output);
DirectoryInfo source_directory = Directory.CreateDirectory(Directory.GetCurrentDirectory() + "\\svgs");

foreach (DirectoryInfo directory in source_directory.EnumerateDirectories()) {
    string output_file = dest_directory + $"\\{directory.Name}.dds";
    string enfusion_output_file = output_file.Replace("P:\\", "");
    enfusion_output_file = enfusion_output_file.Replace("dds", "edds");

    string formatted_directory_name = directory.Name;
    formatted_directory_name = formatted_directory_name.Replace("-", "_");

    File.Delete(dest_directory + $"\\{formatted_directory_name}.imageset");

    IEnumerable<FileInfo> files = directory.GetFiles("*.svg");
    int row_count = (int)Math.Floor(Math.Sqrt(files.Count()));

    int imageset_width = Math.Min(row_count, files.Count()) * IMAGE_SIZE;
    int imageset_height = (int)(Math.Ceiling(files.Count() / (decimal)row_count) * IMAGE_SIZE);

    Surface surface = new(imageset_width, imageset_height);
    ImageSet image_set = new() {
        Name = formatted_directory_name,
        RefSize = new Vec2D(imageset_width, imageset_height),
        Textures = {
            new() {
                mpix = 1,
                path = enfusion_output_file
            }
        }
    };

    MetaFile meta_file = new() {
        Name = enfusion_output_file,
        Author = Environment.UserName,
        ChangeDate = (int)(DateTime.UtcNow - new DateTime(1970, 1, 1)).TotalSeconds
    };

    DateTime file_start = DateTime.Now;
    Console.WriteLine($"Loading {files.Count()} files");

    int x_offset = 0, y_offset = 0;
    foreach (FileInfo file in files) {
        // workbench doesnt like dashes in the name
        string format_name = file.Name;
        format_name = format_name.Replace("-", "_");
        string file_name = Path.GetFileNameWithoutExtension(format_name);

        SvgDocument doc = SvgDocument.Open(file.ToString());
        double w_aspect = 1.0;
        double h_aspect = 1.0;
        if (doc.Bounds.Width > IMAGE_MAX_SIZE || doc.Bounds.Height > IMAGE_MAX_SIZE) {
            w_aspect = doc.Bounds.Width / IMAGE_MAX_SIZE;
        }
        // we bound to the larger of the two aspects
        if (w_aspect > 1.0) {
            h_aspect = 1.0 / w_aspect;
            w_aspect = 1.0;
        }

        int width = (int)(IMAGE_SIZE * w_aspect);
        int height = (int)(IMAGE_SIZE * h_aspect);
        Bitmap bitmap = doc.Draw(width, height);

        int w_centering_offset = (IMAGE_SIZE - width) / 2;
        int h_centering_offset = (IMAGE_SIZE - height) / 2;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                ColorBgra color = ColorBgra.White;
                color.A = bitmap.GetPixel(x, y).A;
                surface[new Point(x + x_offset + w_centering_offset, y + y_offset + h_centering_offset)] = color;
            }
        }

        image_set.Images.Add(new ImageSet.ImageSetDef() {           
            Name = file_name,
            Pos = new Vec2D(x_offset + w_centering_offset, y_offset + h_centering_offset),
            Size = new Vec2D(width, height),
            Flags = 0
        });

        x_offset += IMAGE_SIZE;
        if (x_offset >= row_count * IMAGE_SIZE) {
            x_offset = 0;
            y_offset += IMAGE_SIZE;
        }
    }

    File.WriteAllText(dest_directory + $"\\{directory.Name}.imageset", image_set.ToStringy());

    Console.WriteLine($"ImageSet generated {dest_directory + $"\\{directory.Name}.imageset"} complete, took {(DateTime.Now - file_start).TotalSeconds}");
    File.Delete(output_file);

    DateTime dds_start = DateTime.Now;
    Stream dds_stream = new FileStream(output_file, FileMode.CreateNew);
    DdsFile.Save(
        dds_stream,
        DdsFileFormat.BC3,
        DdsErrorMetric.Perceptual,
        BC7CompressionSpeed.Medium,
        false,
        false,
        ResamplingAlgorithm.Bilinear,
        surface,
        ProgressChanged);

    dds_stream.Close();

    Console.WriteLine($"Output to {output_file} complete, took {(DateTime.Now - dds_start).TotalSeconds}");
}

void ProgressChanged(object sender, ProgressEventArgs e)
{
    Console.Clear();
    Console.WriteLine($"{e.Percent}%");
}    
