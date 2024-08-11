using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using Task = System.Threading.Tasks.Task;

using System.Diagnostics;

namespace FloaterVSIX
{
    /// <summary>
    /// This is the class that implements the package exposed by this assembly.
    /// </summary>
    /// <remarks>
    /// <para>
    /// The minimum requirement for a class to be considered a valid package for Visual Studio
    /// is to implement the IVsPackage interface and register itself with the shell.
    /// This package uses the helper classes defined inside the Managed Package Framework (MPF)
    /// to do it: it derives from the Package class that provides the implementation of the
    /// IVsPackage interface and uses the registration attributes defined in the framework to
    /// register itself and its components with the shell. These attributes tell the pkgdef creation
    /// utility what data to put into .pkgdef file.
    /// </para>
    /// <para>
    /// To get loaded into VS, the package must be referred by &lt;Asset Type="Microsoft.VisualStudio.VsPackage" ...&gt; in .vsixmanifest file.
    /// </para>
    /// </remarks>
    [PackageRegistration(UseManagedResourcesOnly = true, AllowsBackgroundLoading = true)]
    [Guid(FloaterVSIXPackage.PackageGuidString)]
    [ProvideMenuResource("Menus.ctmenu", 1)]
    public sealed class FloaterVSIXPackage : AsyncPackage
    {
        /// <summary>
        /// FloaterVSIXPackage GUID string.
        /// </summary>
        public const string PackageGuidString = "78ae6ed7-f18d-4d89-8435-0c929a6972a1";

        #region Package Members

        /// <summary>
        /// Initialization of the package; this method is called right after the package is sited, so this is the place
        /// where you can put all the initialization code that rely on services provided by VisualStudio.
        /// </summary>
        /// <param name="cancellationToken">A cancellation token to monitor for initialization cancellation, which can occur when VS is shutting down.</param>
        /// <param name="progress">A provider for progress updates.</param>
        /// <returns>A task representing the async work of package initialization, or an already completed task if there is none. Do not return null from this method.</returns>
        protected override async Task InitializeAsync(CancellationToken cancellationToken, IProgress<ServiceProgressData> progress)
        {
            // When initialized asynchronously, the current thread may be a background thread at this point.
            // Do any initialization that requires the UI thread after switching to the UI thread.
            await this.JoinableTaskFactory.SwitchToMainThreadAsync(cancellationToken);
            await OpenExploererCommand.InitializeAsync(this);

            var runningDocumentTable = await GetServiceAsync(typeof(SVsRunningDocumentTable)) as IVsRunningDocumentTable;
            var runningDocumentTableEvents = new RunningDocumentTableEvents(runningDocumentTable);
            runningDocumentTableEvents.AfterSave += OnAfterSave;
        }

        private void OnAfterSave(string filePath)
        {
            if (Path.GetExtension(filePath).Equals(".h", StringComparison.OrdinalIgnoreCase))
            {
                string exePath = @"C:\Path\To\Your\External\Program.exe"; // 실행할 exe 파일의 경로로 변경하세요
                Process.Start(exePath, $"\"{filePath}\"");
            }
        }

        internal class RunningDocumentTableEvents : IVsRunningDocTableEvents3
        {
            public event Action<string> AfterSave;

            public RunningDocumentTableEvents(IVsRunningDocumentTable rdt)
            {
                ThreadHelper.ThrowIfNotOnUIThread();
                rdt.AdviseRunningDocTableEvents(this, out _);
            }

            public int OnAfterSave(uint docCookie)
            {
                ThreadHelper.ThrowIfNotOnUIThread();

                var rdt = Package.GetGlobalService(typeof(SVsRunningDocumentTable)) as IVsRunningDocumentTable;
                //rdt.GetDocumentInfo(docCookie, out _, out _, out _, out string moniker, out _);
                uint flags = 0;
                string path = null;
                uint pitemid = 0;
                IntPtr punkDocData = IntPtr.Zero;
                rdt.GetDocumentInfo(docCookie, out flags, out _, out _, out path, out _, out pitemid, out punkDocData);

                AfterSave?.Invoke(path);
                return Microsoft.VisualStudio.VSConstants.S_OK;
            }

            // 기타 IVsRunningDocTableEvents3 멤버들 (필요하지 않으면 비워둘 수 있습니다)
            public int OnAfterAttributeChange(uint docCookie, uint grfAttribs) => Microsoft.VisualStudio.VSConstants.S_OK;
            public int OnBeforeDocumentWindowShow(uint docCookie, int fFirstShow, IVsWindowFrame pFrame) => Microsoft.VisualStudio.VSConstants.S_OK;
            public int OnAfterDocumentWindowHide(uint docCookie, IVsWindowFrame pFrame) => Microsoft.VisualStudio.VSConstants.S_OK;
            public int OnAfterFirstDocumentLock(uint docCookie, uint dwRDTLockType, uint dwReadLocksRemaining, uint dwEditLocksRemaining) => Microsoft.VisualStudio.VSConstants.S_OK;
            public int OnBeforeLastDocumentUnlock(uint docCookie, uint dwRDTLockType, uint dwReadLocksRemaining, uint dwEditLocksRemaining) => Microsoft.VisualStudio.VSConstants.S_OK;
            public int OnAfterAttributeChangeEx(uint docCookie, uint grfAttribs, IVsHierarchy pHierOld, uint itemidOld, string pszMkDocumentOld, IVsHierarchy pHierNew, uint itemidNew, string pszMkDocumentNew) => Microsoft.VisualStudio.VSConstants.S_OK;
            public int OnBeforeSave(uint docCookie) => Microsoft.VisualStudio.VSConstants.S_OK;
        }
        #endregion
    }
}
