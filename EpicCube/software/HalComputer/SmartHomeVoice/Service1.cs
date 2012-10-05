using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Speech.Synthesis;
using System.Speech.Recognition;

namespace SmartHomeVoice
{
    public partial class SmartHomeService : ServiceBase
    {

        /// <summary>
        /// Gets or sets the Speech Recognition tools
        /// </summary>
        private Recognition.Recognition Recogn { get; set; }
        /// <summary>
        /// Gets or sets the Speech Synthesis tools.
        /// </summary>
        private Speech.Speech Spee { get; set; }


        public SmartHomeService()
        {
            InitializeComponent();
            Recogn = new Recognition.Recognition();
            Spee = new Speech.Speech();
        }


        protected override void OnStart(string[] args)
        {
            TimeSpan now = DateTime.Now.TimeOfDay;
            if (now.Hours < 6 || now.Hours > 21)
            {
                Spee.speaker.Speak("Good Night. What can i do for you tonight.");
            }
            else if (now.Hours < 13 && now.Hours > 5)
            {
                Spee.speaker.Speak("Good Morning. What can i do for you today.");
            }
            else if (now.Hours < 19 && now.Hours > 12)
            {
                Spee.speaker.Speak("Good afternoon. What can i do for you today.");
            }
            else if (now.Hours < 22 && now.Hours > 18)
            {
                Spee.speaker.Speak("Good Night. What can i do for you this evening.");
            }
            else
            {
                Spee.speaker.Speak("What can I do for you");
            }

            // listen for specific command for name Hal

        }

        protected override void OnStop()
        {
        }
    }
}
