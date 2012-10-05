using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Speech.Recognition;
using System.IO.Ports;

namespace SmartHomeAudioControl.Recognition
{
    class Recognition
    {
        private bool verbose;

        public Recognition(bool verbos)
        {
            this.verbose = verbos;
            this.SR = new SpeechRecognizer();
            this.GenerateGrammar();
            this.Gram = new Grammar(this.GB);
            SR.LoadGrammar(this.Gram);
            this.SR.SpeechRecognized += new EventHandler<SpeechRecognizedEventArgs>(SR_SpeechRecognized);

            // Create the Grammar instance.
        }

        void SR_SpeechRecognized(object sender, SpeechRecognizedEventArgs e)
        {
            if (e.Result.Text == "hal")
            {
                Console.WriteLine("Recognized Hal");
                //execute command to com port, and use spee to respond
            }
        }

        public SpeechRecognizer SR { get; set; }

        public Choices voiceChoices { get; set; }

        public Dictionary<string, string[]> GrammarOptions { set; get; }
        
        public GrammarBuilder GB { get; set; }

        public Grammar Gram { get; set; }
        /// <summary>
        /// This will generate a list of grammar to be expected from the voice speaking.
        /// </summary>
        public void GenerateGrammar()
        {
            //initialize all grammar objects
            this.GB = new GrammarBuilder();
            this.voiceChoices = new Choices();
            this.GrammarOptions = new Dictionary<string, string[]>();
            GrammarOptions.Add("Intro",new string[] {"hal"});
            this.voiceChoices.Add(GrammarOptions["Intro"]);
            this.GB.Append(this.voiceChoices);
            
        }
    }
}
