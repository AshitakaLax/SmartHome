﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;
using System.Net;
using System.Web;
using System.Diagnostics;
using System.Media;
using System.Xml;

namespace SharpVoice
{
    class Voice
    {   
        String rnrSEE = null;
	    String source = null;
	    String user = null;
	    String pass = null;
	    String authToken = null;
		String settingsPath = "sharpVoice.xml";

        /*
		 * Links Imported from https://pygooglevoice.googlecode.com/hg/googlevoice/settings.py
		 * Made to be more compatible with porting future python code.
		*/
		//string LOGIN = "https://www.google.com/accounts/ServiceLoginAuth?service=grandcentral";
		string LOGIN = "https://www.google.com/accounts/ClientLogin";

		string[] FEEDS = new String[]{"inbox", "starred", "all", "spam", "trash", "voicemail", "sms",
				"recorded", "placed", "received", "missed"};

		const string BASE = "https://www.google.com/voice/";
		string LOGOUT = BASE + "account/signout";
		string INBOX = BASE + "#inbox";
		string CALL = BASE + "call/connect/";
		string CANCEL = BASE + "call/cancel/";
		string DEFAULT_FORWARD = BASE + "settings/editDefaultForwarding/";
		string FORWARD = BASE + "settings/editForwarding/";
		string DELETE = BASE + "inbox/deleteMessages/";
		string MARK = BASE + "inbox/mark/";
		string STAR = BASE + "inbox/star/";
		string SMS = BASE + "sms/send/";
		string DOWNLOAD = BASE + "media/send_voicemail/";
		string BALANCE = BASE + "settings/billingcredit/";
		
		string XML_SEARCH = BASE + "inbox/search/";
		string XML_CONTACTS = BASE + "contacts/";
		const string XML_RECENT = BASE + "inbox/recent/";
		string XML_INBOX = XML_RECENT + "inbox/";
		string XML_STARRED = XML_RECENT + "starred/";
		string XML_ALL = XML_RECENT + "all/";
		string XML_SPAM = XML_RECENT + "spam/";
		string XML_TRASH = XML_RECENT + "trash/";
		string XML_VOICEMAIL = XML_RECENT + "voicemail/";
		string XML_SMS = XML_RECENT + "sms/";
		string XML_RECORDED = XML_RECENT + "recorded/";
		string XML_PLACED = XML_RECENT + "placed/";
		string XML_RECEIVED = XML_RECENT + "received/";
		string XML_MISSED = XML_RECENT + "missed/";

        public Voice()
        {
			XmlDocument settings = new XmlDocument();
			try
			{
				settings.Load(settingsPath);
				
			}
			catch (FileNotFoundException)
			{
				// request data.

				// save after getting datas
				settings.Save(settingsPath);
			}
			catch
			{
				// general error from one line of code... what?!
			}
			finally
			{
				// and finally... do nothing? save maybe?
			}
			
			
        }

		public Voice(string user, string pass)
		{
			this.user = user;
			this.pass = pass;
		}

		public Voice(String user, String pass, String source, String rnrSee){

		    this.user = user;
		    this.pass = pass;
		    this.rnrSEE = rnrSee;
		    this.source = source;

		    login();
	    }
		/*
		inbox - Recent, unread messages
		starred - Starred messages
		all - All messages
		spam - Messages likely to be spam
		trash - Deleted messages
		voicemail - Voicemail messages
		sms - Text messages
		recorded - Recorced messages
		placed - Outgoing messages
		received - Incoming messages
		missed - Messages not received
		*/

		#region java translated functions
        public String getInbox(){
		    return get(XML_INBOX);
	    }
	    public String getStarred(){
		    return get(XML_STARRED);
	    }
	    public String getRecent(){
		    return get(XML_ALL);
	    }
	    public String getSpam(){
		    return get(XML_SPAM);
	    }
	    public String getRecorded(){
		    return get(XML_RECORDED);
	    }
	    public String getPlaced(){
		    return get(XML_PLACED);
	    }
	    public String getReceived(){
		    return get(XML_RECEIVED);
	    }
	    public String getMissed(){
		    return get(XML_MISSED);
	    }
	    public String getSMS(){
		    return get(XML_SMS);
	    }
		#endregion

        private String get(String urlString){
		    string result = null;

            WebResponse response = null;
            StreamReader reader = null;

            try
            {
                HttpWebRequest request = (HttpWebRequest)WebRequest.Create(urlString + "?auth=" + authToken);
                request.Method = "GET";
                response = request.GetResponse();
                reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8);
                result = reader.ReadToEnd();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.StackTrace);
                throw new IOException(ex.Message);
            }
            finally
            {
                if (reader != null)
                    reader.Close();
                if (response != null)
                    response.Close();
            }
		    return result;
	    }

        public void login(){
            if (user == null)
				throw new IOException("No User Defined");
            if (pass == null)
				throw new IOException("No Pass Defined");
			if (rnrSEE == null)
			{
				try
				{
					rnrSEE = special();
				}
				catch
				{
					throw new IOException("No rnr key Defined");
				}
			}
            
            String data = HttpUtility.UrlEncode("accountType", Encoding.UTF8) + "="
				    + HttpUtility.UrlEncode("GOOGLE", Encoding.UTF8);
		    data += "&" + HttpUtility.UrlEncode("Email", Encoding.UTF8) + "="
				    + HttpUtility.UrlEncode(user, Encoding.UTF8);
		    data += "&" + HttpUtility.UrlEncode("Passwd", Encoding.UTF8) + "="
				    + HttpUtility.UrlEncode(pass, Encoding.UTF8);
		    data += "&" + HttpUtility.UrlEncode("service", Encoding.UTF8) + "="
				    + HttpUtility.UrlEncode("grandcentral", Encoding.UTF8);
		    data += "&" + HttpUtility.UrlEncode("source", Encoding.UTF8) + "="
				    + HttpUtility.UrlEncode(source, Encoding.UTF8);

			string response = makeRequest(LOGIN,data);
			Console.WriteLine(response);
			string[] stringSeparators = new string[] { "=" };
			
			foreach (string line in response.Split(new string[] { Environment.NewLine }, StringSplitOptions.None))
			{
				Console.WriteLine(line);
				Console.WriteLine("======");
				if (line.Contains("Auth="))
				{
					this.authToken = line.Split(stringSeparators, 2, StringSplitOptions.None)[1].Trim();
					//Console.WriteLine("AUTH TOKEN =" + this.authToken);
				}
				else if (line.Contains("Error="))
				{
					throw new IOException(line);
				}
			}

			/*
            WebResponse response = null;
            StreamReader reader = null;
			
            try
            {
                HttpWebRequest request = (HttpWebRequest)WebRequest.Create("https://www.google.com/accounts/ClientLogin");
                
                request.Method = "POST";
                request.ContentType = "application/x-www-form-urlencoded";
                request.ContentLength = data.Length;
                using (Stream writeStream = request.GetRequestStream())
                {
                    UTF8Encoding encoding = new UTF8Encoding();
                    byte[] bytes = encoding.GetBytes(data);
                    writeStream.Write(bytes, 0, bytes.Length);
                }
                response = request.GetResponse();
                
                reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8);
                
                String line;
                string[] stringSeparators = new string[] { "=" };
                while ((line = reader.ReadLine()) != null)
                {
                    // System.out.println(line);
                    Debug.WriteLine(line);
                    if (line.Contains("Auth="))
                    {
                        this.authToken = line.Split(stringSeparators,2,StringSplitOptions.None)[1].Trim();
                        Console.WriteLine("AUTH TOKEN =" + this.authToken);
                    }
                    else if (line.Contains("Error="))
                    {
                        throw new IOException(line);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.StackTrace);
                throw new IOException(ex.Message);
            }
            finally
            {
                if (reader != null)
                    reader.Close();
                if (response != null)
                    response.Close();
            }
            */
		    if (this.authToken == null) {
			    throw new IOException("No Authorization Received.");
		    }
	    }

		public void logout()
		{
			makeRequest(LOGOUT, "");
			rnrSEE = null;
		}

		private string special()
		{
			if (rnrSEE != String.Empty)
				return rnrSEE;
			string sp;
			/*
			try:
				try:
					regex = bytes("('_rnr_se':) '(.+)'", 'utf8')
				except TypeError:
					regex = bytes("('_rnr_se':) '(.+)'")
			except NameError:
				regex = r"('_rnr_se':) '(.+)'"
			try:
				sp = re.search(regex, urlopen(settings.INBOX).read()).group(2)
			except AttributeError:
				sp = None
			*/
			Regex regex = new Regex("('_rnr_se':) '(.+)'");
			try
			{
				HttpWebRequest h = (HttpWebRequest)WebRequest.Create(INBOX);
				MatchCollection m = regex.Matches(new StreamReader(h.GetResponse().GetResponseStream()).ReadToEnd());
				sp = m[1].Value;// group 2, i hope...
			}
			catch
			{
				sp = null;
				throw new IOException();
			}
			return sp;
		}

        public String call(String originNumber, String destinationNumber){
		    String calldata = HttpUtility.UrlEncode("auth", Encoding.UTF8) + "="
				    + HttpUtility.UrlEncode(authToken, Encoding.UTF8);
		    calldata += "&" + HttpUtility.UrlEncode("outgoingNumber", Encoding.UTF8) + "="
				    + HttpUtility.UrlEncode(destinationNumber, Encoding.UTF8);
		    calldata += "&" + HttpUtility.UrlEncode("forwardingNumber", Encoding.UTF8) + "="
				    + HttpUtility.UrlEncode(originNumber, Encoding.UTF8);
		    calldata += "&" + HttpUtility.UrlEncode("subscriberNumber", Encoding.UTF8) + "="
				    + HttpUtility.UrlEncode("undefined", Encoding.UTF8);
		    calldata += "&" + HttpUtility.UrlEncode("remember", Encoding.UTF8) + "="
				    + HttpUtility.UrlEncode("0", Encoding.UTF8);
		    calldata += "&" + HttpUtility.UrlEncode("_rnr_se", Encoding.UTF8) + "="
				    + HttpUtility.UrlEncode(rnrSEE, Encoding.UTF8);
		    // POST /voice/call/connect/ outgoingNumber=[number to
		    // call]&forwardingNumber=[forwarding
		    // number]&subscriberNumber=undefined&remember=0&_rnr_se=[pull from
		    // page]

            return this.makeRequest("https://www.google.com/voice/call/connect/", calldata);
	    }

        public String sendSMS(String destinationNumber, String txt){
		    String smsdata = HttpUtility.UrlEncode("auth", Encoding.UTF8) + "="
				    + HttpUtility.UrlEncode(authToken, Encoding.UTF8);

		    smsdata += "&" + HttpUtility.UrlEncode("phoneNumber", Encoding.UTF8) + "="
				    + HttpUtility.UrlEncode(destinationNumber, Encoding.UTF8);
		    smsdata += "&"
				    + HttpUtility.UrlEncode("text", Encoding.UTF8)
				    + "="
                    + HttpUtility.UrlEncode(txt, Encoding.UTF8);
		    smsdata += "&" + HttpUtility.UrlEncode("_rnr_se", Encoding.UTF8) + "="
				    + HttpUtility.UrlEncode(rnrSEE, Encoding.UTF8);

            return this.makeRequest("https://www.google.com/voice/sms/send/", smsdata);
	    }

        private String makeRequest(String url, String data)
        {
            string result = null;

            WebResponse response = null;
            StreamReader reader = null;
            try
            {
                HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
                request.Method = "POST";
                request.ContentType = "application/x-www-form-urlencoded";
                request.ContentLength = data.Length;
                using (Stream writeStream = request.GetRequestStream())
                {
                    UTF8Encoding encoding = new UTF8Encoding();
                    byte[] bytes = encoding.GetBytes(data);
                    writeStream.Write(bytes, 0, bytes.Length);
                }
                response = request.GetResponse();
                reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8);
                result = reader.ReadToEnd();

            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.StackTrace);
                throw new IOException(ex.Message);
            }
            finally
            {
                if (reader != null)
                    reader.Close();
                if (response != null)
                    response.Close();
            }

            if (result.Equals(""))
            {
                throw new IOException("No Response Data Received.");
            }

            return result;
        }

		private String saveVoicemail(string voiceID){
			//https://www.google.com/voice/media/send_voicemail/[voicemail id]
			// still working on this...
			// 
			return "";
		}

		class inbox : SharpVoice.Util.Folder
		{
			public Folder inbox()
			{
				//parse inbox data


			}
		}

    }
	
	namespace Util{
		class Folder
		{//Folder wrapper for feeds from Google Voice
			int totalSize;//(aka __len__)
			Dictionary<string,int> unreadCounts;
			int resultsPerPage;
			List<Message> messages;//list of Message instances

			public Folder(Voice v, string name, string data){
				//Folder(voice, name, data)
			}

			public List<Message> Messages()
			{//Returns a list of all messages in this folder
				return messages;
			}
		}
		
		class Phone
		{// Wrapper for phone objects used for phone specific methods
			int id;
			string phoneNumber;// i18n phone number
			string formattedNumber;// humanized phone number string
			//data dictionary? we;
			//data dictionary? wd;
			bool verified;
			string name;
			bool smsEndabled;
			bool scheduleSet;
			int policyBitmask;
			//List weekdayTimes;
			//dEPRECATEDDisabled: bool
			bool weekdayAllDay;
			//bool telephonyVerified;
			//List weekendTimes
			bool active;
			bool weekendAllDay;
			bool enabledForOthers;
			int type;// (1 - Home, 2 - Mobile, 3 - Work, 4 - Gizmo)

			
			public Phone(Voice v, string data){
				//Phone(voice, data)
			}

			public void Disable()
			{
				//disables phone
			}

			public void Enable()
			{
				//enables phone
			}

			public string toString()
			{
				return phoneNumber;
			}
		}
		
		class Message{
			//Wrapper for all call/sms message instances stored in Google Voice Attributes are:

			string id;//SHA1 identifier
			bool isTrash;
			//displayStartDateTime: datetime
			bool star;
			bool isSpam;
			//startTime: gmtime
			List<string> labels;
			// displayStartTime: time
			string children;
			string note;
			bool isRead;
			string displayNumber;
			string relativeStartTime;
			string phoneNumber;
			int type;
			
			public Message(string folder, int id, string data)
			{
				//Message(folder, id, data)
				/*
				def __init__(self, voice, name, data):
					self.voice = voice
					self.name = name
					super(AttrDict, self).__init__(data)
				*/
			}
			
			public void Delete()
			{
				Delete(true);
			}

			public void Delete(bool trash)
			{
				//Moves this message to the Trash. Use message.delete(0) to move it out of the Trash.
			}

			public void Download()
			{
				Download(Environment.CurrentDirectory);
			}

			public void Download(string adir)
			{
				//Download the message MP3 (if any). Saves files to adir (defaults to current directory). Message hashes can be found in self.voicemail().messages for example. Returns location of saved file.
			}

			public void Mark(){
				Mark(true);
			}

			public void Mark(bool read)
			{
				//Mark this message as read. Use message.mark(0) to mark it as unread.
			}
			
			public void Star()
			{
				Star(true);
			}

			public void Star(bool star)
			{
				//Star this message. Use message.star(0) to unstar it.
			}

			public string toString()
			{
				return id;
			}

		}

		class XMLParser{
			/*
			class SharpVoice.util.XMLParser(voice, name, datafunc)¶
			XML Parser helper that can dig json and html out of the feeds. The parser takes a Voice instance, page name, and function to grab data from. Calling the parser calls the data function once, sets up the json and html attributes and returns a Folder instance for the given page:

			>>> o = XMLParser(voice, 'voicemail', lambda: 'some xml payload')
			>>> o()
			... <Folder ...>
			>>> o.json
			... 'some json payload'
			>>> o.data
			... 'loaded json payload'
			>>> o.html
			... 'some html payload'
			data
			Returns the parsed json information after calling the XMLParser
			folder
			Returns associated Folder instance for given page (self.name)
			*/
			string json;
			string html;
			string data;

			public XMLParser(Voice v, string name, string datafunc){
				
			}

			public string Data()
			{
				return data;
			}

			public Folder Folder()
			{
				return null;
			}
		}
	}
}