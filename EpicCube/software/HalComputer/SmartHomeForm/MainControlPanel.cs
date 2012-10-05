using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SmartHomeForm
{
    public partial class MainControlPanel : Form
    {
        bool verbose = true;
        Recognition.RecognitionMain Recogn;
        Speech.SpeechMain Spee;

        public MainControlPanel()
        {
            InitializeComponent();
            Spee = new Speech.SpeechMain();
            Recogn = new Recognition.RecognitionMain(this.StatusTextBox, this.Spee);
            this.StatusTextBox.Visible = verbose;
            
        }

        private void StartRecognitionButton_Click(object sender, EventArgs e)
        {
            if (this.StartRecognitionButton.Text == "Start Voice Recognition")
            {
                this.Recogn.StartRecognizingVoice();
                this.StartRecognitionButton.Text = "Stop Voice Recognition";
            }
            else
            {
                this.Recogn.StopRecognizingVoice();
                this.StartRecognitionButton.Text = "Start Voice Recognition";
            }

        }
    }
}
