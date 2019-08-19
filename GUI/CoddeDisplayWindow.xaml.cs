/////////////////////////////////////////////////////////////////////
// CodeDisplayWindow.xaml.cs                                       //
//                                                                 //
// Abinav Murugadass, CSE687                                       //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines one class SelectionWindow that provides a 
 * ListBox for displaying files selected in the main window.
 * 
 * Two way communication is provided:
 * - MainWindow creates SelectionWindow and stores a reference as
 *   member data.
 * - SelectionWindow provides a SetMainwindow(MainWindow mwin) function
 *   which MainWindow uses to give SelectionWindow access to its public
 *   members.
 * 
 * Required Files:
 * ---------------
 * MainWindow.xaml, MainWindow.xaml.cs
 * SelectionWindow.xaml, SelectionWindow.xaml.cs
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 14 April 2019
 * 
 * 
 */
using System;
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

namespace WpfApp1
{
    public partial class CoddeDisplayWindow : Window
    {
        MainWindow mWin = null;
        public CoddeDisplayWindow()
        {
            InitializeComponent();
        }
        public void setMainWindow(MainWindow mwin)
        {
            mWin = mwin;
        }
        //----< called by MainWindow to add a selected file >------------

        public void show(string file)
        {
            string srcText;
            System.IO.StreamReader sr = null;
            try
            {
                sr = System.IO.File.OpenText(file);
            }
            catch
            {
                return;
            }
            srcText = sr.ReadToEnd();
            CodeSelection.Text = srcText;
        }
        //----< unselect files and set mWin.swin to null >---------------

        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
            mWin.unselectFiles();
            mWin.swin = null;
        }
    }
}
