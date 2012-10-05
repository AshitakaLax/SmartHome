using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SmartHomeForm
{
    
    public interface List
    {
        event EventHandler updateGrammar;
    }
    public class CommandEventArgs : EventArgs
    {
        // not sure what "Class members are here"
    }

    /**
    public class CommandList : List
    {
        //needs to have a grammer object.
        public event EventHandler updateGrammar;

        public string command {get;set;}
        public VoiceCntrl.VoiceControl.VoiceCommandFunction voiceFunction {get; set;}

        public CommandList()
        {
            
        }

        void AddCommand()
        {

        }

        protected virtual void Add()
        {
            if()
        }


    }
     * */
}
