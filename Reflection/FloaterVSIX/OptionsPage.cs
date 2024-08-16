using EnvDTE;
using EnvDTE80;
using Microsoft.VisualStudio.Shell;
using System.Collections.Generic;
using System.Linq;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Windows.Forms;


namespace FloaterVSIX
{
    [ComVisible(true)]
    public class OptionsPage : DialogPage
    {
        private List<string> selectedProjects = new List<string>();

        [Category("Project Selection")]
        [DisplayName("Selected Projects")]
        [Description("Select the C++ projects to apply the extension to")]
        public List<string> SelectedProjects
        {
            get { return selectedProjects; }
            set { selectedProjects = value; }
        }
        protected override IWin32Window Window
        {
            get
            {
                ThreadHelper.ThrowIfNotOnUIThread();
                ProjectSelectionControl control = new ProjectSelectionControl();
                List<string> projectNames = new List<string>();
                foreach (Project project in GetCppProjects())
                {
                    projectNames.Add(project.Name);
                }
                control.Projects = projectNames;
                control.SelectedProjects = SelectedProjects;
                control.ProjectSelectionChanged += (sender, e) => SelectedProjects = control.SelectedProjects;
                return control;
            }
        }

        private List<Project> GetCppProjects()
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            DTE2 dte = (DTE2)Package.GetGlobalService(typeof(DTE));
            List<Project> cppProjects = new List<Project>();

            foreach (Project project in dte.Solution.Projects)
            {
                if (project.Kind == "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") // C++ 프로젝트 GUID
                {
                    cppProjects.Add(project);
                }
            }

            return cppProjects;
        }
    }
}
