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
        private DTE2 _dte;
        private EnvDTE.Projects _allProjects;
        private List<string> _projects;
        private List<string> _selectedProjects;

        public event EventHandler ProjectSelectionChanged;


        private System.Windows.Forms.CheckedListBox _checkedListBoxProjects;
        private System.Windows.Forms.Label _labelInstructions;
        public List<string> Projects
        {
            get { return _projects; }
            set
            {
                _projects = value;
                UpdateProjectList();
            }
        }

        public List<string> SelectedProjects
        {
            get { return _selectedProjects; }
            set
            {
                _selectedProjects = value;
                UpdateSelectedProjects();
            }
        }

        public ProjectSelectionControl()
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            InitializeComponent();
            _allProjects = ((DTE2)Package.GetGlobalService(typeof(DTE))).Solution.Projects;
            _dte = (DTE2)Package.GetGlobalService(typeof(DTE));
            _projects = new List<string>();
            UpdateCppProjects();
            _selectedProjects = new List<string>();
            LoadSelectedProjects();
        }

        public bool IsCheckedProject(string projectName)
        {
            return _selectedProjects.Contains(projectName);
        }

        private void InitializeComponent()
        {
            this._checkedListBoxProjects = new System.Windows.Forms.CheckedListBox();
            this._labelInstructions = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // checkedListBoxProjects
            // 
            this._checkedListBoxProjects.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this._checkedListBoxProjects.FormattingEnabled = true;
            this._checkedListBoxProjects.Location = new System.Drawing.Point(3, 36);
            this._checkedListBoxProjects.Name = "checkedListBoxProjects";
            this._checkedListBoxProjects.Size = new System.Drawing.Size(294, 259);
            this._checkedListBoxProjects.TabIndex = 0;
            this._checkedListBoxProjects.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.CheckedListBoxProjects_ItemCheck);
            // 
            // labelInstructions
            // 
            this._labelInstructions.AutoSize = true;
            this._labelInstructions.Location = new System.Drawing.Point(3, 9);
            this._labelInstructions.Name = "labelInstructions";
            this._labelInstructions.Size = new System.Drawing.Size(233, 13);
            this._labelInstructions.TabIndex = 1;
            this._labelInstructions.Text = "Select the C++ projects to apply the extension to:";
            // 
            // ProjectSelectionControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this._labelInstructions);
            this.Controls.Add(this._checkedListBoxProjects);
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
            _checkedListBoxProjects.Items.Clear();
            _checkedListBoxProjects.Items.AddRange(_projects.ToArray());
            UpdateSelectedProjects();
        }

        private void UpdateSelectedProjects()
        {
            for (int i = 0; i < _checkedListBoxProjects.Items.Count; i++)
            {
                string project = _checkedListBoxProjects.Items[i].ToString();
                _checkedListBoxProjects.SetItemChecked(i, _selectedProjects.Contains(project));
            }
        }

        private void CheckedListBoxProjects_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            string project = _checkedListBoxProjects.Items[e.Index].ToString();

            if (e.NewValue == CheckState.Checked && !_selectedProjects.Contains(project))
            {
                _selectedProjects.Add(project);
            }
            else if (e.NewValue == CheckState.Unchecked && _selectedProjects.Contains(project))
            {
                _selectedProjects.Remove(project);
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
            _projects.Clear();
            foreach (Project project in _allProjects)
            {
                if (project.Kind == "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") // C++ 프로젝트 GUID
                {
                    _projects.Add(project.Name);
                }
            }
        }

        private void LoadSelectedProjects()
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            string solutionDir = Path.GetDirectoryName(_dte.Solution.FullName);
            string filePath = Path.Combine(solutionDir, "selectedProjects.json");

            if (File.Exists(filePath))
            {
                string jsonString = File.ReadAllText(filePath);
                _selectedProjects = JsonSerializer.Deserialize<List<string>>(jsonString);
            }
        }

        private void SaveSelectedProjects()
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            string solutionDir = Path.GetDirectoryName(_dte.Solution.FullName);
            string filePath = Path.Combine(solutionDir, "selectedProjects.json");

            string jsonString = JsonSerializer.Serialize(_selectedProjects);
            File.WriteAllText(filePath, jsonString);
        }
    }
}