using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;
using System.Net;
using System.IO;
using Newtonsoft.Json.Linq;
using TMPro;

public class sensorsData : MonoBehaviour
{
    GameObject soilMoistureIndicator;
    GameObject lightIndicator;

    GameObject temperatureValueText;
    GameObject temperatureUnitsText;

    GameObject rainIndicator;
    GameObject isHighlyRaining;
    // Start is called before the first frame update
    void Start()
    {
        WebClient webClient = new WebClient();
        var data = webClient.DownloadString("http://api.thingspeak.com/channels/1326912/feed.json?results=1");
        JObject o = JObject.Parse(data);

        // Soil moisture
        int soilMoisturePercentage = int.Parse(o["feeds"][0]["field1"].ToString());
        soilMoistureIndicator = GameObject.FindWithTag("soilMoistureIndicator");
        soilMoistureIndicator.transform.position += new Vector3(0, (float) soilMoisturePercentage/50, 0);

        // Light
        int lightPercentage = int.Parse(o["feeds"][0]["field2"].ToString());
        lightIndicator = GameObject.FindWithTag("lightIndicator");
        lightIndicator.transform.position += new Vector3(0, (float) lightPercentage/50, 0);

        // Temperature
        System.Globalization.CultureInfo customCulture = (System.Globalization.CultureInfo)System.Threading.Thread.CurrentThread.CurrentCulture.Clone();
        customCulture.NumberFormat.NumberDecimalSeparator = ".";
        System.Threading.Thread.CurrentThread.CurrentCulture = customCulture;

        float temperatureValue = float.Parse(o["feeds"][0]["field3"].ToString());
        temperatureValueText = GameObject.FindWithTag("tempValue");
        temperatureValueText.GetComponent<Text>().text = temperatureValue.ToString("#.0");
        temperatureUnitsText = GameObject.FindWithTag("tempUnits");

        if (temperatureValue < 10){
            ChangeTemperatureTextColor(Color.cyan);
        } else if (temperatureValue > 18){
            ChangeTemperatureTextColor(Color.red);
        }

        // Rain
        int rainPercentage = int.Parse(o["feeds"][0]["field4"].ToString());
        rainIndicator = GameObject.FindWithTag("rainIndicator");
        rainIndicator.transform.position += new Vector3(0, (float) rainPercentage/50, 0);

        // Is high rain
        int isRaining = int.Parse(o["feeds"][0]["field5"].ToString());
        isHighlyRaining = GameObject.FindWithTag("isHighlyRaining");
        if (isRaining == 1)
        {
            isHighlyRaining.SetActive(false);
        }
        else
        {
            isHighlyRaining.SetActive(true);
        }
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void ChangeTemperatureTextColor(Color color)
    {
        temperatureValueText.GetComponent<Text>().color = color;
        temperatureUnitsText.GetComponent<Text>().color = color;
    }

}
