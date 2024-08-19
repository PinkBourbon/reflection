using EnvDTE;
using EnvDTE80;
using Microsoft.VisualStudio.Shell;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;
using System.Windows.Forms;

namespace FloaterVSIX
{
    public class ProjectSelectionControl : UserControl
    {
        private EnvDTE.Projects allProjects;
        private List<string> projects;
        private List<string> selectedProjects;

        public event EventHandler ProjectSelectionChanged;


        private System.Windows.Forms.CheckedListBox checkedListBoxProjects;
        private System.Windows.Forms.Label labelInstructions;
        public List<string> Projects
        {
            get { return projects; }
            set
            {
                projects = value;
                UpdateProjectList();
            }
        }

        public List<string> SelectedProjects
        {
            get { return selectedProjects; }
            set
            {
                selectedProjects = value;
                UpdateSelectedProjects();
            }
        }

        public ProjectSelectionControl()
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            InitializeComponent();
            allProjects = ((DTE2)Package.GetGlobalService(typeof(DTE))).Solution.Projects;
            projects = new List<string>();
            UpdateCppProjects();
            selectedProjects = new List<string>();
            LoadSelectedProjects();
        }

        public bool IsCheckedProject(string projectName)
        {
            return selectedProjects.Contains(projectName);
        }

        private void InitializeComponent()
        {
            this.checkedListBoxProjects = new System.Windows.Forms.CheckedListBox();
            this.labelInstructions = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // checkedListBoxProjects
            // 
            this.checkedListBoxProjects.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.checkedListBoxProjects.FormattingEnabled = true;
            this.checkedListBoxProjects.Location = new System.Drawing.Point(3, 36);
            this.checkedListBoxProjects.Name = "checkedListBoxProjects";
            this.checkedListBoxProjects.Size = new System.Drawing.Size(294, 259);
            this.checkedListBoxProjects.TabIndex = 0;
            this.checkedListBoxProjects.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.checkedListBoxProjects_ItemCheck);
            // 
            // labelInstructions
            // 
            this.labelInstructions.AutoSize = true;
            this.labelInstructions.Location = new System.Drawing.Point(3, 9);
            this.labelInstructions.Name = "labelInstructions";
            this.labelInstructions.Size = new System.Drawing.Size(233, 13);
            this.labelInstructions.TabIndex = 1;
            this.labelInstructions.Text = "Select the C++ projects to apply the extension to:";
            // 
            // ProjectSelectionControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.labelInstructions);
            this.Controls.Add(this.checkedListBoxProjects);
            this.Name = "ProjectSelectionControl";
            this.Size = new System.Drawing.Size(300, 300);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
            UpdateCppProjects();
            UpdateProjectList();
        }

        private void UpdateProjectList()
        {
            checkedListBoxProjects.Items.Clear();
            checkedListBoxProjects.Items.AddRange(projects.ToArray());
            UpdateSelectedProjects();
        }

        private void UpdateSelectedProjects()
        {
            for (int i = 0; i < checkedListBoxProjects.Items.Count; i++)
            {
                string project = checkedListBoxProjects.Items[i].ToString();
                checkedListBoxProjects.SetItemChecked(i, selectedProjects.Contains(project));
            }
        }

        private void checkedListBoxProjects_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            string project = checkedListBoxProjects.Items[e.Index].ToString();

            if (e.NewValue == CheckState.Checked && !selectedProjects.Contains(project))
            {
                selectedProjects.Add(project);
            }
            else if (e.NewValue == CheckState.Unchecked && selectedProjects.Contains(project))
            {
                selectedProjects.Remove(project);
            }

            SaveSelectedProjects();
            BeginInvoke(new Action(() =>
            {
                ProjectSelectionChanged?.Invoke(this, EventArgs.Empty);
            }));
        }

        private void UpdateCppProjects()
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            projects.Clear();
            foreach (Project project in allProjects)
            {
                if (project.Kind == "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") // C++ 프로젝트 GUID
                {
                    projects.Add(project.Name);
                }
            }
        }

        private void LoadSelectedProjects()
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            DTE2 dte = (DTE2)Package.GetGlobalService(typeof(DTE));
            string solutionDir = Path.GetDirectoryName(dte.Solution.FullName);
            string filePath = Path.Combine(solutionDir, "selectedProjects.json");

            if (File.Exists(filePath))
            {
                string jsonString = File.ReadAllText(filePath);
                selectedProjects = JsonSerializer.Deserialize<List<string>>(jsonString);
            }
        }

        private void SaveSelectedProjects()
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            DTE2 dte = (DTE2)Package.GetGlobalService(typeof(DTE));
            string solutionDir = Path.GetDirectoryName(dte.Solution.FullName);
            string filePath = Path.Combine(solutionDir, "selectedProjects.json");

            string jsonString = JsonSerializer.Serialize(selectedProjects);
            File.WriteAllText(filePath, jsonString);
        }
    }
}