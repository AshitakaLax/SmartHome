using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Speech.Synthesis;
using System.Speech.Recognition;
using System.Threading;

namespace SmartHomeAudioControl
{
    class Program
    {
        static bool verbose = true;
        static void Main(string[] args)
        {
            Recognition.Recognition Recogn = new Recognition.Recognition(verbose);
            Speech.Speech Spee = new Speech.Speech();
            
            //while(true)
            for (int i = 0; i < 30; i++ )
            {
                Thread.Sleep(1000);
                Recogn.SR.Enabled = true;
            }
        }
    }
}
