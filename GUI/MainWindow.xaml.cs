///////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - GUI for Project3HelpWPF                      //
// ver 1.0                                                           //
// Abinav Murugadass, CSE687 - Object Oriented Design                //
// Source: Jim Fawcett                                               //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based GUI for Project3HelpWPF demo.  It's 
 * responsibilities are to:
 * - Provide a display of directory contents of a remote ServerPrototype.
 * - It provides a subdirectory list and a filelist for the selected directory.
 * - You can navigate into subdirectories by double-clicking on subdirectory
 *   or the parent directory, indicated by the name "..".
 *   
 * Required Files:
 * ---------------
 * Mainwindow.xaml, MainWindow.xaml.cs
 * Translater.dll
 * 
 * Maintenance History:
 * --------------------
 * Ver 1.2 : 05/01/2019
 * - Edited and added functiuonalities to demonstrate project 4 
 * ver 1/1 : 07 Aug 2018
 * - fixed bug in DirList_MouseDoubleClick by returning when selectedDir is null
 * ver 1.0 : 30 Mar 2018
 * - first release
 * - Several early prototypes were discussed in class. Those are all superceded
 *   by this package.
 */

// Translater has to be statically linked with CommLibWrapper
// - loader can't find Translater.dll dependent CommLibWrapper.dll
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;
using MsgPassingCommunication;

namespace WpfApp1
{
  public partial class MainWindow : Window
  {
    public MainWindow()
    {
      InitializeComponent();
    }
    

    private Stack<string> pathStack_ = new Stack<string>();
    private string dirss { get; set; }
    private Translater translater;
    private CsEndPoint endPoint_;
    private Thread rcvThrd = null;
    private Dictionary<string, Action<CsMessage>> dispatcher_ 
      = new Dictionary<string, Action<CsMessage>>();
    private int selfPort { get; set; }
    private int serverPort { get; set; } 
    public CoddeDisplayWindow swin { get; set; } = null; //Instance of the selection window
    bool unselecting=false;

    //----< process incoming messages on child thread >----------------
    public void unselectFiles()
    {
      unselecting = true;  // needed to avoid using selection logic
      lstsaved.UnselectAll();
    }
    private void processMessages()
    {
      ThreadStart thrdProc = () => {
        while (true)
        {
          CsMessage msg = translater.getMessage();
          string msgId = msg.value("command");
          if (dispatcher_.ContainsKey(msgId))
            dispatcher_[msgId].Invoke(msg);
        }
      };
      rcvThrd = new Thread(thrdProc);
      rcvThrd.IsBackground = true;
      rcvThrd.Start();
    }
    //----< function dispatched by child thread to main thread >-------

    private void clearDirs()
    {
      DirList.Items.Clear();
    }
    //----< function dispatched by child thread to main thread >-------

    private void addDir(string dir)
    {
      DirList.Items.Add(dir);
    }
    //----< function dispatched by child thread to main thread >-------

    private void insertParent()
    {
      DirList.Items.Insert(0, "..");
    }
    //----< function dispatched by child thread to main thread >-------

    private void clearFiles()
    {
      FileList.Items.Clear();
    }
    //----< function dispatched by child thread to main thread >-------

    private void addFile(string file)
    {
      FileList.Items.Add(file);
    }
    //----< add client processing for message with key >---------------

    private void addClientProc(string key, Action<CsMessage> clientProc)
    {
      dispatcher_[key] = clientProc;
    }
    //----< load getDirs processing into dispatcher dictionary >-------

    private void DispatcherLoadGetDirs()
    {
      Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
      {
        Action clrDirs = () =>
        {
          clearDirs();
        };
        Dispatcher.Invoke(clrDirs, new Object[] { });
        var enumer = rcvMsg.attributes.GetEnumerator();
        while (enumer.MoveNext())
        {
          string key = enumer.Current.Key;
          if (key.Contains("dir"))
          {
            Action<string> doDir = (string dir) =>
            {
              addDir(dir);
            };
            Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
          }
        }
        Action insertUp = () =>
        {
          insertParent();
        };
        Dispatcher.Invoke(insertUp, new Object[] { });
      };
      addClientProc("getDirs", getDirs);
    }
    //----< load getFiles processing into dispatcher dictionary >------

    private void DispatcherLoadGetFiles()
    {
      Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
      {
        Action clrFiles = () =>
        {
          clearFiles();
        };
        Dispatcher.Invoke(clrFiles, new Object[] { });
        var enumer = rcvMsg.attributes.GetEnumerator();
        while (enumer.MoveNext())
        {
          string key = enumer.Current.Key;
          if (key.Contains("file"))
          {
            Action<string> doFile = (string file) =>
            {
              addFile(file);
            };
            Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
          }
        }
      };
      addClientProc("getFiles", getFiles);
    }
    //----< get the converted files and load to converted files window >---------------------------
    private void DispatcherLoadGetConvertedFiles()
    {
      Action<CsMessage> getconvertedFiles = (CsMessage rcvMsg) =>
      {
        Action clrConvertedFiles = () =>
        {
            lstConverted.Items.Clear();
        };
        Dispatcher.Invoke(clrConvertedFiles, new Object[] { });
        var enumer = rcvMsg.attributes.GetEnumerator();
        while (enumer.MoveNext())
        {
          string key = enumer.Current.Key;
          if (key.Contains("file"))
          {
            Action<string> doFile = (string file) =>
            {
                lstConverted.Items.Add(file);
            };
            Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
          }
        }
      };
      addClientProc("getconvertedFiles", getconvertedFiles);
    }
        //----< Load the freshly sent files to the client  >---------------------------

        private void DispatcherLoadrecievedfiles()
        {
            Action<CsMessage> filesent = (CsMessage rcvMsg) =>
            {
                Action clrConvertedFiles = () =>
                {
                    lstsaved.Items.Clear();
                };
                Dispatcher.Invoke(clrConvertedFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("filename"))

                    {
                        Action<string> doFile = (string file) =>
                        {
                            lstsaved.Items.Add(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("filesent", filesent);
        }

    //------------<Load all dispathcer functions>----------------
    private void loadDispatcher()
    {
      DispatcherLoadGetDirs();
      DispatcherLoadGetFiles();
      DispatcherLoadGetConvertedFiles();
      DispatcherLoadrecievedfiles();
    }
    //----< start Comm, fill window display with dirs and files >------

    private void Window_Loaded(object sender, RoutedEventArgs e)
    {
      string[] args = Environment.GetCommandLineArgs();
            System.Console.WriteLine(args);
      try{
           selfPort = int.Parse(args[1]);
           serverPort = int.Parse(args[2]);
      }
      catch(Exception er) { Console.WriteLine("{0} Exception caught.", er); }
      endPoint_ = new CsEndPoint();
      endPoint_.machineAddress = "localhost";
      endPoint_.port = selfPort;
      translater = new Translater();
      translater.listen(endPoint_);

      // start processing messages
      processMessages();

      // load dispatcher
      loadDispatcher();

      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = serverPort;
      

      txtPath.Text = "default";
      pathStack_.Push("../");
      txtPatterns.Text = "*.h *.cpp";
      txtRegex.Text = "[X](.*)";
      CsMessage msg = new CsMessage();
      msg.add("to", CsEndPoint.toString(serverEndPoint));
      msg.add("from", CsEndPoint.toString(endPoint_));
      msg.add("command", "getDirs");
      msg.add("path", pathStack_.Peek());
      translater.postMessage(msg);
      msg.remove("command");
      msg.add("command", "getFiles");
      translater.postMessage(msg);
      msg.remove("command");
      msg.add("command", "processArgs");
      msg.add("cmd", "/0 .. /s *.h *.cpp [X](.*)");
      translater.postMessage(msg);
      msg.remove("command");
      msg.remove("cmd");
      msg.remove("path");

      msg.add("command", "fileselected");
      msg.add("fileselected", "XICodePublisher.h.html");
      translater.postMessage(msg);
     
    }
    //----< strip off name of first part of path >---------------------

    private string removeFirstDir(string path)
    {
      string modifiedPath = path;
      int pos = path.IndexOf("/");
      modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
      return modifiedPath;
    }
    //----< respond to mouse double-click on dir name >----------------

    private void DirList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
    {
      // build path for selected dir
      string selectedDir = (string)DirList.SelectedItem;
      if (selectedDir == null)
        return;
      string path;
      if(selectedDir == "..")
      {
        if (pathStack_.Count > 1)  // don't pop off "Storage"
          pathStack_.Pop();
        else
          return;
      }
      else
      {
        path = pathStack_.Peek() + "/" + selectedDir; //
        pathStack_.Push(path);
      }
            // display path in Dir TextBlcok
            txtPath.Text = removeFirstDir(pathStack_.Peek());
      
      // build message to get dirs and post it
      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = serverPort;
      CsMessage msg = new CsMessage();
      msg.add("to", CsEndPoint.toString(serverEndPoint));
      msg.add("from", CsEndPoint.toString(endPoint_));
      msg.add("command", "getDirs");
      msg.add("path", pathStack_.Peek());
      translater.postMessage(msg);
      
      // build message to get files and post it
      msg.remove("command");
      msg.add("command", "getFiles");
      translater.postMessage(msg);
    }

        //----------< publish files according to given arguments>--------------
        private void BtnPublish_Click(object sender, RoutedEventArgs e)
        {
            string mesg = " ";
            if (txtPath.Text != "default")
            {
                mesg = "000 .." + txtPath.Text + " ";
            }
            else
            {
                mesg = "000 .." + " ";
            }
            if (cbRecurse.IsChecked == true)
            {
                mesg += "/s ";
            }
            mesg += txtPatterns.Text + " ";
            mesg = mesg + txtRegex.Text+" ";
            Console.WriteLine(mesg);
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = serverPort;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("path", pathStack_.Peek());
            msg.add("command", "processArgs");
            msg.add("cmd", mesg);
            translater.postMessage(msg);
        }
        //------------------<display files according to selected option>---------------
        private void Lstsaved_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Right)
            {
                if (unselecting)
                {
                    unselecting = false;
                    return;
                }
                if (swin == null)
                {
                    swin = new CoddeDisplayWindow();
                    swin.setMainWindow(this);
                }
                swin.Show();

                if (lstsaved.SelectedIndex == -1)
                    return;
                string selStr = lstsaved.SelectedItem.ToString();

                swin.show(System.IO.Path.GetFullPath("../../../../SavedPages/"+selStr));

            }
            e.Handled = true;
            if (e.ChangedButton == MouseButton.Left)
            {
                if (lstsaved.SelectedIndex == -1) return;

                string selectedItem = lstsaved.SelectedItem.ToString();
                selectedItem = System.IO.Path.GetFullPath("../../../../SavedPages/" + selectedItem);
                System.Diagnostics.Process.Start(selectedItem);

                unselectFiles();
            }


        }
        //-----------<send double clicked files to client from server>---------------
        private void LstConverted_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (lstConverted.SelectedIndex == -1) return;
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = serverPort;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "fileselected");
            msg.add("fileselected", lstConverted.SelectedItem.ToString());
            translater.postMessage(msg);

        }
    }
}
