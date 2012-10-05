using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Speech.Recognition;
using System.IO.Ports;
using System.Windows.Forms;
using System.Threading;

namespace SmartHomeForm.Recognition
{
    /// <summary>
    /// This class will handle the first initial wave of commands till it finds out what path
    /// the user wants to go.
    /// </summary>
    class RecognitionMain
    {
        private TextBox StatusTextBox;
        public enum VoiceStep
        {
            first,
            second,
            Temperature,
            Garage,
            Password
        };

        /// <summary>
        /// Creates an new instance of the Recognition class.
        /// </summary>
        /// <param name="StatusText"></param>
        /// <param name="Spee"></param>
        public RecognitionMain(TextBox StatusText, Speech.SpeechMain Spee)
        {
            this.StatusTextBox = StatusText;
            this.acknowlegdedSpeaker = false;
            this.Spee = Spee;
            this.SRE = new SpeechRecognitionEngine();
            this.SRE.SetInputToDefaultAudioDevice();
            this.SRE.SpeechRecognized += new EventHandler<SpeechRecognizedEventArgs>(SRE_SpeechRecognized);

            this.GenerateGrammar(VoiceStep.first);
            this.VTemp = new Temperature.TemperatureCntrl(Spee);
        }

        /// <summary>
        /// We port the word spoken into the specific class.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void SRE_SpeechRecognized(object sender, SpeechRecognizedEventArgs e)
        {
            //need to add a timer for timeout words so that people can leave mid command and comeback to the start command. Hal
            foreach (RecognizedWordUnit word in e.Result.Words)
            {
                this.StopRecognizingVoice();
                this.StatusTextBox.AppendText(word.Text + " confidence Val="+ word.Confidence + Environment.NewLine);
                if (word.Confidence > 0.5f)
                {
                    if(word.Text == "restart")
                    {
                        this.CurrentStep = VoiceStep.first;
                        this.Spee.Speak("Restarting now");
                        this.Spee.speaker.Volume = 50;
                        this.Spee.Speak("Stupid master doesn't know what he wants.");
                        this.Spee.speaker.Volume = 100;
                        
                    }
                    if (word.Text == "hal" && this.CurrentStep == VoiceStep.first)
                    {
                        // this.StatusTextBox.AppendText("good Confidence value =" + word.Confidence + " " + word.Text);
                        this.CurrentStep = VoiceStep.second;
                        this.Spee.Speak("Yes. Mastur.");
                        // change Grammar
                        this.GenerateGrammar(this.CurrentStep);
                    }
                    else if (this.CurrentStep == VoiceStep.second)
                    {
                        //Temperature
                        if (word.Text.Contains("room"))
                        {
                            this.CurrentStep = VoiceStep.Temperature;
                            this.SRE.UnloadAllGrammars();
                            this.SRE.LoadGrammar(this.VTemp.SelectRoom(word.Text));

                        }
                        else if (word.Text.Contains("garage"))
                        {

                        }
                        else if (word.Text.Contains("password"))
                        {

                        }
                        else
                        {
                            this.Spee.Speak("Would you please repeat that.");
                        }

                    }
                    else if (this.CurrentStep == VoiceStep.Temperature)
                    {
                        if (word.Text.Contains("temperature"))
                        {// load temperature control
                            this.CurrentStep = VoiceStep.Temperature;
                            this.SRE.UnloadAllGrammars();
                            this.SRE.LoadGrammar(this.VTemp.SelectTemperature(word.Text));

                        }
                        else if (word.Text.Contains("outlet"))
                        {

                        }
                        else if (word.Text.Contains("ON"))
                        {
                            this.CurrentStep = VoiceStep.first;
                            this.SRE.UnloadAllGrammars();
                            this.SRE.LoadGrammar(this.VTemp.SetTemperature(word.Text));

                        }
                        else if (word.Text.Contains("Off"))
                        {
                            this.CurrentStep = VoiceStep.first;
                            this.SRE.UnloadAllGrammars();
                            this.SRE.LoadGrammar(this.VTemp.SetTemperature(word.Text));
                        }
                        else
                        {
                            this.Spee.Speak("Would you please repeat that.");
                        }

                           
                    }
                }

            }
            this.StartRecognizingVoice();
        }

        void SR_SpeechRecognized(object sender, SpeechRecognizedEventArgs e)
        {
            MessageBox.Show(e.Result.Text);
            if (e.Result.Text == "hal")
            {
                MessageBox.Show("Recognized Hal");
                //execute command to com port, and use spee to respond
            }
        }

        public SpeechRecognitionEngine SRE { get; set; }
        public Grammar FirstGrammar { get; set; }
        public Grammar SecondGrammar { get; set; }
        public VoiceStep CurrentStep { get; set; }

        public Speech.SpeechMain Spee { get; set; }

        public bool acknowlegdedSpeaker { get; set; }
        public Temperature.TemperatureCntrl VTemp { get; set; }
        /// <summary>
        /// This will generate a list of grammar to be expected from the voice speaking.
        /// </summary>
        public void GenerateGrammar(VoiceStep step)
        {
            //initialize all grammar objects
            GrammarBuilder TempGB = new GrammarBuilder();
           // this.FirstChoice = new Choices();
            // this.GrammarOptions = new Dictionary<string, string[]>();
            // GrammarOptions.Add("Intro",new string[] {"hal"});
            // GrammarOptions.Add("Second", new string[] { "room1", "room2", "room3", "room4", "room5",
            //     "room6", "room7", "room8", "room10", "room11", "room12", "room13","room14", "room15","room16", "garage","password","restart"});
            
            // Generate First Choice of words.
            if (step == VoiceStep.first)
            {
                Choices TempChoice = new Choices();
                TempChoice.Add(new string[] { "hal" });
                TempGB.Append(TempChoice);
                this.FirstGrammar = new Grammar(TempGB);
            this.SRE.UnloadAllGrammars();
            this.SRE.LoadGrammar(this.FirstGrammar);
            }
            // Generate Second Choice of Words
            else if (step == VoiceStep.second)
            {
                Choices TempChoice = new Choices();
                TempChoice.Add(new string[] { "room1", "room2", "room3", "room4", "room5",
                "room6", "room7", "room8", "room10", "room11", "room12", "room13","room14", "room15","room16", "garage","password", "restart"});
                TempGB.Append(TempChoice);
                this.SecondGrammar = new Grammar(TempGB);
            this.SRE.UnloadAllGrammars();
            this.SRE.LoadGrammar(this.SecondGrammar);
            }
        }

        public void StartRecognizingVoice()
        {
            this.SRE.RecognizeAsync(RecognizeMode.Single);
        }

        public void StopRecognizingVoice()
        {
            this.SRE.RecognizeAsyncStop();
        }


    }
}
