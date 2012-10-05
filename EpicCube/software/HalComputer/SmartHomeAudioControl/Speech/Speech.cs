using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Speech.Synthesis;

namespace SmartHomeAudioControl.Speech
{
    class Speech
    {
        public Speech()
        {
            speaker = new SpeechSynthesizer();
            initializeSpeech();
        }

        public SpeechSynthesizer speaker { get; set; }

        public void initializeSpeech()
        {
            this.speaker.SetOutputToDefaultAudioDevice();
            PromptBuilder builder = new PromptBuilder();
            this.speaker.Volume = 100;
            this.speaker.Rate = 1;
            //builder.AppendText("Jen, You are a re-de-culous Girl! And I love you");
            speaker.Speak(builder);
            this.Introduction();

        }
        public void Introduction()
        {
            TimeSpan now = DateTime.Now.TimeOfDay;
            if (now.Hours < 6 || now.Hours > 21)
            {
                this.speaker.Speak("Good Night. What can i do for you tonight.");
            }
            else if (now.Hours < 13 && now.Hours > 5)
            {
                this.speaker.Speak("Good Morning. What can i do for you today.");
            }
            else if (now.Hours < 19 && now.Hours > 12)
            {
                this.speaker.Speak("Good afternoon. What can i do for you today.");
            }
            else if (now.Hours < 22 && now.Hours > 18)
            {
                this.speaker.Speak("Good Night. What can i do for you this evening.");
            }
            else
            {
                this.speaker.Speak("What can I do for you");
            }

        }



    }
}
