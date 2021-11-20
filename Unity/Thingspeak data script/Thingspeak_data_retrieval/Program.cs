using System;
using System.Net;
using System.IO;
using Newtonsoft.Json.Linq;

namespace Thingspeak_data_retrieval
{
    class Program
    {
        static void Main(string[] args)
        {
            WebClient webClient = new WebClient();
            var data = webClient.DownloadString("http://api.thingspeak.com/channels/1326912/feed.json?results=1");
            JObject o = JObject.Parse(data);
            Console.WriteLine(o["feeds"][0]["field3"].ToString());
            Console.ReadLine();

            // Temperature
            System.Globalization.CultureInfo customCulture = (System.Globalization.CultureInfo)System.Threading.Thread.CurrentThread.CurrentCulture.Clone();
            customCulture.NumberFormat.NumberDecimalSeparator = ".";
            System.Threading.Thread.CurrentThread.CurrentCulture = customCulture;

            float temperaturePercentage = float.Parse(o["feeds"][0]["field3"].ToString());
            Console.WriteLine(temperaturePercentage);
            Console.ReadLine();


        }
    }
}
