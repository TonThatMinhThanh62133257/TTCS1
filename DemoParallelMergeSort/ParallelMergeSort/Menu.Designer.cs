namespace ParallelMergeSort
{
    partial class Menu
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.inputBox = new System.Windows.Forms.TextBox();
            this.button = new System.Windows.Forms.Button();
            this.inputLabel = new System.Windows.Forms.Label();
            this.threadLabel = new System.Windows.Forms.Label();
            this.threadBox = new System.Windows.Forms.TextBox();
            this.panel = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // inputBox
            // 
            this.inputBox.BackColor = System.Drawing.SystemColors.InfoText;
            this.inputBox.ForeColor = System.Drawing.Color.Lime;
            this.inputBox.Location = new System.Drawing.Point(767, 538);
            this.inputBox.Name = "inputBox";
            this.inputBox.Size = new System.Drawing.Size(125, 27);
            this.inputBox.TabIndex = 1;
            // 
            // button
            // 
            this.button.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.button.Location = new System.Drawing.Point(897, 530);
            this.button.Name = "button";
            this.button.Size = new System.Drawing.Size(139, 77);
            this.button.TabIndex = 2;
            this.button.Text = "Mô phỏng";
            this.button.UseVisualStyleBackColor = false;
            // 
            // inputLabel
            // 
            this.inputLabel.AutoSize = true;
            this.inputLabel.Location = new System.Drawing.Point(495, 538);
            this.inputLabel.Name = "inputLabel";
            this.inputLabel.Size = new System.Drawing.Size(266, 20);
            this.inputLabel.TabIndex = 3;
            this.inputLabel.Text = "Số lượng phần tử mà từng thread xử lý";
            // 
            // threadLabel
            // 
            this.threadLabel.AutoSize = true;
            this.threadLabel.Location = new System.Drawing.Point(639, 570);
            this.threadLabel.Name = "threadLabel";
            this.threadLabel.Size = new System.Drawing.Size(122, 20);
            this.threadLabel.TabIndex = 4;
            this.threadLabel.Text = "Số lượng threads";
            // 
            // threadBox
            // 
            this.threadBox.BackColor = System.Drawing.SystemColors.InfoText;
            this.threadBox.ForeColor = System.Drawing.Color.Lime;
            this.threadBox.Location = new System.Drawing.Point(767, 570);
            this.threadBox.Name = "threadBox";
            this.threadBox.Size = new System.Drawing.Size(125, 27);
            this.threadBox.TabIndex = 5;
            // 
            // panel
            // 
            this.panel.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.panel.Location = new System.Drawing.Point(12, 12);
            this.panel.Name = "panel";
            this.panel.Size = new System.Drawing.Size(1024, 500);
            this.panel.TabIndex = 6;
            // 
            // Menu
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.ClientSize = new System.Drawing.Size(1049, 620);
            this.Controls.Add(this.panel);
            this.Controls.Add(this.threadBox);
            this.Controls.Add(this.threadLabel);
            this.Controls.Add(this.inputLabel);
            this.Controls.Add(this.button);
            this.Controls.Add(this.inputBox);
            this.ForeColor = System.Drawing.Color.Lime;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Menu";
            this.Text = "Parallel Merge Sort";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private TextBox inputBox;
        private Button button;
        private Label inputLabel;
        private Label threadLabel;
        private TextBox threadBox;
        private Panel panel;
    }
}