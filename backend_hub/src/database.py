
import requests
from dataclasses import dataclass, asdict


@dataclass
class DatabaseParam:
    address: str

@dataclass
class TemperatureLogWrite:
    temperature: float
    device: str

class Database:

    def __init__(self, param: DatabaseParam):
        self._param = param

    
    def write(self, url, payload):
        response = requests.post(
            url = url,
            json = payload
        )
        if 201 != response.status_code:
            print(f"Write to database failed due to: {response.status_code}, {response.text}")
            print(f"Write to database failed with error code: {response.status_code}")
            return False
        return True

    def write_temperature_log(self, temperature_log: TemperatureLogWrite):
        #url = f"{self._param.address}temperature-log/"
        url = f"{self._param.address}"
        payload = asdict(temperature_log)
        print(f"""
URL: {url}
PAYLOAD: {payload}
""")
        return self.write(url, payload)