/*
 * * Copyright 2015 Axibase Corporation or its affiliates. All Rights Reserved.
 * *
 * * Licensed under the Apache License, Version 2.0 (the "License").
 * * You may not use this file except in compliance with the License.
 * * A copy of the License is located at
 * *
 * * https://www.axibase.com/atsd/axibase-apache-2.0.pdf
 * *
 * * or in the "license" file accompanying this file. This file is distributed
 * * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * * express or implied. See the License for the specific language governing
 * * permissions and limitations under the License.
 * */

#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include <avr/pgmspace.h>
#include <IRremote.h>

#define DHCP_WORK_TIME 60
#define TIMEOUT_TO_BLINK 10000
#define BLINK_INTERVAL 2150
#define COLORS_COUNT 19
#define MAX_PARAMS_LENGTH 10
#define MAX_PARAMS_COUNT 4
#define REQUEST_SIZE 50
#define RESULT_SIZE 50
#define BLINK_TIME 350
#define COMMAND_SIZE 4

#define LEDS_COUNT 1
#define COLORS_IN_DIOD_COUNT 3
#define TMP_INT_TO_CHAR_SIZE 16 
#define BEEPPIN 22

//#define REPEAT_CODE "4294967295"
//#define MORE_CODE "16187647"
//#define LESS_CODE "16220287"
#define LIGHT_POWER_LEVEL 32
#define MAC_SIZE 6
#define IP_SIZE 15


const char *OFF_CODES[LEDS_COUNT] = {"16203967"};
const char *ON_CODES[LEDS_COUNT] = {"16236607"};

const char default_page[] PROGMEM = "<html><body><div style='float:left'><form action='./set' mathod='get'><div><select name='color'><option>black</option><option>silver</option><option>gray</option><option>white</option><option>maroon</option><option>red</option><option>purple</option><option>fuchsia</option><option>green</option><option>lime</option><option>olive</option><option>yellow</option><option>navy</option><option>blue</option><option>teal</option><option>aqua</option><option>brown</option><option>orange</option><option>custom</option></select></div><div><input type='submit' value='set color'/></div></form></div><div style='float:left'><form action='./set' mathod='get' ><div><select name='level'><option>critical</option><option>error</option><option>fatal</option><option>down</option><option>fail</option><option>failure</option><option>open</option><option>bad</option><option>acknowledged</option><option>ack</option><option>acknowledge</option><option>normal</option><option>up</option><option>info</option><option>information</option><option>informational</option><option>good</option><option>close</option><option>warning</option><option>warn</option><option>warning</option><option>minor</option><option>marginal</option><option>major</option></select></div><div><input type='submit' value='set level'/></div></form></div><div style='float:left'><form action='./set' mathod='get' ><div><input type='text' name='delay' size='6'></div><div><input type='submit' value='set delay'/></div></form></div><a href='./set?light=on'><input type='button' value='on'></a><a href='./get'><input type='button' value='get info'></a><br><a href='./set?light=off'><input type='button' value='off'></a><a href='./set?beep'><input type='button' value='beep'></a></body></html>\0";
const char http_header[] PROGMEM = "HTTP/1.1 200 OK\n\rContent-Type: text/html\n\rConnection: close\r\n\r\n";
const char level_red[] PROGMEM = "critical error fatal down fail failure open bad";
const char level_silver[] PROGMEM = "unknown";
const char level_white[] PROGMEM = "undefined";
const char level_fuchsia[] PROGMEM = "acknowledged ack acknowledge";
const char level_green[] PROGMEM = "normal up info information informational ok close good";
const char level_yellow[] PROGMEM = "warning warn";
const char level_blue[] PROGMEM = "minor marginal";
const char level_orange[] PROGMEM ="major";

//multy 1 - no change color, 2 - color / 2; 3 - color / 1.5
struct Color {
  const char *name;
  byte r;
  byte g;
  byte b;
  const char *code[LEDS_COUNT];
  Color(const char *name, byte r, byte g, byte b, const char *code){
    this->name = name;
    this->r = r;
    this->g = g;
    this->b = b;
    this->code[0] = code;
  };
  Color() {
    name = "black";
    r = 0;
    g = 0;
    b = 0;
    for(byte i = 0; i < LEDS_COUNT; i++) {
      code[i] = OFF_CODES[i];
    }
  }
};
Color *colors = new Color[COLORS_COUNT];





class EtherShell {
private:
  EthernetServer server;
  EthernetClient client;
public:
  EtherShell(int port): server(port) {
    
  }

  int create_connect() {
    byte mac[] = { 0x00, 0x1F, 0x16, 0xCC, 0xEC, 0x4B };
    long e_time = millis()+DHCP_WORK_TIME;
    while(millis() < e_time) {
        if(Ethernet.begin(mac) != 0 ) {
            return 1;
        }
    }
    IPAddress ip(10,128,12,57);	
    IPAddress dns(10,164,134,69);
    IPAddress gateway(10,128,12,1);
    IPAddress subnet(255,255,254,0);

    //IPAddress ip(192,168,1,96);	
    //IPAddress dns(192,168,1,1);
    //IPAddress gateway(192,168,1,1);
    //IPAddress subnet(255,255,255,0);
    Ethernet.begin(mac, ip, dns, gateway, subnet);
    return 1;
  }

  void get_ip(char *ptr) {
    sprintf(ptr, "%d.%d.%d.%d\0", Ethernet.localIP()[0], Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3]);
  }
  
  void finish(const char *message) {
    if(client) {
      print_progmem(http_header);
      if(strcmp(message, "def") == 0) {
        print_progmem(default_page);
        
      } else {
        client.println(message);
      }
      
      delay(100);
      client.stop();
    }
  }
  
  void print_progmem(const char *progmem_str_ptr) {
    while (pgm_read_byte(progmem_str_ptr)!= '\0') {
    	client.print((char)pgm_read_byte(progmem_str_ptr));
    	progmem_str_ptr++;
    }
  }
  
  void get_request(char *request) {
    char *tmp = request;
    client = (this->server).available();
    char chr;
    if(client) {
      bool find_command = false;
      while (client.available()) {
          chr = client.read();
          if(chr == '\n')
            break;
          if(chr == ' ' && !find_command) {
            find_command = true;
            continue;
          } else if (chr == ' ') {
            break;
          }
          if(find_command) {
            if(strlen(tmp) < (REQUEST_SIZE - 1 )) {
              *tmp++ = chr;
            }
          }
      }
    }
    *tmp = '\0';
  }
};
EtherShell *ether;


struct State {
  State(){
    this->color = &colors[0];
    on = true;
    level = "";
    this->modified_time = -99999999;//for blink on start;
  }
  long modified_time;
  Color *color;
  bool on;
  char *level;
  void modify_time() {
    this->modified_time = millis();
  }
};



class Led {
private:
  byte leds_slot[COLORS_IN_DIOD_COUNT];
  State *state;
  byte index;
public:
  Led(byte index, byte r, byte g, byte b) {
    leds_slot[0] = r;
    leds_slot[1] = g;
    leds_slot[2] = b;
    this->index = index;
    pinMode(leds_slot[0], OUTPUT);
    pinMode(leds_slot[1], OUTPUT);
    pinMode(leds_slot[2], OUTPUT);
    this->state = new State();
    this->lost_info = false;
  }
  boolean lost_info;

  
  byte get_index() {
    return this->index;
  }
  
  void set_state(const bool new_state) {
    
    if(new_state) {
      this->state->on = new_state; 
      IRsend irsend;
      irsend.sendNEC(atol(ON_CODES[this->index]), 32);
      this->change_color(this->state->color);
      this->state->modify_time();
    } else {
      this->change_color(&colors[0], false);
      this->state->on = new_state; 
    }
  }
  
  State *get_state() {
    return this->state; 
  }
  
  void change_color(Color *color) {
    this->change_color(color, true);
  }

  void change_color(Color *color, boolean save_new_color) {
    if(save_new_color) {
      this->state->color = color;    
    }
    if(this->state->on) {
        IRsend irsend;
        if(strcmp(this->state->color->name, "black") == 0 ) {
            irsend.sendNEC(atol(ON_CODES[this->index]), 32); 	
        }
        irsend.sendNEC(atol(color->code[this->index]), 32);
        analogWrite(leds_slot[0], color->r);
        analogWrite(leds_slot[1], color->g);
        analogWrite(leds_slot[2], color->b);
    }
  }
};
Led *leds[LEDS_COUNT];


class Blink {
private:
  unsigned long timeout;//if set to 0, no delay
  unsigned int frequence;
  long last_blink_time;
  boolean hide;
  Led *led;
  boolean need_modify_time;

public:
  Blink(long timeout, byte frequence, Led* led) {
    this->timeout = timeout*1000;
    this->frequence = frequence*1000;
    this->last_blink_time = 0;
    this->hide = true;
    this->led = led;
    this->need_modify_time = false;
  }

  void set_timeout(long timeout) {
    this->timeout = timeout*1000;
    this->led->change_color(this->led->get_state()->color);
    IRsend irsend;
    irsend.sendNEC(atol(this->led->get_state()->color->code[this->led->get_index()]), 32);
    this->led->get_state()->modify_time();
  }

  int get_timeout() {
    return this->timeout/1000;
  }

  void check() {
    long current_time = millis();
    if((current_time - this->led->get_state()->modified_time) > this->timeout && this->timeout > 0) {
      this->led->lost_info = true;
      if ((current_time - this->last_blink_time) > BLINK_TIME) {
        if( this->led->get_state()->on ) {
          if( this->hide ) {
            IRsend irsend;
            irsend.sendNEC(atol(ON_CODES[this->led->get_index()]), 32);
            this->led->change_color(this->led->get_state()->color);
            this->last_blink_time = millis();
            this->hide = false;
          } else {
            if( (current_time - this->last_blink_time) > this->frequence && ((current_time - this->led->get_state()->modified_time)) ) {
              this->led->change_color(&colors[0], false);
              this->last_blink_time = millis();
              this->hide = true;
            }
          }
        }
      }
    }else {
        if(this->hide && this->led->get_state()->on) {
            IRsend irsend;
            irsend.sendNEC(atol(ON_CODES[this->led->get_index()]), 32);
            this->led->change_color(this->led->get_state()->color);
            this->last_blink_time = millis();
            this->hide = false;
        }
    }

  }
};
Blink *blinks[LEDS_COUNT];


class Lcd {
private:
  LiquidCrystal *display;
public:
  Lcd() {
    display = new LiquidCrystal(14, 15, 16, 17, 18, 19);
    display->begin(16, 2);
  }
  
  void update() {
    this->display->clear();
    char ip[IP_SIZE];
    ether->get_ip(ip);
    this->set_text(0, ip);
    char tmp[TMP_INT_TO_CHAR_SIZE + 10] = {0};
    this->get_status(tmp);
    this->set_text(1, tmp);
  }
  
  void get_status(char *ptr) {
    for(byte i = 0; i < LEDS_COUNT; i++) {
      char tmp[TMP_INT_TO_CHAR_SIZE] = {0};
      sprintf(tmp, "%d", blinks[i]->get_timeout());
      strcat(ptr, tmp);
      if(i != (LEDS_COUNT - 1)) {
        strcat(ptr, ", ");
      }
    }
  }

  void set_text(int line, const char *text) {
    this->display->setCursor(0, line);
    while(*text != '\0') {
      this->display->write(*text);
      text++;
    }
  }
};
Lcd *lcd;




class Utility {
public:
  static int color_id_by_level(const char* level) {
    byte result;
    char *str = new char[70];
    read_from_progmem(level_red, str);
    if(strstr(str, level) != NULL) {
      result = 13;
    }else {
      read_from_progmem(level_silver, str);
      if(strstr(str, level) != NULL) {
        result = 12;
      } else {
        read_from_progmem(level_fuchsia, str);
        if(strstr(str, level) != NULL) {
          result = 14;
        } else {
          read_from_progmem(level_green, str);
          if(strstr(str, level) != NULL) {
            result = 3;
          } else {
            read_from_progmem(level_yellow, str);
            if(strstr(str, level) != NULL) {
              result = 16;
            } else {
              read_from_progmem(level_blue, str);
              if(strstr(str, level) != NULL) {
                result = 2;
              } else {
                read_from_progmem(level_orange, str);
                if(strstr(str, level) != NULL) {
                  result = 15;
                } else {
                  read_from_progmem(level_white, str);
                  if(strstr(str, level) != NULL) {
                    result = 17;
                  } else {
                    result = -1;
                  }
                }
              }
            }
          }
        }
      }
    }
    delete[] str;
    return result;
    
  }
  
  static void read_from_progmem(const char *from, char *to) {
    char *tmp = to;
    while (pgm_read_byte(from)!= '\0') {
      *tmp++ = (char)pgm_read_byte(from++);
    }
    *tmp = '\0';
  }  
  
  
};


class Worker {
  char params[MAX_PARAMS_COUNT][MAX_PARAMS_LENGTH];
  char values[MAX_PARAMS_COUNT][MAX_PARAMS_LENGTH];
  char command[COMMAND_SIZE];
  signed char index;
  char result[RESULT_SIZE];
  byte params_count;
public:
  Worker() {
    for(byte i = 0; i < MAX_PARAMS_COUNT; i++) {
      memset(this->params[i], 0, sizeof(this->params[i]));
      memset(this->values[i], 0, sizeof(this->values[i]));
    }
    this->params_count = 0;
    memset(this->command, 0, sizeof(this->command));
    memset(this->result, 0, sizeof(this->result));
    this->index = -1;
  };
  

  bool parse_request(char *request) {    
    char *tmp = command;
    for(int i = 1; i < 4; i++) {
      *tmp++ = *(request + i);
    }
    *tmp = '\0';
    bool param = true;
    char request_no_ex[50];
    tmp = request_no_ex;
    for(byte i = 5; i < strlen(request); i++) {
      *tmp++ = *(request + i);
    }
    *tmp = '\0';  
    char *param_ptr = this->params[0];
    char *value_ptr = this->values[0];

    
    if( strcmp(command, "set") == 0 ) {
      for(byte i = 0; i < strlen(request_no_ex); i++) {
        if(request_no_ex[i] == '=') {
          param = false;
        } else if (request_no_ex[i] == '&') {
          param = true;
          this->params_count++;
          param_ptr = this->params[this->params_count];
          value_ptr = this->values[this->params_count];
        } else {
          if(param) {
            *param_ptr++ = request_no_ex[i];
          } else {
            *value_ptr++ = request_no_ex[i];
          }
        }
        if(this->params_count > (MAX_PARAMS_COUNT - 1)) {
          break;
        }
      }
    
      if( this->params[0][0] != '\0') {
       return true;
      } else {
        return false;
      }
    } else if( strcmp(command, "get") != 0 ) {
        strcpy(command, "def");
    }
    return true;

  };

  char *handle_request() {
    if(strcmp(this->command, "def") == 0 ) {
      this->generate_default_page();
    } else if (strcmp(this->command, "get") == 0 ) {
      lcd->get_status(this->result);
    } else if (strcmp(this->command,"set") == 0 ) {
      this->apply_set();
      lcd->update();
    } else {
      strcpy(this->result, "invalid comand");
    }
    return this->result;
  }
  
private:
  void apply_set() {
    for(byte i = 0; i <= this->params_count; i++) {
      if(strcmp(params[i],"color") == 0 ) {
        this->color_work(this->values[i]);
      } else if (strcmp(params[i],"rgb") == 0 ) {
        this->rgb_work(this->values[i]);
      } else if (strcmp(params[i],"level") == 0 ) {
        this->level_work(this->values[i]);
      } else if (strcmp(params[i],"light") == 0) {
        this->light_work(this->values[i]);
      } else if (strcmp(params[i], "delay") == 0) {
        this->delay_work(this->values[i]);
      } else if (strcmp(params[i], "beep") == 0) {
        this->beep_work(this->values[i]);
      } else if (strcmp(params[i], "id") == 0) {
        this->choose_led_work(this->values[i]);
      } else if (strcmp(params[i],"") != 0) {
        this->unrecognize_work(params[i]);
      }
      if(strcmp(params[i], "") != 0) {
        strcat(result, "</br>");
      }
      
    }
  }
  
  
  void choose_led_work(const char *val) {
    int index = atoi(val);
    if(index < 0 || index >= LEDS_COUNT) {
      this->index = -1;
    } else {
      this->index = index;
    }
    if(this->index != -1) {
      strcat(result, "led: ");
      char tmp[11];
      sprintf(tmp, "%d", this->index);
      strcat(result, tmp);
    } else {
      strcat(result, "led: all");
    }
   
  }
  
  
  void delay_work(const char *val) {
      if(this->index != -1) {
        blinks[this->index]->set_timeout(atol(val));
	if(leds[this->index]->lost_info)
        {
	  this->color_work("white");
          leds[this->index]->lost_info = false;
        }
        char tmp[11];
        sprintf(tmp, "%d", blinks[this->index]->get_timeout());
        strcat(result, tmp);
      } else {
        for(byte i = 0; i < LEDS_COUNT; i++) {
          blinks[i]->set_timeout(atol(val));
	  if(leds[i]->lost_info)
          {
            this->color_work("white");	   
            leds[i]->lost_info = false;
          }
        }
        char tmp[11];
        sprintf(tmp, "%d", blinks[0]->get_timeout());
        strcat(result, tmp);
      }
  }
  
  void color_work(const char *val) {
      int color_id = -1;
      for(byte i = 0; i < COLORS_COUNT; i++) {
        if(strcmp(val, colors[i].name) == 0 ) {
          if(this->index != -1) {
            leds[this->index]->change_color(&(colors[i]));
            leds[this->index]->get_state()->modify_time();  
            strcat(result, leds[this->index]->get_state()->color->name);
          } else {
            for(byte j = 0; j < LEDS_COUNT; j++) {
              leds[j]->change_color(&(colors[i]));
              leds[j]->get_state()->modify_time();  
            }
            strcat(result, leds[0]->get_state()->color->name);
          }
          return;
        }
      }
      strcat(result, "color `");
      strcat(result, val);
      strcat(result, "` unrecognize");
  }
  
  void rgb_work(const char *val) {
    strcat(result, "rgb not support now");
    /*
    if(strlen(val) != 6 ) {
      strcat(result, "wrong color code");
      return;
    }
    for(byte i = 0; i < COLORS_COUNT; i++) {
      if(strcmp(colors[i].name, "custom") == 0) {
        char tmp[2];
        tmp[0] = val[0];
        tmp[1] = val[1];
        (colors[i]).r = Utility::hex_to_int(tmp);
        tmp[0] = val[2];
        tmp[1] = val[3];
        (colors[i]).g = Utility::hex_to_int(tmp);
        tmp[0] = val[4];
        tmp[1] = val[5];
        (colors[i]).b = Utility::hex_to_int(tmp);
        leds[i]->change_color(&colors[i]);
        leds[index]->get_state()->modify_time();
        strcat(result, "custom: `");
        strcat(result, val);
        strcat(result, "`");
        return;
      }
    }
    strcat(result, "error to set");
    */
  }
  
  void level_work(const char *val) {
    int color_id = Utility::color_id_by_level(val);
    if( color_id == -1 ) {
      strcat(result, "level `");
      strcat(result, val);
      strcat(result, "` unrecognize");
    } else {
      this->color_work(colors[color_id].name);
    }
  }
  
  void light_work(const char *val) {
      if(strcmp(val, "on") == 0) {
        if(this->index != -1) {
          leds[this->index]->set_state(true);
        } else {
          for(byte i = 0; i < LEDS_COUNT; i++) {
            leds[i]->set_state(true);
          }
        }
        strcat(result, val);
      } else if (strcmp(val, "off") == 0) {
        if(this->index != -1) {
          leds[this->index]->set_state(false);
        } else {
          for(byte i = 0; i < LEDS_COUNT; i++) {
            leds[i]->set_state(false);
          }
        } 
        strcat(result, val);
      } else {
        strcat(result, "light-state `");
        strcat(result, val);
        strcat(result, "` unrecognize");
      }
  }
  
  void beep_work(const char *val) {
    byte beep_time = 150;
    if(strcmp(val, "") != 0 ) {
      beep_time = atoi(val);
    }
    analogWrite(BEEPPIN, 255);
    delay(beep_time);
    analogWrite(BEEPPIN, 0);
    strcat(result, "beep");
  }
  
  void unrecognize_work(const char *val) {
    strcat(result, "unrecognize work `");
    strcat(result, val);
    strcat(result, "`");
  }
  
  void generate_default_page() {
    strcat(this->result, "def");
  }
};






void setup()
{
    colors[0] = Color("black", 0, 0, 0, OFF_CODES[0]);
    colors[1] = Color("navy", 0, 0, 255, "16212127");//blue
    colors[2] = Color("blue", 0, 0, 255, "16212127");
    colors[3] = Color("green", 40, 255, 0, "16228447");
    colors[4] = Color("teal", 0, 255, 255, "16230487");//unknown
    colors[5] = Color("lime", 32, 255, 0, "16228447");//green
    colors[6] = Color("aqua", 0, 255, 255, "16230487");//unknown
    colors[7] = Color("maroon", 255, 0, 0, "16195807");//red
    colors[8] = Color("purple", 255, 0, 255, "16214167");//fuchsia
    colors[9] = Color("olive", 0, 255, 255, "16230487");//unknown
    colors[10] = Color("gray", 255, 255, 255, "16244767");//white
    colors[11] = Color("brown", 0, 255, 255, "16230487");//unknown
    colors[12] = Color("silver", 255, 255, 255, "16244767");//white
    colors[13] = Color("red", 255, 0, 0, "16195807");
    colors[14] = Color("fuchsia", 255, 0, 255, "16214167");  
    colors[15] = Color("orange", 255, 50, 0, "16199887");
    colors[16] = Color("yellow", 255, 150, 0, "16197847");  
    colors[17] = Color("white", 255, 255, 255, "16244767");
    colors[18] = Color("custom", 0, 0, 0, "16230487");//unknown

    ether = new EtherShell(8080);
    leds[0] = new Led(0, 9, 6, 5);
    for(int i = 0; i < LEDS_COUNT; i++) {
        leds[i]->change_color(&(colors[17]));  
        blinks[i] = new Blink(60, 2, leds[i]);
    }
    lcd = new Lcd();
    lcd->set_text(0, "starting...");
    lcd->set_text(1, "waiting for ip");
    if( ether->create_connect() == 0 ) {
        lcd->set_text(0, "cant get connect");
        lcd->set_text(1, "try restart");	
    }
    lcd->update(); 
}

void loop()
{ 
    char *request = new char[REQUEST_SIZE];
    for(byte i = 0; i < REQUEST_SIZE; i++) {
	    *(request + i) = NULL;
    }
    ether->get_request(request);
    if(*request != '\0') {
        Worker *manager = new Worker();
        if(manager->parse_request(request)) 
        { 
           char *handle_result = manager->handle_request();
           ether->finish(handle_result);
        } else 
        {
           ether->finish("invalid request");
        }
        delete manager;
    }
    for(byte i = 0; i < LEDS_COUNT; i++) {
        blinks[i]->check();
    }
    delete[] request;
    delay(100);

};
//16195807 16228447 16212127
//16199887 16232527 16216207
//16189687 16222327 16206007
//16197847 16230487 16214167
//16197847 16230487 16214167

