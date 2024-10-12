// src/App.js

import React, { useEffect, useState } from 'react';
import './App.css';
import { get_latest_temperature } from './api/temperature';

function App() {
  const [data, setData] = useState("");

  useEffect(() => {

  }, []);

  const handleClick = () => {
    setData("");

    get_latest_temperature("0xf4ce3620277fadba")
      .then((data) => {
        setData(data);
      })
      .catch((error) => {
        setData("Error")
      })
  };

  return (
    <div className="App">
      <h1>Hey World! {data}</h1>
      <button onClick={handleClick}>Send GET request</button>
    </div>
  );
}

export default App;
