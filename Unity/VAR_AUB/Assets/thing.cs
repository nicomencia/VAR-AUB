using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Net;
using System.IO;
using Newtonsoft.Json.Linq;

public class thing : MonoBehaviour
{
    GameObject soilMoistureIndicator;
    // Start is called before the first frame update
    void Start()
    {
        WebClient webClient = new WebClient();
        var data = webClient.DownloadString("http://api.thingspeak.com/channels/1326912/feed.json?results=1");
        JObject o = JObject.Parse(data);
        Console.WriteLine(o["feeds"][0]["field1"]);
        Console.ReadLine();

        int soilMoisturePercentage = int.Parse(o["feeds"][0]["field1"].ToString());
        soilMoistureIndicator = GameObject.FindWithTag("soilMoistureIndicator");
        soilMoistureIndicator.transform.position += new Vector3(0, soilMoisturePercentage/50, 0);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

}
