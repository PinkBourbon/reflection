using EnvDTE;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FloaterVSIX
{
    internal class HeaderFileProcesser : IVsRunningDocTableEvents3
    {
        //private IVsRunningDocumentTable _rdt;
        private RunningDocumentTable _rdt;
        private DTE _dte;
        public void Initialize(Package package)
        {
            Microsoft.VisualStudio.Shell.ThreadHelper.ThrowIfNotOnUIThread();
            _dte = (DTE)Package.GetGlobalService(typeof(DTE));
            _rdt = new RunningDocumentTable(package);
            _rdt.Advise(this);
        }


        public int OnAfterAttributeChange(uint docCookie, uint grfAttribs) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnAfterAttributeChangeEx(uint docCookie, uint grfAttribs, IVsHierarchy pHierOld, uint itemidOld, string pszMkDocumentOld, IVsHierarchy pHierNew, uint itemidNew, string pszMkDocumentNew) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnBeforeSave(uint docCookie) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnAfterSave(uint docCookie)
        {
            //var documentInfo = _rdt.GetDocumentInfo(docCookie, out uint flags, out uint readLocks, out uint editLocks, out string docPath, out IVsHierarchy hier, out uint itemId, out IntPtr docData);
            RunningDocumentInfo docInfo = _rdt.GetDocumentInfo(docCookie);
            if (docInfo.Moniker.EndsWith(".h"))
            {
                string docPath = docInfo.Moniker;

                string exePath = "C:\\Users\\kocca61\\Desktop\\reflection\\Reflection\\x64\\Debug\\HeaderTool.exe";
                ProcessStartInfo startInfo = new ProcessStartInfo
                {
                    FileName = exePath,
                    Arguments = docPath,
                    UseShellExecute = true,
                    RedirectStandardOutput = false,
                    CreateNoWindow = true
                };

                System.Diagnostics.Process process = new System.Diagnostics.Process { StartInfo = startInfo };
                process.Start();
                process.WaitForExit();
                if(process.ExitCode != 0)
                {
                    // Error
                }
                else 
                {
                }
                int i = 0;
                // Do something
            }

            return Microsoft.VisualStudio.VSConstants.S_OK;
        }
        public int OnAfterFirstDocumentLock(uint docCookie, uint dwRDTLockType, uint dwReadLocksRemaining, uint dwEditLocksRemaining) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnBeforeLastDocumentUnlock(uint docCookie, uint dwRDTLockType, uint dwReadLocksRemaining, uint dwEditLocksRemaining) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnAfterDocumentWindowHide(uint docCookie, IVsWindowFrame pFrame) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnBeforeDocumentWindowShow(uint docCookie, int fFirstShow, IVsWindowFrame pFrame) { return Microsoft.VisualStudio.VSConstants.S_OK; }
    }
}
