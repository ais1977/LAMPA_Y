// логин-пароль WiFi
#define AP_SSID "ais1977"
#define AP_PASS "www254201"
#define GH_INCLUDE_PORTAL // включить сайт в память программы (не нужно загружать файлы в память)
#define LED_PIN D6        // пин ленты
#define LED_NUM 3         // кол-во светодиодов

#include <Arduino.h>

#include <FileData.h>
#include <LittleFS.h>

#include <FastLED.h>
CRGB leds[LED_NUM];

#include <GyverHub.h>
GyverHub hub("dom_esp", "DOM", "");

struct Wifi
{
  char ssid[32] = "";
  char pass[32] = "";
};
Wifi w;

FileData wifi_(&LittleFS, "/data.dat", 'B', &w, sizeof(w));

//---------------------------------------------
bool byt_1, byt_2, byt_3;
uint8_t sla_1, sla_2, tab, sel_0, sel_1, sel_2;
GHcolor col;
//---------------------------------------------

void build()
{
  hub.BeginWidgets();  // начинает новую горизонтальную строку виджетов
  hub.WidgetSize(100); // сменим ширину
  if (hub.Tabs(&tab, F("Управление,Настройки"), GH_NO_LABEL))
  {
    hub.refresh(); // обновить страницу
  }

  switch (tab)
  {
  case 0:
  {
    hub.WidgetSize(25);
    if (hub.SwitchIcon_(F("B1"), &byt_1, "Зал"))
    {
      byt_1 ? sla_1 = 10 : sla_1 = 0;
      hub.sendUpdate("S1", String(byt_1 ? 10 : 0));
    }
    hub.WidgetSize(75);
    if (hub.Slider_(F("S1"), &sla_1, GH_UINT8, F("яркость"), 0, 10, 1))
    {
      sla_1 ? byt_1 = 1 : byt_1 = 0;
      hub.sendUpdate("B1", String(sla_1 ? 1 : 0));
    }
    hub.WidgetSize(25);
    if (hub.SwitchIcon_(F("B2"), &byt_2, "Спальня"))
    {
      byt_2 ? sla_2 = 10 : sla_2 = 0;
      hub.sendUpdate("S2", String(byt_2 ? 10 : 0));
    }
    hub.WidgetSize(75);
    if (hub.Slider_(F("S2"), &sla_2, GH_UINT8, F("яркость"), 0, 10, 1))
    {
      sla_2 ? byt_2 = 1 : byt_2 = 0;
      hub.sendUpdate("B2", String(sla_2 ? 1 : 0));
    }

    hub.WidgetSize(25);
    if (hub.SwitchIcon_(F("B3"), &byt_3, "Окошки"))
    {
    }
    hub.WidgetSize(37);
    if (hub.Select(&sel_0, F("цвет,эфекты"), F("Опции")))
    {
      hub.refresh();
    };
    switch (sel_0)
    {
    case 0:
    {
      hub.Color_("COL", &col, F("Color"));
      break;
    }
    case 1:
    {
      hub.Select(&sel_2, F("kek,puk,lol"), F("Эфекты"));
      break;
    }
    }

    break;
  }

  case 1:
  {
    hub.Title(F("Настройки WiFi"));
    hub.WidgetSize(100);
    bool flag_w = 0;
    flag_w |= hub.Input(&w.ssid, GH_CSTR, F("SSID"), 32);
    flag_w |= hub.Pass(&w.pass, GH_CSTR, F("PASS"), 32);
    if (flag_w)
    {
      wifi_.update();
    }
    break;
  }
  }

  hub.EndWidgets(); // закончим отрисовку виджетов
}

void setup()
{
  Serial.begin(115200);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.setBrightness(50);
  FastLED.show();

  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println(WiFi.localIP());

  hub.setupMQTT("m7.wqtt.ru", 12914, "dom_esp", "www254201");

  hub.onBuild(build); // подключаем билдер
  hub.begin();        // запускаем систему
}

void loop()
{
  hub.tick(); // обязательно тикаем тут
  static GHtimer tmr(1000);
  if (tmr.ready())
  {
    hub.sendGet("B1", String(byt_1));
    hub.sendGet("S1", String(sla_1));
    hub.sendGet("C", String(col));

    leds[0].setHSV(255, sla_2 * 25, sla_1 * 25);
    leds[1] = col;
    FastLED.show();
  }
}
