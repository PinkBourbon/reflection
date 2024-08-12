using System;
using System.Runtime.InteropServices;
using System.Diagnostics;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;

namespace FloaterVSIX
{
    [PackageRegistration(UseManagedResourcesOnly = true, AllowsBackgroundLoading = true)]
    [Guid(PackageGuidString)]
    [ProvideAutoLoad(UIContextGuids80.SolutionExists, PackageAutoLoadFlags.BackgroundLoad)]
    public sealed class HeaderFileListenerPackage : AsyncPackage
    {
        public const string PackageGuidString = "9c5c9690-e1e8-4fa3-bfb7-b09b7083782b";

        protected override async System.Threading.Tasks.Task InitializeAsync(System.Threading.CancellationToken cancellationToken, IProgress<ServiceProgressData> progress)
        {
            await base.InitializeAsync(cancellationToken, progress);

            // 여기서 RunningDocumentTable에 접근하여 문서 이벤트를 구독합니다.
            var rdt = new RunningDocumentTable(this);
            rdt.Advise(new HeaderFileListener());
        }
    }

    public class HeaderFileListener : IVsRunningDocTableEvents
    {
        private readonly AsyncPackage _package;

        public HeaderFileListener()
        {
            _package = (AsyncPackage)Package.GetGlobalService(typeof(AsyncPackage));
        }

        public int OnAfterSave(uint docCookie)
        {
            IVsRunningDocumentTable rdt = Package.GetGlobalService(typeof(SVsRunningDocumentTable)) as IVsRunningDocumentTable;
            if (rdt != null)
            {
                uint flags, readLocks, editLocks;
                string documentPath;
                IVsHierarchy hierarchy;
                uint itemId;
                IntPtr docData;

                rdt.GetDocumentInfo(docCookie, out flags, out readLocks, out editLocks, out documentPath, out hierarchy, out itemId, out docData);

                if (System.IO.Path.GetExtension(documentPath).ToLower() == ".h")
                {
                    // 여기에서 exe 실행 및 헤더 파일 수정 로직을 구현합니다.
                    RunExeAndModifyHeader(documentPath);
                }
            }
            return Microsoft.VisualStudio.VSConstants.S_OK;
        }

        private void RunExeAndModifyHeader(string headerFilePath)
        {
            // exe 실행 및 헤더 파일 수정 로직 구현
            string exePath = @"C:\Users\kocca61\Desktop\reflection\Reflection\x64\Debug\HeaderTool.exe";

            ProcessStartInfo startInfo = new ProcessStartInfo
            {
                FileName = exePath,
                Arguments = $"\"{headerFilePath}\"",
                UseShellExecute = false,
                RedirectStandardOutput = true,
                CreateNoWindow = true
            };

            using (Process process = new Process { StartInfo = startInfo })
            {
                bool ret = process.Start();
                string output = process.StandardOutput.ReadToEnd();
                process.WaitForExit();

                if (process.ExitCode == 0)
                {
                    // 성공적으로 헤더 파일이 생성됨
                }
                else
                {
                    // 오류 발생
                }
            }
        }

        // IVsRunningDocTableEvents의 다른 메서드들은 필요에 따라 구현합니다.
        public int OnAfterFirstDocumentLock(uint docCookie, uint dwRDTLockType, uint dwReadLocksRemaining, uint dwEditLocksRemaining) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnBeforeLastDocumentUnlock(uint docCookie, uint dwRDTLockType, uint dwReadLocksRemaining, uint dwEditLocksRemaining) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnAfterAttributeChange(uint docCookie, uint grfAttribs) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnBeforeDocumentWindowShow(uint docCookie, int fFirstShow, IVsWindowFrame pFrame) { return Microsoft.VisualStudio.VSConstants.S_OK; }
        public int OnAfterDocumentWindowHide(uint docCookie, IVsWindowFrame pFrame) { return Microsoft.VisualStudio.VSConstants.S_OK; }
    }
}
