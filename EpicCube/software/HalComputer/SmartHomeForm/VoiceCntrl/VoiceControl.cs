using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SmartHomeForm.VoiceCntrl
{
    /// <summary>
    /// This class will handle all of the voice recognition and speech.
    /// with a simple object that will call a function.
    /// we can always use the same reference through voice control
    /// </summary>
    class VoiceControl
    {
        //The delegate will be called only when the voice has been
        // recognized.  in the function we need to create a bunch of 
        // objects with commands and delgates tied to the command.
        // in order to create those functions we need to have
        // this object(voice reg and speech) tied with it.
        // we also want a simple list of commands and delegates
        // that we can create the expected grammar, and then use it.
        //
        public delegate void VoiceCommandFunction(Speech.SpeechMain speaker, VoiceCntrl.VoiceControl VoiceRecogn);//this is what we need.

        public List<commandObject> ListOfCommands;

        public struct commandObject
        {
            public string command;
            public VoiceCommandFunction Function;
        }

        /// <summary>
        /// 
        /// </summary>
        public VoiceControl()
        {
            ListOfCommands = new List<commandObject>();
            
            //test to be removed
            commandObject yesMaster = new commandObject();
            yesMaster.command = "HAL";
            yesMaster.Function = ResetVoice;
            ListOfCommands.Add(yesMaster);
            //end of test
        }

        public void VoiceCommand(string humanCommand, VoiceCommandFunction callback)
        {

        }


        VoiceCommandFunction Reset = ResetVoice;
        /// <summary>
        /// Resets the Voice control to the intial state of listening to the word HAL.
        /// </summary>
        public static void ResetVoice(Speech.SpeechMain speaker, VoiceCntrl.VoiceControl VoiceRecogn)
        {
            //reset to main.
        }

        //simple function to update grammar after all strings have been added. // not the best Scalable approach, but a simple one.
    }
}
