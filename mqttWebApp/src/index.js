import _ from 'lodash';
import * as mqtt from 'mqtt'

const referenciared = document.getElementById("red"); 
const referenciaorange = document.getElementById("orange");
const referenciagreen = document.getElementById("green"); 
const startmaintenanceModeButton = document.getElementById("startmaintenanceMode"); 
const stopMaintenanceModeButton = document.getElementById("stopMaintenanceMode"); 

const statusConnection = document.getElementById("connectionStatus"); 


startmaintenanceModeButton.addEventListener("click", ()=>{
    client.publish("maintenanceMode", "1");
    // startmaintenanceMode();
})

stopMaintenanceModeButton.addEventListener("click", ()=>{
    client.publish("maintenanceMode", "0");
    // console.log("Stop Btn Presset")
    //stopMaintenanceMode();
})

let intervalReference;
let timeOutReference;


const client = mqtt.connect('ws://localhost:9001');


function setStateStop() {
     referenciared.setAttribute("class", "circle red");
     referenciaorange.setAttribute("class", "circle gray");
     referenciagreen.setAttribute("class", "circle gray");
}
  function setStateWarning() {
    referenciared.setAttribute("class", "circle gray");
    referenciaorange.setAttribute("class", "circle orange");
    referenciagreen.setAttribute("class", "circle gray");    
}
 function setStateGo() {
    referenciared.setAttribute("class", "circle gray");
    referenciaorange.setAttribute("class", "circle gray");
    referenciagreen.setAttribute("class", "circle green");
}
 function setStateDisable() {
    referenciared.setAttribute("class", "circle gray");
    referenciaorange.setAttribute("class", "circle gray");
    referenciagreen.setAttribute("class", "circle gray");
}

const  startmaintenanceMode = ()=>{
    intervalReference = setInterval(()=>{
        setStateDisable();
        timeOutReference = setTimeout(()=>{
            setStateStop();
        },500)
    },1000)
}
const stopMaintenanceMode = ()=>{
   clearInterval(intervalReference);
   clearTimeout(timeOutReference);
   setStateStop();
}

const setConnected = ()=>{
    statusConnection.setAttribute("class", "connected");
    statusConnection.innerHTML= `<span id="textStatusConnection" class="text-white">Conectado</span>`;
    
}
const setDisconnected = ()=>{
    statusConnection.setAttribute("class", "disconnected");
    statusConnection.innerHTML= `<span id="textStatusConnection" class="text-white">Desconectado</span>`;
}


client.on("connect", () => {    
    //  Suscribes to Traffict Light Topics
    console.log("Connect successfull");

    setConnected();
        
    client.subscribe("trafficLightStatus", (err) => {
        if (err) {
            console.log("Error Suscribring to trafficLightStatus:"+err);
        }
    });

    client.subscribe("maintenanceModeStatus", (err) => {
        if (err) {
            console.log("Error Suscribring to maintenanceModeStatus:"+err);
        }
    });
});

client.on("disconnect", (topic, message) => {
    // message is Buffer
    console.log("Client Disconnected");
    setDisconnected();
});

client.on("message", (topic, message) => {
    // message is Buffer
    const customMessage = message.toString(); 
    
    if(topic ==='trafficLightStatus'){
        
        console.log("Traffict Light is sendding a message");        
        
        if(customMessage === 'stop'){
            console.log("Turn On:", customMessage);
            setStateStop();
            // TODO Connect to Web Application
        }
        if(customMessage === 'warning'){
            console.log("Turn On:", customMessage);
            // TODO Connect to Web Application
            setStateWarning();
        }
        if(customMessage === 'go'){
            console.log("Turn On:", customMessage);
            setStateGo();
            // TODO Connect to Web Application
        }
    }

    if(topic ==='maintenanceModeStatus'){
        if(customMessage==='on'){
            startmaintenanceMode();
        }
        if(customMessage ==='off'){

            console.log("Saliendo de modo mantenimiento");
            stopMaintenanceMode();
        }
        console.log("App Sending a message to change mantenance status mode: "+customMessage);
    }
});


export {
    startmaintenanceMode,
    stopMaintenanceMode
}