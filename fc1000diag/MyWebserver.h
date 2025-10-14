/*


  HTML + CSS styling + javascript all in and undebuggable environment

  get all your HTML code (from html to /html) and past it into this test site
  muck with the HTML and CSS code until it's what you want
  https://www.w3schools.com/html/tryit.asp?filename=tryhtml_intro

  No clue how to debug javascrip other that write, compile, upload, refresh, guess, repeat

  I'm using class designators to set styles and id's for data updating
  for example:
  the CSS class .tabledata defines with the cell will look like
  <td><div class="tabledata" id = "switch"></div></td>

  the XML code will update the data where id = "switch"
  java script then uses getElementById
  document.getElementById("switch").innerHTML="Switch is OFF";


  .. now you can have the class define the look AND the class update the content, but you will then need
  a class for every data field that must be updated, here's what that will look like
  <td><div class="switch"></div></td>

  the XML code will update the data where class = "switch"
  java script then uses getElementsByClassName
  document.getElementsByClassName("switch")[0].style.color=text_color;


  the main general sections of a web page are the following and used here

  <html>
    <style>
    // dump CSS style stuff in here
    </style>
    <body>
      <header>
      // put header code for cute banners here
      </header>
      <main>
      // the buld of your web page contents
      </main>
      <footer>
      // put cute footer (c) 2021 xyz inc type thing
      </footer>
    </body>
    <script>
    // you java code between these tags
    </script>
  </html>


*/


const char PAGE_MAIN[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en" class="js-focus-visible">

<title>FC1000 Antenna Tuner Diag Mode</title>

  <style>
    table {
      position: relative;
      width:100%;
      border-spacing: 0px;
    }
    tr {
      border: 1px solid white;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
    }
    th {
      height: 20px;
      padding: 3px 15px;
      background-color: #343a40;
      color: #FFFFFF !important;
      }
    td {
      height: 20px;
       padding: 3px 15px;
    }
    .tabledata {
      font-size: 24px;
      position: relative;
      padding-left: 5px;
      padding-top: 5px;
      height:   25px;
      border-radius: 5px;
      color: #FFFFFF;
      line-height: 20px;
      transition: all 200ms ease-in-out;
      background-color: #00AA00;
    }
    .fanrpmslider {
      width: 30%;
      height: 55px;
      outline: none;
      height: 25px;
    }
    .bodytext {
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 24px;
      text-align: left;
      font-weight: light;
      border-radius: 5px;
      display:inline;
    }
    .navbar {
      width: 100%;
      height: 50px;
      margin: 0;
      padding: 10px 0px;
      background-color: #FFF;
      color: #000000;
      border-bottom: 5px solid #293578;
    }
    .fixed-top {
      position: fixed;
      top: 0;
      right: 0;
      left: 0;
      z-index: 1030;
    }
    .navtitle {
      float: left;
      height: 50px;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 30px;
      font-weight: bold;
      line-height: 50px;
      padding-left: 20px;
    }
   .navheading {
     position: fixed;
     left: 60%;
     height: 50px;
     font-family: "Verdana", "Arial", sans-serif;
     font-size: 20px;
     font-weight: bold;
     line-height: 20px;
     padding-right: 20px;
   }
   .navdata {
      justify-content: flex-end;
      position: fixed;
      left: 70%;
      height: 50px;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
      font-weight: bold;
      line-height: 20px;
      padding-right: 20px;
   }
    .category {
      font-family: "Verdana", "Arial", sans-serif;
      font-weight: bold;
      font-size: 32px;
      line-height: 50px;
      padding: 20px 10px 0px 10px;
      color: #000000;
    }
    .heading {
      font-family: "Verdana", "Arial", sans-serif;
      font-weight: normal;
      font-size: 28px;
      text-align: left;
    }
  
    .btn {
      background-color: #444444;
      border: none;
      color: white;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 4px 2px;
      cursor: pointer;
    }
    .foot {
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
      position: relative;
      height:   30px;
      text-align: center;   
      color: #AAAAAA;
      line-height: 20px;
    }
    .container {
      max-width: 1800px;
      margin: 0 auto;
    }
    table tr:first-child th:first-child {
      border-top-left-radius: 5px;
    }
    table tr:first-child th:last-child {
      border-top-right-radius: 5px;
    }
    table tr:last-child td:first-child {
      border-bottom-left-radius: 5px;
    }
    table tr:last-child td:last-child {
      border-bottom-right-radius: 5px;
    }

  </style>

  <body style="background-color: #efefef" onload="process()">
  
    <header>
      <div class="navbar fixed-top">
          <div class="container">
            <div class="navtitle">FC1000 Antenna Tuner Diag Mode</div>
            <div class="navdata" id = "date">mm/dd/yyyy</div>
            <div class="navheading">DATE</div><br>
            <div class="navdata" id = "time">00:00:00</div>
            <div class="navheading">TIME</div>
          </div>
      </div>
    </header>
  
    <main class="container" style="margin-top:70px">
      <div class="category">System Status</div>
      <div style="border-radius: 10px !important;">
      <table style="width:50%">
      <colgroup>
        <col span="1" style="background-color:rgb(230,230,230); width: 20%; color:#000000 ;">
        <col span="1" style="background-color:rgb(200,200,200); width: 15%; color:#000000 ;">
        <col span="1" style="background-color:rgb(180,180,180); width: 15%; color:#000000 ;">
      </colgroup>
      <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
      <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
      <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
      <tr>
        <th colspan="1"><div class="heading">Pin</div></th>
        <th colspan="1"><div class="heading">Bits</div></th>
        <th colspan="1"><div class="heading">Volts</div></th>
      </tr>
      <tr>
        <td><div class="bodytext">Forward Power</div></td>
        <td><div class="tabledata" id = "b0"></div></td>
        <td><div class="tabledata" id = "v0"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">Refrect Power</div></td>
        <td><div class="tabledata" id = "b1"></div></td>
        <td><div class="tabledata" id = "v1"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">SWR1</div></td>
        <td><div class="tabledata" id = "swr1"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">SWR2</div></td>
        <td><div class="tabledata" id = "swr2"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">SWR3</div></td>
        <td><div class="tabledata" id = "swr3"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">Hiz</div></td>
        <td><div class="tabledata" id = "hiz"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">Phi</div></td>
        <td><div class="tabledata" id = "phi"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">Power</div></td>
        <td><div class="tabledata" id = "pwr3"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">Digital switch</div></td>
        <td><div class="tabledata" id = "switch"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">Emergency Mode</div></td>
        <td><div class="tabledata" id="emergencyMode"></div>
        </td>
      </tr>
</table>
<br>
      <table>
      <colgroup>
        <col span="1" ; width: 15%; color:#000000 ;">
        <col span="1" ; width: 10%; color:#000000 ;">
        <col span="1" ; width: 10%; color:#000000 ;">
        <col span="1" ; width: 15%; color:#000000 ;">
        <col span="1" ; width: 10%; color:#000000 ;">
        <col span="1" ; width: 10%; color:#000000 ;">
        <col span="1" ; width: 15%; color:#000000 ;">
        <col span="1" ; width: 10%; color:#000000 ;">
        <col span="1" ; width: 10%; color:#000000 ;">
      </colgroup>
      <tr>
        <td><div class="bodytext">RL105</div></td>
        <td><div class="tabledata" id = "rl105"></div></td>
        <td><button type="button" class = "btn" id = "btn105" onclick="ButtonPress105()">Toggle</button></td>
        
        <td><div class="bodytext">RL118</div></td>
        <td><div class="tabledata" id = "rl118"></div></td>
        <td><button type="button" class = "btn" id = "btn118" onclick="ButtonPress118()">Toggle</button></td>
        
        <td><div class="bodytext">RL112</div></td>
        <td><div class="tabledata" id = "rl112"></div></td>
        <td><button type="button" class = "btn" id = "btn112" onclick="ButtonPress112()">Toggle</button></td>
      </tr>
      <tr>
        <td><div class="bodytext">RL106</div></td>
        <td><div class="tabledata" id = "rl106"></div></td>
        <td><button type="button" class = "btn" id = "btn106" onclick="ButtonPress106()">Toggle</button></td>
        
        <td><div class="bodytext">RL119</div></td>
        <td><div class="tabledata" id = "rl119"></div></td>
        <td><button type="button" class = "btn" id = "btn119" onclick="ButtonPress119()">Toggle</button></td>
        
        <td><div class="bodytext">RL113</div></td>
        <td><div class="tabledata" id = "rl113"></div></td>
        <td><button type="button" class = "btn" id = "btn113" onclick="ButtonPress113()">Toggle</button></td>
      </tr>
      <tr>
        <td><div class="bodytext">RL107</div></td>
        <td><div class="tabledata" id = "rl107"></div></td>
        <td><button type="button" class = "btn" id = "btn107" onclick="ButtonPress107()">Toggle</button></td>
        
        <td><div class="bodytext">RL120</div></td>
        <td><div class="tabledata" id = "rl120"></div></td>
        <td><button type="button" class = "btn" id = "btn120" onclick="ButtonPress120()">Toggle</button></td>
        
        <td><div class="bodytext">RL114</div></td>
        <td><div class="tabledata" id = "rl114"></div></td>
        <td><button type="button" class = "btn" id = "btn114" onclick="ButtonPress114()">Toggle</button></td>
      </tr>
      <tr>
        <td><div class="bodytext">RL108</div></td>
        <td><div class="tabledata" id = "rl108"></div></td>
        <td><button type="button" class = "btn" id = "btn108" onclick="ButtonPress108()">Toggle</button></td>
        
        <td><div class="bodytext">RL121</div></td>
        <td><div class="tabledata" id = "rl121"></div></td>
        <td><button type="button" class = "btn" id = "btn121" onclick="ButtonPress121()">Toggle</button></td>
        
        <td><div class="bodytext">RL115</div></td>
        <td><div class="tabledata" id = "rl115"></div></td>
        <td><button type="button" class = "btn" id = "btn115" onclick="ButtonPress115()">Toggle</button></td>
      </tr>
      <tr>
        <td><div class="bodytext">RL109</div></td>
        <td><div class="tabledata" id = "rl109"></div></td>
        <td><button type="button" class = "btn" id = "btn109" onclick="ButtonPress109()">Toggle</button></td>
        
        <td><div class="bodytext">RL122</div></td>
        <td><div class="tabledata" id = "rl122"></div></td>
        <td><button type="button" class = "btn" id = "btn122" onclick="ButtonPress122()">Toggle</button></td>
        
        <td><div class="bodytext">RL116</div></td>
        <td><div class="tabledata" id = "rl116"></div></td>
        <td><button type="button" class = "btn" id = "btn116" onclick="ButtonPress116()">Toggle</button></td>
      </tr>
      <tr>
        <td><div class="bodytext">RL110</div></td>
        <td><div class="tabledata" id = "rl110"></div></td>
        <td><button type="button" class = "btn" id = "btn110" onclick="ButtonPress110()">Toggle</button></td>
        
        <td><div class="bodytext">RL103</div></td>
        <td><div class="tabledata" id = "rl103"></div></td>
        <td><button type="button" class = "btn" id = "btn103" onclick="ButtonPress103()">Toggle</button></td>
        
        <td><div class="bodytext">RL117</div></td>
        <td><div class="tabledata" id = "rl117"></div></td>
        <td><button type="button" class = "btn" id = "btn117" onclick="ButtonPress117()">Toggle</button></td>
      </tr>
      <tr>
        <td><div class="bodytext">RL111</div></td>
        <td><div class="tabledata" id = "rl111"></div></td>
        <td><button type="button" class = "btn" id = "btn111" onclick="ButtonPress111()">Toggle</button></td>
        
        <td><div class="bodytext">RL104</div></td>
        <td><div class="tabledata" id = "rl104"></div></td>
        <td><button type="button" class = "btn" id = "btn104" onclick="ButtonPress104()">Toggle</button></td>
        
        <td><div class="bodytext">RL101</div></td>
        <td><div class="tabledata" id = "rl101"></div></td>
        <td><button type="button" class = "btn" id = "btn101" onclick="ButtonPress101()">Toggle</button></td>
      </tr>
      </table>
      <div style="float: right; width: 50%; padding: 0px 20px;">
        <div class="category">Cores Used</div>
        <table style="width:100%">
          <colgroup>
            <col span="1" style="background-color: #343a40; width: 20%; color: #FFFFFF;">
            <col span="1" style="background-color:rgb(0,0,0); color:#FFFFFF">
          </colgroup>
          <tr>
            <th colspan="1"><div class="heading">Core</div></th>
            <th colspan="1"><div class="heading">Status</div></th>
          </tr>
          <tr>
            <td><div class="bodytext">Core 0</div></td>
            <td><div id="core0Status" class="tabledata"></div></td>
          </tr>
          <tr>
            <td><div class="bodytext">Core 1</div></td>
            <td><div id="core1Status" class="tabledata"></div></td>
          </tr>
        </table>
        <!-- Add the live tasks section -->
        <div style="margin-top: 20px;">
          <div class="category">Task Information</div>
          <div style="display: flex; justify-content: center; align-items: center; flex-direction: column;">
            <div class="heading">Total Tasks</div>
            <div class="circle">
              <div id="taskCount" class="circle-data"></div>
            </div>
          </div>
            <!-- Add this section for HDC1000 live readings -->
          <div style="margin-top: 20px;">
            <div class="category">Live Readings</div>
            <table style="width:50%">
              <tr>
                <td><div class="heading">Temp(&deg;C)</div></td>
                <td><div class="tabledata" id="dhtTemperature"></div></td>
              </tr>
              <tr>
                <td><div class="heading">Humidity (%)</div></td>
                <td><div class="tabledata" id="dhtHumidity"></div></td>
              </tr>
            </table>
          </div>
        </div>

        <style>
          .circle {
            width: 100px;
            height: 100px;
            border-radius: 50%;
            background-color: #343a40;
            color: #FFFFFF;
            display: flex;
            justify-content: center;
            align-items: center;
            text-align: center;
            margin-top: 10px;
          }

          .circle-data {
            font-size: 40px;
            font-weight: bold;
          }
        </style>
      </div>
    </div>
    <br>
    <div class="category">Relay Controls</div>
    <br>
    <div class="bodytext">Inbuilt LED </div>
    <button type="button" class = "btn" id = "btn0" onclick="ButtonPress0()">Toggle</button>
    </div>
    <br>
    <div class="bodytext">Switch</div>
    <button type="button" class = "btn" id = "btn1" onclick="ButtonPress1()">Toggle</button>
    </div>
    <br>

    <br>
    <div class="bodytext">Relay Speed Control (ms: <span id="fanrpm"></span>)</div>
    <br>
    <input type="range" class="fanrpmslider" min="100" max="5000" value = "0" width = "0%" oninput="UpdateSlider(this.value)"/>
    <br>
    <br>
  </main>

  <footer div class="foot" id = "temp" >JJ1VQD/JE2VGT</div></footer>
  
  </body>


  <script type = "text/javascript">
  
    // global variable visible to all java functions
    var xmlHttp=createXmlHttpObject();

    // function to create XML object
    function createXmlHttpObject(){
      if(window.XMLHttpRequest){
        xmlHttp=new XMLHttpRequest();
      }
      else{
        xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
      }
      return xmlHttp;
    }

    // function to handle button press from HTML code above
    // and send a processing string back to server
    // this processing string is use in the .on method
    function ButtonPress0() {
      var xhttp = new XMLHttpRequest(); 
      var message;
      // if you want to handle an immediate reply (like status from the ESP
      // handling of the button press use this code
      // since this button status from the ESP is in the main XML code
      // we don't need this
      // remember that if you want immediate processing feedbac you must send it
      // in the ESP handling function and here
      /*
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          message = this.responseText;
          // update some HTML data
        }
      }
      */
       
      xhttp.open("PUT", "BUTTON_0", false);
      xhttp.send();
    }

    // function to handle button press from HTML code above
    // and send a processing string back to server
    // this processing string is use in the .on method
    function ButtonPress1() {
      var xhttp = new XMLHttpRequest(); 
      /*
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("button1").innerHTML = this.responseText;
        }
      }
      */
      xhttp.open("PUT", "BUTTON_1", false);
      xhttp.send(); 
    }

    function ButtonPress101() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_101", false);
      xhttp.send(); 
    }    

    function ButtonPress103() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_103", false);
      xhttp.send(); 
    }

    function ButtonPress104() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_104", false);
      xhttp.send(); 
    }    

    function ButtonPress105() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_105", false);
      xhttp.send(); 
    }  
    function ButtonPress106() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_106", false);
      xhttp.send(); 
    }  
    function ButtonPress107() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_107", false);
      xhttp.send(); 
    }  
    function ButtonPress108() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_108", false);
      xhttp.send(); 
    }  
    function ButtonPress109() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_109", false);
      xhttp.send(); 
    }
    function ButtonPress110() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_110", false);
      xhttp.send(); 
    }  
    function ButtonPress111() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_111", false);
      xhttp.send(); 
    }  


    function UpdateSlider(value) {
      var xhttp = new XMLHttpRequest();
      // this time i want immediate feedback to the fan speed
      // yea yea yea i realize i'm computing fan speed but the point
      // is how to give immediate feedback
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // update the web based on reply from  ESP
          document.getElementById("fanrpm").innerHTML=this.responseText;
        }
      }
      // this syntax is really weird the ? is a delimiter
      // first arg UPDATE_SLIDER is use in the .on method
      // server.on("/UPDATE_SLIDER", UpdateSlider);
      // then the second arg VALUE is use in the processing function
      // String t_state = server.arg("VALUE");
      // then + the controls value property
      xhttp.open("PUT", "UPDATE_SLIDER?VALUE="+value, true);
      xhttp.send();
    }

    // function to handle the response from the ESP
    function response(){
      var message;
      var barwidth;
      var currentsensor;
      var xmlResponse;
      var xmldoc;
      var dt = new Date();
      var color = "#e8e8e8";
     
      // get the xml stream
      xmlResponse=xmlHttp.responseXML;

      // get host date and time
      document.getElementById("time").innerHTML = dt.toLocaleTimeString();
      document.getElementById("date").innerHTML = dt.toLocaleDateString();

      // A0
      xmldoc = xmlResponse.getElementsByTagName("B0"); //bits for A0
      message = xmldoc[0].firstChild.nodeValue;
      
      if (message > 2048){
      color = "#aa0000";
      }
      else {
        color = "#0000aa";
      }
      
      barwidth = message / 40.95;
      document.getElementById("b0").innerHTML=message;
      document.getElementById("b0").style.width=(barwidth+"%");
      // if you want to use global color set above in <style> section
      // other wise uncomment and let the value dictate the color
      //document.getElementById("b0").style.backgroundColor=color;
      //document.getElementById("b0").style.borderRadius="5px";
      
      xmldoc = xmlResponse.getElementsByTagName("V0"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("v0").innerHTML=message;
      document.getElementById("v0").style.width=(barwidth+"%");
      // you can set color dynamically, maybe blue below a value, red above
      document.getElementById("v0").style.backgroundColor=color;
      //document.getElementById("v0").style.borderRadius="5px";
  
      // A1
      xmldoc = xmlResponse.getElementsByTagName("B1");
      message = xmldoc[0].firstChild.nodeValue;
      if (message > 2048){
      color = "#aa0000";
      }
      else {
        color = "#0000aa";
      }
      document.getElementById("b1").innerHTML=message;
      width = message / 40.95;
      document.getElementById("b1").style.width=(width+"%");
      document.getElementById("b1").style.backgroundColor=color;
      //document.getElementById("b1").style.borderRadius="5px";
      
      xmldoc = xmlResponse.getElementsByTagName("V1");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("v1").innerHTML=message;
      document.getElementById("v1").style.width=(width+"%");
      document.getElementById("v1").style.backgroundColor=color;
      //document.getElementById("v1").style.borderRadius="5px";
  
      xmldoc = xmlResponse.getElementsByTagName("LED");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("switch").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("btn0").innerHTML="Turn ON";
      }
      else{
        document.getElementById("btn0").innerHTML="Turn OFF";
      }

      xmldoc = xmlResponse.getElementsByTagName("SWR1");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("switch").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("swr1").innerHTML="Over 1.5";
        document.getElementById("switch").style.color="#0000AA"; 
      }
      else{
        document.getElementById("swr1").innerHTML="Less 1.5";
        document.getElementById("switch").style.color="#00AA00";
      }
      xmldoc = xmlResponse.getElementsByTagName("SWR2");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("switch").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("swr2").innerHTML="Over 2";
        document.getElementById("switch").style.color="#0000AA"; 
      }
      else{
        document.getElementById("swr2").innerHTML="Less 2";
        document.getElementById("switch").style.color="#00AA00";
      }
      xmldoc = xmlResponse.getElementsByTagName("SWR3");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("switch").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("swr3").innerHTML="Over 3";
        document.getElementById("switch").style.color="#0000AA"; 
      }
      else{
        document.getElementById("swr3").innerHTML="Less 3";
        document.getElementById("switch").style.color="#00AA00";
      }

      xmldoc = xmlResponse.getElementsByTagName("HIZ");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("switch").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("hiz").innerHTML="Over 50";
        document.getElementById("switch").style.color="#0000AA"; 
      }
      else{
        document.getElementById("hiz").innerHTML="Less 50";
        document.getElementById("switch").style.color="#00AA00";
      }

      xmldoc = xmlResponse.getElementsByTagName("PHI");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("switch").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("phi").innerHTML="Inductive";
        document.getElementById("switch").style.color="#0000AA"; 
      }
      else{
        document.getElementById("phi").innerHTML="Capacitive";
        document.getElementById("switch").style.color="#00AA00";
      }

      xmldoc = xmlResponse.getElementsByTagName("PWR3");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("switch").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("pwr3").innerHTML="Over 3W";
        document.getElementById("switch").style.color="#0000AA"; 
      }
      else{
        document.getElementById("pwr3").innerHTML="Less 3W";
        document.getElementById("switch").style.color="#00AA00";
      }

      xmldoc = xmlResponse.getElementsByTagName("SWITCH");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("switch").style.backgroundColor="rgb(200,200,200)";
      // update the text in the table
      if (message == 0){
        document.getElementById("switch").innerHTML="Switch is OFF";
        document.getElementById("btn1").innerHTML="Turn ON";
        document.getElementById("switch").style.color="#0000AA"; 
      }
      else {
        document.getElementById("switch").innerHTML="Switch is ON";
        document.getElementById("btn1").innerHTML="Turn OFF";
        document.getElementById("switch").style.color="#00AA00";
      }

      xmldoc = xmlResponse.getElementsByTagName("RL101");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("rl101").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("rl101").innerHTML="Switch is OFF";
        document.getElementById("btn101").innerHTML="Turn ON";
        document.getElementById("rl101").style.color="#0000AA"; 
      }
      else {
        document.getElementById("rl101").innerHTML="Switch is ON";
        document.getElementById("btn101").innerHTML="Turn OFF";
        document.getElementById("rl101").style.color="#00AA00";
      }

      xmldoc = xmlResponse.getElementsByTagName("RL103");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("btn103").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("rl103").innerHTML="Switch is OFF";
        document.getElementById("btn103").innerHTML="Turn ON";
        document.getElementById("rl103").style.color="#0000AA"; 
      }
      else {
        document.getElementById("rl103").innerHTML="Switch is ON";
        document.getElementById("btn103").innerHTML="Turn OFF";
        document.getElementById("rl103").style.color="#00AA00";
      }

      xmldoc = xmlResponse.getElementsByTagName("RL104");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("btn104").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("rl104").innerHTML="Switch is OFF";
        document.getElementById("btn104").innerHTML="Turn ON";
        document.getElementById("rl104").style.color="#0000AA"; 
      }
      else {
        document.getElementById("rl104").innerHTML="Switch is ON";
        document.getElementById("btn104").innerHTML="Turn OFF";
        document.getElementById("rl104").style.color="#00AA00";
      }

      xmldoc = xmlResponse.getElementsByTagName("RL105");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("btn105").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("rl105").innerHTML="Switch is OFF";
        document.getElementById("btn105").innerHTML="Turn ON";
        document.getElementById("rl105").style.color="#0000AA"; 
      }
      else {
        document.getElementById("rl105").innerHTML="Switch is ON";
        document.getElementById("btn105").innerHTML="Turn OFF";
        document.getElementById("rl105").style.color="#00AA00";
      }

      xmldoc = xmlResponse.getElementsByTagName("RL106");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("btn106").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("rl106").innerHTML="Switch is OFF";
        document.getElementById("btn106").innerHTML="Turn ON";
        document.getElementById("rl106").style.color="#0000AA"; 
      }
      else {
        document.getElementById("rl106").innerHTML="Switch is ON";
        document.getElementById("btn106").innerHTML="Turn OFF";
        document.getElementById("rl106").style.color="#00AA00";
      }

      xmldoc = xmlResponse.getElementsByTagName("RL107");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("btn107").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("rl107").innerHTML="Switch is OFF";
        document.getElementById("btn107").innerHTML="Turn ON";
        document.getElementById("rl107").style.color="#0000AA"; 
      }
      else {
        document.getElementById("rl107").innerHTML="Switch is ON";
        document.getElementById("btn107").innerHTML="Turn OFF";
        document.getElementById("rl107").style.color="#00AA00";
      }

      xmldoc = xmlResponse.getElementsByTagName("RL108");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("btn108").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("rl108").innerHTML="Switch is OFF";
        document.getElementById("btn108").innerHTML="Turn ON";
        document.getElementById("rl108").style.color="#0000AA"; 
      }
      else {
        document.getElementById("rl108").innerHTML="Switch is ON";
        document.getElementById("btn108").innerHTML="Turn OFF";
        document.getElementById("rl108").style.color="#00AA00";
      }

      xmldoc = xmlResponse.getElementsByTagName("RL109");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("btn109").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("rl109").innerHTML="Switch is OFF";
        document.getElementById("btn109").innerHTML="Turn ON";
        document.getElementById("rl109").style.color="#0000AA"; 
      }
      else {
        document.getElementById("rl109").innerHTML="Switch is ON";
        document.getElementById("btn109").innerHTML="Turn OFF";
        document.getElementById("rl109").style.color="#00AA00";
      }

      xmldoc = xmlResponse.getElementsByTagName("RL110");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("btn110").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("rl110").innerHTML="Switch is OFF";
        document.getElementById("btn110").innerHTML="Turn ON";
        document.getElementById("rl110").style.color="#0000AA"; 
      }
      else {
        document.getElementById("rl110").innerHTML="Switch is ON";
        document.getElementById("btn110").innerHTML="Turn OFF";
        document.getElementById("rl110").style.color="#00AA00";
      }

      xmldoc = xmlResponse.getElementsByTagName("RL111");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("btn111").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){
        document.getElementById("rl111").innerHTML="Switch is OFF";
        document.getElementById("btn111").innerHTML="Turn ON";
        document.getElementById("rl111").style.color="#0000AA"; 
      }
      else {
        document.getElementById("rl111").innerHTML="Switch is ON";
        document.getElementById("btn111").innerHTML="Turn OFF";
        document.getElementById("rl111").style.color="#00AA00";
      }

      // Add this part to update the content of the Emergency Mode cell
      xmldoc = xmlResponse.getElementsByTagName("EMERGENCY_MODE");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("emergencyMode").style.backgroundColor="rgb(200,200,200)";
      // var emergencyModeCell = document.getElementById("emergencyMode");
      if (message == 0) {
        document.getElementById("emergencyMode").innerHTML="OFF";
        document.getElementById("emergencyMode").style.color="#00AA00";  // Green color for OFF
      } else {
        document.getElementById("emergencyMode").innerHTML="ON";
        document.getElementById("emergencyMode").style.color = "#AA0000";  // Red color for ON
      }
      // Fetch and update Core status
      xmldoc = xmlResponse.getElementsByTagName("CORE0_STATUS");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("core0Status").innerHTML = message === "1" ? "In Use" : "Not In Use";

      xmldoc = xmlResponse.getElementsByTagName("CORE1_STATUS");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("core1Status").innerHTML = message === "1" ? "In Use" : "Not In Use";
      xmldoc = xmlResponse.getElementsByTagName("TASK_COUNT");
      var taskCount = xmldoc[0].firstChild.nodeValue;
      document.getElementById("taskCount").innerHTML = taskCount;
      // Fetch and update DHT11 live readings
      xmldoc = xmlResponse.getElementsByTagName("DHT_READINGS");
      var dhtTemperature = xmldoc[0].getElementsByTagName("TEMP")[0].firstChild.nodeValue;
      var dhtHumidity = xmldoc[0].getElementsByTagName("HUMIDITY")[0].firstChild.nodeValue;

      document.getElementById("dhtTemperature").innerHTML = dhtTemperature;
      document.getElementById("dhtHumidity").innerHTML = dhtHumidity;
     }
  
    // general processing code for the web page to ask for an XML steam
    // this is actually why you need to keep sending data to the page to 
    // force this call with stuff like this
    // server.on("/xml", SendXML);
    // otherwise the page will not request XML from the ESP, and updates will not happen
    function process(){
     
     if(xmlHttp.readyState==0 || xmlHttp.readyState==4) {
        xmlHttp.open("PUT","xml",true);
        xmlHttp.onreadystatechange=response;
        xmlHttp.send(null);
      }       
        // you may have to play with this value, big pages need more porcessing time, and hence
        // a longer timeout
        setTimeout("process()",100);
    }
  
  
  </script>

</html>




)=====";