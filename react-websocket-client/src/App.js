import './App.css';
import { w3cwebsocket as W3CWebSocket } from "websocket";
import React, { useState, useEffect, useRef } from 'react';

function App() {
  const websocket = useRef(null);
  const [LED, setLED] = useState(false);

  useEffect(() => {
    websocket.current = new W3CWebSocket('ws://192.168.1.184/ws');
    websocket.current.onmessage = (message) => {
      const dataFromServer = JSON.parse(message.data);
      if (dataFromServer.type === "message") {
        setLED(dataFromServer.LED)
      }
    };
    return () => websocket.current.close();
  }, [])

  function sendUpdate({ led = LED }) {
    websocket.current.send(
      JSON.stringify({
        type: "message",
        LED: led,
      })
    );
  }

  const LEDon = () => {
    sendUpdate({ led: true });
  }
  const LEDoff = () => {
    sendUpdate({ led: false });
  }

  return (
    <div className='body'>
      <div className="centered">
        <div align="center">
        <h1 align = "center">ESP32 React app hosting Demo</h1>
          <h3>Currently {LED ? "ON" : "OFF"}</h3>
          <button variant="contained" onClick={LEDon}>
            ON
          </button>
          <button onClick={LEDoff }>
            OFF
          </button>
        </div>
      </div>
    </div>
  );
}

export default App;
