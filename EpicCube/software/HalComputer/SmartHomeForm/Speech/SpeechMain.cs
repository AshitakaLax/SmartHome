using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Speech.Synthesis;

namespace SmartHomeForm.Speech
{
    class SpeechMain
    {
        public SpeechMain()
        {
            speaker = new SpeechSynthesizer();
            initializeSpeech();
        }

        public SpeechSynthesizer speaker { get; set; }
        /// <summary>
        /// simply initialize the speaker. // may look into creating our own voice output for each command.
        /// </summary>
        public void initializeSpeech()
        {
            this.speaker.SetOutputToDefaultAudioDevice();// we can select which output we want. (when we want to customize the speaker output.)
            //PromptBuilder builder = new PromptBuilder();
            this.speaker.Volume = 100;
            this.speaker.Rate = 1;
            
        //    builder.AppendText("MAGGIE!");
        //    for (int i = 0; i < 10;i++ )
        //        speaker.Speak(builder);
        //    this.Introduction(); // this will be a function of the voice control.

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

        public void Speak(string speakingPhrase)
        {
            this.speaker.Speak(speakingPhrase);
        }


    }
}
