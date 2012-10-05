using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Speech.Recognition;

namespace SmartHomeForm.Temperature
{
    /// <summary>
    /// In this class we need to have the grammar and a way for the 
    /// computer to distiguish which word is being spoken after a certain point.
    /// </summary>
    class TemperatureCntrl
    {
        private Speech.SpeechMain Spee;
        public TemperatureCntrl(Speech.SpeechMain Speaker)
        {
            Spee = Speaker;
            //initialize all grammar objects
            // this.TempGB = new GrammarBuilder();
            // this.TempChoice = new Choices();
            // this.GrammarOptions = new Dictionary<string, string[]>();
            // GrammarOptions.Add("Intro", new string[] { "hal" });
            // this.TempChoice.Add("");
            // this.voiceChoices.Add(GrammarOptions["Intro"]);
            // this.GB.Append(this.voiceChoices);
        }

        public GrammarBuilder TempGB { get; set; }
        public Grammar TempGrammar { get; set; }
        private int RoomNumber { get; set; }

        public Grammar SelectRoom(string roomStr)
        {
            RoomNumber = Int16.Parse(roomStr.Substring(roomStr.IndexOf("m") + 1));
            Spee.Speak("What would you like in room" + RoomNumber);
            return this.GenerateGrammmar(1);

        }

        public Grammar SelectOutlet(string outletString)
        {
            //Give status on specific outlet
            return this.GenerateGrammmar(2);
        }

        public Grammar SelectTemperature(string temperatureStr)
        {
            //Givee status on specific temperature in room.
            this.Spee.Speak("Room " + this.RoomNumber + "'s Temperature is " + 72 + "and " + "on.");// need to change Temperature and on or off
            // int desiredTemperature = Int32.Parse(temperatureStr.Substring(2));

            return this.GenerateGrammmar(3);
        }
        public Grammar SetTemperature(string temperatureStr)
        {
            if (temperatureStr == "Off")
            {
                this.Spee.Speak("I have turned of the air in that room.");
            }
            else if (temperatureStr.Contains("on"))
            {
                int desiredTemperature = Int32.Parse(temperatureStr.Substring(2));
                this.Spee.Speak("I have turned on the air in that room.");
            }
            //Givee status on specific temperature in room.
           // this.Spee.Speak("Room has changed to ");
            // return to initial state
            return this.GenerateGrammmar(4);
        }

        private Grammar GenerateGrammmar(int step)
        {
            this.TempGB = new GrammarBuilder();
            if (step == 1)
            {

                Choices TempChoice = new Choices();
                TempChoice.Add(new string[] { "outlet1", "outlet2", "outlet3", "outlet4", 
                    "outlet5", "outlet6", "outlet7", "outlet8", "outlet9", "temperature", "restart"});
                TempGB.Append(TempChoice);
                return new Grammar(TempGB);
            }
            else if (step == 2)//outlet control
            {
                Choices TempChoice = new Choices();
                TempChoice.Add(new string[] { "Off", "on", "status", "restart"});
                TempGB.Append(TempChoice);
                return new Grammar(TempGB);
            }
            else if (step == 3)
            {
                Choices TempChoice = new Choices();
                TempChoice.Add(new string[] { "Off", "on60", "on61", "on62", "on63", "on64",
                    "on65","on66","on67","on68","on69","on70","on71","on72","on73","on74","on75",
                    "on76","on77","on78","on79","on80","on81","on82","on83","on84","restart" });
                TempGB.Append(TempChoice);
                return new Grammar(TempGB);
            }
            else if (step == 4)
            {
                Choices TempChoice = new Choices();
                TempChoice.Add(new string[] { "hal"});
                TempGB.Append(TempChoice);
                return new Grammar(TempGB);
            }
            return null;
        }
    }
}
