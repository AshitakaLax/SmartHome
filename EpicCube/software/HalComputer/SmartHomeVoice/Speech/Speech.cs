using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Speech.Synthesis;

namespace SmartHomeVoice.Speech
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
            builder.AppendText("Jen, You are a re-de-culous Girl! And I love you");
            speaker.Speak(builder);
        }



    }
}
