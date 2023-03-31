#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Check the GxEPD2 examples to find the proper class for your display. This is from the "HelloWorld"-example. 
// Adjust initializer to pinout configuration
GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=5*/ SS, /*DC=*/22, /*RST=*/21, /*BUSY=*/4)); // GDEW042T2 400x300, UC8176 (IL0398)

// Global counter for keeping track of updates
uint8_t counter = 0;
// Store JSON from webservice in memory, so it can be referenced again easily
StaticJsonDocument<2000> doc;

/*
Connection state indicator, used for displaying error messages in case of connection loss
0 = not connected
1 = connected, ok
2 = disconnected from wifi
3 = GET !200
*/
uint8_t connectionState = 0; 

void setup()
{
    Serial.begin(115200);
    display.init(115200);
    connectWifi();    
    refreshData();
    setupInit();    
    display.hibernate();
}

void loop()
{     
    display.setPartialWindow(0, 0, display.width(), 60);
    display.firstPage();
    do
    {
        //display.fillScreen(GxEPD_WHITE);
        if (counter >= 30)
        {
            refreshData();
            counter = 0;            
        }
        updateData();
    } while (display.nextPage());

    display.setPartialWindow(0, 60, display.width(), display.height() - 60);
    display.firstPage();
    do
    {                
        updateChart();        
    } while (display.nextPage());
    counter += 1;
    delay(1000);
};

void connectWifi()
{
    if(WiFi.status() == WL_CONNECTED)
        return; 
    
    Serial.println("Connecting to WiFi"); 
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    WiFi.begin("SSID", "PASS");
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(500);
    }
    connectionState = (WiFi.status() == WL_CONNECTED) ? 0 : 1;    
    Serial.println(WiFi.localIP());    
}

void updateChart()
{
    double max = atof( (const char*)doc["max"]); 
    // Clear chart    
    display.fillScreen(GxEPD_WHITE);
    uint8_t numPrices = doc["prices"].size(); 
    if (numPrices <= 0)
    {        
        display.setFont(&FreeSans9pt7b);
        writeText("Waiting for data", 0, 120);
        return;
    }
    // Bottom line
    display.drawLine(0, display.height() - 30, display.width(), display.height() - 30, GxEPD_BLACK);
    
    // Draw chart   
    uint16_t yValOffset = 35, xPos = 0, pct, val, yPos;    
    display.setFont(&FreeSans9pt7b);
    for (uint8_t i = 0; i < numPrices; i++)
    {
        const char *hour = doc["hours"][i];
        const char *price_c = doc["prices"][i];        
        double price = atof((char *)price_c);
       
        xPos += (i == 0) ? 0 : 30;
        pct = price * 100 / max;
        val = (pct <= 10) ? 10 : 150 * (int)pct / 100; 
        yPos = display.height() - (val + yValOffset);
        // Draw bar
        display.fillRect(xPos + 12, yPos, 19, val, GxEPD_BLACK);
        // Hour
        writeText((char *)hour, xPos + 11, display.height() - 5);
        // Price
        writeText((char *)price_c, xPos + 8, yPos - 5);
    }
}


void setupInit()
{
    display.setRotation(2);
    display.setTextColor(GxEPD_BLACK);
    display.setFullWindow();
    display.firstPage();
    do
    {      
    } while (display.nextPage());
    Serial.println("Setupinit complete");
}

void updateData()
{       
    writeHeader();
    showConnectionMessage();   
    updateTime();  
    updatePrice(); 
    updateDate();    
}

void showConnectionMessage()
{ 
    display.setFont(&FreeSans9pt7b);
    switch(connectionState)
    {
        case 0: writeText("Not connected", 0,40); break;
        case 1: display.drawRect(0,30,150,40, GxEPD_WHITE); break;
        case 2: writeText("Disconnected!", 0,40); break;
        case 3: writeText("Server unavailable", 0,40); break; 
    }
}

void writeHeader()
{   
    display.setFont(&FreeSans18pt7b);
    writeText("Power", 0, 25);  
}

void updateTime()
{    
    const char *time = (const char *)doc["time"];    
    display.setFont(&FreeSans18pt7b);    
    writeText((char*)time, 310,30);     
}

void updatePrice()
{    
    const char *price = doc["prices"][0];    
    char txt[15] = "Price: ";
    char *msg = strcat(txt, price);
    display.setFont(&FreeSans12pt7b);   
    writeText((char*)msg, 295,55);    
}

void updateDate()
{
    const char *date = doc["date"];
    display.setFont(&FreeSans9pt7b);   
    writeText((char*)date, 150,15);    
}

void writeText(char *text, int x, int y)
{
    display.setCursor(x, y);
    display.print(text);
}

void refreshData()
{
    Serial.write("Refresh data...");        
    connectWifi();

    HTTPClient http;
    http.begin("http://192.168.50.92:3000/power/report");

    Serial.write("Update data request response");
    uint16_t rCode = http.GET();
    if (rCode == 200)
    {
        Serial.println("GET 200");        
        connectionState = 1;        
        DeserializationError error = deserializeJson(doc, http.getString());        
        if(error) { writeText("Data error", 0,30); }
    }else{
        Serial.print("Disconnected");         
        connectionState = 3;        
    }    
}