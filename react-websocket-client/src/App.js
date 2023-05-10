import './App.css';
import { w3cwebsocket as W3CWebSocket } from "websocket";
import React, { useState, useEffect, useRef } from 'react';
import Button from '@mui/material/Button';

function App() {
  const websocket = useRef(null);
  const [LED, setLED] = useState(false);

  useEffect(() => {
    websocket.current = new W3CWebSocket('ws://192.168.2.1/ws');
    websocket.current.onmessage = (message) => {
      const dataFromServer = JSON.parse(message.data);
      // console.log('got reply! ', dataFromServer);
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
          <h3>Project Demo</h3>
          <h1>Currently {LED ? "ON" : "OFF"}</h1>
          <button variant="contained" onClick={LED ? LEDoff : LEDon}>
            ON
          </button>
          <button onClick={LED ? LEDoff : LEDon}>
            OFF
          </button>
        </div>
      </div>
    </div>
  );
}

export default App;
