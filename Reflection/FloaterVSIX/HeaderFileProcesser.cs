using EnvDTE;
using Microsoft.VisualStudio.ComponentModelHost;
using Microsoft.VisualStudio.Editor;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio.Text;
using Microsoft.VisualStudio.TextManager.Interop;
using System;
using System.IO;
using System.Diagnostics;
using System.Linq;
using Microsoft.VisualStudio;
using Microsoft.VisualStudio.OLE.Interop;
using Microsoft.VisualStudio.Shell.Design.Serialization;


namespace FloaterVSIX
{
    internal class HeaderFileProcesser : IVsRunningDocTableEvents3
    {
        //private IVsRunningDocumentTable _rdt;
        private RunningDocumentTable _rdt = null!;
        private DTE _dte;

        private IVsOutputWindowPane _outputPane;
        private Guid _outputGuid;

        private ProjectSelectionControl _projectSelectionControl;

        public void Initialize(Package package, ProjectSelectionControl selectionControl)
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            _dte = Package.GetGlobalService(typeof(DTE)) as DTE;
            _rdt = new RunningDocumentTable(package);
            _rdt.Advise(this);

            IVsOutputWindow outputWindow = Package.GetGlobalService(typeof(SVsOutputWindow)) as IVsOutputWindow;
            _outputGuid = Guid.NewGuid();
            outputWindow.CreatePane(ref _outputGuid, "HeaderFileProcesser", 1, 1);
            outputWindow.GetPane(ref _outputGuid, out _outputPane);

            _projectSelectionControl = selectionControl;

            PrintOutput("Initialized HeaderFileProcesser");
        }

        private void PrintOutput(string message)
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            _outputPane.OutputString(message + Environment.NewLine);
        }
        private bool RefreshDoc(RunningDocumentInfo docInfo)
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            if (docInfo.DocData is IVsPersistDocData persistDocData)
            {
                int hr = persistDocData.ReloadDocData((uint)_VSRELOADDOCDATA.RDD_IgnoreNextFileChange);
                if (ErrorHandler.Succeeded(hr))
                {
                    return true;
                }
            }
            return false;
        }
        public int OnAfterAttributeChange(uint docCookie, uint grfAttribs) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnAfterAttributeChangeEx(uint docCookie, uint grfAttribs, IVsHierarchy pHierOld, uint itemidOld, string pszMkDocumentOld, IVsHierarchy pHierNew, uint itemidNew, string pszMkDocumentNew) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnBeforeSave(uint docCookie) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnAfterSave(uint docCookie)
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            //Document activeDoc = _dte.ActiveDocument;
            //string projectName = activeDoc.ProjectItem.ContainingProject.Name;

            RunningDocumentInfo docInfo = _rdt.GetDocumentInfo(docCookie);

            IVsHierarchy hierarchy = docInfo.Hierarchy;

            hierarchy.GetProperty(VSConstants.VSITEMID_ROOT, (int)__VSHPROPID.VSHPROPID_ProjectName, out object projectNameObj);
            string projectName = projectNameObj as string;

            if (!_projectSelectionControl.IsCheckedProject(projectName))
            {
                return Microsoft.VisualStudio.VSConstants.S_OK;
            }

            Project project = null;
            hierarchy.GetProperty(VSConstants.VSITEMID_ROOT, (int)__VSHPROPID.VSHPROPID_ExtObject, out object projectObject);
            project = projectObject as Project;
            string projectPath = project.FullName;

            if (docInfo.Moniker.EndsWith(".h"))
            {
                string docPath = docInfo.Moniker;

                string projectDirectory = Path.GetDirectoryName(projectPath);

                Uri from = new Uri(projectDirectory);
                Uri to = new Uri(docPath);

                Uri relativeUri = from.MakeRelativeUri(to);
                string relativeDocPath = relativeUri.ToString();

                string exePath = "C:\\Users\\cgykp\\Desktop\\reflection\\Reflection\\x64\\Debug\\HeaderTool.exe";
                ProcessStartInfo startInfo = new ProcessStartInfo
                {
                    FileName = exePath,
                    Arguments = docPath + " " + projectDirectory + " " + relativeDocPath,
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true
                };

                System.Diagnostics.Process process = new System.Diagnostics.Process { StartInfo = startInfo };
                process.Start();
                process.WaitForExit();
                if (process.ExitCode == 0)
                {
                    if (RefreshDoc(docInfo))
                    {
                        string msg = "Success - Create Reflection Data Code : " + docInfo.Moniker;
                        PrintOutput(msg);
                        return Microsoft.VisualStudio.VSConstants.S_OK;
                    }
                }

                // Error
                string errorMsg = "Error - Fail To Create Reflection Data Code : " + process.ExitCode.ToString();
                PrintOutput(errorMsg);
            }

            return Microsoft.VisualStudio.VSConstants.S_OK;
        }
        public int OnAfterFirstDocumentLock(uint docCookie, uint dwRDTLockType, uint dwReadLocksRemaining, uint dwEditLocksRemaining) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnBeforeLastDocumentUnlock(uint docCookie, uint dwRDTLockType, uint dwReadLocksRemaining, uint dwEditLocksRemaining) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnAfterDocumentWindowHide(uint docCookie, IVsWindowFrame pFrame) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnBeforeDocumentWindowShow(uint docCookie, int fFirstShow, IVsWindowFrame pFrame) { return Microsoft.VisualStudio.VSConstants.S_OK; }
    }
}
