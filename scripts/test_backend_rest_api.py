
import json
import requests
from dataclasses import dataclass, asdict

DEVICE_SERIAL_NO = "0xf4ce3620277fadba"
BACKEND_URL = "http://localhost:8000/temperature-log/"

@dataclass
class TemperatureLog:
    temperature: float
    created_at: str = ""

    def __post_init__(self):
        self.temperature = float(self.temperature)

    def __str__(self):
        output = f"""
TemperatureLog:
    temperature:  {self.serial_no}
"""
        return output

def add_temperature_log(log: TemperatureLog, serial_no: str):
    data = asdict(log)
    data.pop('created_at')
    data['device'] = serial_no
    print(data)
    response = requests.post(
        url=BACKEND_URL,
        json=data
    )
    print(f"Response: {response.status_code}")

def get_temperature_logs(serial_no: str):
    url = f"{BACKEND_URL}{serial_no}/"
    response = requests.get(url=url)
    output = []
    print(response.text)
    data = json.loads(response.text)
    for log in data:
        print(log)
        output.append(TemperatureLog(**log))
    return output

