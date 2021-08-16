using DayZ_Bin_Editor.Editor;
using DayZ_Bin_Editor.Enfusion;
using Microsoft.Win32;
using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace DayZ_Bin_Editor
{
    /// <summary>
    /// Interaction logic for AppWindow.xaml
    /// </summary>
    public partial class AppWindow : Window
    {
        protected AppWindowViewModel m_AppWindowViewModel;

        public AppWindow()
        {
            InitializeComponent();

            DataContext = m_AppWindowViewModel = new AppWindowViewModel();
        }

        private void OpenCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            OpenFileDialog file_dialog = new OpenFileDialog {
                Filter = "DayZ Editor Files (*.dze)|*.dze"
            };

            file_dialog.ShowDialog();

            EnfusionSerializer stream = new EnfusionSerializer(file_dialog.FileName, FileMode.Open, FileAccess.Read);

            m_AppWindowViewModel.SaveData.Read(stream);
            stream.Close();
        }

        private void SaveCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            SaveFileDialog save_dialog = new SaveFileDialog();
            save_dialog.ShowDialog();

            EnfusionSerializer stream = new EnfusionSerializer(save_dialog.FileName, FileMode.Create, FileAccess.Write);
            m_AppWindowViewModel.SaveData.Write(stream);
            stream.Close();
        }

        private void ExitCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }
    }

    public class AppWindowViewModel
    {
        public EditorSaveData SaveData { get; set; } = new EditorSaveData();
    }
}
