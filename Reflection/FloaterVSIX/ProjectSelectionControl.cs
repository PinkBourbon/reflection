using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FloaterVSIX
{
    public class ProjectSelectionControl : UserControl
    {
        private List<string> projects;
        private List<string> selectedProjects;

        public List<string> Projects
        {
            get { return projects; }
            set
            {
                projects = value;
                RefreshProjectList();
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

        public event EventHandler ProjectSelectionChanged;

        public ProjectSelectionControl()
        {
            InitializeComponent();
            projects = new List<string>();
            selectedProjects = new List<string>();
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

        private System.Windows.Forms.CheckedListBox checkedListBoxProjects;
        private System.Windows.Forms.Label labelInstructions;

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
            RefreshProjectList();
        }

        private void RefreshProjectList()
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

            // 이벤트를 지연시켜 ItemCheck 이벤트가 완전히 처리된 후 발생하도록 합니다.
            BeginInvoke(new Action(() =>
            {
                ProjectSelectionChanged?.Invoke(this, EventArgs.Empty);
            }));
        }
    }
}
